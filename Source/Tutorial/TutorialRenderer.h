/**
 * \file	Tutorial/TutorialRenderer.h
 *
 * An abstract class defining methods for rendering parts in tutorial window
 */
#pragma once
#include "Tutorial.h"

class ITutorialRenderer {
  virtual void renderTextWidget(TWText tw_text) = 0;
  virtual void renderImageWidget(TWImage tw_image) = 0;
  virtual void renderAnimatedImageWidget(TWAnimatedImage tw_animated_image) = 0;
  virtual void renderTaskWidget(TWTask tw_task) = 0;
  virtual void renderHintWidget(TWHint tw_hint) = 0;

};
