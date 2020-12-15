
#include "sceneGraph/ShaderProgram.h"

namespace pgr {
namespace sg {

BasicShaderProgram::BasicShaderProgram(GLuint prId):
  m_programId(prId),
  m_PVMmatrix(-1),
  m_Vmatrix(-1),
  m_Mmatrix(-1),
  m_NormalMatrix(-1),
  m_time(-1) {
}

BasicShaderProgram::~BasicShaderProgram() {
  pgr::deleteProgramAndShaders(m_programId);
}

void BasicShaderProgram::initLocations() {
  m_PVMmatrix    =  glGetUniformLocation(m_programId, "PVMmatrix");
  m_Vmatrix      =  glGetUniformLocation(m_programId, "Vmatrix");
  m_Mmatrix      =  glGetUniformLocation(m_programId, "Mmatrix");
  m_NormalMatrix =  glGetUniformLocation(m_programId, "NormalMatrix");

  m_time         =  glGetUniformLocation(m_programId, "time" );
}

void BasicShaderProgram::updateUniforms(SceneNode *nd) {
}

MeshShaderProgram::MeshShaderProgram(GLuint prId):
  BasicShaderProgram(prId),
  m_diffuse(-1),
  m_ambient(-1),
  m_specular(-1),
  m_shininess(-1),
  m_texSampler(-1),
  m_pos(-1),
  m_normal(-1),
  m_texCoord(-1),
  m_useTexture(-1) {
}

void MeshShaderProgram::initLocations() {
  // if the shader does not have this uniform - return -1
  m_pos          =  glGetAttribLocation( m_programId, "position");
  m_normal       =  glGetAttribLocation( m_programId, "normal");
  m_texCoord     =  glGetAttribLocation( m_programId, "texCoord");

  m_ambient      =  glGetUniformLocation(m_programId, "material.ambient");
  m_diffuse      =  glGetUniformLocation(m_programId, "material.diffuse");
  m_specular     =  glGetUniformLocation(m_programId, "material.specular");
  m_shininess    =  glGetUniformLocation(m_programId, "material.shininess");

  m_texSampler   = glGetUniformLocation(m_programId, "texSampler");
  m_useTexture   = glGetUniformLocation( m_programId, "useTexture");

  BasicShaderProgram::initLocations();
}

void MeshShaderProgram::updateUniforms(SceneNode *nd) {
  BasicShaderProgram::updateUniforms(nd);
}

} // end namespace sg
} // end namespace pgr

