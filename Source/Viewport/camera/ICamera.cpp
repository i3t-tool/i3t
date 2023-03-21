#include "ICamera.h"

#include "glm/ext/matrix_clip_space.hpp"

using namespace Vp;

void ICamera::size(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

glm::mat4 ICamera::createProjectionMatrix(bool nonShrinking) const
{
	if (nonShrinking)
	{
		// Non shrinking resizing
		// Method 1
		float scale = m_zNear * tan(glm::radians(m_fov / 2));
		float l = -scale;
		float r = scale;
		float b = -scale;
		float t = scale;
		float aspect = std::min(m_width, m_height) / (float)std::max(m_width, m_height);
		if (m_width > m_height)
		{
			t *= aspect;
			b *= aspect;
		}
		else
		{
			l *= aspect;
			r *= aspect;
		}
		return glm::frustum(l, r, b, t, m_zNear, m_zFar);
		//		// Method 2
		//		if (width > height)
		//		{
		//			glm::mat4 projection = glm::perspective(glm::radians(fov), height / (float)width, zNear, zFar);
		//			float tmp = projection[0][0];
		//			projection[0][0] = projection[1][1];
		//			projection[1][1] = tmp;
		//			return projection;
		//		}
		//		else
		//		{
		//			return glm::perspective(glm::radians(fov), width / (float)height, zNear, zFar);
		//		}
	}
	else
	{
		return glm::perspective(glm::radians(m_fov), m_width / (float)m_height, m_zNear, m_zFar);
	}
}

float ICamera::getZNear() const { return m_zNear; }
void ICamera::setZNear(float zNear) { this->m_zNear = zNear; }
float ICamera::getZFar() const { return m_zFar; }
void ICamera::setZFar(float zFar) { this->m_zFar = zFar; }
float ICamera::getFov() const { return m_fov; }
void ICamera::setFov(float fov) { this->m_fov = fov; }

const glm::mat4& ICamera::getView() const { return m_view; }
const glm::mat4& ICamera::getProjection() const { return m_projection; }
int ICamera::getWidth() const { return m_width; }
int ICamera::getHeight() const { return m_height; }
const glm::vec3& ICamera::getPosition() const { return m_position; }
const glm::vec3& ICamera::getDirection() const { return m_direction; }
const glm::vec3& ICamera::getUp() const { return m_up; }
const glm::vec3& ICamera::getRight() const { return m_right; }
