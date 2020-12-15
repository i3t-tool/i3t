
#ifndef __TRACKBALL_H
#define __TRACKBALL_H

#include "pgr.h"

namespace pgr {

/// abstract class for the trackball implementation
class CTrackball {

public:

  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, scaled so they are from (-1.0 ... 1.0).
   */
  CTrackball(float startPointX = 0.0, float startPointY = 0.0, float endPointX = 0.0, float endPointY = 0.0) {}
  virtual ~CTrackball() {}

  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, scaled so they are from (-1.0 ... 1.0).
   */
  virtual void setRotation(float startPointX, float startPointY, float endPointX, float endPointY) = 0;
  virtual void addRotation(float startPointX, float startPointY, float endPointX, float endPointY) = 0;
  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, they are from (0 ... winWidth-1) x (0 ... winHeight-1) range.
   */
  virtual void setRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) = 0;
  virtual void addRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight) = 0;

  /**
   * Builds and returns a rotation matrix.
   */
  virtual void getRotationMatrix(glm::mat4& matrix) {}

protected:

  /**
   * Project an x,y pair onto a sphere of radius radius or a hyperbolic sheet
   * if we are away from the center of the sphere.
   */
  float projectToSphere(float radius, float x, float y);

  /**
   * Maps screen space coordinates into the normalized space, i.e. they are from range (-1.0 ... 1.0) x (-1.0 ... 1.0).
   */
  virtual void mapScreenCoords(float& outX, float& outY, int screenX, int screenY, int winWidth, int winHeight);

  void computeRotationAxisAndAngle(glm::vec3& axis, float& angle, float startPointX, float startPointY, float endPointX, float endPointY);

};

/// trackball implemented using a 4x4 matrix
class CClassicTrackball : public CTrackball {

  glm::mat4 _rotationMatrix;

public:

  CClassicTrackball(float startPointX = 0.0, float startPointY = 0.0, float endPointX = 0.0, float endPointY = 0.0);
  virtual ~CClassicTrackball();

  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, scaled so they are from (-1.0 ... 1.0).
   */
  virtual void setRotation(float startPointX, float startPointY, float endPointX, float endPointY);
  virtual void addRotation(float startPointX, float startPointY, float endPointX, float endPointY);
  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, they are from (0 ... winWidth-1) x (0 ... winHeight-1) range.
   */
  virtual void setRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight);
  virtual void addRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight);

  /**
   * Returns a trackball rotation matrix.
   */
  virtual void getRotationMatrix(glm::mat4& matrix);

private:

  /**
   * Calculates transform that corresponds to the rotation between startPoint and endPoint.
   */
  void computeRotation(glm::mat4& rotation, float startPointX, float startPointY, float endPointX, float endPointY);

};

/// trackball implemented using a quaternion
class CQuaternionTrackball : public CTrackball {

  glm::quat _quaternion;

public:

  CQuaternionTrackball(float startPointX = 0.0, float startPointY = 0.0, float endPointX = 0.0, float endPointY = 0.0);
  virtual ~CQuaternionTrackball();

  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, scaled so they are from (-1.0 ... 1.0).
   *
   * The resulting rotation is returned as a quaternion rotation in the
   * class variable _quaternion.
   */
  virtual void setRotation(float startPointX, float startPointY, float endPointX, float endPointY);
  virtual void addRotation(float startPointX, float startPointY, float endPointX, float endPointY);
  /**
   * Pass the x and y coordinates of the last and current positions of
   * the mouse, they are from (0 ... winWidth-1) x (0 ... winHeight-1) range.
   *
   * The resulting rotation is returned as a quaternion rotation in the
   * class variable _quaternion.
   */
  virtual void setRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight);
  virtual void addRotation(int startPointX, int startPointY, int endPointX, int endPointY, int winWidth, int winHeight);

  /**
   * Builds a rotation matrix based on given quaternion (stored in class variable _quaternion).
   */
  virtual void getRotationMatrix(glm::mat4& matrix);

private:

  /**
   * Calculates quaternion that corresponds to the rotation between startPoint and endPoint.
   */
  void computeQuaternion(glm::quat& quaternion, float startPointX, float startPointY, float endPointX, float endPointY);

};

} // end namespace pgr

#endif /* of __TRACKBALL_H */
