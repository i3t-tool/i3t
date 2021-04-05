#pragma once

#include "NodeImpl.h"

#if defined(WIN32)
#undef far
#undef near
#endif

namespace Core
{
FORCE_INLINE bool isTransform(NodePtr& node)
{
	auto it = std::find_if(g_transforms.begin(), g_transforms.end(), [&node] (const Operation& o) {
		return o.keyWord == node->getOperation()->keyWord;
	});
	return it != g_transforms.end();
}


class Transformation : public NodeBase
{
	friend class Sequence;

protected:
	Transformation(const Operation* transformType) : NodeBase(transformType) {}
	Ptr<NodeBase> m_currentSequence = nullptr;
	void notifySequence();
};


class Free : public Transformation
{
public:
	Free() : Transformation(getTransformProps(ETransformType::Free))
	{
		m_currentMap = Transform::g_Free;
		m_initialMap = Transform::g_Free;
	}

	[[nodiscard]] ValueSetResult setValue(const glm::mat4& mat) override
	{
		setInternalValue(mat);
		return ValueSetResult{};
	}

	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override
	{
		setInternalValue(val, coords);
		return ValueSetResult{};
	}

	void reset() override { setInternalValue(glm::mat4(1.0f)); };
	void setDataMap(const Transform::DataMap& map) override{};
};


class Scale : public Transformation
{
	glm::vec3 m_initialScale;

public:
	explicit Scale(glm::vec3 initialScale = glm::vec3(1.0f), const Transform::DataMap& map = Transform::g_UniformScale)
			: Transformation(getTransformProps(ETransformType::Scale)), m_initialScale(initialScale)
	{
		m_initialMap = map;
		m_currentMap = map;
	}
	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4& mat) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
	void setDataMap(const Transform::DataMap& map) override;

  float getX();
  float getY();
  float getZ();

  ValueSetResult setX(float v);
  ValueSetResult setY(float v);
  ValueSetResult setZ(float v);
};


/**
 * \code
 *   1      0       0       0
 *   0    cos(T)  -sin(T)   0
 *   0    sin(T)   cos(T)   0
 *   0      0       0       1
 * \endcode
 */
class EulerRotX : public Transformation
{
	float m_initialRot;

public:
	explicit EulerRotX(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerX)
			: Transformation(getTransformProps(ETransformType::EulerX)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
};


/**
 * \code
 *  cos(T)   0   sin(T)   0
 *    0      1     0      0
 * -sin(T)   0   cos(T)   0
 *    0      0     0      1
 * \endcode
 */
class EulerRotY : public Transformation
{
	float m_initialRot;

public:
	explicit EulerRotY(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerY)
			: Transformation(getTransformProps(ETransformType::EulerY)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
};


/**
 * \code
 *   cos(T)  -sin(T)   0    0
 *   sin(T)   cos(T)   0    0
 *     0        0      1    0
 *     0        0      0    1
 * \endcode
 */
class EulerRotZ : public Transformation
{
	float m_initialRot;

public:
	explicit EulerRotZ(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerZ)
			: Transformation(getTransformProps(ETransformType::EulerZ)), m_initialRot(initialRot)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;
};


class Translation : public Transformation
{
	glm::vec3 m_initialTrans;

public:
	explicit Translation(glm::vec3 initialTrans = glm::vec3(0.0f),
	                     const Transform::DataMap& map = Transform::g_Translate)
			: Transformation(getTransformProps(ETransformType::Translation)), m_initialTrans(initialTrans)
	{
		m_initialMap = map;
		m_currentMap = map;
	}

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
	void reset() override;

	float getX();
	float getY();
	float getZ();

	ValueSetResult setX(float v);
  ValueSetResult setY(float v);
  ValueSetResult setZ(float v);
};


//===-- Other transformations ---------------------------------------------===//
class AxisAngleRot : public Transformation
{
	float m_initialRads;
	glm::vec3 m_initialAxis;

public:
	AxisAngleRot(float rads = glm::radians(70.0f), const glm::vec3& axis = {1.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::AxisAngle)), m_initialRads(rads), m_initialAxis(axis)
	{
		m_initialMap = Transform::g_AllLocked;
		m_currentMap = m_initialMap;
	}

