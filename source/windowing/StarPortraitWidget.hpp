#ifndef STAR_PORTRAIT_WIDGET_HPP
#define STAR_PORTRAIT_WIDGET_HPP

#include "StarWidget.hpp"
#include "StarPlayer.hpp"

namespace Star {

STAR_CLASS(Player);
STAR_CLASS(PortraitWidget);

class PortraitWidget : public Widget {
public:
  PortraitWidget(PortraitEntityPtr entity, PortraitMode mode = PortraitMode::Full);
  PortraitWidget();
  virtual ~PortraitWidget() {}

  void setEntity(PortraitEntityPtr entity);
  void setMode(PortraitMode mode);
  void setScale(float scale);
  void setIconMode();
  bool sendEvent(InputEvent const& event);

protected:
  virtual RectI getScissorRect() const;
  virtual void renderImpl();

private:
  void init();
  void updateSize();

  PortraitEntityPtr m_entity;
  PortraitMode m_portraitMode;
  String m_noEntityImageFull;
  String m_noEntityImagePart;
  float m_scale;

  bool m_iconMode;
  String m_iconImage;
  Vec2I m_iconOffset;
};

}
#endif
