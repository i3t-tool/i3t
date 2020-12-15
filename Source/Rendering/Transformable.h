//---------------------------------------------------------------------------
/**
 * \file	transformable.h
 *
 * Class for representing transformable object
 * 
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague		  
 */
//---------------------------------------------------------------------------

#ifndef _TRANSFORMABLE_H_
#define _TRANSFORMABLE_H_

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

/**
 * \brief Container for a transformation matrix
 */
class Transformable
{
protected:
  glm::mat4 trans; ///< stored (model) transformation matrix

public:

  /**
   * \brief Virtual class, used in Object and CameraObject for updating the stored transformation using setTransform()
   */
  virtual void update()
  {
  };
  //virtual void draw(Shader &shader, Camera * camera){};

  /**
   * \brief Constructs an identity matrix
   */
  Transformable()
  {
    trans = glm::mat4(1.0f);
  }

  virtual ~Transformable()
  = default;
  //{};
  // matrix
  void setTrans(const glm::mat4 mat)
  {
    trans = mat;
  }

  /**
   * \brief Get the stored transformation
   * \return stored matrix
   */
  virtual glm::mat4 getTrans()
  {
    return trans;
  }

  // translate
  void translate(const glm::vec3 vec)
  {
    //trans = glm::translate(trans, vec);
    trans[3] += glm::vec4(vec, 0.0f);
  }

  void translate(float x, float y, float z)
  {
    //trans = glm::translate(trans, glm::vec3(x, y, z));
    trans[3] += glm::vec4(x, y, z, 0.0f);
  }

  // // set translation
  void setTranslation(const glm::vec3 vec)
  {
    trans[3] = glm::vec4(vec, 1.0f);
  }

  void setTranslation(const float x, const float y, const float z)
  {
    trans[3] = glm::vec4(x, y, z, 1.0f);
  }

  // // get position
  glm::vec3 getPosition()
  {
    return glm::vec3(trans[3]);
  }

  // rotation
  void rotate(const float rads, const glm::vec3 axis)
  {
    trans = glm::rotate(trans, rads, axis);
  }

  // // rotateX
  void rotateX(const float rads)
  {
    trans = glm::rotate(trans, rads, glm::vec3(1.0f, 0.0f, 0.0f));
  }

  // // rotateY
  void rotateY(const float rads)
  {
    trans = glm::rotate(trans, rads, glm::vec3(0.0f, 1.0f, 0.0f));
  }

  // // rotateZ
  void rotateZ(const float rads)
  {
    trans = glm::rotate(trans, rads, glm::vec3(0.0f, 0.0f, 1.0f));
  }

  // scale
  void scale(const float s)
  {
    trans = glm::scale(trans, glm::vec3(s, s, s));
  }

  void scale(const float sx, const float sy, const float sz)
  {
    trans = glm::scale(trans, glm::vec3(sx, sy, sz));
  }

  float getScaleX()
  {
    return glm::length(glm::vec3(trans[0]));
  }

  float getScaleY()
  {
    return glm::length(glm::vec3(trans[1]));
  }

  float getScaleZ()
  {
    return glm::length(glm::vec3(trans[2]));
  }

  // lookAt Z
  void uniformLookAtZ(const glm::vec3 c)
  {
    const glm::vec3 dir = glm::normalize(c - getPosition());
    const glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), dir)); // Y-axis
    const glm::vec3 up = glm::normalize(glm::cross(dir, side));

    const float sx = getScaleX();

    trans[0] = glm::vec4(side * sx, 0.0f);
    trans[1] = glm::vec4(up * sx, 0.0f);
    trans[2] = glm::vec4(dir * sx, 0.0f);
  }

  // lookAt Y
  void uniformLookAtY(const glm::vec3 c)
  {
    const glm::vec3 dir = glm::normalize(c - getPosition());
    const glm::vec3 side = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))); // Y-axis
    const glm::vec3 up = glm::normalize(glm::cross(side, dir));

    //float sx = getScaleX();
    const float sx = 1.0f;
    trans[0] = glm::vec4(side * sx, 0.0f);
    trans[2] = glm::vec4(up * sx, 0.0f);
    trans[1] = glm::vec4(dir * sx, 0.0f);
  }
};

#endif