	void reset() override;
};


class QuatRot : public Transformation
{
	glm::quat m_initialQuat;

public:
	QuatRot(const glm::quat& q = {1.0f, 0.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::Quat)), m_initialQuat(q)
	{
	}
};


class OrthoProj : public Transformation
{
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;

public:
	OrthoProj(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	          float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Ortho)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
		m_initialMap = Transform::g_Ortho;
		m_currentMap = m_initialMap;
	}

	/// No synergies required.
	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

  float getLeft() { return m_left; }
  float getRight() { return m_right; }
  float getBottom() { return m_bottom; }
  float getTop() { return m_top; }
  float getNear() { return m_near; }
  float getFar() { return m_far; }

  ValueSetResult setLeft(float val);
  ValueSetResult setRight(float val);
  ValueSetResult setBottom(float val);
  ValueSetResult setTop(float val);
  ValueSetResult setNear(float val);
  ValueSetResult setFar(float val);
};


class PerspectiveProj : public Transformation
{
	float m_initialFOW;
	float m_initialAspect;
	float m_initialZNear;
	float m_initialZFar;

public:
	PerspectiveProj(float fow = glm::radians(70.0f), float aspect = 1.333f, float zNear = 1.0f, float zFar = 10.0f)
			: Transformation(getTransformProps(ETransformType::Perspective)), m_initialFOW(fow), m_initialAspect(aspect),
				m_initialZNear(zNear), m_initialZFar(zFar)
	{
		m_initialMap = Transform::g_Perspective;
		m_currentMap = m_initialMap;
	}

	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	float getFOW() { return m_initialFOW; }
	float getAspect() { return m_initialAspect; }
	float getZNear() { return m_initialZNear; }
	float getZFar() { return m_initialZFar; }

	ValueSetResult setFOW(float v);
	ValueSetResult setAspect(float v);
	ValueSetResult setZNear(float v);
	ValueSetResult setZFar(float v);
};


class Frustum : public Transformation
{
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;

public:
	Frustum(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	        float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Frustum)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
		m_initialMap = Transform::g_Frustum;
		m_currentMap = m_initialMap;
	}

	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	float getLeft() { return m_left; }
	float getRight() { return m_right; }
	float getBottom() { return m_bottom; }
	float getTop() { return m_top; }
	float getNear() { return m_near; }
	float getFar() { return m_far; }

	ValueSetResult setLeft(float val);
	ValueSetResult setRight(float val);
	ValueSetResult setBottom(float val);
	ValueSetResult setTop(float val);
	ValueSetResult setNear(float val);
	ValueSetResult setFar(float val);
};


/**
 * Same as perspective projection node, but all values are locked.
 */
class LookAt : public Transformation
{
	glm::vec3 m_initialEye;
	glm::vec3 m_initialCenter;
	glm::vec3 m_initialUp;

public:
	LookAt(const glm::vec3& eye = {0.0f, 0.0f, 10.0f}, const glm::vec3 center = {0.0f, 0.0f, 0.0f},
	       const glm::vec3& up = {0.0f, 1.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::LookAt)), m_initialEye(eye), m_initialCenter(center),
				m_initialUp(up)
	{
		m_initialMap = Transform::g_AllLocked;
		m_currentMap = m_initialMap;
	}

	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	const glm::vec3& getEye() { return m_initialEye; }
	const glm::vec3& getCenter() { return m_initialCenter; }
	const glm::vec3& getUp() { return m_initialUp; }

	ValueSetResult setEye(const glm::vec3& eye);
	ValueSetResult setCenter(const glm::vec3& center);
	ValueSetResult setUp(const glm::vec3& up);
};
} // namespace Core
