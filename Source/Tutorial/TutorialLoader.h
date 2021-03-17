#pragma once
#include "Tutorial.h"

class TutorialLoader
{
public:
  static TutorialHeader loadTutorialHeader(std::string filename);
  static std::unique_ptr<Tutorial> loadTutorial(TutorialHeader header);
private:
  TutorialLoader() = default;
  static void loadImages();
  static unsigned int loadImageOpenGL(std::string filename);
};
