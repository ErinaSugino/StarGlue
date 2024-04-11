#include "StarJsonPatch.hpp"
#include "StarJsonPath.hpp"
#include "StarLexicalCast.hpp"

namespace Star {

Json jsonPatch(Json const& base, JsonArray const& patch) {
  auto res = base;
  try {
    for (auto const& operation : patch) {
      res = JsonPatching::applyOperation(res, operation);
    }
    return res;
  } catch (JsonException const& e) {
    throw JsonPatchException(strf("Could not apply patch to base. %s", e.what()));
  }
}

namespace JsonPatching {

  static const StringMap<std::function<Json(Json, Json)>> functionMap = StringMap<std::function<Json(Json, Json)>>{
      {"test", std::bind(applyTestOperation, _1, _2)},
      {"remove", std::bind(applyRemoveOperation, _1, _2)},
      {"add", std::bind(applyAddOperation, _1, _2)},
      {"replace", std::bind(applyReplaceOperation, _1, _2)},
      {"move", std::bind(applyMoveOperation, _1, _2)},
      {"copy", std::bind(applyCopyOperation, _1, _2)},
  };

  Json applyOperation(Json const& base, Json const& op) {
    try {
      auto operation = op.getString("op");
      return JsonPatching::functionMap.get(operation)(base, op);
    } catch (JsonException const& e) {
      throw JsonPatchException(strf("Could not apply operation to base. %s", e.what()));
    } catch (MapException const&) {
      throw JsonPatchException(strf("Invalid operation: %s", op.getString("op")));
    }
  }

  Json applyTestOperation(Json const& base, Json const& op) {
    auto path = op.getString("path");
    auto value = op.opt("value");
    auto inverseTest = op.getBool("inverse", false);

    auto pointer = JsonPath::Pointer(path);

    try {
      auto testValue = pointer.get(base);
      if (!value) {
        if (inverseTest)
          throw JsonPatchTestFail(strf("Test operation failure, expected %s to be missing.", op.getString("path")));
        return base;
      }

      if ((value && (testValue == *value)) ^ inverseTest) {
        return base;
      }

      throw JsonPatchTestFail(strf("Test operation failure, expected %s found %s.", value, testValue));
    } catch (JsonPath::TraversalException& e) {
      if (inverseTest)
        return base;
      throw JsonPatchTestFail(strf("Test operation failure: %s", e.what()));
    }
  }

  Json applyRemoveOperation(Json const& base, Json const& op) {
    return JsonPath::Pointer(op.getString("path")).remove(base);
  }

  Json applyAddOperation(Json const& base, Json const& op) {
    return JsonPath::Pointer(op.getString("path")).add(base, op.get("value"));
  }

  Json applyReplaceOperation(Json const& base, Json const& op) {
    auto pointer = JsonPath::Pointer(op.getString("path"));
    return pointer.add(pointer.remove(base), op.get("value"));
  }

  Json applyMoveOperation(Json const& base, Json const& op) {
    auto fromPointer = JsonPath::Pointer(op.getString("from"));
    auto toPointer = JsonPath::Pointer(op.getString("path"));

    Json value = fromPointer.get(base);
    return toPointer.add(fromPointer.remove(base), value);
  }

  Json applyCopyOperation(Json const& base, Json const& op) {
    auto fromPointer = JsonPath::Pointer(op.getString("from"));
    auto toPointer = JsonPath::Pointer(op.getString("path"));

    return toPointer.add(base, fromPointer.get(base));
  }
}

}
