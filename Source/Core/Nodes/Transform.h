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
} // namespace Core
