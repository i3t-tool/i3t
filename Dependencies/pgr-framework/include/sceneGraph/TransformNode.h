#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

#include "SceneNode.h"

namespace pgr {
namespace sg {

/// TransformNode represents transformation of scene graph subtree.
class TransformNode : public SceneNode {
public:
  TransformNode(const std::string & name = "<TransformNode>", SceneNode* parent = NULL);
  ~TransformNode() {}

  /// resets local matrix to identity
  void setIdentity();
  /// translate this node by vector tr
  void translate(const glm::vec3 &tr);
  /// rotate this node by angle a along vector vec
  void rotate(float angle, const glm::vec3 &axis);
  /// scale this and children nodes
  void scale(const glm::vec3 &scale);
};

} // end namespace sg
} // end namespace pgr

#endif // of __TRANSFORMNODE_H
