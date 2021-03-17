#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct TWidget
{
  virtual ~TWidget() = default;
  //std::string m_id;
};

struct TWText : TWidget  // can also contain bullets and other MD syntaxe for now 
{
  TWText(std::string text) : m_text(std::move(text)) {}
  std::string m_text;
};

struct TWTask : TWidget 
{
  TWTask(std::string task) : m_task(std::move(task)) {}
  std::string m_task;
};

struct TWHint : TWidget 
{
  TWHint(std::string hint) : m_hint(std::move(hint)) {}
  std::string m_hint;
};

struct TWSpacing : TWidget 
{
  TWSpacing() = default;
};

struct TWImage : TWidget
{
  TWImage(std::string filename) : m_filename(std::move(filename)) {}

  std::string m_filename;
};

struct TWAnimatedImage : TWidget
{
  std::string m_filename;
  // todo animation stuf
};

struct TWButton : TWidget
{
  std::string m_text;
  // todo command
};

struct TStep
{
  //explicit TStep() { m_title = "undefined"; }
  explicit TStep(std::string title) : m_title(std::move(title)) {}
  std::string m_title;
  // HACK using unique pointers because I need pointers in general in order to avoid slicing of subclass objects (TWText,...) into the base class (TWidget) when storing them in this vector.
  // also, it is preffered to have steps also as unique pointers, 
  std::vector<std::unique_ptr<TWidget>> m_content;
};

/**
 * \brief Structure for holding information need for showing a specific tutorial. Is filled by \fn TutorialLoader::loadFile() function.
 */
class Tutorial
{
public:
  Tutorial();
  ~Tutorial();
private:
  // general
  std::string m_filename;
  std::string m_title;
  std::string m_description;
  std::string m_thumb_filename;
  std::string m_scene_filename;

  // step content
  std::vector<TStep> m_steps;
  // support structures
  std::unordered_map<std::string, unsigned int> m_images; // filename to GLuint
  // properties

public:
  [[nodiscard]] std::string getFilename() const { return m_filename; }
  [[nodiscard]] std::string getTitle() const { return m_title; }
  [[nodiscard]] std::string getDescription() const { return m_description; }
  [[nodiscard]] int getStepCount() const { return m_steps.size(); }
};



