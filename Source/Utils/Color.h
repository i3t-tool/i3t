//---------------------------------------------------------------------------
/**
 * \file	color.h
 *
 * Class for representing color helper class 
  * 
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague		  
*/
//---------------------------------------------------------------------------

#ifndef _COLOR_H_
#define _COLOR_H_

#include <glm/glm.hpp>

#include <cstdlib>

struct Color final
{
  static glm::vec3 WHITE;
  static glm::vec3 BLACK;
  static glm::vec3 LIGHT_GRAY;
  static glm::vec3 GRAY;
  static glm::vec3 DARK_GRAY;
  static glm::vec3 RED;
  static glm::vec3 GREEN;
  static glm::vec3 BLUE;
  static glm::vec3 YELLOW;
  static glm::vec3 TEAL;
  static glm::vec3 MAGENTA;
  static glm::vec3 LIGHT_BLUE;
  static glm::vec3 ORANGE;
  static glm::vec3 BROWN;

  static glm::vec3 getColor(const float r, const float g, const float b)
  {
    return glm::vec3(r, g, b);
  }

  static glm::vec3 randomColor()
  {
    return glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
  }
};

#endif
