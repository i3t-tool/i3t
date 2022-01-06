#pragma once

#include "Node.h"

#if defined(WIN32)
#undef far
#undef near
#endif

namespace Core
{
FORCE_INLINE bool isTransform(const NodePtr& node)
{
	/*
	auto it = std::find_if(g_transforms.begin(), g_transforms.end(),
	                       [&node](const std::pair<Operation, std::map<std::string, EValueType>>& pair) { return pair.first.keyWord == node->getOperation()->keyWord; });
	return it != g_transforms.end();
	 */
	auto& keyWord = node->getOperation()->keyWord;
	auto type     = magic_enum::enum_cast<ETransformType>(keyWord);
	return type.has_value();
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

/// b1, b2 - b1 is editable bit, b2 is synergies bit
enum class EValueState
{
	Editable    = 0x0002,  ///< 10
	EditableSyn = 0x0003,  ///< 11
	Locked      = 0x0000,  ///< 00
	LockedSyn   = 0x0001,  ///< 01
};

inline bool canEditValue(EValueState valueState)
{
	return valueState == EValueState::Editable || valueState == EValueState::EditableSyn;
}

class Transformation : public Node
{
	friend class Storage;

	/// \todo MH Use Node::m_owner.
	Ptr<NodeBase> m_currentSequence = nullptr;
	int m_currentIndex = -1;

public:
	bool          isInSequence()          { return m_currentSequence != nullptr; }
	Ptr<NodeBase> getCurrentSequence()    { return m_currentSequence; }
	int           getCurrentIndex() const { return m_currentIndex; }

	EValueState getValueState(glm::ivec2 coords);

	// ValueSetResult setValue(float val, glm::ivec2 coords) override;
	// virtual ValueSetResult onSetValue(float val, glm::ivec2 coords) {}

	virtual ETransformState isValid() const { return ETransformState::Unknown; }
	bool isLocked() const;
	void lock();
	void unlock();
	bool hasSynergies() const { return m_hasEnabledSynergies; }
	void disableSynergies()   { m_hasEnabledSynergies = false; }
	void enableSynergies()    { m_hasEnabledSynergies = true; }

	void resetModifiers()
	{
		m_isLocked            = true;
		m_hasEnabledSynergies = true;
	}

	ValueSetResult setValue(const glm::mat4& mat) override;

protected:
	using ValueMap = std::map<std::string, std::string>;

public:
	/**
	 *
	 * \return A map of valueName and value pairs.
	 */
	virtual ValueMap getDefaultValues() { return ValueMap(); }
	virtual void     setDefaultValues() {};

protected:
	explicit Transformation(const Operation* transformType) : NodeBase(transformType) {}
	void notifySequence();

public:
	/// \todo MH these should not be public.
	void nullSequence()
	{
		m_currentSequence = nullptr;
		m_currentIndex    = -1;
	}

	void setSequence(Ptr<NodeBase>&& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex    = index;
	}

	void setSequence(Ptr<NodeBase>& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex    = index;
	}

protected:
	bool m_hasEnabledSynergies = true;
	bool m_isLocked            = true;
};
using TransformPtr = Ptr<Transformation>;


template <ETransformType T>
class TransformImpl : public Transformation
{
};


template <>
class TransformImpl<ETransformType::Free> : public Transformation
{
public:
	TransformImpl() : Transformation(getTransformProps(ETransformType::Free))
	{
	}

	ETransformState isValid() const override { return ETransformState::Valid; }

	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override
	{
		setInternalValue(val, coords);
		notifySequence();
		return ValueSetResult{};
	}

	void reset() override { setInternalValue(glm::mat4(1.0f)); };
};


template <>
class TransformImpl<ETransformType::Scale> : public Transformation
{
	glm::vec3 m_initialScale;

public:
	explicit TransformImpl(glm::vec3 initialScale = glm::vec3(1.0f))
			: Transformation(getTransformProps(ETransformType::Scale)), m_initialScale(initialScale)
	{
		enableSynergies();
	}

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;

	ValueMap getDefaultValues() override;

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
template <>
class TransformImpl<ETransformType::EulerX> : public Transformation
{
	float m_initialRot;
	float m_currentRot;

public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformProps(ETransformType::EulerX)), m_initialRot(initialRot), m_currentRot(initialRot)
	{
		enableSynergies();
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	[[nodiscard]] float getRot() const { return m_currentRot; }
	float getAngle() const { return m_currentRot; }

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
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
template <>
class TransformImpl<ETransformType::EulerY> : public Transformation
{
	float m_initialRot;
	float m_currentRot;

public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformProps(ETransformType::EulerY)), m_initialRot(initialRot), m_currentRot(initialRot)
	{
		enableSynergies();
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	[[nodiscard]] float getRot() const { return m_currentRot; }
	float getAngle() const { return m_currentRot; }

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
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
template <>
class TransformImpl<ETransformType::EulerZ> : public Transformation
{
	float m_initialRot;
	float m_currentRot;

public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformProps(ETransformType::EulerZ)), m_initialRot(initialRot)
	{
		enableSynergies();
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	float getAngle() const { return m_initialRot; }
	[[nodiscard]] float getRot() const { return m_initialRot; }

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};


template <>
class TransformImpl<ETransformType::Translation> : public Transformation
{
	glm::vec3 m_initialTrans;

public:
	explicit TransformImpl(glm::vec3 initialTrans = glm::vec3(0.0f))
			: Transformation(getTransformProps(ETransformType::Translation)), m_initialTrans(initialTrans)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	float getX();
	float getY();
	float getZ();

	ValueSetResult setX(float v);
	ValueSetResult setY(float v);
	ValueSetResult setZ(float v);

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};

//===-- Other transformations ---------------------------------------------===//

template <>
class TransformImpl<ETransformType::AxisAngle> : public Transformation
{
	float     m_initialRads;
	glm::vec3 m_initialAxis;

public:
	explicit TransformImpl(float rads = glm::radians(70.0f), const glm::vec3& axis = {1.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::AxisAngle)), m_initialRads(rads), m_initialAxis(axis)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	float getRot() const             { return m_initialRads; };
	const glm::vec3& getAxis() const { return m_initialAxis; };

	ValueSetResult setRot(float rads);
	ValueSetResult setAxis(const glm::vec3& axis);

	ValueSetResult setValue(float rads) override;
	ValueSetResult setValue(const glm::vec3& axis) override;

	void reset() override;
};


template <>
class TransformImpl<ETransformType::Quat> : public Transformation
{
	glm::quat m_initialQuat;
	glm::quat m_normalized;

public:
	explicit TransformImpl(const glm::quat& q = {1.0f, 0.0f, 0.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::Quat))
	{
		m_initialQuat = q;
		m_normalized  = glm::normalize(q);
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	const glm::quat& getQuat() const { return m_initialQuat; };
	const glm::quat& getNormalized() const;

	ValueSetResult setValue(const glm::quat& vec) override;
	ValueSetResult setValue(const glm::vec4& vec) override;

	void reset() override;
};

template <>
class TransformImpl<ETransformType::Ortho> : public Transformation
{
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;

public:
	explicit TransformImpl(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	          float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Ortho)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	float getLeft() const   { return m_left; }
	float getRight() const  { return m_right; }
	float getBottom() const { return m_bottom; }
	float getTop() const    { return m_top; }
	float getNear() const   { return m_near; }
	float getFar() const    { return m_far; }

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


template <>
class TransformImpl<ETransformType::Perspective> : public Transformation
{
	float m_initialFOW;
	float m_initialAspect;
	float m_initialZNear;
	float m_initialZFar;

public:
	explicit TransformImpl(float fow = glm::radians(70.0f), float aspect = 1.333f, float zNear = 1.0f, float zFar = 10.0f)
			: Transformation(getTransformProps(ETransformType::Perspective)), m_initialFOW(fow), m_initialAspect(aspect),
				m_initialZNear(zNear), m_initialZFar(zFar)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

	float getFOW() const    { return m_initialFOW; }
	float getAspect() const { return m_initialAspect; }
	float getZNear() const  { return m_initialZNear; }
	float getZFar() const   { return m_initialZFar; }

	ValueSetResult setFOW(float v);
	ValueSetResult setAspect(float v);
	ValueSetResult setZNear(float v);
	ValueSetResult setZFar(float v);

	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};


template <>
class TransformImpl<ETransformType::Frustum> : public Transformation
{
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;

public:
	explicit TransformImpl(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
	        float far = 10.0f)
			: Transformation(getTransformProps(ETransformType::Frustum)), m_left(left), m_right(right), m_bottom(bottom),
				m_top(top), m_near(near), m_far(far)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

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
template <>
class TransformImpl<ETransformType::LookAt> : public Transformation
{
	glm::vec3 m_initialEye;
	glm::vec3 m_initialCenter;
	glm::vec3 m_initialUp;

public:
	explicit TransformImpl(const glm::vec3& eye = {0.0f, 0.0f, 10.0f}, const glm::vec3 center = {0.0f, 0.0f, 0.0f},
	       const glm::vec3& up = {0.0f, 1.0f, 0.0f})
			: Transformation(getTransformProps(ETransformType::LookAt)), m_initialEye(eye), m_initialCenter(center),
				m_initialUp(up)
	{
	}

	ETransformState isValid() const override;

	ValueMap getDefaultValues() override;

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
