#ifndef STAR_MAIN_INTERFACE_CONFIG_HPP
#define STAR_MAIN_INTERFACE_CONFIG_HPP

#include "StarJson.hpp"
#include "StarPoly.hpp"
#include "StarBiMap.hpp"
#include "StarRegisteredPaneManager.hpp"
#include "StarAnimation.hpp"

namespace Star {

STAR_STRUCT(MainInterfaceConfig);

enum class MainInterfacePanes {
  EscapeDialog,
  Inventory,
  Codex,
  Cockpit,
  Tech,
  Songbook,
  Ai,
  Popup,
  Confirmation,
  JoinRequest,
  Options,
  QuestLog,
  ActionBar,
  TeamBar,
  StatusPane,
  Chat,
  WireInterface,
  PlanetText,
  RadioMessagePopup,
  CraftingPlain,
  QuestTracker,
  MmUpgrade,
  Collections,
  CustomButton1,
  CustomButton2,
  CustomButton3
};

enum class MainBarLayoutTypes {
	Vertical,
	Horizontal,
	Free
};
extern EnumMap<MainBarLayoutTypes> const LayoutTypeLookup;

typedef RegisteredPaneManager<MainInterfacePanes> MainInterfacePaneManager;

struct MainInterfaceConfig {
  static MainInterfaceConfigPtr loadFromAssets();

  unsigned fontSize;

  String inventoryImage;
  String inventoryImageHover;
  String inventoryImageGlow;
  String inventoryImageGlowHover;
  String inventoryImageOpen;
  String inventoryImageOpenHover;

  String beamDownImage;
  String beamDownImageHover;

  String deployImage;
  String deployImageHover;
  String deployImageDisabled;
  String beamUpImage;
  String beamUpImageHover;

  String craftImage;
  String craftImageHover;
  String craftImageOpen;
  String craftImageOpenHover;

  String codexImage;
  String codexImageHover;
  String codexImageOpen;
  String codexImageHoverOpen;

  String questLogImage;
  String questLogImageHover;
  String questLogImageOpen;
  String questLogImageHoverOpen;

  String mmUpgradeImage;
  String mmUpgradeImageHover;
  String mmUpgradeImageOpen;
  String mmUpgradeImageHoverOpen;
  String mmUpgradeImageDisabled;

  String collectionsImage;
  String collectionsImageHover;
  String collectionsImageOpen;
  String collectionsImageHoverOpen;
  String collectionsImageDisabled;

  bool hasCustom1;
  String custom1Image;
  String custom1ImageHover;
  String custom1ImageOpen;
  String custom1ImageHoverOpen;
  String custom1PaneConfig;

  bool hasCustom2;
  String custom2Image;
  String custom2ImageHover;
  String custom2ImageOpen;
  String custom2ImageHoverOpen;
  String custom2PaneConfig;

  bool hasCustom3;
  String custom3Image;
  String custom3ImageHover;
  String custom3ImageOpen;
  String custom3ImageHoverOpen;
  String custom3PaneConfig;

  Vec2I mainBarInventoryButtonOffset;
  Vec2I mainBarCraftButtonOffset;
  Vec2I mainBarCodexButtonOffset;
  Vec2I mainBarBeamButtonOffset;
  Vec2I mainBarDeployButtonOffset;
  Vec2I mainBarQuestLogButtonOffset;
  Vec2I mainBarMmUpgradeButtonOffset;
  Vec2I mainBarCollectionsButtonOffset;
  Vec2I mainBarCustom1ButtonOffset;
  Vec2I mainBarCustom2ButtonOffset;
  Vec2I mainBarCustom3ButtonOffset;

  PolyI mainBarInventoryButtonPoly;
  PolyI mainBarCraftButtonPoly;
  PolyI mainBarCodexButtonPoly;
  PolyI mainBarBeamButtonPoly;
  PolyI mainBarDeployButtonPoly;
  PolyI mainBarQuestLogButtonPoly;
  PolyI mainBarMmUpgradeButtonPoly;
  PolyI mainBarCollectionsButtonPoly;
  PolyI mainBarCustom1ButtonPoly;
  PolyI mainBarCustom2ButtonPoly;
  PolyI mainBarCustom3ButtonPoly;

  PolyI mainBarPoly;
  Vec2I mainBarSize;
  int buttonSize;
  int buttonPolySize;
  MainBarLayoutTypes layoutType;

  Vec2I itemCountRightAnchor;
  Vec2I inventoryItemMouseOffset;

  unsigned maxMessageCount;
  String overflowMessageText;

  Vec2I messageBarPos;
  Vec2I messageItemOffset;

  String messageTextContainer;
  Vec2I messageTextContainerOffset;
  Vec2I messageTextOffset;

  float messageTime;
  float messageHideTime;
  Vec2I messageActiveOffset;
  Vec2I messageHiddenOffset;
  Vec2I messageHiddenOffsetBar;
  float messageWindowSpring;
  float monsterHealthBarTime;

  String hungerIcon;

  float planetNameTime;
  float planetNameFadeTime;
  String planetNameFormatString;
  unsigned planetNameFontSize;
  String planetNameDirectives;
  Vec2I planetNameOffset;

  bool renderVirtualCursor;
  Json cursorItemSlot;

  Vec2I debugOffset;
  unsigned debugFontSize;
  float debugSpatialClearTime;
  float debugMapClearTime;
  Color debugBackgroundColor;
  int debugBackgroundPad;

  StringMap<StringList> macroCommands;
};

}

#endif