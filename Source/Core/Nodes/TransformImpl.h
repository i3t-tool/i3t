#pragma once

#include "Transform.h"

namespace Core
{
#define I3T_TRANSFORM_CLONE(T)                                                                                         \
	Ptr<Node> clone() override                                                                                           \
	{                                                                                                                    \
		auto node = Builder::createTransform<T>();                                                                         \
                                                                                                                       \
		isLocked() ? node->lock() : node->unlock();                                                                        \
		hasSynergies() ? node->enableSynergies() : node->disableSynergies();                                               \
                                                                                                                       \
		node->setDefaultValues(getDefaultValues());                                                                        \
		node->setValue(getData(0).getMat4());                                                                              \
                                                                                                                       \
		return node;                                                                                                       \
	}

template <ETransformType T>
class TransformImpl : public Transformation
{};

namespace Builder
{
	template <ETransformType T>
	Ptr<Transformation> createTransform()
	{
		const auto defaultValues = getTransformDefaults(T);
		auto       ret           = std::make_shared<TransformImpl<T>>();
		ret->init();
		ret->createDefaults();
		ret->initDefaults();

		return ret;
	}
} // namespace Builder



template <>
class TransformImpl<ETransformType::Free> : public Transformation
{
public:
	TransformImpl() : Transformation(getTransformOperation(ETransformType::Free)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Free)

	ETransformState isValid() const override { return ETransformState::Valid; }

	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override
	{
		setInternalValue(val, coords);
		notifySequence();
		return ValueSetResult{};
	}

	void onReset() override { setInternalValue(glm::mat4(1.0f)); };
};


template <>
class TransformImpl<ETransformType::Scale> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Scale)) { enableSynergies(); }

	I3T_TRANSFORM_CLONE(ETransformType::Scale)

	void initDefaults() override { setDefaultValue("scale", glm::vec3{1.0f, 1.0f, 1.0f}); }

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
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
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerX)) { enableSynergies(); }

	I3T_TRANSFORM_CLONE(ETransformType::EulerX)

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
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
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerY)) { enableSynergies(); }

	I3T_TRANSFORM_CLONE(ETransformType::EulerY)

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
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
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerZ)) { enableSynergies(); }

	I3T_TRANSFORM_CLONE(ETransformType::EulerZ)

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float rad) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
};


template <>
class TransformImpl<ETransformType::Translation> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Translation)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Translation)

	ETransformState isValid() const override;

	[[nodiscard]] ValueSetResult setValue(float val) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
	[[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
	[[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
};

//===-- Other transformations ---------------------------------------------===//

template <>
class TransformImpl<ETransformType::AxisAngle> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::AxisAngle)) {}

	I3T_TRANSFORM_CLONE(ETransformType::AxisAngle)

	void initDefaults() override { setDefaultValue("axis", glm::vec3{0.0f, 1.0f, 0.0f}); }

	ETransformState isValid() const override;

	ValueSetResult setValue(float rads) override;
	ValueSetResult setValue(const glm::vec3& axis) override;

	void onReset() override;
};


template <>
class TransformImpl<ETransformType::Quat> : public Transformation
{
	glm::quat m_initialQuat;
	glm::quat m_normalized;

public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Quat)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Quat)

	ETransformState isValid() const override;

	const glm::quat& getQuat() const { return m_initialQuat; };
	const glm::quat& getNormalized() const;

	ValueSetResult setValue(const glm::quat& vec) override;
	ValueSetResult setValue(const glm::vec4& vec) override;

	void onReset() override;
};


template <>
class TransformImpl<ETransformType::Ortho> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Ortho)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Ortho)

	void initDefaults() override
	{
		setDefaultValue("left", -5.0f);
		setDefaultValue("right", 5.0f);
		setDefaultValue("bottom", -5.0f);
		setDefaultValue("top", 5.0f);
		setDefaultValue("near", 1.0f);
		setDefaultValue("far", 10.0f);
	}

	ETransformState isValid() const override;

	/// No synergies required.
	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
};


template <>
class TransformImpl<ETransformType::Perspective> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Perspective)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Perspective)

	void initDefaults() override
	{
		setDefaultValue("fov", glm::radians(70.0f));
		setDefaultValue("aspect", 1.33f);
		setDefaultValue("zNear", 1.0f);
		setDefaultValue("zFar", 10.0f);
	}

	ETransformState isValid() const override;

	ValueSetResult setValue(float val, glm::ivec2 coords) override;

	void onReset() override;
};


template <>
class TransformImpl<ETransformType::Frustum> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Frustum)) {}

	I3T_TRANSFORM_CLONE(ETransformType::Frustum)

	void initDefaults() override
	{
		setDefaultValue("left", -5.0f);
		setDefaultValue("right", 5.0f);
		setDefaultValue("bottom", -5.0f);
		setDefaultValue("top", 5.0f);
		setDefaultValue("near", 1.0f);
		setDefaultValue("far", 10.0f);
	}

	ETransformState isValid() const override;

	void onReset() override;

	ValueSetResult setValue(float val, glm::ivec2 coords) override;
};


/**
 * Same as perspective projection node, but all values are locked.
 */
template <>
class TransformImpl<ETransformType::LookAt> : public Transformation
{
public:
	explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::LookAt)) {}

	I3T_TRANSFORM_CLONE(ETransformType::LookAt)

	void initDefaults() override
	{
		setDefaultValue("eye", glm::vec3{0.0, 0.0, 10.0});
		setDefaultValue("center", glm::vec3{0.0, 0.0, 0.0});
		setDefaultValue("up", glm::vec3{0.0, 1.0, 0.0});
	}

	ETransformState isValid() const override;

	void           onReset() override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override;
};
} // namespace Core
