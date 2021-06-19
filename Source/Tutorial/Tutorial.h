#pragma once

#include "GUIImage.h"
#include "TutorialRenderer.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// forward declaration from TutorialRenderer.h to avoid cyclic dependency
class ITutorialRenderer;

enum class Language
{
	English, Czech
};

struct TutorialElement
{
  TutorialElement()
  {
    m_content = "";
  }
  TutorialElement(std::string content)
    : m_content(std::move(content)) {}
  virtual ~TutorialElement() = default;

  std::string m_content;
  //std::string m_id; // not used

  virtual void acceptRenderer(ITutorialRenderer* tutorialRenderer) = 0;
};

struct Explanation : TutorialElement  // can also contain bullets and other MD syntax for now 
{
  //Explanation()
  //{
  //  m_content = "";
  //}
  Explanation(std::string explanation)
    : TutorialElement(std::move(explanation)) {}

  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderExplanation(this);
  }
};

struct Task : TutorialElement 
{
  Task(std::string task)
    : TutorialElement(std::move(task)),
      m_completed(false)
  {
  }

  bool m_completed; // todo future feature

  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderTask(this);
  }
};

struct Hint : TutorialElement 
{
  Hint(std::string hint)
    : TutorialElement(std::move(hint)),
      m_collapsed(true)
  {
  }
  bool m_collapsed; // todo future feature
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderHint(this);
  }
};

struct ChoiceTask : TutorialElement
{
  ChoiceTask(std::string question, std::vector<std::string> choices, int correctChoice)
    : TutorialElement(std::move(question)),
      m_choices(std::move(choices)),
      m_correctChoice(correctChoice)
  {
  }
  std::vector<std::string> m_choices;
  int m_correctChoice;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderChoiceTask(this);
  }
};

struct MultiChoiceTask : TutorialElement
{
  MultiChoiceTask(std::string question, std::vector<std::string> choices, std::vector<int> correctChoices)
    : TutorialElement(std::move(question)),
      m_choices(std::move(choices)),
      m_correctChoices(std::move(correctChoices))
  {
  }
  std::vector<std::string> m_choices;
  std::vector<int> m_correctChoices;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderMultiChoiceTask(this);
  }
};

struct InputTask : TutorialElement
{
  InputTask(std::string question, std::unordered_set<std::string> correctAnswers)
    : TutorialElement(std::move(question)),
      m_correctAnswers(std::move(correctAnswers))
  {
  }
  std::unordered_set<std::string> m_correctAnswers;
  void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
  {
    tutorialRenderer->renderInputTask(this);
  }
};


struct TStep
{
  TStep() = default;

  // std::string m_title; // deprecated
  std::vector<std::shared_ptr<TutorialElement>> m_content; // NOTE: need a pointer to avoid object slicing

  // todo
  // maybe call task?
  // tasks - ptrs to all task widgets
  // questions - ptrss to all question widgets
  // isCompleted - true if all tasks and questions completed (also check in each update vs check after every change)

  // old HACK - using unique pointers because I need pointers in general in order to avoid slicing of subclass objects (TWText,...) into the base class (TWidget) when storing them in this vector.
  // also, it is preffered to have steps also as unique pointers, 
};

struct TutorialHeader
{
  TutorialHeader(std::string filename,
                 std::string title,
                 std::string description,
                 std::shared_ptr<GUIImage> thumbnail,
                 std::string sceneFilename)
    : m_filename(std::move(filename)),
      m_title(std::move(title)),
      m_description(std::move(description)),
      m_thumbnailImage(std::move(thumbnail)),
      m_sceneFilename(std::move(sceneFilename))
  {
  }
  ~TutorialHeader() = default;

  std::string m_filename;
  std::string m_title;
  std::string m_description;
  std::shared_ptr<GUIImage> m_thumbnailImage;
  std::string m_sceneFilename;
  
};

/**
 * \brief Structure for holding information need for showing a specific tutorial. Should be created by \fn TutorialLoader::loadFile() function.
 */
struct Tutorial
{
  Tutorial(std::shared_ptr<TutorialHeader> header, 
           std::vector<TStep> steps, 
           std::unordered_map<std::string, std::shared_ptr<GUIImage>> filenameToImageMap)
    : m_header(std::move(header)),
      m_steps(std::move(steps)),
      m_filenameToImage(std::move(filenameToImageMap))
  { 
  }
  ~Tutorial() = default;
  // general
  std::shared_ptr<TutorialHeader> m_header;
  // step content
  std::vector<TStep> m_steps;
  // support structures
  std::unordered_map<std::string, std::shared_ptr<GUIImage>> m_filenameToImage; // filename to GUIImage (including GLuint id)
  // other properties
  int getStepCount() const { return m_steps.size(); }
};

