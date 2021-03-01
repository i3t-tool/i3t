#pragma once

#include "NodeImpl.h"

namespace Core
{
class Scale : public NodeBase
{
	glm::vec3 m_initialScale;

public:
	explicit Scale(glm::vec3 initialScale = glm::vec3(1.0f), const Transform::DataMap& map = Transform::g_UniformScale)
			: NodeBase(getTransformProps(ETransformType::Scale)), m_initialScale(initialScale)
	{
		m_initialMap = map;
		m_currentMap = map;
	}
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4& mat) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
	void setDataMap(const Transform::DataMap& map) override;
	void updateValues(int inputIndex) override;
};

/**
 * \code
 *   1      0       0       0
 *   0    cos(T)  -sin(T)   0
 *   0    sin(T)   cos(T)   0
 *   0      0       0       1
 * \endcode
 */
class EulerRotX : public NodeBase
{
	float m_initialRot;

public:
	explicit EulerRotX(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerX)
			: NodeBase(getTransformProps(ETransformType::EulerX)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
		getData().setValue(glm::rotate(initialRot, glm::vec3(1.0f, 0.0f, 0.0f)));
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
	void updateValues(int inputIndex) override;
};

class EulerRotY : public NodeBase
{
	float m_initialRot;

public:
	explicit EulerRotY(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerY)
			: NodeBase(getTransformProps(ETransformType::EulerY)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	void reset() override;
	void updateValues(int inputIndex) override;
};

class EulerRotZ : public NodeBase
{
	float m_initialRot;

public:
	explicit EulerRotZ(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerZ)
			: NodeBase(getTransformProps(ETransformType::EulerZ)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	void reset() override;
	void updateValues(int inputIndex) override;
};

class Translation : public NodeBase
{
	glm::vec3 m_initialTrans;

public:
	explicit Translation(glm::vec3 initialTrans = glm::vec3(0.0f),
	                     const Transform::DataMap& map = Transform::g_Translate)
			: NodeBase(getTransformProps(ETransformType::Translation)), m_initialTrans(initialTrans)
	{
		m_initialMap = map;
		m_currentMap = map;
		getData().setValue(glm::translate(initialTrans));
	}

	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
	void updateValues(int inputIndex) override;
};

class AxisAngleRot : public NodeBase
{
	float m_initialRads;
	glm::vec3 m_initialAxis;

public:
	AxisAngleRot(float rads = glm::radians(70.0f), const glm::vec3& axis = {1.0f, 0.0f, 0.0f})
			: NodeBase(getTransformProps(ETransformType::AxisAngle)), m_initialRads(rads), m_initialAxis(axis)
	{
	}

	void updateValues(int inputIndex) override;
};

class QuatRot : public NodeBase
{
	glm::quat m_initialQuat;

public:
	QuatRot(const glm::quat& q = {1.0f, 0.0f, 0.0f, 0.0f})
			: NodeBase(getTransformProps(ETransformType::Quat)), m_initialQuat(q)
	{
	}
};

class OrthoProj : public NodeBase
{

public:
	OrthoProj(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	          float far = 10.0f)
	{
	}

	void updateValues(int inputIndex) override;
};

class PerspectiveProj : public NodeBase
{
	float m_initialFOW;
	float m_initialAspect;
	float m_initialZNear;
	float m_initialZFar;

public:
	PerspectiveProj(float fow = glm::radians(70.0f), float aspect = 1.333f, float zNear = 1.0f, float zFar = 10.0f)
			: NodeBase(getTransformProps(ETransformType::Perspective)), m_initialFOW(fow), m_initialAspect(aspect),
				m_initialZNear(zNear), m_initialZFar(zFar)
	{
	}

	void updateValues(int inputIndex) override;
};

class Frustum : public NodeBase
{
	// \todo MH Frustum initial values.
public:
	Frustum(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	        float far = 10.0f)
			: NodeBase(getTransformProps(ETransformType::Frustum))
	{
	}

	void updateValues(int inputIndex) override;
};

/**
 * Same as perspective projection node, but all values are locked.
 */
class LookAt : public NodeBase
{
	glm::vec3 m_initialEye;
	glm::vec3 m_initialCenter;
	glm::vec3 m_initialUp;

public:
	LookAt(const glm::vec3& eye = {0.0f, 0.0f, 10.0f}, const glm::vec3 center = {0.0f, 0.0f, 0.0f},
	       const glm::vec3& up = {0.0f, 1.0f, 0.0f})
			: NodeBase(getTransformProps(ETransformType::LookAt)), m_initialEye(eye), m_initialCenter(center),
				m_initialUp(up)
	{
	}

	void updateValues(int inputIndex) override;
};
} // namespace Core
