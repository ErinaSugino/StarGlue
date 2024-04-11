#include "StarLuaRoot.hpp"
#include "StarAssets.hpp"

namespace Star {

LuaRoot::LuaRoot() {
  auto& root = Root::singleton();

  m_luaEngine = LuaEngine::create(root.configuration()->get("safeScripts").toBool());

  m_luaEngine->setRecursionLimit(root.configuration()->get("scriptRecursionLimit").toUInt());
  m_luaEngine->setInstructionLimit(root.configuration()->get("scriptInstructionLimit").toUInt());
  m_luaEngine->setProfilingEnabled(root.configuration()->get("scriptProfilingEnabled").toBool());
  m_luaEngine->setInstructionMeasureInterval(root.configuration()->get("scriptInstructionMeasureInterval").toUInt());

  m_scriptCache = make_shared<ScriptCache>();

  m_rootReloadListener = make_shared<CallbackListener>([cache = m_scriptCache]() {
      cache->clear();
    });
  root.registerReloadListener(m_rootReloadListener);

  m_storageDirectory = root.toStoragePath("lua");
}

LuaRoot::~LuaRoot() {
  auto profile = m_luaEngine->getProfile();
  if (!profile.empty()) {
    profile.sort([](auto const& a, auto const& b) {
        return a.totalTime > b.totalTime;
      });

    std::function<Json (LuaProfileEntry const&)> jsonFromProfileEntry = [&](LuaProfileEntry const& entry) -> Json {
        JsonObject profile;
        profile.set("function", entry.name.value("<function>"));
        profile.set("scope", entry.nameScope.value("?"));
        profile.set("source", strf("%s:%s", entry.source, entry.sourceLine));
        profile.set("self", entry.selfTime);
        profile.set("total", entry.totalTime);
        List<LuaProfileEntry> calls;
        for (auto p : entry.calls)
          calls.append(*p.second);
        profile.set("calls", calls.sorted([](auto const& a, auto const& b) { return a.totalTime > b.totalTime; }).transformed(jsonFromProfileEntry));
        return profile;
      };

    String profileSummary = Json(profile.transformed(jsonFromProfileEntry)).repr(1);

    if (!File::isDirectory(m_storageDirectory)) {
      Logger::info("Creating lua storage directory");
      File::makeDirectory(m_storageDirectory);
    }

    String filename = strf("%s.luaprofile", Time::printCurrentDateAndTime("<year>-<month>-<day>-<hours>-<minutes>-<seconds>-<millis>"));
    String path = File::relativeTo(m_storageDirectory, filename);
    Logger::info("Writing lua profile %s", filename);
    File::writeFile(profileSummary, path);
  }
}

void LuaRoot::loadScript(String const& assetPath) {
  m_scriptCache->loadScript(*m_luaEngine, assetPath);
}

bool LuaRoot::scriptLoaded(String const& assetPath) const {
  return m_scriptCache->scriptLoaded(assetPath);
}

void LuaRoot::unloadScript(String const& assetPath) {
  m_scriptCache->unloadScript(assetPath);
}

LuaContext LuaRoot::createContext(String const& script) {
  return createContext(StringList{script});
}

LuaContext LuaRoot::createContext(StringList const& scriptPaths) {
  auto newContext = m_luaEngine->createContext();

  auto cache = m_scriptCache;
  newContext.setRequireFunction([cache](LuaContext& context, LuaString const& module) {
    if (!context.get("_SBLOADED").is<LuaTable>())
      context.set("_SBLOADED", context.createTable());
    auto t = context.get<LuaTable>("_SBLOADED");
    if (!t.contains(module)) {
      t.set(module, true);
      cache->loadContextScript(context, module.toString());
    }
  });

  for (auto const& scriptPath : scriptPaths)
    cache->loadContextScript(newContext, scriptPath);

  return newContext;
}

void LuaRoot::collectGarbage(Maybe<unsigned> steps) {
  m_luaEngine->collectGarbage(steps);
}

void LuaRoot::setAutoGarbageCollection(bool autoGarbageColleciton) {
  m_luaEngine->setAutoGarbageCollection(autoGarbageColleciton);
}

void LuaRoot::tuneAutoGarbageCollection(float pause, float stepMultiplier) {
  m_luaEngine->tuneAutoGarbageCollection(pause, stepMultiplier);
}

size_t LuaRoot::luaMemoryUsage() const {
  return m_luaEngine->memoryUsage();
}

size_t LuaRoot::scriptCacheMemoryUsage() const {
  return m_scriptCache->memoryUsage();
}

void LuaRoot::clearScriptCache() const {
  return m_scriptCache->clear();
}

LuaEngine& LuaRoot::luaEngine() const {
  return *m_luaEngine;
}

void LuaRoot::ScriptCache::loadScript(LuaEngine& engine, String const& assetPath) {
  auto assets = Root::singleton().assets();
  RecursiveMutexLocker locker(mutex);
  scripts[assetPath] = engine.compile(*assets->bytes(assetPath), assetPath);
}

bool LuaRoot::ScriptCache::scriptLoaded(String const& assetPath) const {
  RecursiveMutexLocker locker(mutex);
  return scripts.contains(assetPath);
}

void LuaRoot::ScriptCache::unloadScript(String const& assetPath) {
  RecursiveMutexLocker locker(mutex);
  scripts.remove(assetPath);
}

void LuaRoot::ScriptCache::clear() {
  RecursiveMutexLocker locker(mutex);
  scripts.clear();
}

void LuaRoot::ScriptCache::loadContextScript(LuaContext& context, String const& assetPath) {
  RecursiveMutexLocker locker(mutex);
  if (!scriptLoaded(assetPath))
    loadScript(context.engine(), assetPath);
  context.load(scripts.get(assetPath));
}

size_t LuaRoot::ScriptCache::memoryUsage() const {
  RecursiveMutexLocker locker(mutex);
  size_t total = 0;
  for (auto const& p : scripts)
    total += p.second.size();
  return total;
}

}
