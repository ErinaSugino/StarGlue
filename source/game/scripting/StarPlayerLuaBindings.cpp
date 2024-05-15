#include "StarPlayerLuaBindings.hpp"
#include "StarClientContext.hpp"
#include "StarItem.hpp"
#include "StarItemDatabase.hpp"
#include "StarPlayer.hpp"
#include "StarPlayerInventory.hpp"
#include "StarPlayerTech.hpp"
#include "StarPlayerLog.hpp"
#include "StarQuestManager.hpp"
#include "StarWarping.hpp"
#include "StarStatistics.hpp"
#include "StarPlayerUniverseMap.hpp"
#include "StarJsonExtra.hpp"

namespace Star {

LuaCallbacks LuaBindings::makePlayerCallbacks(Player* player) {
  LuaCallbacks callbacks;

  callbacks.registerCallback("id", [player]() { return player->entityId(); });
  callbacks.registerCallback("uniqueId", [player]() { return player->uniqueId(); });
  callbacks.registerCallback("species", [player]() { return player->species(); });
  callbacks.registerCallback("gender", [player]() { return GenderNames.getRight(player->gender()); });
  callbacks.registerCallback("setGender", [player](String const& gender) {
      player->setGender(GenderNames.getLeft(gender));
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("humanoidIdentity", [player]() { return player->humanoid()->identity().toJson(); });
  
  callbacks.registerCallback("bodyDirectives", [player]() { return player->identity().bodyDirectives; });
  callbacks.registerCallback("setBodyDirectives", [player](String const& str) {
      player->setBodyDirectives(str);
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("emoteDirectives", [player]() { return player->identity().emoteDirectives; });
  callbacks.registerCallback("setEmoteDirectives", [player](String const& str) {
      player->setEmoteDirectives(str);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("hairGroup", [player]() { return player->identity().hairGroup; });
  callbacks.registerCallback("setHairGroup", [player](String const& str) {
      player->setHairType(str, player->identity().hairType);
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("hairType", [player]() { return player->identity().hairType; });
  callbacks.registerCallback("setHairType", [player](String const& str) {
      player->setHairType(player->identity().hairGroup, str);
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("hairDirectives", [player]() { return player->identity().hairDirectives; });
  callbacks.registerCallback("setHairDirectives", [player](String const& str) {
      player->setHairDirectives(str);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("facialHairGroup", [player]() { return player->identity().facialHairGroup; });
  callbacks.registerCallback("setFacialHairGroup", [player](String const& str) {
      player->setFacialHair(str, player->identity().facialHairType, player->identity().facialHairDirectives);
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("facialHairType", [player]() { return player->identity().facialHairType; });
  callbacks.registerCallback("setFacialHairType", [player](String const& str) {
      player->setFacialHair(player->identity().facialHairGroup, str, player->identity().facialHairDirectives);
      player->setUpdateLuaContext(true);
    });
  callbacks.registerCallback("facialHairDirectives", [player]() { return player->identity().facialHairDirectives; });
  callbacks.registerCallback("setFacialHairDirectives", [player](String const& str) {
      player->setFacialHair(player->identity().facialHairGroup, player->identity().facialHairType, str);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("name", [player]() { return player->name(); });

  callbacks.registerCallback("imagePath", [player]() { return player->identity().imagePath; });
  callbacks.registerCallback("setImagePath", [player](Maybe<String> const& imagePath) {
      player->setImagePath(imagePath);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("bellyImage", [player]() { return player->identity().bellyImage; });
  callbacks.registerCallback("setBellyImage", [player](Maybe<String> const& bellyImage) {
      player->setBellyImage(bellyImage);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("groinImage", [player]() { return player->identity().groinImage; });
  callbacks.registerCallback("setGroinImage", [player](Maybe<String> const& groinImage) {
      player->setGroinImage(groinImage);
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("personality", [player]() { return player->identity().personalityJson(); });
  callbacks.registerCallback("setPersonality", [player](Json const& personalityConfig) {
      player->setPersonality(parsePersonality(personalityConfig));
      player->setUpdateLuaContext(true);
    });

  callbacks.registerCallback("isAdmin", [player]() { return player->isAdmin(); });

  callbacks.registerCallback("interact", [player](String const& type, Json const& configData, Maybe<EntityId> const& sourceEntityId) {
      player->interact(InteractAction(type, sourceEntityId.value(NullEntityId), configData));
  });

  callbacks.registerCallback("shipUpgrades", [player]() { return player->clientContext()->shipUpgrades().toJson(); });

  callbacks.registerCallback("upgradeShip", [player](Json const& upgrades) {
      player->clientContext()->rpcInterface()->invokeRemote("ship.applyShipUpgrades", upgrades);
  });

  callbacks.registerCallback("setUniverseFlag", [player](String const& flagName) {
      player->clientContext()->rpcInterface()->invokeRemote("universe.setFlag", flagName);
  });

  callbacks.registerCallback("giveBlueprint", [player](Json const& item) { player->addBlueprint(ItemDescriptor(item)); });

  callbacks.registerCallback("blueprintKnown", [player](Json const& item) { return player->blueprintKnown(ItemDescriptor(item)); });

  callbacks.registerCallback("makeTechAvailable", [player](String const& tech) {
      player->techs()->makeAvailable(tech);
  });
  callbacks.registerCallback("makeTechUnavailable", [player](String const& tech) {
      player->techs()->makeUnavailable(tech);
  });
  callbacks.registerCallback("enableTech", [player](String const& tech) {
      player->techs()->enable(tech);
  });
  callbacks.registerCallback("equipTech", [player](String const& tech) {
      player->techs()->equip(tech);
  });
  callbacks.registerCallback("unequipTech", [player](String const& tech) {
      player->techs()->unequip(tech);
  });
  callbacks.registerCallback("availableTechs", [player]() {
      return player->techs()->availableTechs();
  });
  callbacks.registerCallback("enabledTechs", [player]() {
      return player->techs()->enabledTechs();
  });
  callbacks.registerCallback("equippedTech", [player](String typeName) {
      return player->techs()->equippedTechs().maybe(TechTypeNames.getLeft(typeName));
  });

  callbacks.registerCallback("currency", [player](String const& currencyType) { return player->currency(currencyType); });
  callbacks.registerCallback("addCurrency", [player](String const& currencyType, uint64_t amount) {
      player->inventory()->addCurrency(currencyType, amount);
  });
  callbacks.registerCallback("consumeCurrency", [player](String const& currencyType, uint64_t amount) {
      return player->inventory()->consumeCurrency(currencyType, amount);
  });

  callbacks.registerCallback("cleanupItems", [player]() {
      player->inventory()->cleanup();
  });

  callbacks.registerCallback("giveItem", [player](Json const& item) {
      player->giveItem(ItemDescriptor(item));
  });

  callbacks.registerCallback("giveEssentialItem", [player](String const& slotName, Json const& item) {
      auto itemDatabase = Root::singleton().itemDatabase();
      player->inventory()->setEssentialItem(EssentialItemNames.getLeft(slotName), itemDatabase->item(ItemDescriptor(item)));
  });

  callbacks.registerCallback("essentialItem", [player](String const& slotName) {
      return itemSafeDescriptor(player->inventory()->essentialItem(EssentialItemNames.getLeft(slotName))).toJson();
  });

  callbacks.registerCallback("removeEssentialItem", [player](String const& slotName) {
      player->inventory()->setEssentialItem(EssentialItemNames.getLeft(slotName), {});
  });

  callbacks.registerCallback("setEquippedItem", [player](String const& slotName, Json const& item) {
      auto itemDatabase = Root::singleton().itemDatabase();
      auto slot = InventorySlot(EquipmentSlotNames.getLeft(slotName));
      player->inventory()->setItem(slot, itemDatabase->item(ItemDescriptor(item)));
  });

  callbacks.registerCallback("equippedItem", [player](String const& slotName) {
      auto slot = InventorySlot(EquipmentSlotNames.getLeft(slotName));
      if (auto item = player->inventory()->itemsAt(slot))
        return item->descriptor().toJson();
      return Json();
  });

  callbacks.registerCallback("hasItem", [player](Json const& item, Maybe<bool> exactMatch) {
      return player->hasItem(ItemDescriptor(item), exactMatch.value(false));
  });

  callbacks.registerCallback("hasCountOfItem", [player](Json const& item, Maybe<bool> exactMatch) {
      return player->hasCountOfItem(ItemDescriptor(item), exactMatch.value(false));
  });

  callbacks.registerCallback("consumeItem", [player](Json const& item, Maybe<bool> consumePartial, Maybe<bool> exactMatch) {
      return player->takeItem(ItemDescriptor(item), consumePartial.value(false), exactMatch.value(false)).toJson();
  });

  callbacks.registerCallback("inventoryTags", [player]() {
      StringMap<size_t> inventoryTags;
      for (auto const& item : player->inventory()->allItems()) {
        for (auto tag : item->itemTags())
          ++inventoryTags[tag];
      }
      return inventoryTags;
  });

  callbacks.registerCallback("itemsWithTag", [player](String const& tag) {
      JsonArray items;
      for (auto const& item : player->inventory()->allItems()) {
        for (auto itemTag : item->itemTags()) {
          if (itemTag == tag)
            items.append(item->descriptor().toJson());
        }
      }
      return items;
  });

  callbacks.registerCallback("consumeTaggedItem", [player](String const& itemTag, uint64_t count) {
      for (auto const& item : player->inventory()->allItems()) {
        if (item->hasItemTag(itemTag)) {
          uint64_t takeCount = min(item->count(), count);
          player->takeItem(item->descriptor().singular().multiply(takeCount));
          count -= takeCount;
          if (count == 0)
            break;
        }
      }
    });

  callbacks.registerCallback("hasItemWithParameter", [player](String const& parameterName, Json const& parameterValue) {
      for (auto const& item : player->inventory()->allItems()) {
        if (item->instanceValue(parameterName, Json()) == parameterValue)
          return true;
      }
      return false;
    });

  callbacks.registerCallback("consumeItemWithParameter", [player](String const& parameterName, Json const& parameterValue, uint64_t count) {
      for (auto const& item : player->inventory()->allItems()) {
        if (item->instanceValue(parameterName, Json()) == parameterValue) {
          uint64_t takeCount = min(item->count(), count);
          player->takeItem(item->descriptor().singular().multiply(takeCount));
          count -= takeCount;
          if (count == 0)
            break;
        }
      }
    });

  callbacks.registerCallback("getItemWithParameter", [player](String const& parameterName, Json const& parameterValue) -> Json {
      for (auto const& item : player->inventory()->allItems()) {
        if (item->instanceValue(parameterName, Json()) == parameterValue)
          return item->descriptor().toJson();
      }
      return {};
    });

  callbacks.registerCallback("primaryHandItem", [player]() -> Maybe<Json> {
      if (!player->primaryHandItem())
        return {};
      return player->primaryHandItem()->descriptor().toJson();
    });

  callbacks.registerCallback("altHandItem", [player]() -> Maybe<Json> {
      if (!player->altHandItem())
        return {};
      return player->altHandItem()->descriptor().toJson();
    });

  callbacks.registerCallback("primaryHandItemTags", [player]() -> StringSet {
      if (!player->primaryHandItem())
        return {};
      return player->primaryHandItem()->itemTags();
    });

  callbacks.registerCallback("altHandItemTags", [player]() -> StringSet {
      if (!player->altHandItem())
        return {};
      return player->altHandItem()->itemTags();
    });

  callbacks.registerCallback("swapSlotItem", [player]() -> Maybe<Json> {
      if (!player->inventory()->swapSlotItem())
        return {};
      return player->inventory()->swapSlotItem()->descriptor().toJson();
    });

  callbacks.registerCallback("setSwapSlotItem", [player](Json const& item) {
      auto itemDatabase = Root::singleton().itemDatabase();
      player->inventory()->setSwapSlotItem(itemDatabase->item(ItemDescriptor(item)));
    });

  callbacks.registerCallback("canStartQuest",
      [player](Json const& quest) { return player->questManager()->canStart(QuestArcDescriptor::fromJson(quest)); });

  callbacks.registerCallback("startQuest", [player](Json const& quest, Maybe<String> const& serverUuid, Maybe<String> const& worldId) {
      auto questArc = QuestArcDescriptor::fromJson(quest);
      auto followUp = make_shared<Quest>(questArc, 0, player);
      if (serverUuid)
        followUp->setServerUuid(Uuid(*serverUuid));
      if (worldId)
        followUp->setWorldId(parseWorldId(*worldId));
      player->questManager()->offer(followUp);
      return followUp->questId();
    });

  callbacks.registerCallback("hasQuest", [player](String const& questId) {
      return player->questManager()->hasQuest(questId);
    });

  callbacks.registerCallback("hasAcceptedQuest", [player](String const& questId) {
      return player->questManager()->hasAcceptedQuest(questId);
    });

  callbacks.registerCallback("hasActiveQuest", [player](String const& questId) {
      return player->questManager()->isActive(questId);
    });

  callbacks.registerCallback("hasCompletedQuest", [player](String const& questId) {
      return player->questManager()->hasCompleted(questId);
    });

  callbacks.registerCallback("currentQuestWorld", [player]() -> Maybe<String> {
      auto maybeQuest = player->questManager()->currentQuest();
      if (maybeQuest) {
        auto quest = *maybeQuest;
        if (auto worldId = quest->worldId())
          return printWorldId(*worldId);
      }
      return {};
    });

  callbacks.registerCallback("questWorlds", [player]() -> List<pair<String, bool>> {
      List<pair<String, bool>> res;
      auto maybeCurrentQuest = player->questManager()->currentQuest();
      for (auto q : player->questManager()->listActiveQuests()) {
        if (auto worldId = q->worldId()) {
          bool isCurrentQuest = maybeCurrentQuest && maybeCurrentQuest.get()->questId() == q->questId();
          res.append(pair<String, bool>(printWorldId(*worldId), isCurrentQuest));
        }
      }
      return res;
    });

  callbacks.registerCallback("currentQuestLocation", [player]() -> Json {
      auto maybeQuest = player->questManager()->currentQuest();
      if (maybeQuest) {
        if (auto questLocation = maybeQuest.get()->location())
          return JsonObject{{"system", jsonFromVec3I(questLocation->first)}, {"location", jsonFromSystemLocation(questLocation->second)}};
      }
      return {};
    });

  callbacks.registerCallback("questLocations", [player]() -> List<pair<Json, bool>> {
      List<pair<Json, bool>> res;
      auto maybeCurrentQuest = player->questManager()->currentQuest();
      for (auto q : player->questManager()->listActiveQuests()) {
        if (auto questLocation = q->location()) {
          bool isCurrentQuest = maybeCurrentQuest && maybeCurrentQuest.get()->questId() == q->questId();
          auto locationJson = JsonObject{{"system", jsonFromVec3I(questLocation->first)}, {"location", jsonFromSystemLocation(questLocation->second)}};
          res.append(pair<Json, bool>(locationJson, isCurrentQuest));
        }
      }
      return res;
    });

  callbacks.registerCallback("enableMission", [player](String const& mission) {
      AiState& aiState = player->aiState();
      if (!aiState.completedMissions.contains(mission))
        aiState.availableMissions.add(mission);
    });

  callbacks.registerCallback("completeMission", [player](String const& mission) {
      AiState& aiState = player->aiState();
      aiState.availableMissions.remove(mission);
      aiState.completedMissions.add(mission);
    });

  callbacks.registerCallback("hasCompletedMission", [player](String const& mission) -> bool {
      return player->aiState().completedMissions.contains(mission);
    });

  callbacks.registerCallback("radioMessage", [player](Json const& messageConfig, Maybe<float> const& delay) {
      player->queueRadioMessage(messageConfig, delay.value(0));
    });

  callbacks.registerCallback("worldId", [player]() { return printWorldId(player->clientContext()->playerWorldId()); });

  callbacks.registerCallback("serverUuid", [player]() { return player->clientContext()->serverUuid().hex(); });

  callbacks.registerCallback("ownShipWorldId", [player]() { return printWorldId(ClientShipWorldId(player->uuid())); });

  callbacks.registerCallback("lounge", [player](EntityId entityId, Maybe<size_t> anchorIndex) {
      return player->lounge(entityId, anchorIndex.value(0));
    });
  callbacks.registerCallback("isLounging", [player]() { return (bool)player->loungingIn(); });
  callbacks.registerCallback("loungingIn", [player]() -> Maybe<EntityId> {
      if (auto anchorState = player->loungingIn())
        return anchorState->entityId;
      return {};
    });
  callbacks.registerCallback("stopLounging", [player]() { player->stopLounging(); });

  callbacks.registerCallback("playTime", [player]() { return player->log()->playTime(); });

  callbacks.registerCallback("introComplete", [player]() { return player->log()->introComplete(); });
  callbacks.registerCallback("setIntroComplete", [player](bool complete) {
      return player->log()->setIntroComplete(complete);
    });

  callbacks.registerCallback("warp", [player](String action, Maybe<String> animation, Maybe<bool> deploy) {
      player->setPendingWarp(action, animation, deploy.value(false));
    });

  callbacks.registerCallback("canDeploy", [player]() {
      return player->canDeploy();
    });

  callbacks.registerCallback("isDeployed", [player]() -> bool {
      return player->isDeployed();
    });

  callbacks.registerCallback("confirm", [player](Json dialogConfig) {
      auto pair = RpcPromise<Json>::createPair();
      player->queueConfirmation(dialogConfig, pair.second);
      return pair.first;
    });

  callbacks.registerCallback("playCinematic", [player](Json const& cinematic, Maybe<bool> unique) {
      player->setPendingCinematic(cinematic, unique.value(false));
    });

  callbacks.registerCallback("recordEvent", [player](String const& eventName, Json const& fields) {
      player->statistics()->recordEvent(eventName, fields);
    });

  callbacks.registerCallback("worldHasOrbitBookmark", [player](Json const& coords) -> bool {
      CelestialCoordinate coordinate = CelestialCoordinate(coords);
      return player->universeMap()->worldBookmark(coordinate).isValid();
    });

  callbacks.registerCallback("orbitBookmarks", [player]() -> List<pair<Vec3I, Json>> {
      return player->universeMap()->orbitBookmarks().transformed([](pair<Vec3I, OrbitBookmark> const& p) -> pair<Vec3I, Json> {
        return {p.first, p.second.toJson()};
      });
    });

  callbacks.registerCallback("systemBookmarks", [player](Json const& coords) -> List<Json> {
      CelestialCoordinate coordinate = CelestialCoordinate(coords);
      return player->universeMap()->systemBookmarks(coordinate).transformed([](OrbitBookmark const& bookmark) {
          return bookmark.toJson();
        });
    });

  callbacks.registerCallback("addOrbitBookmark", [player](Json const& system, Json const& bookmarkConfig) {
      CelestialCoordinate coordinate = CelestialCoordinate(system);
      return player->universeMap()->addOrbitBookmark(coordinate, OrbitBookmark::fromJson(bookmarkConfig));
    });

  callbacks.registerCallback("removeOrbitBookmark", [player](Json const& system, Json const& bookmarkConfig) {
      CelestialCoordinate coordinate = CelestialCoordinate(system);
      return player->universeMap()->removeOrbitBookmark(coordinate, OrbitBookmark::fromJson(bookmarkConfig));
    });

  callbacks.registerCallback("teleportBookmarks", [player]() -> List<Json> {
      return player->universeMap()->teleportBookmarks().transformed([](TeleportBookmark const& bookmark) -> Json {
        return bookmark.toJson();
      });
    });

  callbacks.registerCallback("addTeleportBookmark", [player](Json const& bookmarkConfig) {
      return player->universeMap()->addTeleportBookmark(TeleportBookmark::fromJson(bookmarkConfig));
    });

  callbacks.registerCallback("removeTeleportBookmark", [player](Json const& bookmarkConfig) {
      player->universeMap()->removeTeleportBookmark(TeleportBookmark::fromJson(bookmarkConfig));
    });

  callbacks.registerCallback("isMapped", [player](Json const& coords) {
      CelestialCoordinate coordinate = CelestialCoordinate(coords);
      return player->universeMap()->isMapped(coordinate);
    });

  callbacks.registerCallback("mappedObjects", [player](Json const& coords) -> Json {
      CelestialCoordinate coordinate = CelestialCoordinate(coords);
      JsonObject json;
      for (auto p : player->universeMap()->mappedObjects(coordinate)) {
        JsonObject object = {
          {"typeName", p.second.typeName},
          {"orbit", jsonFromMaybe<CelestialOrbit>(p.second.orbit, [](CelestialOrbit const& o) { return o.toJson(); })},
          {"parameters", p.second.parameters}
        };
        json.set(p.first.hex(), object);
      }
      return json;
    });

  callbacks.registerCallback("collectables", [player](String const& collection) {
      return player->log()->collectables(collection);
    });

  callbacks.registerCallback("getProperty", [player](String const& name, Maybe<Json> const& defaultValue) -> Json {
      return player->getGenericProperty(name, defaultValue.value(Json()));
    });

  callbacks.registerCallback("setProperty", [player](String const& name, Json const& value) {
      player->setGenericProperty(name, value);
    });

  callbacks.registerCallback("addScannedObject", [player](String const& objectName) -> bool {
      return player->log()->addScannedObject(objectName);
    });

  callbacks.registerCallback("removeScannedObject", [player](String const& objectName) {
      player->log()->removeScannedObject(objectName);
    });

  return callbacks;
}

}
