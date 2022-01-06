#include "Transform.h"

#include "Logger/Logger.h"
#include "Utils/Format.h"

namespace Core
{
//===-- Value masks -------------------------------------------------------===//

using ValueMask = std::array<uint8_t, 16>;

constexpr uint8_t VM_ZERO = 0;
constexpr uint8_t VM_ONE  = 1;
constexpr uint8_t VM_ANY  = 2;

constexpr ValueMask g_ScaleMask = {
		VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_TranslateMask = {
		VM_ONE,  VM_ZERO, VM_ZERO, VM_ANY,
		VM_ZERO, VM_ONE,  VM_ZERO, VM_ANY,
		VM_ZERO, VM_ZERO, VM_ONE,  VM_ANY,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateXMask = {
		VM_ONE,  VM_ZERO, VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateYMask = {
		VM_ANY,  VM_ZERO, VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ONE,  VM_ZERO, VM_ZERO,
		VM_ANY,  VM_ZERO, VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateZMask = {
		VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO,
		VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ONE,  VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_OrthoMask = {
		VM_ANY,  VM_ZERO, VM_ZERO, VM_ANY,
		VM_ZERO, VM_ANY,  VM_ZERO, VM_ANY,
		VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_PerspectiveMask = {
		VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,
		VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO,
};

constexpr ValueMask g_FrustumMask = {
		VM_ANY,  VM_ZERO, VM_ANY, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ANY, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ANY, VM_ANY,
		VM_ZERO, VM_ZERO, VM_ANY, VM_ZERO,
};

constexpr bool validateValue(const ValueMask& mask, glm::ivec2 coords, float value)
{
	const uint8_t maskValue = mask[coords.x * 4 + coords.y];

	return maskValue == VM_ANY && Math::eq((float) maskValue, value);
}

constexpr bool validateValues(const ValueMask& mask, const glm::mat4& matrix)
{
	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			const float value = matrix[c][r];

			if (!validateValue(mask, { c, r }, value))
			{
				return false;
			}
		}
	}
	return true;
}

EValueState Transformation::getValueState(glm::ivec2 coords)
{
	const int idx = 15 - (coords.y * 4 + coords.x);  // flip idx (little endian)
	auto& map = getTransformMap(getOperation()->keyWord);

	std::bitset<2> bitResult;
	bitResult[1 - 0] = map[idx] || !m_isLocked;            // editable bit
	bitResult[1 - 1] = map[idx] && m_hasEnabledSynergies;  // synergies bit

	auto result = bitResult.to_ulong();

	return static_cast<EValueState>(result);
}

/*
ValueSetResult Transformation::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(getTransformMap(getOperation()->keyWord), coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	return onSetValue(val, coords);
}
 */

//===----------------------------------------------------------------------===//

bool Transformation::isLocked() const
{
	return m_isLocked;
}

void Transformation::lock()
{
	// m_currentMap = &Transform::g_AllLocked;
	m_isLocked = true;
}

void Transformation::unlock()
{
	// m_currentMap = &Transform::g_Free;
	m_isLocked = false;
}

ValueSetResult Transformation::setValue(const glm::mat4& mat)
{
	ValueSetResult result;

	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			auto coords           = glm::ivec2(c, r);
			const auto valueState = getValueState(coords);

			if (canEditValue(valueState))
			{
				const float val = mat[c][r];

				// MSVC was unable to compile this expresion without using Node::
				result = Node::setValue(val, coords);

				if (result.status != ValueSetResult::Status::Ok)
				{
					notifySequence();

					return result;
				}
			}
		}
	}
	notifySequence();

	return ValueSetResult{};
}

void Transformation::notifySequence()
{
	if (m_currentSequence)
	{
		m_currentSequence->updateValues(-1);
	}
}

//===----------------------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Scale>::isValid() const
{

	bool result = validateValues(g_ScaleMask, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]);
	}

	return ETransformState(result);
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val)
{
	notifySequence();
	return NodeBase::setValue(glm::vec3(val));
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec3& vec)
{
	if (hasSynergies())
	{
		if (Math::areElementsSame(vec))
		{
			setInternalValue(glm::scale(vec));
		}
		else
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
														"Given vector does not have all three values same."};
	}
	else
	{
		setInternalValue(glm::scale(vec));
	}

