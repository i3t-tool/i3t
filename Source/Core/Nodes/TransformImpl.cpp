#include "TransformImpl.h"

#include <math.h>

//#include "pgr.h"
#include "Utils/Format.h"
#include<glm/gtx/vector_angle.hpp>

#ifndef M_PI
/// define Pi for compatibility issues (MSVC vs GCC)
#define M_PI 3.14159f
#endif

namespace Core
{
//===-- Value masks -------------------------------------------------------===//
// Written in ROW order - flipped to COLUMN order in validateValue()!!!!
constexpr ValueMask g_ScaleMask = {
		VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ZERO, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_TranslateMask = {  // row-order
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
		VM_ZERO, VM_ZERO, VM_MINUS_ONE,  VM_ZERO,
};

constexpr ValueMask g_FrustumMask = {
		VM_ANY,  VM_ZERO, VM_ANY, VM_ZERO,
		VM_ZERO, VM_ANY,  VM_ANY, VM_ZERO,
		VM_ZERO, VM_ZERO, VM_ANY, VM_ANY,
	  VM_ZERO, VM_ZERO, VM_MINUS_ONE, VM_ZERO,
};

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
	if (!canSetValue(g_ScaleMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (hasSynergies() && coords.x == coords.y && coords.x != 3)
	{
		setInternalValue(glm::scale(glm::vec3(val)));
	}
	else
	{
		setInternalValue(val, coords);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Scale>::onReset()
{
//	m_hasEnabledSynergies = true; // JH When setting X value in non-uniform scale -> this switch to uniform scale (due to enable synergies)
//	m_isLocked            = true;

	auto scale = getDefaultValue("scale").getVec3();

	if (m_hasEnabledSynergies)
	{
		scale.y = scale.x;
		scale.z = scale.x;
	}

	setInternalValue(glm::scale(scale));
}

//===-- Euler rotation around X axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerX>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateXMask, mat);

	float angle			 = glm::atan(-mat[2][1], mat[2][2]);   // glm::atan executes ::std::atan2
	auto expectedMat = glm::eulerAngleX(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(float val)
{
	setDefaultValue("rotation", val);
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
	if (!canSetValue(g_RotateXMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val;   // unlimited value, clamped if synergies

	if (hasSynergies())
	{
		//PF Commented out - the return value is thrown away anyway
		//if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		//}

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f);  
		
		if (coords == glm::ivec2(1, 2))
		{
			// -sin(T)
			mat[1][2] =  clampedVal; // repair the setup without synergies
			mat[2][1] = -clampedVal;
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[1][1] = cos;
			mat[2][2] = cos;
		}
		else if (coords == glm::ivec2(1, 1) || coords == glm::ivec2(2, 2))
		{
			// cos(T)
			mat[1][1] = clampedVal;
			mat[2][2] = clampedVal;
			halfspaceSign.cos = glm::sign(clampedVal);

			auto sin = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.sin < 0.0f)
				sin *= -1.0f; // allow negative sin values while changing cos - avoid jump in rotation
			mat[1][2] = sin;
			mat[2][1] = -sin;
		}
		else if (coords == glm::ivec2(2, 1))
		{
			// sin(T)
			mat[1][2] = -clampedVal;
			mat[2][1] =  clampedVal;   // repair the setup without synergies
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[1][1] = cos;
			mat[2][2] = cos;
		}
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerX>::onReset()
{
	m_hasEnabledSynergies = true;
	m_isLocked            = true;

	setInternalValue(glm::eulerAngleX(getDefaultValue("rotation").getFloat()));
}

//===-- Euler rotation around Y axis --------------------------------------===//
// PF Important - cos returns angles <0, M_PI> only - we have to use sin to get the whole circle
ETransformState TransformImpl<ETransformType::EulerY>::isValid() const
{
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateYMask, mat);

 	float angle			 = glm::atan(mat[2][0], mat[2][2]);    // glm::atan executes ::std::atan2
	auto expectedMat = glm::eulerAngleY(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(float val)
{
	setDefaultValue("rotation", val);
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

	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val;     // unlimited value, clamped if synergies

	if (hasSynergies())
	{
		//PF Commented out - the return value is thrown away anyway
		//if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		//}

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f); 

		if (coords == glm::ivec2(0, 2))
		{
			// -sin(T)
			mat[0][2]	=  clampedVal; // repair the setup without synergies
			mat[2][0] = -clampedVal;
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[0][0] = cos;
			mat[2][2] = cos;
		}
		else if (coords == glm::ivec2(0, 0) || coords == glm::ivec2(2, 2))
		{
			// cos(T)
			mat[0][0] = clampedVal;
			mat[2][2] = clampedVal;
			halfspaceSign.cos = glm::sign(clampedVal);

			auto sin = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.sin < 0.0f)
				sin *= -1.0f; // allow negative sin values while changing cos - avoid jump in rotation
			mat[0][2] = -sin;
			mat[2][0] = sin;
		}
		else if (coords == glm::ivec2(2, 0))
		{
			// sin(T)
			mat[0][2] = -clampedVal;
			mat[2][0]	=  clampedVal; // repair the setup without synergies
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[0][0] = cos;
			mat[2][2] = cos;
		}
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerY>::onReset()
{
	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

//===-- Euler rotation around Z axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerZ>::isValid() const
{
	auto& mat   = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateZMask, mat);

	float angle      = glm::atan(mat[0][1], mat[0][0]);  // glm::atan executes ::std::atan2
	auto expectedMat = glm::eulerAngleZ(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}
ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(float val)
{
	setDefaultValue("rotation", val);
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

  // PF: remembering the halfspace sign for each box to avoid jumps during interaction with rotation matrix
  
	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val;    // unlimited value, clamped if synergies, works in all 4x4 coordinates

	if (hasSynergies())
	{
		//PF Commented out - the return value is thrown away anyway
		//if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		//}

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f); 

		if (coords == glm::ivec2(0, 1))
		{
			// -sin(T)
			mat[0][1]					= clampedVal; // repair the setup without synergies
			mat[1][0]					= -clampedVal;
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
		else if (coords == glm::ivec2(0, 0) || coords == glm::ivec2(1, 1))
		{
			// cos(T)
			mat[0][0]					 = clampedVal;
			mat[1][1]					 = clampedVal;
			halfspaceSign.cos = glm::sign(clampedVal);

			auto sin = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.sin < 0.0f)
				sin *= -1.0f; // allow negative sin values while changing cos - avoid jump in rotation
			mat[0][1] = sin;
			mat[1][0] = -sin;
		}
		else if (coords == glm::ivec2(1, 0))
		{
			// sin(T)
			mat[0][1]					= -clampedVal;
			mat[1][0]					= clampedVal; // repair the setup without synergies
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid jump in rotation
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
	}


	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerZ>::onReset()
{
	resetModifiers();
	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 0.0f, 1.0f)));
}

//===-- Translation -------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Translation>::isValid() const
{
	bool result = validateValues(g_TranslateMask, m_internalData[0].getMat4());
	// PF - err - translate has no synergies
	//if (hasSynergies())
	//{
	//	auto& mat = m_internalData[0].getMat4();
	//	result = result && Math::eq(mat[3][0], mat[3][1]) && Math::eq(mat[3][1], mat[3][2]);
	//}

	return ETransformState(result);
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
	if (!canSetValue(g_TranslateMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Translation>::onReset()
{
	setInternalValue(glm::translate(
			getDefaultValue("translation").getVec3()
	));
	notifySequence();
}

//===-- Axis angle rotation -----------------------------------------------===//

ETransformState TransformImpl<ETransformType::AxisAngle>::isValid() const
{
	return ETransformState::Unknown;
	// \todo PF: check if the rotation vector is of non-zero length?
}

void TransformImpl<ETransformType::AxisAngle>::onReset()
{
	setInternalValue(glm::rotate(
			getDefaultValue("rotation").getFloat(),
			getDefaultValue("axis").getVec3()
	));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(float rads)
{
	setDefaultValue("rotation", rads);

	return ValueSetResult{};
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(const glm::vec3& axis)
{
	setDefaultValue("axis", axis);

	return ValueSetResult{};
}

//===-- Quaternion rotation -----------------------------------------------===//

ETransformState TransformImpl<ETransformType::Quat>::isValid() const
{
	return ETransformState::Unknown;
	// \todo PF: Check, if it is of unit length?
}

void TransformImpl<ETransformType::Quat>::onReset()
{
	setInternalValue(glm::toMat4(m_normalized));
	notifySequence();
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

void TransformImpl<ETransformType::Ortho>::onReset()
{
	setInternalValue(glm::ortho(
			getDefaultValue("left").getFloat(),
			getDefaultValue("right").getFloat(),
			getDefaultValue("bottom").getFloat(),
			getDefaultValue("top").getFloat(),
			getDefaultValue("near").getFloat(),
			getDefaultValue("far").getFloat()
	));
	notifySequence();
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

//===-- Perspective -------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Perspective>::isValid() const
{
	return ETransformState(
			validateValues(g_PerspectiveMask, m_internalData[0].getMat4())
	);
}

void TransformImpl<ETransformType::Perspective>::onReset()
{
	setInternalValue(glm::perspective(
			getDefaultValue("fov").getFloat(),
			getDefaultValue("aspect").getFloat(),
			getDefaultValue("zNear").getFloat(),
			getDefaultValue("zFar").getFloat()
	));
	notifySequence();
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

//===-- Frustum ------------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Frustum>::isValid() const
{
	return ETransformState(
			validateValues(g_FrustumMask, m_internalData[0].getMat4())
	);
}

void TransformImpl<ETransformType::Frustum>::onReset()
{
	setInternalValue(glm::frustum(
			getDefaultValue("left").getFloat(),
			getDefaultValue("right").getFloat(),
			getDefaultValue("bottom").getFloat(),
			getDefaultValue("top").getFloat(),
			getDefaultValue("near").getFloat(),
			getDefaultValue("far").getFloat()
	));
	notifySequence();
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

//===-- Look At -----------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::LookAt>::isValid() const
{
	return ETransformState::Unknown;
	// \todo PF: check, if center is different than eye, up-vector is not zero,
	// and up is not collinear with camera direction center - eye
}

void TransformImpl<ETransformType::LookAt>::onReset()
{
	setInternalValue(glm::lookAt(
			getDefaultValue("eye").getVec3(),
			getDefaultValue("center").getVec3(),
			getDefaultValue("up").getVec3()
	));
	notifySequence();
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
}
