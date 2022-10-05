#include "Core/Resources/Mesh.h"

#include "sceneGraph/Resources.h"
#include "sceneGraph/ShaderProgram.h"

#include "Core/Resources/ResourceManager.h"

namespace Core
{
MeshNode::~MeshNode() {}

void MeshNode::destroy() { glDeleteVertexArrays(1, &m_vertexArrayObject); }

void MeshNode::transform(const glm::mat4& mat)
{
	m_local_mat = mat;
	m_global_mat = mat;
}

void MeshNode::loadProgram()
{
	m_program = dynamic_cast<pgr::sg::MeshShaderProgram*>(
	    ResourceManager::instance().getShader("ModelShader"));
	m_program->initLocations();
}
} // namespace Core
