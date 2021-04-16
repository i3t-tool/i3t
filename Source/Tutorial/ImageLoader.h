#pragma once
#include <string>
#include "GUIImage.h"


class GUIImageLoaderGL
{
public:
  GUIImageLoaderGL() = default;
  static GUIImage loadImage(std::string& path, int prefferedWidth);
};

inline GUIImage GUIImageLoaderGL::loadImage(std::string& path, int prefferedWidth)
{
}
