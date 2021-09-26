#pragma once

#include "NodeImpl.h"

#if defined(WIN32)
#undef far
#undef near
#endif

namespace Core
{
FORCE_INLINE bool isTransform(const NodePtr& node)
{
	auto it = std::find_if(g_transforms.begin(), g_transforms.end(),
	                       [&node](const Operation& o) { return o.keyWord == node->getOperation()->keyWord; });
	return it != g_transforms.end();
}

template <typename Node>
FORCE_INLINE bool isRot(Node&& node)
{
	// static_assert(std::is_base_of_v<NodeBase, Node>);

	auto& type = node->getOperation()->keyWord;
	return type == "EulerX" || type == "EulerY" || type == "EulerZ" || type == "AxisAngle";
}

enum class ETransformState
{
	Invalid = 0,
	Valid,
	Unknown
};

class Transformation : public NodeBase
{
	friend class Storage;

	bool m_hasEnabledSynergies = true;

	/// \todo MH Use Node::m_owner.
	Ptr<NodeBase> m_currentSequence = nullptr;
	int m_currentIndex = -1;

public:
	bool isInSequence() { return m_currentSequence != nullptr; }
	Ptr<NodeBase> getCurrentSequence() { return m_currentSequence; }
	int getCurrentIndex() const { return m_currentIndex; }

	virtual ETransformState isValid() const { return ETransformState::Unknown; }
	virtual void lock();
	virtual void unlock();
	bool hasSynergies() const { return m_hasEnabledSynergies; }
	void disableSynergies() { m_hasEnabledSynergies = false; }
	void enableSynergies() { m_hasEnabledSynergies = true; }

protected:
	explicit Transformation(const Operation* transformType) : NodeBase(transformType) {}
	void notifySequence();

public:
	/// \todo MH these should not be public.
	void nullSequence()
	{
		m_currentSequence = nullptr;
		m_currentIndex = -1;
	}

	void setSequence(Ptr<NodeBase>&& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex = index;
	}

	void setSequence(Ptr<NodeBase>& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex = index;
	}
};
using TransformPtr = Ptr<Transformation>;


class Free : public Transformation
{
public:
	Free() : Transformation(getTransformProps(ETransformType::Free))
	{
		m_currentMap = &Transform::g_Free;
		m_initialMap = &Transform::g_Free;
	}

	ETransformState isValid() const override { return ETransformState::Valid; }

	[[nodiscard]] ValueSetResult setValue(const glm::mat4& mat) override
	{
		setInternalValue(mat);
		notifySequence();
		return ValueSetResult{};
	}

	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override
	{
		setInternalValue(val, coords);
		notifySequence();
		return ValueSetResult{};
	}

	void reset() override { setInternalValue(glm::mat4(1.0f)); };
};


class Scale : public Transformation
{
	glm::vec3 m_initialScale;

public:
	explicit Scale(glm::vec3 initialScale = glm::vec3(1.0f), const Transform::DataMap& map = Transform::g_Scale)
			: Transformation(getTransformProps(ETransformType::Scale)), m_initialScale(initialScale)
	{
		m_initialMap = &map;
		m_currentMap = &map;
		enableSynergies();
	}

	ETransformState isValid() const override;
	void lock() override;

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4& mat) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;

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
	float m_currentRot;

public:
	explicit EulerRotX(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerX)
			: Transformation(getTransformProps(ETransformType::EulerX)), m_initialRot(initialRot), m_currentRot(initialRot)
	{
		m_initialMap = &map;
		m_currentMap = &map;
		enableSynergies();
	}

	ETransformState isValid() const override;
	void lock() override;

	[[nodiscard]] float getRot() const { return m_currentRot; }
	float getAngle() const { return m_currentRot; }

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
	float m_currentRot;

public:
	explicit EulerRotY(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerY)
			: Transformation(getTransformProps(ETransformType::EulerY)), m_initialRot(initialRot), m_currentRot(initialRot)
	{
		m_initialMap = &map;
		m_currentMap = &map;
		enableSynergies();
	}

	ETransformState isValid() const override;
	void lock() override;

	[[nodiscard]] float getRot() const { return m_currentRot; }
	float getAngle() const { return m_currentRot; }

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
	float m_currentRot;

public:
	explicit EulerRotZ(float initialRot = 0.0f, const Transform::DataMap& map = Transform::g_EulerZ)
			: Transformation(getTransformProps(ETransformType::EulerZ)), m_initialRot(initialRot)
	{
		m_initialMap = &map;
		m_currentMap = &map;
		enableSynergies();
	}

	ETransformState isValid() const override;
	void lock() override;

	float getAngle() const { return m_initialRot; }
	[[nodiscard]] float getRot() const { return m_initialRot; }

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
		m_initialMap = &map;
		m_currentMap = &map;
	}

	ETransformState isValid() const override;
	void lock() override;

	float getX();
	float getY();
	float getZ();

	ValueSetResult setX(float v);
	ValueSetResult setY(float v);
	ValueSetResult setZ(float v);

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::mat4&) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};

