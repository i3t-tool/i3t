#include "ShaperElements.h"

void ShaperElements::alignBox(const glm::vec3 ppp, const glm::vec3 nnn, const glm::vec3 color)
{
  shaper->setColor(color);

  // front
  shaper->line(glm::vec3(ppp.x, ppp.y, ppp.z), glm::vec3(nnn.x, ppp.y, ppp.z));
  shaper->line(glm::vec3(nnn.x, ppp.y, ppp.z), glm::vec3(nnn.x, nnn.y, ppp.z));
  shaper->line(glm::vec3(nnn.x, nnn.y, ppp.z), glm::vec3(ppp.x, nnn.y, ppp.z));
  shaper->line(glm::vec3(ppp.x, nnn.y, ppp.z), glm::vec3(ppp.x, ppp.y, ppp.z));

  // back
  shaper->line(glm::vec3(ppp.x, ppp.y, nnn.z), glm::vec3(nnn.x, ppp.y, nnn.z));
  shaper->line(glm::vec3(nnn.x, ppp.y, nnn.z), glm::vec3(nnn.x, nnn.y, nnn.z));
  shaper->line(glm::vec3(nnn.x, nnn.y, nnn.z), glm::vec3(ppp.x, nnn.y, nnn.z));
  shaper->line(glm::vec3(ppp.x, nnn.y, nnn.z), glm::vec3(ppp.x, ppp.y, nnn.z));

  // sides
  shaper->line(glm::vec3(ppp.x, ppp.y, ppp.z), glm::vec3(ppp.x, ppp.y, nnn.z));
  shaper->line(glm::vec3(nnn.x, ppp.y, ppp.z), glm::vec3(nnn.x, ppp.y, nnn.z));
  shaper->line(glm::vec3(nnn.x, nnn.y, ppp.z), glm::vec3(nnn.x, nnn.y, nnn.z));
  shaper->line(glm::vec3(ppp.x, nnn.y, ppp.z), glm::vec3(ppp.x, nnn.y, nnn.z));
}

void ShaperElements::gridXY(const glm::vec2 pos, const float step, const int count)
{
  for (int i = 0; i <= count; i++) shaper->line(glm::vec3(pos.x + i * step, 0.0f, 0.0f),
                                                glm::vec3(pos.x + i * step, step * count, 0.0f));
  for (int i = 0; i <= count; i++) shaper->line(glm::vec3(0.0f, pos.y + i * step, 0.0f),
                                                glm::vec3(step * count, pos.x + i * step, 0.0f));
}

Shaper* ShaperElements::shaper = NULL;

/**
 * \brief Unit cube vertices for drawElements(GL_LINES,...)
 */
glm::vec3 ShaperElements::unitCube[24] = {
  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f),
  glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),
  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
  glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f),
  glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
  glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
  glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f),
  glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
  glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f),
  glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f),
  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
};
