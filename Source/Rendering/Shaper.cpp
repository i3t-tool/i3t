#include "Shaper.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int Shaper::initShaders()
{
  std::cout << "shaper shader init" << std::endl;

  GLuint shaders[] = {
      pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
      pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),
      0,
  };

  if (shaders[0] == 0 || shaders[1] == 0)
  {
    std::cout << "shader files error" << std::endl;
    return 1;
  }

  id = pgr::createProgram(shaders);

  if (id == 0)
  {
    std::cout << "shader compile error" << std::endl;
    return 2;
  }
  // else {
  //	std::cout << "compiled... id = " << id << std::endl;
  //}

  positionLoc = glGetAttribLocation(id, "a_position");
  colorLoc = glGetAttribLocation(id, "a_color");
  PVMLoc = glGetUniformLocation(id, "u_PVM");

  return 0;
}

/**
 * \brief Take the prepared geometry of lines (stored in vertices and colors) and copy it to the OpenGL buffers.
 * Do not delete the prepared geometry (stored in vertices and colors)
 */
void Shaper::rebuildVaos()
{
  size_t size = vertices.size();

  glBindVertexArray(vao);

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, vertsId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // colors
  glBindBuffer(GL_ARRAY_BUFFER, colorsId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, colors.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(colorLoc);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindVertexArray(0);
}

void Shaper::drawAndPreserve(Camera* cam) const
{
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  glBindVertexArray(vao);

  glUseProgram(id);

  glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(cam->getCombined()));

  // draw
  const auto size = static_cast<const GLsizei>(vertices.size());
  glDrawArrays(GL_LINES, 0, size / 3);

  CHECK_GL_ERROR();

  glBindVertexArray(0);

  glUseProgram(0);
}

void Shaper::drawOnceAndDestroy(Camera* cam)
{
  rebuildVaos(); // copy prepared geometry (vertices and colors) to OpenGL buffers

  drawAndPreserve(cam);
  // !!! //
  clearPreparedVertices(); // delete the prepared vertices and colors
  // deleteBuffers();  //deleteOpenGL Buffers
}

// PF - reimplemented above
// void Shaper::flush(Camera * cam) {
//
//	glDisable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//
//	rebuildVaos();
//
//	glBindVertexArray(vao);
//
//	// begin shader
//	glUseProgram(id);
//
//	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(cam->getCombined()));
//
//	// draw
//	const int size = vertices.size();
//	glDrawArrays(GL_LINES, 0, size / 3);
//
//
//	CHECK_GL_ERROR();
//
//	glBindVertexArray(0);
//
//	glUseProgram(0);
//
//	// !!! //
//	clearBuffers(); // delete the prepared vertices and colors
//  clearPreparedVertices(); // renamed
//
//
//	//glDeleteBuffers(1, &vertsId);
//	//glDeleteBuffers(1, &colorsId);
//	//glDeleteVertexArrays(1, &vao);
//	//std::cout << vertices.size() << std::endl;
//
//}

// delete VAO
void Shaper::deleteBuffers() const
{
  glDeleteBuffers(1, &vertsId);
  glDeleteBuffers(1, &colorsId);
  glDeleteVertexArrays(1, &vao);
  // std::cout << vertices.size() << std::endl;
}

/**
 * \brief VS: Transforms the vertex by \a u_PVM matrix and copies the vertex color \a a_color to \a v_color.
 */
const std::string Shaper::vertexShaderSrc("#version 140\n"
                                          "in vec3 a_position;\n"
                                          "in vec3 a_color;\n"
                                          "uniform mat4 u_PVM;\n"
                                          "out vec3 v_color;\n"
                                          "void main() {\n"
                                          "	v_color = a_color;\n"
                                          "	gl_Position = u_PVM * vec4(a_position, 1.0);\n"
                                          "}\n");

/**
 * \brief FS: Copies the interpolated vertex color \a v_color
 */
const std::string Shaper::fragmentShaderSrc("#version 140\n"
                                            "out vec4 fragColor;\n"
                                            "in vec3 v_color;\n"
                                            "void main() {\n"
                                            "  fragColor = vec4(v_color, 1.0);\n"
                                            //"  fragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                            "}\n");

// shader
GLuint Shaper::id;
GLint Shaper::positionLoc;
GLint Shaper::colorLoc;
GLint Shaper::PVMLoc;
