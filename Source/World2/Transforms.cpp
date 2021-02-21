#include "Transforms.h"
#include "World2.h"
#include "glm/gtx/norm.hpp"

glm::vec2 world2screen(glm::vec3 pos)
{
  float viewport[4];
  glGetFloatv(GL_VIEWPORT, viewport);

  glm::vec4 pos1 = World2::perspective * World2::mainCamera * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
  pos1 /= pos1[3];
  glm::vec2 spos1 = glm::vec2((pos1[0] + 1.0f) * 0.5f * viewport[2] + viewport[0],
                              (pos1[1] + 1.0f) * 0.5f * viewport[3] + viewport[1]);
  return spos1;
}
glm::vec3 world2viewport(glm::vec3 pos)
{
  glm::vec4 pos1 = World2::perspective * World2::mainCamera * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
  pos1 /= pos1[3];
  return (glm::vec3)pos1;
}

glm::vec2 mouse2viewport(glm::vec2 pos)
{
  float viewport[4];
  glGetFloatv(GL_VIEWPORT, viewport);
  return glm::vec2((pos[0] - viewport[0]) / (viewport[2] * 0.5f) - 1.0f,
                   (pos[1] - viewport[1]) / (viewport[3] * 0.5f) - 1.0f);
}

glm::vec3 mouseray(glm::vec2 pos)
{
  float viewport[4];
  glGetFloatv(GL_VIEWPORT, viewport);
  glm::mat4 inv = glm::inverse(World2::perspective * World2::mainCamera);
  glm::vec4 mouse = glm::vec4((pos[0] - viewport[0]) / (viewport[2] * 0.5f) - 1.0f,
                              (pos[1] - viewport[1]) / (viewport[3] * 0.5f) - 1.0f, -0.7f, 1.0f);

  glm::vec4 world1 = inv * mouse;
  mouse[2] = 0.9f;
  glm::vec4 world2 = inv * mouse;

  world1 /= world1[3];
  world2 /= world2[3];

  return -glm::normalize((glm::vec3)(world2 - world1));
}
glm::mat4 getOrtho(glm::mat4 transform, int referenceAxis)
{
  float bias = 0.005f * 0.005f;
  int mapaxis[3] = {referenceAxis, (referenceAxis + 1) % 3, (referenceAxis + 2) % 3};
  glm::vec3 axes[3] = {(glm::vec3)transform[mapaxis[0]], (glm::vec3)transform[mapaxis[1]],
                       (glm::vec3)transform[mapaxis[2]]};
  if (glm::length2(axes[0]) < bias)
  {
    axes[0] = glm::vec3(0.0f);
    axes[0][mapaxis[0]] = 1.0f;
  }
  if (glm::length2(axes[0]) < bias)
  {
    axes[1] = glm::vec3(0.0f);
    axes[1][mapaxis[1]] = 1.0f;
  }
  if (glm::length2(axes[0]) < bias)
  {
    axes[2] = glm::vec3(0.0f);
    axes[2][mapaxis[2]] = 1.0f;
  }

  axes[0] = glm::normalize(axes[0]);
  axes[0] *= glm::length((glm::vec3)transform[mapaxis[0]]);
  axes[1] = glm::normalize(glm::cross(axes[2], axes[0]));
  axes[1] *= glm::length((glm::vec3)transform[mapaxis[1]]);
  axes[2] = glm::normalize(glm::cross(axes[0], axes[1]));
  axes[1] *= glm::length((glm::vec3)transform[mapaxis[2]]);

  glm::mat4 rot = glm::mat4(1.0f);
  *((glm::vec3*)(&rot[mapaxis[0]])) = axes[0];
  *((glm::vec3*)(&rot[mapaxis[1]])) = axes[1];
  *((glm::vec3*)(&rot[mapaxis[2]])) = axes[2];
  *((glm::vec3*)(&rot[3])) = transform[3];
  return rot;
}
glm::mat4 getNormalized(glm::mat4 transform)
{
  float bias = 0.005f;
  glm::vec3 lens = glm::vec3(glm::length((glm::vec3)transform[0]), glm::length((glm::vec3)transform[1]),
                             glm::length((glm::vec3)transform[2]));
  if (lens[0] < bias)
  {
    transform[0] = glm::vec4(0.0f);
    transform[0][0] = 1.0f;
    lens[0] = 1.0f;
  }
  if (lens[1] < bias)
  {
    transform[1] = glm::vec4(0.0f);
    transform[1][1] = 1.0f;
    lens[1] = 1.0f;
  }
  if (lens[2] < bias)
  {
    transform[2] = glm::vec4(0.0f);
    transform[2][2] = 1.0f;
    lens[2] = 1.0f;
  }
  transform[0] /= lens[0];
  transform[1] /= lens[1];
  transform[2] /= lens[2];
  return transform;
}

glm::mat4 getRotation(glm::mat4 transform, int referenceAxis)
{
  float bias = 0.005f * 0.005f;
  int mapaxis[3] = {referenceAxis, (referenceAxis + 1) % 3, (referenceAxis + 2) % 3};
  glm::vec3 axes[3] = {(glm::vec3)transform[mapaxis[0]], (glm::vec3)transform[mapaxis[1]],
                       (glm::vec3)transform[mapaxis[2]]};
  if (glm::length2(axes[0]) < bias)
  {
    axes[0] = glm::vec3(0.0f);
    axes[0][mapaxis[0]] = 1.0f;
  }
  if (glm::length2(axes[0]) < bias)
  {
    axes[1] = glm::vec3(0.0f);
    axes[1][mapaxis[1]] = 1.0f;
  }
  if (glm::length2(axes[0]) < bias)
  {
    axes[2] = glm::vec3(0.0f);
    axes[2][mapaxis[2]] = 1.0f;
  }

  axes[0] = glm::normalize(axes[0]);
  axes[1] = glm::normalize(glm::cross(axes[2], axes[0]));
  axes[2] = glm::normalize(glm::cross(axes[0], axes[1]));

  glm::mat4 rot = glm::mat4(1.0f);
  *((glm::vec3*)(&rot[mapaxis[0]])) = axes[0];
  *((glm::vec3*)(&rot[mapaxis[1]])) = axes[1];
  *((glm::vec3*)(&rot[mapaxis[2]])) = axes[2];

  return rot;
}
glm::vec3 getScale(glm::mat4 transform)
{
  return glm::vec3(glm::length((glm::vec3)transform[0]), glm::length((glm::vec3)transform[1]),
                   glm::length((glm::vec3)transform[2]));
}
glm::mat4 getFullTransform(GameObject* obj)
{
  glm::mat4 transform = glm::mat4(1.0f);
  while (obj != NULL)
  {
    transform = obj->transform * transform;
    obj = obj->parent;
  }
  return transform;
}
float angle2(float x, float y)
{
  float a = glm::degrees(atan(y / x));
  if (a < 0.0f)
  {
    a = 180.0f + a;
  }
  if (y < 0.0f)
  {
    a += 180.0f;
  }
  return a;
}