#include "StarMainInterfaceTypes.hpp"
#include "StarJsonExtra.hpp"
#include "StarRoot.hpp"
#include "StarAssets.hpp"
#include "StarImageMetadataDatabase.hpp"

namespace Star {

EnumMap<MainBarLayoutTypes> const LayoutTypeLookup{
    {MainBarLayoutTypes::Vertical, "vertical"},
    {MainBarLayoutTypes::Horizontal, "horizontal"},
    {MainBarLayoutTypes::Free, "free"}
};

MainInterfaceConfigPtr MainInterfaceConfig::loadFromAssets() {
  auto& root = Root::singleton();
  auto assets = root.assets();
  auto imageMetadata = root.imageMetadataDatabase();

  auto config = make_shared<MainInterfaceConfig>();

  config->fontSize = assets->json("/interface.config:font.baseSize").toInt();

  config->inventoryImage = assets->json("/interface.config:mainBar.inventory.base").toString();
  config->inventoryImageHover = assets->json("/interface.config:mainBar.inventory.hover").toString();
  config->inventoryImageGlow = assets->json("/interface.config:mainBar.inventory.glow").toString();
  config->inventoryImageGlowHover = assets->json("/interface.config:mainBar.inventory.glowHover").toString();
  config->inventoryImageOpen = assets->json("/interface.config:mainBar.inventory.open").toString();
  config->inventoryImageOpenHover = assets->json("/interface.config:mainBar.inventory.openHover").toString();

  config->beamDownImage = assets->json("/interface.config:mainBar.beam.base").toString();
  config->beamDownImageHover = assets->json("/interface.config:mainBar.beam.hover").toString();

  config->deployImage = assets->json("/interface.config:mainBar.deploy.base").toString();
  config->deployImageHover = assets->json("/interface.config:mainBar.deploy.hover").toString();
  config->deployImageDisabled = assets->json("/interface.config:mainBar.deploy.disabled").toString();
  config->beamUpImage = assets->json("/interface.config:mainBar.beamUp.base").toString();
  config->beamUpImageHover = assets->json("/interface.config:mainBar.beamUp.hover").toString();

  config->craftImage = assets->json("/interface.config:mainBar.craft.base").toString();
  config->craftImageHover = assets->json("/interface.config:mainBar.craft.hover").toString();
  config->craftImageOpen = assets->json("/interface.config:mainBar.craft.open").toString();
  config->craftImageOpenHover = assets->json("/interface.config:mainBar.craft.openHover").toString();

  config->codexImage = assets->json("/interface.config:mainBar.codex.base").toString();
  config->codexImageHover = assets->json("/interface.config:mainBar.codex.hover").toString();
  config->codexImageOpen = assets->json("/interface.config:mainBar.codex.open").toString();
  config->codexImageHoverOpen = assets->json("/interface.config:mainBar.codex.openHover").toString();

  config->questLogImage = assets->json("/interface.config:mainBar.questLog.base").toString();
  config->questLogImageHover = assets->json("/interface.config:mainBar.questLog.hover").toString();
  config->questLogImageOpen = assets->json("/interface.config:mainBar.questLog.open").toString();
  config->questLogImageHoverOpen = assets->json("/interface.config:mainBar.questLog.openHover").toString();

  config->mmUpgradeImage = assets->json("/interface.config:mainBar.mmUpgrade.base").toString();
  config->mmUpgradeImageHover = assets->json("/interface.config:mainBar.mmUpgrade.hover").toString();
  config->mmUpgradeImageOpen = assets->json("/interface.config:mainBar.mmUpgrade.open").toString();
  config->mmUpgradeImageHoverOpen = assets->json("/interface.config:mainBar.mmUpgrade.openHover").toString();
  config->mmUpgradeImageDisabled = assets->json("/interface.config:mainBar.mmUpgrade.disabled").toString();

  config->collectionsImage = assets->json("/interface.config:mainBar.collections.base").toString();
  config->collectionsImageHover = assets->json("/interface.config:mainBar.collections.hover").toString();
  config->collectionsImageOpen = assets->json("/interface.config:mainBar.collections.open").toString();
  config->collectionsImageHoverOpen = assets->json("/interface.config:mainBar.collections.openHover").toString();

  config->mainBarSize = jsonToVec2I(assets->json("/interface.config:mainBar.size"));

  /**
  TODO: Rework this whole mess to load proper dynamic sizings instead of stiff config driven offsets
  TODO: Move buttons to the bottom (still above the beam buttons)
  Will break every UI mod, but is overall a 250% more sane option.
  */

  Json mainBar = assets->json("/interface.config:mainBar");
  config->buttonSize = mainBar.getInt("buttonSize", 20);
  config->buttonPolySize = mainBar.getInt("buttonPolySize", 17);
  config->layoutType = LayoutTypeLookup.getLeft(mainBar.getString("layoutType", "vertical"));

  Json custom = mainBar.get("custom", Json());
  Json curCustom;
  int optOffset = 0;

  if (!custom.isNull()) {
      size_t l = custom.size();
      config->hasCustom1 = l > 0;
      config->hasCustom2 = l > 1;
      config->hasCustom3 = l > 2;
  }
  else {
      config->hasCustom1 = false;
      config->hasCustom2 = false;
      config->hasCustom3 = false;
  }

  if (config->hasCustom1) {
      curCustom = custom.get(0);
      config->custom1Image = curCustom.getString("base", "");
      config->custom1ImageHover = curCustom.getString("hover", "");
      config->custom1ImageOpen = curCustom.getString("open", "");
      config->custom1ImageHoverOpen = curCustom.getString("openHover", "");
      config->custom1PaneConfig = curCustom.getString("paneConfig", "");
      optOffset += config->buttonSize;
  }
  if (config->hasCustom2) {
      curCustom = custom.get(1);
      config->custom1Image = curCustom.getString("base", "");
      config->custom1ImageHover = curCustom.getString("hover", "");
      config->custom1ImageOpen = curCustom.getString("open", "");
      config->custom1ImageHoverOpen = curCustom.getString("openHover", "");
      config->custom1PaneConfig = curCustom.getString("paneConfig", "");
      optOffset += config->buttonSize;
  }
  if (config->hasCustom3) {
      curCustom = custom.get(2);
      config->custom1Image = curCustom.getString("base", "");
      config->custom1ImageHover = curCustom.getString("hover", "");
      config->custom1ImageOpen = curCustom.getString("open", "");
      config->custom1ImageHoverOpen = curCustom.getString("openHover", "");
      config->custom1PaneConfig = curCustom.getString("paneConfig", "");
      optOffset += config->buttonSize;
  }

  config->mainBarInventoryButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.inventory.pos"));
  config->mainBarCraftButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.craft.pos"));
  config->mainBarBeamButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.beam.pos"));
  config->mainBarDeployButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.deploy.pos"));
  config->mainBarCodexButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.codex.pos"));
  config->mainBarQuestLogButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.questLog.pos"));
  config->mainBarMmUpgradeButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.mmUpgrade.pos"));
  config->mainBarCollectionsButtonOffset = jsonToVec2I(assets->json("/interface.config:mainBar.collections.pos"));
  config->mainBarCustom1ButtonOffset = jsonToVec2I(mainBar.get("custom1pos", Json().parseJson("[0, 40]")));
  config->mainBarCustom2ButtonOffset = jsonToVec2I(mainBar.get("custom2pos", Json().parseJson("[0, 60]")));
  config->mainBarCustom3ButtonOffset = jsonToVec2I(mainBar.get("custom3pos", Json().parseJson("[0, 80]")));

  config->mainBarInventoryButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.inventory.poly"));
  config->mainBarCraftButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.craft.poly"));
  config->mainBarBeamButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.beam.poly"));
  config->mainBarDeployButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.deploy.poly"));
  config->mainBarCodexButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.codex.poly"));
  config->mainBarQuestLogButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.questLog.poly"));
  config->mainBarMmUpgradeButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.mmUpgrade.poly"));
  config->mainBarCollectionsButtonPoly = jsonToPolyI(assets->json("/interface.config:mainBar.collections.poly"));
  config->mainBarCustom1ButtonPoly = jsonToPolyI(mainBar.get("custom1poly", Json().parseJson("[[0, 40], [17, 40], [17, 57], [0, 57]]")));
  config->mainBarCustom2ButtonPoly = jsonToPolyI(mainBar.get("custom2poly", Json().parseJson("[[0, 60], [17, 60], [17, 77], [0, 77]]")));
  config->mainBarCustom3ButtonPoly = jsonToPolyI(mainBar.get("custom3poly", Json().parseJson("[[0, 80], [17, 80], [17, 97], [0, 97]]")));

  auto offsetPoly = [](PolyI &p, int o, bool x) {
      for (int i = 0; i < p.vertexes().count(); i++) {
          Vec2I &v = p[i];
          x ? v[0] += o : v[1] += o;
      }
  };

  if (config->layoutType == MainBarLayoutTypes::Vertical) {
      config->mainBarSize[1] += optOffset;
      config->mainBarInventoryButtonOffset[1] += optOffset;
      config->mainBarCraftButtonOffset[1] += optOffset;
      config->mainBarCodexButtonOffset[1] += optOffset;
      config->mainBarQuestLogButtonOffset[1] += optOffset;
      config->mainBarMmUpgradeButtonOffset[1] += optOffset;
      config->mainBarCollectionsButtonOffset[1] += optOffset;
      offsetPoly(config->mainBarInventoryButtonPoly, optOffset, false);
      offsetPoly(config->mainBarCraftButtonPoly, optOffset, false);
      offsetPoly(config->mainBarCodexButtonPoly, optOffset, false);
      offsetPoly(config->mainBarQuestLogButtonPoly, optOffset, false);
      offsetPoly(config->mainBarMmUpgradeButtonPoly, optOffset, false);
      offsetPoly(config->mainBarCollectionsButtonPoly, optOffset, false);
  }
  else if (config->layoutType == MainBarLayoutTypes::Horizontal) {
      config->mainBarSize[0] += optOffset;
      config->mainBarInventoryButtonOffset[0] += optOffset;
      config->mainBarCraftButtonOffset[0] += optOffset;
      config->mainBarCodexButtonOffset[0] += optOffset;
      config->mainBarQuestLogButtonOffset[0] += optOffset;
      config->mainBarMmUpgradeButtonOffset[0] += optOffset;
      config->mainBarCollectionsButtonOffset[0] += optOffset;
      offsetPoly(config->mainBarInventoryButtonPoly, optOffset, true);
      offsetPoly(config->mainBarCraftButtonPoly, optOffset, true);
      offsetPoly(config->mainBarCodexButtonPoly, optOffset, true);
      offsetPoly(config->mainBarQuestLogButtonPoly, optOffset, true);
      offsetPoly(config->mainBarMmUpgradeButtonPoly, optOffset, true);
      offsetPoly(config->mainBarCollectionsButtonPoly, optOffset, true);
  }

  config->itemCountRightAnchor = jsonToVec2I(assets->json("/interface.config:itemCountRightAnchor"));
  config->inventoryItemMouseOffset = jsonToVec2I(assets->json("/interface.config:inventoryItemMouseOffset"));

  config->maxMessageCount = assets->json("/interface.config:maxMessageCount").toUInt();
  config->overflowMessageText = assets->json("/interface.config:overflowMessageText").toString();

  config->messageBarPos = jsonToVec2I(assets->json("/interface.config:message.barPos"));
  config->messageItemOffset = jsonToVec2I(assets->json("/interface.config:message.itemOffset"));

  config->messageTextContainer = assets->json("/interface.config:message.textContainer").toString();
  config->messageTextContainerOffset = jsonToVec2I(assets->json("/interface.config:message.textContainerOffset"));
  config->messageTextOffset = jsonToVec2I(assets->json("/interface.config:message.textOffset"));

  config->messageTime = assets->json("/interface.config:message.showTime").toFloat();
  config->messageHideTime = assets->json("/interface.config:message.hideTime").toFloat();
  config->messageActiveOffset = jsonToVec2I(assets->json("/interface.config:message.offset"));
  config->messageHiddenOffset = jsonToVec2I(assets->json("/interface.config:message.offsetHidden"));
  config->messageHiddenOffsetBar = jsonToVec2I(assets->json("/interface.config:message.offsetHiddenBar"));
  config->messageWindowSpring = assets->json("/interface.config:message.windowSpring").toFloat();

  config->monsterHealthBarTime = assets->json("/interface.config:monsterHealth.showTime").toFloat();

  config->hungerIcon = assets->json("/interface.config:hungerIcon").toString();

  config->planetNameTime = assets->json("/interface.config:planetNameTime").toFloat();
  config->planetNameFadeTime = assets->json("/interface.config:planetNameFadeTime").toFloat();
  config->planetNameFormatString = assets->json("/interface.config:planetNameFormatString").toString();
  config->planetNameFontSize = assets->json("/interface.config:font.planetSize").toInt();
  config->planetNameDirectives = assets->json("/interface.config:planetNameDirectives").toString();
  config->planetNameOffset = jsonToVec2I(assets->json("/interface.config:planetTextOffset"));

  config->renderVirtualCursor = assets->json("/interface.config:renderVirtualCursor").toBool();
  config->cursorItemSlot = assets->json("/interface.config:cursorItemSlot");

  config->debugOffset = jsonToVec2I(assets->json("/interface.config:debugOffset"));
  config->debugFontSize = assets->json("/interface.config:debugFontSize").toUInt();
  config->debugSpatialClearTime = assets->json("/interface.config:debugSpatialClearTime").toFloat();
  config->debugMapClearTime = assets->json("/interface.config:debugMapClearTime").toFloat();
  config->debugBackgroundColor = jsonToColor(assets->json("/interface.config:debugBackgroundColor"));
  config->debugBackgroundPad = assets->json("/interface.config:debugBackgroundPad").toUInt();

  for (auto const& path : assets->scanExtension("macros")) {
    for (auto const& pair : assets->json(path).iterateObject())
      config->macroCommands.add(pair.first, jsonToStringList(pair.second));
  }

  return config;
}

}
