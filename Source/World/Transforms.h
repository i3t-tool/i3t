#pragma once
//----------------
/**
 * \file Transforms.h
 * \author Daniel Gruncl
 * \brief Transformation utils.
 */
//---------------
#include "GameObject.h"
#include "pgr.h"
#include "Source/Core/Nodes/GraphManager.h"

glm::vec2 world2screen(glm::vec3 pos);
glm::vec2 vecWorldscreen(glm::vec3 pos,glm::vec3 dir);
glm::vec3 world2viewport(glm::vec3 pos);
glm::vec2 mouse2viewport(glm::vec2 pos);
glm::vec3 mouseray(glm::vec2 pos);
glm::mat4 getOrtho(glm::mat4 transform, int referenceAxis);
glm::mat4 getNormalized(glm::mat4 transform);
glm::mat4 getRotation(glm::mat4 transform, int referenceAxis);
glm::vec3 getScale(glm::mat4 transform);
glm::mat4 getFullTransform(GameObject* obj);
glm::mat4 getNodeTransform(const Ptr<Core::NodeBase>*node,const Ptr<Core::Sequence>*parent);
glm::vec3 planeIntersect(glm::vec3 px,glm::vec3 py,glm::vec3 p0);
void setLen(glm::vec3* vec, float len);
float angle2(float x, float y);
bool dirEqual(glm::vec3 v1,glm::vec3 v2,float bias = 0.00005f);