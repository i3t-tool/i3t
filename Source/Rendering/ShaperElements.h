/**
 * \file	shaperElements.h
 *
 * Drawer of some special shapes with shaper 
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague		  
 */
#pragma once

#include <iostream>

#include "Rendering/Shaper.h"
#include "Utils/Color.h"

/**
 * \brief Static class with drawing routines using a stored Shaper.
 * Used for drawing of crosses, axes, and frustrum by means of \ref Shaper::line()
 */
class ShaperElements
{
private:

  static glm::vec3 unitCube[24];

  static Shaper* shaper;   ///< active Shaper - all commands are applied to this Shaper vertices and OpenGL buffers

public:

  static void setShaper(Shaper* sh)
  {
    shaper = sh;
  }

  static Shaper* getShaper()
  {
    return shaper;
  }

  /**
   * \brief Prepare Coordinate axes (vertices and colors to vertices and colors float vectors)
   * \param pos Origin
   * \param size Length of the axes
   */
  static void coordinateAxes(const glm::vec3 pos, const float size)
  {
    shaper->line(pos, pos + glm::vec3(1.0f, 0.0f, 0.0f) * size, Color::RED);
    shaper->line(pos, pos + glm::vec3(0.0f, 1.0f, 0.0f) * size, Color::GREEN);
    shaper->line(pos, pos + glm::vec3(0.0f, 0.0f, 1.0f) * size, Color::BLUE);
  }

  /**
   * \brief Draw little coordinate frame and a yellow direction vector
   * \param pos Coordinate frame origin
   * \param dir Vector to draw
   */
  static void direction(const glm::vec3 pos, const glm::vec3 dir)
  {
    coordinateAxes(pos, 0.2f);
    shaper->line(pos, pos + dir, Color::YELLOW);
  }

  /**
   * \brief Draw a 3D cross using RGB colors
   * \param pos Cross center
   * \param hSize Cross size - length of cross arms.
   */
  static void cross(const glm::vec3 pos, const float hSize)
  {
    shaper->line(pos - glm::vec3(hSize, 0.0f, 0.0f), pos + glm::vec3(hSize, 0.0f, 0.0f), Color::RED);
    shaper->line(pos - glm::vec3(0.0f, hSize, 0.0f), pos + glm::vec3(0.0f, hSize, 0.0f), Color::GREEN);
    shaper->line(pos - glm::vec3(0.0f, 0.0f, hSize), pos + glm::vec3(0.0f, 0.0f, hSize), Color::BLUE);
  }

  /**
   * \brief Draw a 3D cross using a single given \a color
   * \param pos Cross center
   * \param hSize Cross size - length of cross arms.
   * \param color Cross color
   */
  static void cross(glm::vec3 pos, float hSize, glm::vec3 color)
  {
    shaper->line(pos - glm::vec3(hSize, 0.0f, 0.0f), pos + glm::vec3(hSize, 0.0f, 0.0f), color);
    shaper->line(pos - glm::vec3(0.0f, hSize, 0.0f), pos + glm::vec3(0.0f, hSize, 0.0f), color);
    shaper->line(pos - glm::vec3(0.0f, 0.0f, hSize), pos + glm::vec3(0.0f, 0.0f, hSize), color);
  }

  /**
   * \brief Draw a coordinate frame based on column values of the given matrix 
   * 
   * \rename Rename to something like coordinateFrameOfTheMatrix, because it does not call OpenGL draw, but adds other object to the current Shaper vertices
   * \param ori Given matrix
   * \param size Cross size - length of cross arms.
   */
  static void drawOrientation(glm::mat4 ori, const float size)
  {
    const glm::vec3 pos = glm::vec3(ori[3]);

    shaper->line(pos, pos + glm::vec3(ori[0]) * size, Color::RED, Color::RED);
    shaper->line(pos, pos + glm::vec3(ori[1]) * size, Color::GREEN, Color::GREEN);
    shaper->line(pos, pos + glm::vec3(ori[2]) * size, Color::BLUE, Color::BLUE);
  }

  /**
   * \brief Create box wire-frame of the given color.
   * \param ppp First corner
   * \param nnn Second corner 
   * \param color Color of the lines
   */
  static void alignBox(glm::vec3 ppp, glm::vec3 nnn, glm::vec3 color);

  static void gridXY(glm::vec2 pos, float step, int count);

  static void drawUnitCube(glm::mat4 trans)
  {
    for (int i = 0; i < 24; i += 2)
    {
      shaper->line(glm::vec3(trans * glm::vec4(unitCube[i], 1.0f)),
                   glm::vec3(trans * glm::vec4(unitCube[i + 1], 1.0f)));
    }
  }

  /**
   * \brief Draw the frustrum as wireframe 
   * \param p Projection matrix
   * \param v View matrix - most probably E^(-1)
   */
  static void drawFrustum(const glm::mat4 p, const glm::mat4 v)
  {
    //if (a.w < 0) cout << "je zaporny " << a.w << std::endl;


    const glm::mat4 tr = glm::inverse(p * glm::inverse(v));

    glm::vec4 a, b;

    for (int i = 0; i < 24; i += 2)
    {
      a = tr * glm::vec4(unitCube[i], 1.0f);
      a /= abs(a.w);

      b = tr * glm::vec4(unitCube[i + 1], 1.0f);
      b /= abs(b.w);

      shaper->line(glm::vec3(a), glm::vec3(b));
    }
  }
};
