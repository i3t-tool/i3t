#include "FrustumShader.h"
#include "Utils/Color.h"

using namespace Vp;

FrustumShader::FrustumShader(GLuint id) : ColorShader(id) { init(false); }

void FrustumShader::init(bool initSuperclass)
{
	if (initSuperclass)
		ColorShader::init(true);

	inverseProjectionMatrixId = glGetUniformLocation(m_id, "projectionInverseMatrix");
}

void FrustumShader::setUniforms()
{
	ColorShader::setUniforms();

	glm::mat4 projectionInverseMatrix = glm::inverse(m_frustumProjectionMatrix * m_frustumViewMatrix);
	glUniformMatrix4fv(inverseProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(projectionInverseMatrix));
}
