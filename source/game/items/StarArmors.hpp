#ifndef STAR_ARMORS_HPP
#define STAR_ARMORS_HPP

#include "StarGameTypes.hpp"
#include "StarItem.hpp"
#include "StarStatusEffectItem.hpp"
#include "StarEffectSourceItem.hpp"
#include "StarPreviewableItem.hpp"

namespace Star {

STAR_CLASS(ArmorItem);
STAR_CLASS(HeadArmor);
STAR_CLASS(ChestArmor);
STAR_CLASS(LegsArmor);
STAR_CLASS(BackArmor);

class ArmorItem : public Item, public StatusEffectItem, public EffectSourceItem {
public:
  ArmorItem(Json const& config, String const& directory, Json const& data);
  virtual ~ArmorItem() {}

  virtual List<PersistentStatusEffect> statusEffects() const override;
  virtual StringSet effectSources() const override;

  List<String> const& colorOptions();

  String const& directives() const;

  bool hideBody() const;

  Maybe<String> const& techModule() const;

private:
  void refreshIconDrawables();
  void refreshStatusEffects();

  List<String> m_colorOptions;
  List<PersistentStatusEffect> m_statusEffects;
  StringSet m_effectSources;
  String m_directives;
  bool m_hideBody;
  Maybe<String> m_techModule;
};

class HeadArmor : public ArmorItem, public PreviewableItem {
public:
  HeadArmor(Json const& config, String const& directory, Json const& data);
  virtual ~HeadArmor() {}

  virtual ItemPtr clone() const;

  String const& frameset(Gender gender) const;
  String const& maskDirectives() const;

  virtual List<Drawable> preview(PlayerPtr const& viewer = {}) const;

private:
  String m_maleImage;
  String m_femaleImage;
  String m_maskDirectives;
};

class ChestArmor : public ArmorItem, public PreviewableItem {
public:
  ChestArmor(Json const& config, String const& directory, Json const& data);
  virtual ~ChestArmor() {}

  virtual ItemPtr clone() const;

  // Will have :run, :normal, :duck, and :portrait
  String const& bodyFrameset(Gender gender) const;
  // Will have :idle[1-5], :duck, :rotation, :walk[1-5], :run[1-5], :jump[1-4],
  // :fall[1-4]
  String const& frontSleeveFrameset(Gender gender) const;
  // Same as FSleeve
  String const& backSleeveFrameset(Gender gender) const;

  virtual List<Drawable> preview(PlayerPtr const& viewer = {}) const;

private:
  String m_maleBodyImage;
  String m_maleFrontSleeveImage;
  String m_maleBackSleeveImage;

  String m_femaleBodyImage;
  String m_femaleFrontSleeveImage;
  String m_femaleBackSleeveImage;
};

class LegsArmor : public ArmorItem, public PreviewableItem {
public:
  LegsArmor(Json const& config, String const& directory, Json const& data);
  virtual ~LegsArmor() {}

  virtual ItemPtr clone() const;

  // Will have :idle, :duck, :walk[1-8], :run[1-8], :jump[1-4], :fall[1-4]
  String const& frameset(Gender gender) const;

  virtual List<Drawable> preview(PlayerPtr const& viewer = {}) const;

private:
  String m_maleImage;
  String m_femaleImage;
};

class BackArmor : public ArmorItem, public PreviewableItem {
public:
  BackArmor(Json const& config, String const& directory, Json const& data);
  virtual ~BackArmor() {}

  virtual ItemPtr clone() const;

  // Will have :idle, :duck, :walk[1-8], :run[1-8], :jump[1-4], :fall[1-4]
  String const& frameset(Gender gender) const;

  virtual List<Drawable> preview(PlayerPtr const& viewer = {}) const;

private:
  String m_maleImage;
  String m_femaleImage;
};

}

#endif
