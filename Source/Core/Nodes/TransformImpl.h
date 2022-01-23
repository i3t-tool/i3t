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
public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformOperation(ETransformType::EulerX))
	{
		enableSynergies();
		setDefaultValue("rotation", initialRot);
	}

	ETransformState isValid() const override;

	float getAngle() const { return getDefaultValue("rotation").getFloat(); }

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
public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformOperation(ETransformType::EulerY))
	{
		enableSynergies();
		setDefaultValue("rotation", initialRot);
	}

	ETransformState isValid() const override;

	float getAngle() const { return getDefaultValue("rotation").getFloat(); }

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
public:
	explicit TransformImpl(float initialRot = 0.0f)
			: Transformation(getTransformOperation(ETransformType::EulerZ))
	{
		enableSynergies();
		setDefaultValue("rotation", initialRot);
	}

	ETransformState isValid() const override;

	float getAngle() const { return getDefaultValue("rotation").getFloat(); }

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void reset() override;
};


template <>
class TransformImpl<ETransformType::Translation> : public Transformation
{
public:
	explicit TransformImpl(glm::vec3 initialTrans = glm::vec3(0.0f))
			: Transformation(getTransformOperation(ETransformType::Translation))
	{
		setDefaultValue("translation", initialTrans);
	}

	ETransformState isValid() const override;

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
public:
	explicit TransformImpl(float rads = glm::radians(70.0f), const glm::vec3& axis = {1.0f, 0.0f, 0.0f})
			: Transformation(getTransformOperation(ETransformType::AxisAngle))
	{
		setDefaultValue("rotation", rads);
		setDefaultValue("axis", axis);
	}

	ETransformState isValid() const override;

	float getRot() const             { return getDefaultValue("rotation").getFloat(); };
	const glm::vec3& getAxis() const { return getDefaultValue("axis").getVec3(); };

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

	const glm::quat& getQuat() const { return m_initialQuat; };
	const glm::quat& getNormalized() const;

	ValueSetResult setValue(const glm::quat& vec) override;
	ValueSetResult setValue(const glm::vec4& vec) override;

	void reset() override;
};

template <>
class TransformImpl<ETransformType::Ortho> : public Transformation
{
public:
	explicit TransformImpl(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
												 float far = 10.0f)
			: Transformation(getTransformOperation(ETransformType::Ortho))
	{
		setDefaultValue("left", left);
		setDefaultValue("right", right);
		setDefaultValue("bottom", bottom);
		setDefaultValue("top", top);
		setDefaultValue("near", near);
		setDefaultValue("far", far);
	}

	ETransformState isValid() const override;

	float getLeft() const { return getDefaultValue("left").getFloat(); }
	float getRight() const { return getDefaultValue("right").getFloat(); }
	float getBottom() const { return getDefaultValue("bottom").getFloat(); }
	float getTop() const { return getDefaultValue("top").getFloat(); }
	float getNear() const { return getDefaultValue("near").getFloat(); }
	float getFar() const { return getDefaultValue("far").getFloat(); }

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
public:
	explicit TransformImpl(float fow = glm::radians(70.0f), float aspect = 1.333f, float zNear = 1.0f, float zFar = 10.0f)
			: Transformation(getTransformOperation(ETransformType::Perspective))
	{
		setDefaultValue("fow", fow);
		setDefaultValue("aspect", aspect);
		setDefaultValue("zNear", zNear);
		setDefaultValue("zFar", zFar);
	}

	ETransformState isValid() const override;

	float getFOW() const    { return getDefaultValue("fow").getFloat(); }
	float getAspect() const { return getDefaultValue("aspect").getFloat(); }
	float getZNear() const  { return getDefaultValue("zNear").getFloat(); }
	float getZFar() const   { return getDefaultValue("zFar").getFloat(); }

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
public:
	explicit TransformImpl(float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f, float near = 1.0f,
												 float far = 10.0f)
			: Transformation(getTransformOperation(ETransformType::Frustum))
	{
		setDefaultValue("left", left);
		setDefaultValue("right", right);
		setDefaultValue("bottom", bottom);
		setDefaultValue("top", top);
		setDefaultValue("near", near);
		setDefaultValue("far", far);
	}

	ETransformState isValid() const override;

	float getLeft() const { return getDefaultValue("left").getFloat(); }
	float getRight() const { return getDefaultValue("right").getFloat(); }
	float getBottom() const { return getDefaultValue("bottom").getFloat(); }
	float getTop() const { return getDefaultValue("top").getFloat(); }
	float getNear() const { return getDefaultValue("near").getFloat(); }
	float getFar() const { return getDefaultValue("far").getFloat(); }

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
public:
	explicit TransformImpl(const glm::vec3& eye = {0.0f, 0.0f, 10.0f}, const glm::vec3 center = {0.0f, 0.0f, 0.0f},
												 const glm::vec3& up = {0.0f, 1.0f, 0.0f})
			: Transformation(getTransformOperation(ETransformType::LookAt))
	{
		setDefaultValue("eye", eye);
		setDefaultValue("center", center);
		setDefaultValue("up", up);
	}

	ETransformState isValid() const override;

	void reset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	const glm::vec3& getEye() { return getDefaultValue("eye").getVec3(); }
	const glm::vec3& getCenter() { return getDefaultValue("center").getVec3(); }
	const glm::vec3& getUp() { return getDefaultValue("up").getVec3(); }

	ValueSetResult setEye(const glm::vec3& eye);
	ValueSetResult setCenter(const glm::vec3& center);
	ValueSetResult setUp(const glm::vec3& up);
};
}
