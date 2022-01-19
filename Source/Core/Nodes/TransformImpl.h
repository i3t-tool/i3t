#pragma once

#include "Transform.h"

namespace Core
{
template <ETransformType T>
class TransformImpl : public Transformation
{
};


template <>
class TransformImpl<ETransformType::Free> : public Transformation
{
public:
	TransformImpl() : Transformation(getTransformOperation(ETransformType::Free))
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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Scale))
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
			: Transformation(getTransformOperation(ETransformType::EulerX)), m_initialRot(initialRot), m_currentRot(initialRot)
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
			: Transformation(getTransformOperation(ETransformType::EulerY)), m_initialRot(initialRot), m_currentRot(initialRot)
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
			: Transformation(getTransformOperation(ETransformType::EulerZ)), m_initialRot(initialRot)
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
			: Transformation(getTransformOperation(ETransformType::Translation)), m_initialTrans(initialTrans)
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
			: Transformation(getTransformOperation(ETransformType::AxisAngle)), m_initialRads(rads), m_initialAxis(axis)
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
			: Transformation(getTransformOperation(ETransformType::Quat))
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
			: Transformation(getTransformOperation(ETransformType::Ortho)), m_left(left), m_right(right), m_bottom(bottom),
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
			: Transformation(getTransformOperation(ETransformType::Perspective)), m_initialFOW(fow), m_initialAspect(aspect),
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
			: Transformation(getTransformOperation(ETransformType::Frustum)), m_left(left), m_right(right), m_bottom(bottom),
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
			: Transformation(getTransformOperation(ETransformType::LookAt)), m_initialEye(eye), m_initialCenter(center),
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