	notifySequence();
	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_ScaleMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (getValueState(coords) == EValueState::Editable)
	{
		if (hasSynergies())
		{
			setInternalValue(glm::scale(glm::vec3(val)));
		}
		else
		{
			setInternalValue(val, coords);
		}
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Scale>::reset()
{
	m_hasEnabledSynergies = true;
	m_isLocked            = true;

	setInternalValue(glm::scale(m_initialScale));
}

Transformation::ValueMap TransformImpl<ETransformType::Scale>::getDefaultValues()
{
	return {{"scale", Utils::toString(m_initialScale)}};
}

float TransformImpl<ETransformType::Scale>::getX()
{
	return getInternalData().getMat4()[0][0];
}

float TransformImpl<ETransformType::Scale>::getY()
{
	return getInternalData().getMat4()[1][1];
}

float TransformImpl<ETransformType::Scale>::getZ()
{
	return getInternalData().getMat4()[2][2];
}

ValueSetResult TransformImpl<ETransformType::Scale>::setX(float v)
{
	return setValue(v, {0, 0});
}

ValueSetResult TransformImpl<ETransformType::Scale>::setY(float v)
{
	return setValue(v, {1, 1});
}

ValueSetResult TransformImpl<ETransformType::Scale>::setZ(float v)
{
	return setValue(v, {2, 2});
}

//===-- Euler rotation around X axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerX>::isValid() const
{
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateXMask, mat);

	float angle = std::atan2(-mat[2][1], mat[2][2]);
	auto expectedMat = glm::eulerAngleX(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

Transformation::ValueMap TransformImpl<ETransformType::EulerX>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(1.0f, 0.0f, 0.0f)));

	notifySequence();
	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_RotateXMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (!Math::withinInterval(val, -1.0f, 1.0f))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
													"Value must be within [-1.0, 1.0] interval."};
	}

	auto mat = getData().getMat4();

	mat[coords.x][coords.y] = val;
	if (hasSynergies())
	{
		if (coords == glm::ivec2(1, 2))
		{
			// -sin(T)
			mat[2][1] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[1][1] = cos;
			mat[2][2] = cos;
		}
		else if (coords == glm::ivec2(1, 1) || coords == glm::ivec2(2, 2))
		{
			// cos(T)
			mat[1][1] = val;
			mat[2][2] = val;

			auto sin = sqrt(1.0f - (val * val));
			mat[1][2] = sin;
			mat[2][1] = -sin;
		}
		else if (coords == glm::ivec2(2, 1))
		{
			// sin(T)
			mat[1][2] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[1][1] = cos;
			mat[2][2] = cos;
		}
	}
	// todo set current val?
	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerX>::reset()
{
	m_hasEnabledSynergies = true;
	m_isLocked            = true;

	setInternalValue(glm::eulerAngleX(m_initialRot));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Y axis --------------------------------------===//
ETransformState TransformImpl<ETransformType::EulerY>::isValid() const
{
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateYMask, mat);

	float angle = std::asin(mat[2][0]);
	auto expectedMat = glm::eulerAngleY(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

Transformation::ValueMap TransformImpl<ETransformType::EulerY>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 1.0f, 0.0f)));
	notifySequence();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_RotateYMask, coords, val))
	{
		ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (!Math::withinInterval(val, -1.0f, 1.0f))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
													"Value must be within [-1.0, 1.0] interval."};
	}

	auto mat = getData().getMat4();

	mat[coords.x][coords.y] = val;
	if (hasSynergies())
	{
		if (coords == glm::ivec2(0, 2))
		{
			// -sin(T)
			mat[2][0] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[0][0] = cos;
			mat[2][2] = cos;
		}
		else if (coords == glm::ivec2(0, 0) || coords == glm::ivec2(2, 2))
		{
			// cos(T)
			mat[0][0] = val;
			mat[2][2] = val;

			auto sin = sqrt(1.0f - (val * val));
			mat[0][2] = -sin;
			mat[2][0] = sin;
		}
		else if (coords == glm::ivec2(2, 0))
		{
			// sin(T)
			mat[0][2] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[0][0] = cos;
			mat[2][2] = cos;
		}
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerY>::reset()
{
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Z axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerZ>::isValid() const
{
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateZMask, mat);

	float angle = glm::atan(mat[0][1], mat[0][0]);
	auto expectedMat = glm::eulerAngleZ(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

Transformation::ValueMap TransformImpl<ETransformType::EulerZ>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 0.0f, 1.0f)));
	notifySequence();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_RotateZMask, coords, val))
	{
		ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (!Math::withinInterval(val, -1.0f, 1.0f))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
													"Value must be within [-1.0, 1.0] interval."};
	}

	auto mat = getData().getMat4();

	mat[coords.x][coords.y] = val;
	if (hasSynergies())
	{
		if (coords == glm::ivec2(0, 1))
		{
			// -sin(T)
			mat[1][0] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
		else if (coords == glm::ivec2(0, 0) || coords == glm::ivec2(1, 1))
		{
			// cos(T)
			mat[0][0] = val;
			mat[1][1] = val;

			auto sin = sqrt(1.0f - (val * val));
			mat[0][1] = sin;
			mat[1][0] = -sin;
		}
		else if (coords == glm::ivec2(1, 0))
		{
			// sin(T)
			mat[0][1] = -val;

			auto cos = sqrt(1.0f - (val * val));
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerZ>::reset()
{
	resetModifiers();
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 0.0f, 1.0f)));
	m_currentRot = m_initialRot;
}

