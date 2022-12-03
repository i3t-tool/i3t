#pragma once
//----------------
/**
 * \file Transforms.h
 * \author Daniel Gruncl
 * \brief Transformation utils.
 * \date 2.3.2021
 */
//---------------
#include "pgr.h"

#include "Core/Nodes/GraphManager.h"

#include "GameObject.h"

glm::vec2 world2screen(glm::vec3 pos); ///< Point from world coordinates to screen
                                       ///< coordinates (dimensions in pixels)
glm::vec2 vecWorldscreen(glm::vec3 pos,
                         glm::vec3 dir); ///, Vector from world coordinates to screen
                                         /// coordinates (dimensions in pixels)
glm::vec3 world2viewport(glm::vec3 pos); ///< Point from world coordinates to
                                         ///< viewport coordinates (<-1;1>^3)
glm::vec2 mouse2viewport(glm::vec2 pos); ///< Mouse cursor from screen coordinates to
                                         ///< viewport coordinates (<-1;1>^2)
glm::vec3 mouseray(glm::vec2 pos);       ///< Mouse cursor from screen coordinates to ray in
                                         ///< world coordinates (z1=0.7, z2=0.9)
glm::mat4 getOrtho(glm::mat4 transform,
                   int referenceAxis);        ///< sets first three vectros to be orthonormal
                                              ///< taken as vec3. Reference axis is kept.
glm::mat4 getNormalized(glm::mat4 transform); ///< normalize first three vectros taken as vec3.
glm::mat4 getRotation(glm::mat4 transform,
                      int referenceAxis);    ///< returns rotation part of matrix
glm::vec3 getScale(glm::mat4 transform);     ///< returns length of first three
                                             ///< columns taken as vec3
glm::mat4 getFullTransform(GameObject* obj); ///< accumulate all model transformations from the object
                                             ///< parents (up to parent == NULL)
glm::mat4 getNodeTransform(const Ptr<Core::NodeBase>* node, const Ptr<Core::Sequence>* parent,
                           bool invLookAt = false); ///< get the PVMmatrix from core
glm::vec3 planeIntersect(glm::vec3 px, glm::vec3 py,
                         glm::vec3 p0); ///< returns intersection point of nouse ray with given plane
glm::mat4 getProjParams(glm::mat4 projinv,
                        bool isPersp); ///< get the parameters defining a projection
                                       ///< transformation (near,far, ...)
void setLen(glm::vec3* vec,
            float len); ///< set the length of the given vector vec
float angle2(float x,
             float y); ///< Slope angle of the line [0,0]-[x,y]. Oriented angle
                       ///< from the x-axis to this segment.
bool dirEqual(glm::vec3 v1, glm::vec3 v2, float bias = 0.00005f);