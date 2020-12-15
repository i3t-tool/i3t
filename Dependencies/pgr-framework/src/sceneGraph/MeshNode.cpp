
#include "sceneGraph/MeshNode.h"
#include "sceneGraph/MeshGeometry.h"
#include "sceneGraph/Resources.h"
#include "sceneGraph/ShaderProgram.h"


namespace pgr {
namespace sg {

MeshNode::MeshNode(const std::string &name, SceneNode* parent):
  SceneNode(name, parent), m_program(0), m_vertexArrayObject(0), m_mesh(NULL) {
  glGenVertexArrays(1, &m_vertexArrayObject );
}

MeshNode::~MeshNode() {
  glDeleteVertexArrays( 1, &m_vertexArrayObject );
  if(m_program)
    ShaderManager::Instance()->release("MeshNode-shader");
}

void MeshNode::loadProgram() {
  if(m_program)
    ShaderManager::Instance()->release("MeshNode-shader");

  if(!ShaderManager::Instance()->exists("MeshNode-shader")) {
    GLuint shaderList[] = {
      pgr::createShaderFromFile(GL_VERTEX_SHADER,   pgr::frameworkRoot() + "data/sceneGraph/MeshNode.vert"),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, pgr::frameworkRoot() + "data/sceneGraph/MeshNode.frag"),
      0
    };

    m_program = new MeshShaderProgram(pgr::createProgram(shaderList));
    ShaderManager::Instance()->insert("MeshNode-shader", m_program);
  } else
    m_program = dynamic_cast<MeshShaderProgram*>(ShaderManager::Instance()->get("MeshNode-shader"));

  m_program->initLocations();
}

void MeshNode::setGeometry(MeshGeometry* mesh_p) {
  if(m_program == 0) {
    loadProgram();
  }

  if(mesh_p == NULL)
    return;

  m_mesh = mesh_p;

  glBindVertexArray( m_vertexArrayObject );
  glBindBuffer(GL_ARRAY_BUFFER, mesh_p->getVertexBuffer());
  glEnableVertexAttribArray(m_program->m_pos);
  glVertexAttribPointer(m_program->m_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

  if(m_mesh->hasNormals() == true) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->getNormalBuffer());
    glEnableVertexAttribArray(m_program->m_normal);
    glVertexAttribPointer(m_program->m_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  // todo: up to 4 texture coordinates can be there
  if(m_mesh->hasTexCoords() == true) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->getTexCoordBuffer());
    glEnableVertexAttribArray(m_program->m_texCoord);
    glVertexAttribPointer(m_program->m_texCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);   //(str)
  }

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh_p->getElementBuffer() );

  glBindVertexArray( 0 );
}

void MeshNode::draw(const glm::mat4 & view_matrix, const glm::mat4 & projection_matrix) {
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  // inherited draw - draws all children
  SceneNode::draw(view_matrix, projection_matrix);

  glm::mat4 PVMmatrix = projection_matrix  * view_matrix * globalMatrix();
  glm::mat4 Vmatrix = view_matrix;
  glm::mat4 Mmatrix = globalMatrix();

  glUseProgram(m_program->m_programId);

  glUniformMatrix4fv(m_program->m_PVMmatrix, 1, GL_FALSE, glm::value_ptr(PVMmatrix) );			// model-view-projection
  glUniformMatrix4fv(  m_program->m_Vmatrix, 1, GL_FALSE, glm::value_ptr(  Vmatrix) );			// view
  glUniformMatrix4fv(  m_program->m_Mmatrix, 1, GL_FALSE, glm::value_ptr(  Mmatrix) );			// model
  glm::mat4 NormalMatrix = glm::transpose( glm::inverse( Vmatrix * Mmatrix ));			// should be this way, but inverse returns bad matrix
  glUniformMatrix4fv(m_program->m_NormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix) );    // correct matrix for non-rigid transf

  glUniform1f( m_program->m_time, (float)m_time );        // in seconds

  //glUniform1i(m_texSamplerID, 0);

  glBindVertexArray( m_vertexArrayObject );

  // draw all submeshes = all material groups from SubMeshList
  MeshGeometry::SubMesh* subMesh_p = NULL;

  for(unsigned mat=0; mat<m_mesh->getSubMeshCount(); mat++) {

    subMesh_p = m_mesh->getSubMesh(mat);

    glUniform3fv(m_program->m_diffuse,  1, subMesh_p->diffuse);  // 2nd parameter must be 1 - it declares number of vectors in the vector array
    glUniform3fv(m_program->m_ambient,  1, subMesh_p->ambient);
    glUniform3fv(m_program->m_specular, 1, subMesh_p->specular);
    glUniform1f(m_program->m_shininess,    subMesh_p->shininess);

    if(subMesh_p->textureID != 0 && m_mesh->hasTexCoords() == true) {
      glUniform1i(m_program->m_useTexture, 1);		 // do texture sampling

      glUniform1i(m_program->m_texSampler,   0);  // texturing unit 0 -> samplerID   [for the GPU linker]
      glActiveTexture(GL_TEXTURE0 + 0);  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
      glBindTexture(GL_TEXTURE_2D, subMesh_p->textureID);
    } else {
      glUniform1i(m_program->m_useTexture, 0);		// do not sample the texture
    }

    //glDrawElements( GL_TRIANGLES, subMesh_p->nIndices, GL_UNSIGNED_INT, (void *) (subMesh_p->startIndex * sizeof(unsigned int)));
    // base vertex must be added to the indices for each block (as they are rellative inside the submesh and start from 0)
    // do it in Resources::Load() and use DrawElements, or use glDrawElementsBaseVertex
    glDrawElementsBaseVertex( GL_TRIANGLES, subMesh_p->nIndices, GL_UNSIGNED_INT, (void *) (subMesh_p->startIndex * sizeof(unsigned int)), subMesh_p->baseVertex );
  }

  glBindVertexArray( 0 );
}

} // end namespace sg
} // end namespace pgr

