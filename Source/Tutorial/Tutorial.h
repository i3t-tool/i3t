#pragma once

#include "TutorialRenderer.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// forward declaration from TutorialRenderer.h to avoid cyclic dependency
class ITutorialRenderer;

struct TWidget
{
  virtual ~TWidget() = default;
  //std::string m_id;
  virtual void acceptRenderer(ITutorialRenderer* tutorialRenderer) = 0;
};

struct TWText : TWidget  // can also contain bullets and other MD syntaxe for now 
{
  TWText(std::string text) : m_text(text) {}
  std::string m_text;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderTextWidget(this);
  }
};

struct TWTask : TWidget 
{
  TWTask(std::string task) : m_task(task) {}
  std::string m_task;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderTaskWidget(this);
  }
};

struct TWHint : TWidget 
{
  TWHint(std::string hint) : m_hint(hint) {}
  std::string m_hint;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderHintWidget(this);
  }
};

struct TWSpacing : TWidget 
{
  TWSpacing() = default;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderSpacingWidget(this);
  }
};

struct TWImage : TWidget
{
  TWImage(std::string filename) : m_filename(filename) {}
  std::string m_filename;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderImageWidget(this);
  }
};

struct TWAnimatedImage : TWidget
{
  std::string m_filename;
  // todo animation stuf
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderAnimatedImageWidget(this);
  }
};

// struct TWButton : TWidget
// {
//   std::string m_text;
//   // todo command
//   void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
//   {
//     tutorialRenderer->renderButtonWidget(this);
//   }
// };

struct TStep
{
  explicit TStep() { m_title = "undefined"; }
  explicit TStep(std::string title) : m_title(std::move(title)) {}
  std::string m_title;
  // HACK using unique pointers because I need pointers in general in order to avoid slicing of subclass objects (TWText,...) into the base class (TWidget) when storing them in this vector.
  // also, it is preffered to have steps also as unique pointers, 
  std::vector<std::unique_ptr<TWidget>> m_content;
};

struct TutorialHeader
{
  TutorialHeader();
  ~TutorialHeader();
  std::string m_filename;
  std::string m_title;
  std::string m_description;
  std::string m_thumb_filename;
  unsigned int m_thumb_gl_id;
  std::string m_scene_filename;
  
};

/**
 * \brief Structure for holding information need for showing a specific tutorial. Is filled by \fn TutorialLoader::loadFile() function.
 */
struct Tutorial
{
  Tutorial(TutorialHeader header);
  ~Tutorial();
  // general
  TutorialHeader m_header;
  // step content
  std::vector<TStep> m_steps;
  // support structures
  std::unordered_map<std::string, unsigned int> m_image_to_gl_id; // filename to GLuint
  // other properties
  int getStepCount() const { return m_steps.size(); }
};


inline TutorialHeader::TutorialHeader()
{
  m_filename = "";
  m_title = "undefined";
  m_description = "undefined";
  m_thumb_filename = "";
  m_thumb_gl_id = 0;
  m_scene_filename = "";
}

inline TutorialHeader::~TutorialHeader() = default;

inline Tutorial::Tutorial(TutorialHeader header) {
  m_header = header;
}

inline Tutorial::~Tutorial()
{
  // todo free loaded images from video memory
  // EDIT NOPE! use destructor in the proper widget ... ah... well, its opengl..
  // then decide, depending on where youll do the opengl initializing stuff
  // also, you should probably decide if OOP is the top one you use, or if you are trying to force some MVC stuff in here
}
