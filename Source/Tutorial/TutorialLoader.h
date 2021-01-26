#pragma once
#include "Tutorial.h"

class TutorialLoader
{
public:
  static std::unique_ptr<Tutorial> loadFile(std::string filename);
private:
  TutorialLoader() = default;
  static void loadImages();
  static unsigned int loadImageOpenGL(std::string filename);
};