//===-- Other transformations ---------------------------------------------===//
class AxisAngleRot : public Transformation
{
	float m_initialRads;
	glm::vec3 m_initialAxis;

public:
	explicit AxisAngleRot(float rads = glm::radians(70.0f), const glm::vec3& axis = {1.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::AxisAngle)), m_initialRads(rads), m_initialAxis(axis)
	{
		m_initialMap = &Transform::g_AllLocked;
		m_currentMap = m_initialMap;
	}

	ETransformState isValid() const override;

	float getRot() const { return m_initialRads; };
	const glm::vec3& getAxis() const { return m_initialAxis; };

	ValueSetResult setRot(float rads);
	ValueSetResult setAxis(const glm::vec3& axis);

	ValueSetResult setValue(float rads) override;
	ValueSetResult setValue(const glm::vec3& axis) override;

	void reset() override;
};

class QuatRot : public Transformation
{
	glm::quat m_initialQuat;
	glm::quat m_normalized;

public:
	explicit QuatRot(const glm::quat& q = {1.0f, 0.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::Quat))
	{
		m_initialQuat = q;
		m_normalized = glm::normalize(q);
	}

	ETransformState isValid() const override;

	const glm::quat& getQuat() const { return m_initialQuat; };
	const glm::quat& getNormalized() const;

	ValueSetResult setValue(const glm::quat& vec) override;
	ValueSetResult setValue(const glm::vec4& vec) override;

	void reset() override;
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
	explicit OrthoProj(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	          float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Ortho)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
		m_initialMap = &Transform::g_Ortho;
		m_currentMap = m_initialMap;
	}

	ETransformState isValid() const override;
	void lock() override;

	float getLeft() const { return m_left; }
	float getRight() const { return m_right; }
	float getBottom() const { return m_bottom; }
	float getTop() const { return m_top; }
	float getNear() const { return m_near; }
	float getFar() const { return m_far; }

	ValueSetResult setLeft(float val);
	ValueSetResult setRight(float val);
	ValueSetResult setBottom(float val);
	ValueSetResult setTop(float val);
	ValueSetResult setNear(float val);
	ValueSetResult setFar(float val);

	/// No synergies required.
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};

class PerspectiveProj : public Transformation
{
	float m_initialFOW;
	float m_initialAspect;
	float m_initialZNear;
	float m_initialZFar;

public:
	explicit PerspectiveProj(float fow = glm::radians(70.0f), float aspect = 1.333f, float zNear = 1.0f, float zFar = 10.0f)
			: Transformation(getTransformProps(ETransformType::Perspective)), m_initialFOW(fow), m_initialAspect(aspect),
				m_initialZNear(zNear), m_initialZFar(zFar)
	{
		m_initialMap = &Transform::g_Perspective;
		m_currentMap = &Transform::g_Perspective;
		m_currentMap = m_initialMap;
	}

	ETransformState isValid() const override;
	void lock() override;

	float getFOW() const { return m_initialFOW; }
	float getAspect() const { return m_initialAspect; }
	float getZNear() const { return m_initialZNear; }
	float getZFar() const { return m_initialZFar; }

	ValueSetResult setFOW(float v);
	ValueSetResult setAspect(float v);
	ValueSetResult setZNear(float v);
	ValueSetResult setZFar(float v);

	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
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
	explicit Frustum(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	        float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Frustum)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
		m_initialMap = &Transform::g_Frustum;
		m_currentMap = m_initialMap;
	}

	ETransformState isValid() const override;
	void lock() override;

	float getLeft() const { return m_left; }
	float getRight() const { return m_right; }
	float getBottom() const { return m_bottom; }
	float getTop() const { return m_top; }
	float getNear() const { return m_near; }
	float getFar() const { return m_far; }

	ValueSetResult setLeft(float val);
	ValueSetResult setRight(float val);
	ValueSetResult setBottom(float val);
	ValueSetResult setTop(float val);
	ValueSetResult setNear(float val);
	ValueSetResult setFar(float val);

	void reset() override;

	ValueSetResult setValue(float val, glm::ivec2 coords) override;
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
	explicit LookAt(const glm::vec3& eye = {0.0f, 0.0f, 10.0f}, const glm::vec3 center = {0.0f, 0.0f, 0.0f},
	       const glm::vec3& up = {0.0f, 1.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::LookAt)), m_initialEye(eye), m_initialCenter(center),
				m_initialUp(up)
	{
		m_initialMap = &Transform::g_AllLocked;
		m_currentMap = m_initialMap;
	}

	ETransformState isValid() const override;

	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	const glm::vec3& getEye() { return m_initialEye; }
	const glm::vec3& getCenter() { return m_initialCenter; }
	const glm::vec3& getUp() { return m_initialUp; }

	ValueSetResult setEye(const glm::vec3& eye);
	ValueSetResult setCenter(const glm::vec3& center);
	ValueSetResult setUp(const glm::vec3& up);
};
}
