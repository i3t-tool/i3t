#include "TutorialLoader.h"
#include <iostream>
#include <fstream>
#include <utility>

std::unique_ptr<Tutorial> TutorialLoader::loadFile(std::string filename)
{
  // parsing
  //std::ofstream tut_file(filename);

  std::unique_ptr<Tutorial> tutorial(new Tutorial()); // we create our tutorial object on heap
  tutorial->m_filename = std::move(filename);
  tutorial->m_description = "blabla";
  tutorial->m_title = "blablu";


  // nicely fill it at that heap place ^^
  // if this looks like magic, feel free to change it, but remember not to copy unique_ptrs!
  tutorial->m_steps.emplace_back(TStep()).m_content.push_back(std::make_unique<TWText>("yay"));
  // the first emplace returns a refernce to that newly created TStep here
  // the second can either be push or emplace, since both will automatically move instead of copy
  // make unique creates the <> specified object with passed arguments on heap (sadly a separate place) and returns the unique_ptr to it (or a referemnce to the uptr)

  // important is only to create the TStep with its vector first, and only then fill it with uptrs to widgets (not the other way around, since they would need to get copied into it)
  // another way of doing it:
  TStep& step = tutorial->m_steps.emplace_back(TStep());
  step.m_content.push_back(std::make_unique<TWText>("fldsmdfr"));
  step.m_content.push_back(std::make_unique<TWText>("haha"));
  // WARNING! do not attempt to reassign the reference!

  TStep& step2 = tutorial->m_steps.emplace_back(TStep());
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
