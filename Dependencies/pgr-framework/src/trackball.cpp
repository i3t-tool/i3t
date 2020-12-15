/*
 * Implementation of a virtual trackball.
 *
 * Done by Tomas Barak in August 2013.
 * Based on the code by Gavin Bell, lots of ideas from Thant Tessman 
 * and
 * the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
 * https://github.com/danping/CoSLAM/blob/master/src/gui/trackball.cpp
 *
 * Much mucking with by:
 * Gavin Bell
 */

#include <math.h>
#include "trackball.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pgr {

// -----------------------------------------------------------------------
// -------------------  CTrackball  --------------------------------------
// -----------------------------------------------------------------------


/*
 * Ok, simulate a trackball. Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */

/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */

#define TRACKBALLSIZE  (0.8f)

/*
 * Project an x,y pair onto a sphere of radius radius or a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
float CTrackball::projectToSphere(float radius, float x, float y) {
  double d, t, z;

  d = sqrt(x*x + y*y);
  if(d < radius * 0.70710678118654752440) {    /* Inside sphere */
    z = sqrt(radius*radius - d*d);
  } else {                                     /* On hyperbola */
    t = radius / 1.41421356237309504880;
    z = t*t / d;
  }

  return (float)z;
}

void CTrackball::computeRotationAxisAndAngle(glm::vec3& axis, float& angle, float startPointX, float startPointY, float endPointX, float endPointY) {

  /* First, figure out z-coordinates for projection of P1 and P2 to deformed sphere */
  glm::vec3 p1(startPointX, startPointY, projectToSphere(TRACKBALLSIZE, startPointX, startPointY));
  glm::vec3 p2(endPointX, endPointY, projectToSphere(TRACKBALLSIZE, endPointX, endPointY));

  /* Now, we want the cross product of P1 and P2 */
  axis = glm::normalize(glm::cross(p1, p2));

  /* Figure out how much to rotate around that axis. */
  glm::vec3 d = p1 - p2;

  double t = glm::length(d) / (2.0 * TRACKBALLSIZE);

  /* Avoid problems with out-of-control values...s */
  if(t > 1.0)
    t = 1.0;
  if(t < -1.0)
    t = -1.0;

	angle = float(2.0f * asin(t)); /* how much to rotate about axis (in radians) */
}

// -----------------------------------------------------------------------
// -------------------  CQuaternionTrackball  ----------------------------
// -----------------------------------------------------------------------
void CQuaternionTrackball::computeQuaternion(glm::quat& quaternion, float startPointX, float startPointY, float endPointX, float endPointY) {

  if((startPointX == endPointX) && (startPointY == endPointY)) {
    /* Zero rotation */
    quaternion = glm::quat(); //glm::angleAxis(0.0f, 1.0f, 0.0f, 0.0f);

    return;
  }

  glm::vec3 axis;
  float angle;

  computeRotationAxisAndAngle(axis, angle, startPointX, startPointY, endPointX, endPointY);

  /*  Given an axis and angle, compute quaternion. */
  quaternion = glm::angleAxis(angle, axis);
}

void CQuaternionTrackball::setRotation(float startPointX, float startPointY, float endPointX, float endPointY) {

  computeQuaternion(_quaternion, startPointX, startPointY, endPointX, endPointY);
}

void CTrackball::mapScreenCoords(float& outX, float& outY, int screenX, int screenY, int winWidth, int winHeight) {

  outX = -1.0f + 2.0f * screenX / winWidth;
  outY =  1.0f - 2.0f * screenY / winHeight;
}

void CQuaternionTrackball::setRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) {
  float endX, endY, startX, startY;

  if((startPointX == endPointX) && (startPointY == endPointY))
    return;

  mapScreenCoords(endX, endY, endPointX, endPointY, winWidth, winHeight);
  mapScreenCoords(startX, startY, startPointX, startPointY, winWidth, winHeight);

  computeQuaternion(_quaternion, startX, startY, endX, endY);
}

