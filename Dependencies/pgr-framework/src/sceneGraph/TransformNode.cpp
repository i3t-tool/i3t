
#include "sceneGraph/TransformNode.h"

#include "pgr.h"   // includes all PGR libraries, like shader, glm, assimp ...

namespace pgr {
namespace sg {

TransformNode::TransformNode(const std::string &name, SceneNode *parent):
  SceneNode(name, parent) {
}

void TransformNode::setIdentity() {
  m_local_mat = glm::mat4(1.0f);
}

void TransformNode::translate(const glm::vec3 &tr) {
  m_local_mat = glm::translate( m_local_mat, tr);
}

void TransformNode::rotate(float angle, const glm::vec3 &axis) {
  m_local_mat = glm::rotate( m_local_mat, angle, axis);
}

void TransformNode::scale(const glm::vec3 &scale) {
  m_local_mat = glm::scale( m_local_mat, scale);
}

} // end namespace sg
} // end namespace pgr