//===-- Translation -------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Translation>::isValid() const
{
	bool result = validateValues(g_TranslateMask, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[3][0], mat[3][1]) && Math::eq(mat[3][1], mat[3][2]);
	}

	return ETransformState(result);
}

Transformation::ValueMap TransformImpl<ETransformType::Translation>::getDefaultValues()
{
	return {{"translation", Utils::toString(m_initialTrans)}};
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val)
{
	return setValue(glm::vec3(val));
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec3& vec)
{
	setInternalValue(glm::translate(vec));
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_TranslateMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Translation>::reset()
{
	setInternalValue(glm::translate(m_initialTrans));
}

float TransformImpl<ETransformType::Translation>::getX()
{
	return getInternalData().getMat4()[3][0];
}

float TransformImpl<ETransformType::Translation>::getY()
{
	return getInternalData().getMat4()[3][1];
}

float TransformImpl<ETransformType::Translation>::getZ()
{
	return getInternalData().getMat4()[3][2];
}

ValueSetResult TransformImpl<ETransformType::Translation>::setX(float v)
{
	return setValue(v, {3, 0});
}

ValueSetResult TransformImpl<ETransformType::Translation>::setY(float v)
{
	return setValue(v, {3, 1});
}

ValueSetResult TransformImpl<ETransformType::Translation>::setZ(float v)
{
	return setValue(v, {3, 2});
}

//===-- Axis angle rotation -----------------------------------------------===//

ETransformState TransformImpl<ETransformType::AxisAngle>::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap TransformImpl<ETransformType::AxisAngle>::getDefaultValues()
{
	return {{"axis", Utils::toString(m_initialAxis)}, {"rotation", Utils::toString(m_initialRads)}};
}

void TransformImpl<ETransformType::AxisAngle>::reset()
{
	notifySequence();
	setInternalValue(glm::rotate(m_initialRads, m_initialAxis));
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(float rads)
{
	return setRot(rads);
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(const glm::vec3& axis)
{
	return setAxis(axis);
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setRot(float rads)
{
	m_initialRads = rads;
	setInternalValue(glm::rotate(rads, m_initialAxis));
	return ValueSetResult();
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setAxis(const glm::vec3& axis)
{
	m_initialAxis = axis;
	setInternalValue(glm::rotate(m_initialRads, axis));
	return ValueSetResult();
}

//===-- Quaternion rotation -----------------------------------------------===//

ETransformState TransformImpl<ETransformType::Quat>::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap TransformImpl<ETransformType::Quat>::getDefaultValues()
{
	return {{"quat", Utils::toString(m_initialQuat)}};
}

void TransformImpl<ETransformType::Quat>::reset()
{
	notifySequence();
	setInternalValue(glm::toMat4(m_normalized));
}

const glm::quat& TransformImpl<ETransformType::Quat>::getNormalized() const
{
	return m_normalized;
};

ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::quat& q)
{
	m_initialQuat = q;
	m_normalized = glm::normalize(q);
	setInternalValue(glm::toMat4(m_normalized));
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::vec4& vec)
{
	glm::quat q(vec);
	return setValue(glm::quat(q));
}

//===-- Orthographic projection -------------------------------------------===//

ETransformState TransformImpl<ETransformType::Ortho>::isValid() const
{
	return ETransformState(
			validateValues(g_OrthoMask, m_internalData[0].getMat4())
	);
}

Transformation::ValueMap TransformImpl<ETransformType::Ortho>::getDefaultValues()
{
	return {
			{"left", Utils::toString(m_left)},
			{"right", Utils::toString(m_right)},
			{"bottom", Utils::toString(m_bottom)},
			{"top", Utils::toString(m_top)},
			{"near", Utils::toString(m_near)},
			{"far", Utils::toString(m_far)},
	};
}

void TransformImpl<ETransformType::Ortho>::reset()
{
	notifySequence();
	setInternalValue(glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_OrthoMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an editable field."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Perspective -------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Perspective>::isValid() const
{
	return ETransformState(
			validateValues(g_PerspectiveMask, m_internalData[0].getMat4())
	);
}

Transformation::ValueMap TransformImpl<ETransformType::Perspective>::getDefaultValues()
{
	return {
			{"fov", Utils::toString(m_initialFOW)},
			{"aspect", Utils::toString(m_initialAspect)},
			{"zNear", Utils::toString(m_initialZNear)},
			{"zFar", Utils::toString(m_initialZFar)},
	};
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setFOW(float v)
{
	m_initialFOW = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setAspect(float v)
{
	m_initialAspect = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setZNear(float v)
{
	m_initialZNear = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setZFar(float v)
{
	m_initialZFar = v;
	reset();
	return ValueSetResult{};
}

void TransformImpl<ETransformType::Perspective>::reset()
{
	notifySequence();
	setInternalValue(glm::perspective(m_initialFOW, m_initialAspect, m_initialZNear, m_initialZFar));
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_PerspectiveMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

//===-- Frusum ------------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Frustum>::isValid() const
{
	return ETransformState(
			validateValues(g_FrustumMask, m_internalData[0].getMat4())
	);
}

Transformation::ValueMap TransformImpl<ETransformType::Frustum>::getDefaultValues()
{
	return {
			{"left", Utils::toString(m_left)},
			{"right", Utils::toString(m_right)},
			{"bottom", Utils::toString(m_bottom)},
			{"top", Utils::toString(m_top)},
			{"near", Utils::toString(m_near)},
			{"far", Utils::toString(m_far)},
	};
}

void TransformImpl<ETransformType::Frustum>::reset()
{
	notifySequence();
	setInternalValue(glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_FrustumMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Look At -----------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::LookAt>::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap TransformImpl<ETransformType::LookAt>::getDefaultValues()
{
	return {
			{"eye", Utils::toString(m_initialEye)},
			{"center", Utils::toString(m_initialCenter)},
			{"up", Utils::toString(m_initialUp)},
	};
}

void TransformImpl<ETransformType::LookAt>::reset()
{
	notifySequence();
	setInternalValue(glm::lookAt(m_initialEye, m_initialCenter, m_initialUp));
}

ValueSetResult TransformImpl<ETransformType::LookAt>::setValue(float val, glm::ivec2 coords)
{
	if (isLocked())
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::LookAt>::setEye(const glm::vec3& eye)
{
	m_initialEye = eye;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::LookAt>::setCenter(const glm::vec3& center)
{
	m_initialCenter = center;
	reset();
	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::LookAt>::setUp(const glm::vec3& up)
{
	m_initialUp = up;
	reset();
	return ValueSetResult{};
}
}
