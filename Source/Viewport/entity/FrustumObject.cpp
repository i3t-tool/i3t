#include "FrustumObject.h"

#include "Viewport/shader/FrustumShader.h"

using namespace Vp;

FrustumObject::FrustumObject(Core::Mesh* mesh, FrustumShader* shader) : ColoredObject(mesh, shader)
{
	m_backFaceCull = true;
	setDisplayType(DisplayType::Frustum);
}

void FrustumObject::update(Scene& scene)
{
	ColoredObject::update(scene);
	this->m_modelMatrix = glm::inverse(m_frustumViewMatrix); // TODO: Test if needed
}

void FrustumObject::render(glm::mat4 view, glm::mat4 projection)
{
	FrustumShader* frustumShader = static_cast<FrustumShader*>(m_shader);
	frustumShader->m_frustumProjectionMatrix = m_frustumProjectionMatrix;
	frustumShader->m_frustumViewMatrix = m_frustumViewMatrix;
	ColoredObject::render(view, projection);
}
