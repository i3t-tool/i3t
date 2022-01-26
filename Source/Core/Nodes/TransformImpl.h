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
	TransformImpl() : Transformation(getTransformOperation(ETransformType::Free)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::EulerX))
	{
		enableSynergies();
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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::EulerY))
	{
		enableSynergies();
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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::EulerZ))
	{
		enableSynergies();
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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Translation)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::AxisAngle)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Quat)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Ortho)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Perspective)) {}

	ETransformState isValid() const override;

	float getFOV() const    { return getDefaultValue("fov").getFloat(); }
	float getAspect() const { return getDefaultValue("aspect").getFloat(); }
	float getZNear() const  { return getDefaultValue("zNear").getFloat(); }
	float getZFar() const   { return getDefaultValue("zFar").getFloat(); }

	ValueSetResult setFOV(float v);
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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::Frustum)) {}

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
	explicit TransformImpl()
			: Transformation(getTransformOperation(ETransformType::LookAt)) {}

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
