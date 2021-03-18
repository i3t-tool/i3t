/**
 * \file	Tutorial/TutorialRenderer.h
 *
 * An abstract class defining methods for rendering parts in tutorial window
 */
#pragma once
// forward declarations from Tutorial.h to avoid cyclic dependency
struct TWText;
struct TWImage;
struct TWAnimatedImage;
struct TWTask;
struct TWHint;
struct TWSpacing;

class ITutorialRenderer {
public:
  virtual ~ITutorialRenderer() = default;

  virtual void renderTextWidget(TWText* tw_text) = 0;
  virtual void renderImageWidget(TWImage* tw_image) = 0;
  virtual void renderAnimatedImageWidget(TWAnimatedImage* tw_animated_image) = 0;
  virtual void renderTaskWidget(TWTask* tw_task) = 0;
  virtual void renderHintWidget(TWHint* tw_hint) = 0;
  virtual void renderSpacingWidget(TWSpacing* tw_hint) = 0;

};
