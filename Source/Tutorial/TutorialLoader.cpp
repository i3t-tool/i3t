#include "TutorialLoader.h"
#include "Logger/Logger.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <yaml-cpp/yaml.h>

std::unique_ptr<Tutorial> TutorialLoader::loadFile(std::string filename)
{


  // CREATE TUTORIAL
  std::unique_ptr<Tutorial> tutorial(new Tutorial()); // we create our tutorial object on heap
  // filename
  tutorial->m_filename = filename;

  // CHECK
  std::ifstream tutorial_stream(filename);
  if (!tutorial_stream.good()) {
    LOG_ERROR("Tutorial file '" + filename + "' was not found")
    return tutorial; // return a dummy tutorial
  }

  // PARSE GENERAL INFO - YAML
  YAML::Node tutorial_yaml = YAML::LoadFile(filename);
  std::cout << tutorial_yaml << std::endl;
  // title
  if (tutorial_yaml["title"])
  {
    tutorial->m_title = tutorial_yaml["title"].as<std::string>();
    LOG_DEBUG(tutorial->m_title);
  }
  else
  {
    LOG_ERROR("Tutorial title not specified");
  }
  // description
  if (tutorial_yaml["description"])
  {
    tutorial->m_description = tutorial_yaml["description"].as<std::string>();
    LOG_DEBUG(tutorial->m_description);
  }
  else
  {
    LOG_ERROR("Tutorial description not specified");
  }

  // PARSE STEPS - CUSTOM + IMGUI MARKDOWN


  // nicely fill it at that heap place ^^
  // if this looks like magic, feel free to change it, but remember not to copy unique_ptrs!
  tutorial->m_steps.emplace_back(TStep("Krok 1")).m_content.push_back(std::make_unique<TWText>("yay"));
  // the first emplace returns a refernce to that newly created TStep here
  // the second can either be push or emplace, since both will automatically move instead of copy
  // make unique creates the <> specified object with passed arguments on heap (sadly a separate place) and returns the unique_ptr to it (or a referemnce to the uptr)

  // important is only to create the TStep with its vector first, and only then fill it with uptrs to widgets (not the other way around, since they would need to get copied into it)
  // another way of doing it:
  TStep& step = tutorial->m_steps.emplace_back(TStep("Krok 2"));
  step.m_content.push_back(std::make_unique<TWText>("fldsmdfr"));
  step.m_content.push_back(std::make_unique<TWText>("haha"));
  // WARNING! do not attempt to reassign the reference!

  TStep& step2 = tutorial->m_steps.emplace_back(TStep("Krok 3"));
  step2.m_content.push_back(std::make_unique<TWText>("juj"));
  step2.m_content.push_back(std::make_unique<TWText>("omg"));

  return tutorial; // NOTE: this will automatically move the unique pointer, no need to worry 
}

void TutorialLoader::loadImages()
{
  // todo for cyklus
}

unsigned int TutorialLoader::loadImageOpenGL(std::string filename)
{
  // todo opengl image loading, zvazit co pouzit za knihovnu
  return 0;
}
