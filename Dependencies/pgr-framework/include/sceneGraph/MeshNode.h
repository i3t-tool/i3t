#ifndef MESH_HODE_H
#define MESH_HODE_H

#include "pgr.h"
#include "SceneNode.h"

namespace pgr {
namespace sg {

class MeshGeometry;
class MeshShaderProgram;

/// manages rendering of a MeshGeometry
class MeshNode : public SceneNode {
public:
  MeshNode(const std::string & name = "<MeshNode>", SceneNode* parent = NULL);
  ~MeshNode();

  /// associates mesh with this node (also calls loadProgram())
  void setGeometry(MeshGeometry* mesh);

  /// reimplemented draw
  void draw(const glm::mat4 & view_matrix, const glm::mat4 & projection_matrix);

protected:
  /// creates shader
  virtual void loadProgram();

  /// shader program to use during the draw() procedure
  MeshShaderProgram * m_program;
  /// identifier for the vertex array object
  GLuint m_vertexArrayObject;
  /// geometry associated with this MeshObject
  MeshGeometry* m_mesh;
};


} // end namespace sg
} // end namespace pgr


#endif
