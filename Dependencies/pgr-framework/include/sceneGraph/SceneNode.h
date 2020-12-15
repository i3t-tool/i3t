#ifndef __SCENENODE_H
#define __SCENENODE_H

#include <vector>
#include <string>

#include "pgr.h"

/**
 * \namespace pgr::sg
 * \author Tomas Barak, Petr Felkel, Jaroslav Sloup
 * \date 2011-2013
 *
 * \brief Contains all scene graph classes.
 *
 * All nodes are derived from the base class SceneNode.
 * The ResourceManager class takes care of the resource reference counting, as the textures and models are shared between nodes
 */

namespace pgr {
namespace sg {

/** Basic scene graph node
 *
 * You can derive this class and reimplement update() and draw() methods.
 */
class SceneNode {
public:
  typedef std::vector<SceneNode *> Children;

  SceneNode(const std::string & name = "<SceneNode>", SceneNode* parent = NULL);

  /// destroy children
  virtual ~SceneNode();

  /** recalculates global matrix and updates all children
   *
   * Derived classes should also call this method (using SceneNode::update()).
   * If you want to play with transformations, don't forget to set m_loc_mat matrix!
   */
  virtual void update(double elapsed_time);

  /// calls draw on child nodes
  virtual void draw(const glm::mat4 & view_matrix, const glm::mat4 & projection_matrix);

  const SceneNode* parentNode() const { return m_parent; }
  SceneNode* parentNode() { return m_parent; }

  /// re-parent this node
  void setParentNode(SceneNode * new_parent);

  void addChildNode(SceneNode* node);

  /// removes child node (in O(n))
  void removeChildNode(SceneNode* node);

  /// Returns node name.
  const std::string & nodeName() const { return m_name; }

  /// calculated global matrix (valid after update() call)
  const glm::mat4 & globalMatrix() const { return m_global_mat; }

  /// local matrix
  const glm::mat4  & localMatrix() const { return m_local_mat; }

  /// dumps the node + subtree to stdout (you can reimplement this to display additional stuff)
  virtual void dump(unsigned indent = 0);

protected:
  std::string m_name;    ///< node name
  SceneNode*  m_parent;
  Children    m_children;
  double      m_time;  // updated in update()
  glm::mat4   m_global_mat; ///< final global model matrix, calculated in update()
  glm::mat4   m_local_mat;  ///< local model matrix, derived transformation nodes should calculate it
};

} // end namespace sg
} // end namespace pgr

#endif // of __SCENENODE_H
