//---------------------------------------------------------------------------
/**
 * \file	shaper.h
 *
 * Header for representing line drawer (for debugging)
 *
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _SHAPER_H_
#define _SHAPER_H_

class Camera;

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "pgr.h"

/**
 * \class Shaper
 * \brief A class capable of drawing a set of lines using OpenGL buffers.
 *
 * After taking the prepared geometry of lines (stored in vertices and colors)
 * and copying it into the OpenGL buffers, it can draw the buffers (preserving
 * or deleting the them).
 *
 * Very similar to TabShade, GlyphShader, and AxesNode.
 *
 * Used in World for drawing of world axes, axes of models and wireframe objects
 * (such as projection frustrum) */
class Shaper final
{
private:
  static const std::string vertexShaderSrc;
  static const std::string fragmentShaderSrc;

  // shader
  static GLuint id;         ///< shader program ID
  static GLint positionLoc; ///< location od position
  static GLint colorLoc;    ///< location od color
  static GLint PVMLoc;      ///< location od PVM matrix

  // buffers
  GLuint vao;
  GLuint vertsId;
  GLuint colorsId;

  glm::vec3 currentColor; ///< variable for storage of current draw color

  std::vector<float> vertices; ///< XYZ points prepared for copying to buffers and drawing
  std::vector<float> colors;   ///< RGB colors prepared for copying to buffers and

  void addVector(const float x, const float y, const float z)
  {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
  }

  void addColor(const glm::vec3 c)
  {
    colors.push_back(c.x);
    colors.push_back(c.y);
    colors.push_back(c.z);
  }

public:
  /**
   * \brief Generate OpenGL buffer names
   */
  Shaper()
  {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertsId);
    glGenBuffers(1, &colorsId);
  }

  /**
   * \brief Delete OpenGL buffers
   */
  ~Shaper()
  {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertsId);
    glDeleteBuffers(1, &colorsId);
  }

  void reserveBuffers(const int newCapacity)
  {
    vertices.reserve(newCapacity);
    colors.reserve(newCapacity);
  }

  /**
   * \brief Set the current drawing color.
   * \param color New drawing color "currentColor"
   */
  void setColor(const glm::vec3 color)
  {
    currentColor[0] = color[0];
    currentColor[1] = color[1];
    currentColor[2] = color[2];
  }

  /**
   * \brief Prepare geometry for line from \a a to \a b using the single global color \a currentColor
   * \param a point from
   * \param b point to
   */
  void line(const glm::vec3 a, const glm::vec3 b)
  {
    addVector(a.x, a.y, a.z);
    addVector(b.x, b.y, b.z);
    addColor(currentColor);
    addColor(currentColor);
  }

  /**
   * \brief Prepare geometry for line from \a a to \a b using the single color \a color for the whole line (both line
   * ends) \param a Start point \param b End point \param color Line color
   */
  void line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color)
  {
    addVector(a.x, a.y, a.z);
    addVector(b.x, b.y, b.z);
    addColor(color);
    addColor(color);
  }

  /**
   * \brief Prepare geometry for line from \a a to \a b using the single color \a color for the whole line (both line
   * ends) \param a Start point \param b End point \param color_a Start-point color \param color_b End-point color
   */
  void line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color_a, const glm::vec3 color_b)
  {
    addVector(a.x, a.y, a.z);
    addVector(b.x, b.y, b.z);
    addColor(color_a);
    addColor(color_b);
  }

  /**
   * \brief Clear prepared geometry (vertices and colors).
   */
  void clearPreparedVertices()
  {
    vertices.clear();
    colors.clear();
  }

  /**
   * \brief Dispose the Shaper - Delete program a Shaders.
   *
   * The colors and vertices, and VAO and VBuffers are deleted by the destructor
   * Called from main, finalizeApplication()
   */
  static void dispose() { pgr::deleteProgramAndShaders(id); }

  // initialize OpenGL staff
  /**
   *  \brief Create the Shaper shader program and get attribute locations.
   *
   * \retval 0 OK
   * \retval 1 shader files error
   * \retval 2 shader compile error
   *
   */
  static int initShaders();

  void rebuildVaos();

  /**
   * \brief delete prepared OpenGL buffers (VAO and verts and color VBOs)
   */
  void deleteBuffers() const;

  /**
   * \brief Draw lines already prepared in the vertex buffer and VAO.
   * Does not change the VAO and VBOs contents, neither the stored vertices and colors.
   *
   * (former staticFlush() renamed to drawAndPreserve() - other possible name is e.g. drawPreparedBuffers())
   * \param cam Camera from which the PVM transformation matrix is taken from using (Camera::getCombined()).
   */
  void drawAndPreserve(Camera* cam) const;

  /**
   * \brief Move the prepared geometry (from vertices and colors) to OpenGL vertex buffer objects. And draw them.
   * The prepared geometry is then cleared (vertices and colors).
   * \todo Who generates the buffer names? Delete also the VBOs and VAO - try call to deleteBuffers()? Now deleted in
   * the destructor  ~Shaper() (former  flush() renamed to drawOnceAndDestroy() - other possible name
   * drawPreparedGeometryAndDeleteIt()) \param cam Camera from which the PVM transformation matrix is taken.
   */
  void drawOnceAndDestroy(Camera* cam);
};

#endif
