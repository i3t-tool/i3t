#include "GUIProjection.h"

void GUIProjection::setProjection(const glm::vec2 _pos, const glm::vec2 _size, const float _scale)
{
  pos = _pos;
  size = _size;
  scale = _scale;

  PV = glm::mat3();

  /*
  PV[0][0] = 2.0f * _scale / (size.x);
  PV[1][1] = -2.0f * _scale / (size.y);
  PV[2][0] = -(1.0f + _pos.x / _size.x * _scale * 2.0f);
  PV[2][1] = 1.0f + _pos.y / _size.y * _scale * 2.0f;
  */

  PV[0][0] = 2.0f / size.x;
  PV[1][1] = -2.0f / size.y;
  PV[2][0] = -1.0f;
  PV[2][1] = 1.0f;

  PV = PV * glm::inverse(glm::mat3(_scale, 0.0f, 0.0f,
                                   0.0f, _scale, 0.0f,
                                   _pos.x, _pos.y, 1.0f));


  //PV[2][0] = _pos.x;
  //PV[2][1] = _pos.y;

  //PV[3][0] = pos.x;
  //PV[3][1] = pos.y;
}

// statics
glm::vec2 GUIProjection::screenSize = glm::vec2();
float GUIProjection::screenScale = 1.0f;

glm::mat3 GUIProjection::PV = glm::mat3();
glm::vec2 GUIProjection::pos = glm::vec2();
glm::vec2 GUIProjection::size = glm::vec2();
float GUIProjection::scale = 1.0f;

bool GUIProjection::viewPort = false;