void CQuaternionTrackball::addRotation(float startPointX, float startPointY, float endPointX, float endPointY) {

  glm::quat quaternion;

  computeQuaternion(quaternion, startPointX, startPointY, endPointX, endPointY);

  _quaternion = glm::normalize(quaternion * _quaternion);
}

void CQuaternionTrackball::addRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) {
  float endX, endY, startX, startY;

  if((startPointX == endPointX) && (startPointY == endPointY))
    return;

  mapScreenCoords(endX, endY, endPointX, endPointY, winWidth, winHeight);
  mapScreenCoords(startX, startY, startPointX, startPointY, winWidth, winHeight);

  glm::quat quaternion;

  computeQuaternion(quaternion, startX, startY, endX, endY);

  _quaternion = glm::normalize(quaternion * _quaternion);
}

CQuaternionTrackball::CQuaternionTrackball(float startPointX, float startPointY, float endPointX, float endPointY) : CTrackball(startPointX, startPointY, endPointX, endPointY) {

  setRotation(startPointX, startPointY, endPointX, endPointY);
}

CQuaternionTrackball::~CQuaternionTrackball() {
}

/*
 * Build a rotation matrix, given a quaternion rotation.
 */
void CQuaternionTrackball::getRotationMatrix(glm::mat4& matrix) {

  matrix = glm::mat4_cast(_quaternion);
}

// -----------------------------------------------------------------------
// -------------------  CClassicTrackball --------------------------------
// -----------------------------------------------------------------------
void CClassicTrackball::computeRotation(glm::mat4& rotation, float startPointX, float startPointY, float endPointX, float endPointY) {

  if((startPointX == endPointX) && (startPointY == endPointY)) {
    /* Zero rotation */
    rotation = glm::mat4(1.0);

    return;
  }

  glm::vec3 axis;
  float angle;

  computeRotationAxisAndAngle(axis, angle, startPointX, startPointY, endPointX, endPointY);

  rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void CClassicTrackball::setRotation(float startPointX, float startPointY, float endPointX, float endPointY) {

  computeRotation(_rotationMatrix, startPointX, startPointY, endPointX, endPointY);
}

void CClassicTrackball::setRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) {
  float endX, endY, startX, startY;

  if((startPointX == endPointX) && (startPointY == endPointY))
    return;

  mapScreenCoords(endX, endY, endPointX, endPointY, winWidth, winHeight);
  mapScreenCoords(startX, startY, startPointX, startPointY, winWidth, winHeight);

  computeRotation(_rotationMatrix, startX, startY, endX, endY);
}

void CClassicTrackball::addRotation(float startPointX, float startPointY, float endPointX, float endPointY) {

  glm::mat4 newRotation;

  computeRotation(newRotation, startPointX, startPointY, endPointX, endPointY);

  _rotationMatrix = newRotation * _rotationMatrix;
}

void CClassicTrackball::addRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) {
  float endX, endY, startX, startY;

  if((startPointX == endPointX) && (startPointY == endPointY))
    return;

  mapScreenCoords(endX, endY, endPointX, endPointY, winWidth, winHeight);
  mapScreenCoords(startX, startY, startPointX, startPointY, winWidth, winHeight);

  glm::mat4 newRotation;

  computeRotation(newRotation, startX, startY, endX, endY);

  _rotationMatrix = newRotation * _rotationMatrix;
}

CClassicTrackball::CClassicTrackball(float startPointX, float startPointY, float endPointX, float endPointY) : CTrackball(startPointX, startPointY, endPointX, endPointY) {

  setRotation(startPointX, startPointY, endPointX, endPointY);
  //_rotationMatrix = glm::mat4(1.0);
}

CClassicTrackball::~CClassicTrackball() {
}

/*
 * Build a rotation matrix.
 */
void CClassicTrackball::getRotationMatrix(glm::mat4& matrix) {

  matrix = _rotationMatrix;
}

} // end namespace pgr
