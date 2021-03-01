#pragma once
//----------------
/**
 * \file GameObject.h
 * \author Daniel Gruncl
 * \brief scene objects
 * intialize new scene object using mesh, shader program, GL texture and drawing callback
 * objects can be drawn, transformed and be parent of another object (parentses matrix is passed to self->draw())
 */
//---------------
#pragma once
#include "Component.h"
#include "World2.h"
#include "pgr.h"
#include <vector>

class Component;

/// Scene object
/**
  Contains transformation matricies, optionally geometry, shader program and texture
  Object's scale, position and rotation are stored in separate matricies.
  Changing them directly will not change transform matrix, which is the one, that is passed to shaders.
  Transfromation functions of object will update transform matrix.
  All transformations are additive.
*/
class GameObject
{
private:

  GLuint vbo_positions; ///< vertices buffer
  GLuint vbo_indices;   ///< triangle indicies buffer
  GLuint num_vertices;  ///< number of vertices of mesh of this object
  GLuint num_triangles; ///< number of triangles of mesh of this object
  GLuint num_attribs;   ///< number of attributes per vertex

  GLuint vao = 0;  ///< GL vertex array object
  // GLuint buffer;

public:
  struct Shader2*shader;                ///< shader program used for rendering of this object
  int primitive = GL_TRIANGLES; ///< GL_TRIANGLES, GL_LINES, etc...

  GLuint texture; ///< GL texture
  glm::vec4 color;///< color,in shader texture*color

  GameObject* parent = NULL;         ///< parent of this object - should not be NULL for other objects than scene root
  std::vector<GameObject*> children; ///< child objects of this object - relation child-parent is designed to be
                                     ///< traceable
                                 ///< in both directions
  std::vector<Component*> components; ///< components with functionalities - have render and update functions, that are called in app loop

  bool isRender;         ///< enable/disable rendering
  glm::mat4x4 transformation; ///< transformation matrix of this object

  /// Unparent this object
  /**
    \param[in] keepTransform keep observable transformation after re-parenting?
  */
  static glm::mat4 inheritedTransform(GameObject* obj);
  void unparent(bool keepTransform);
  void setParent(GameObject* parent, bool keepTransform);
  void rmChild(GameObject* obj, bool keepTransform);
  void addChild(GameObject* obj, bool keepTransform);
  void addComponent(Component* c);
  Component* getComponent(const char* type);

  void (*draw_callback)(GameObject* o) = NULL; ///< if not NULL, this function called before object is drawn (good for animating, etc...)
  /// Constructor.
  /**
    Creates 'blank' object without any geometry.
    This is useful as container for cameras, etc...
  */
  GameObject();
  /// Constructor.
  /**
    Creates scene object.
    \param[in] mesh Mesh data of this object
    \param[in] shader Shader program that is used for rendering this object
    \param[in] param Callback Function that is called on draw(). Can be NULL. \todo PF -chybejici parametr
    \param[in] texture Texture of this object. Set 0 for no texture, object is then rendered solid black
  */
  GameObject(const pgr::MeshData mesh, struct Shader2* shader, GLuint texture);
  /// Translate, scale and rotate object at once
  /**
    Because calling rotate, scale and rotate separately makes init of many objects very tedious.

    \param[in] trans Translation
    \param[in] scale Scale
    \param[in] rotAxis Rotation axis
    \param[in] degrees Angle in degrees
  */
  void transform(glm::vec3 trans, glm::vec3 scale, glm::vec3 rotAxis, float degrees);
  /// Translate object
  /**
    \param[in] translate Translation
  */
  void translate(glm::vec3 translate);
  /// Rotate object
  /**
    \param[in] axis Rotation axis
    \param[in] angleDegrees Angle in degrees
  */
  void rotateCamera(glm::vec3 axis, float angleDegrees);
  /// Rotate object as camera (translate,THEN rotate)
  /**
    \param[in] axis Rotation axis
    \param[in] angleDegrees Angle in degrees
  */
  void rotate(glm::vec3 axis, float angleDegrees);
  /// Scale object
  /**
    \param[in] scale Scale
  */
  void scale(glm::vec3 scale);

  /// Draw object
  /**
    parentTransform is matrix created by multiplied matrices of all superordinate objects,
    multiplication goes from left to right, beggining with top level superordinate
    \param[in] parentTransform
  */
  void draw(glm::mat4 parentTransform);
  // void drawLines(glm::mat4 parentTransform);
};
