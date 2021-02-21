#pragma once
#include "GameObject.h"
#include "pgr.h"


glm::vec2 world2screen(glm::vec3 pos);
glm::vec3 world2viewport(glm::vec3 pos);
glm::vec2 mouse2viewport(glm::vec2 pos);
glm::vec3 mouseray(glm::vec2 pos);
glm::mat4 getOrtho(glm::mat4 transform, int referenceAxis);
glm::mat4 getNormalized(glm::mat4 transform);
glm::mat4 getRotation(glm::mat4 transform, int referenceAxis);
glm::vec3 getScale(glm::mat4 transform);
glm::mat4 getFullTransform(GameObject* obj);
float angle2(float x, float y);