#ifndef AXESNODE_H
#define AXESNODE_H

#include "SceneNode.h"

namespace pgr {
namespace sg {

/// draws RGB lines in XYZ axes
class AxesNode : public SceneNode {
public:
  AxesNode(const std::string & name = "<AxesNode>", SceneNode * parent = NULL);
  ~AxesNode();

  /// reimplemented draw
  void draw(const glm::mat4  & view_matrix, const glm::mat4  & projection_matrix);

protected:
  /// identifier for the vertex array object
  static GLuint m_vertexArrayObject;
  /// identifier for the buffer object
  static GLuint m_vertexBufferObject;

  /// identifier for the program
  static GLuint m_program;
  /// shader matrix location
  static GLint m_PVMmatrixLoc;
  /// position attribute location
  static GLint m_posLoc;
  /// color attribute location
  static GLint m_colLoc;
};

} // end namespace sg
} // end namespace pgr

#endif
