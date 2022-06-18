/**
 * \file Core/NodesTransformImpl.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \updates Petr Felkel, felkepet@fel.cvut.cz
 *
 * \brief Implementation of individual transformation functionality
 *
 * Each transform nodes represent 4x4 transformations. Each has two types of data, describing them:
 *    1. the 4x4 matrix itself (called the inner value and shown in the Full LOD)
 *    2. the value used in glm to define the transformation (called default values and shown in SetValues LOD)
 *
 *
 */

#include "TransformImpl.h"

#include <math.h>

//#include "pgr.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp> // Euler angle rotations
#include "Utils/Format.h"

#ifndef M_PI
/// define Pi for compatibility issues (MSVC vs GCC)
#define M_PI 3.14159f
#endif

namespace Core
{
//===-- Value masks -------------------------------------------------------===//
// Written in ROW order - flipped to COLUMN order in validateValue()!!!!
constexpr ValueMask g_ScaleMask = {
    VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO, VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_TranslateMask = {
    // row-order
    VM_ONE,  VM_ZERO, VM_ZERO, VM_ANY, VM_ZERO, VM_ONE,  VM_ZERO, VM_ANY,
    VM_ZERO, VM_ZERO, VM_ONE,  VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateXMask = {
    VM_ONE,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO,
    VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateYMask = {
    VM_ANY, VM_ZERO, VM_ANY, VM_ZERO, VM_ZERO, VM_ONE,  VM_ZERO, VM_ZERO,
    VM_ANY, VM_ZERO, VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_RotateZMask = {
    VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ONE,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_AxisAngleMask = {
    VM_ANY, VM_ANY, VM_ANY, VM_ZERO, VM_ANY,  VM_ANY,  VM_ANY,  VM_ZERO,
    VM_ANY, VM_ANY, VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_OrthoMask = {
    VM_ANY,  VM_ZERO, VM_ZERO, VM_ANY, VM_ZERO, VM_ANY,  VM_ZERO, VM_ANY,
    VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

constexpr ValueMask g_PerspectiveMask = {
    VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO,      VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO, VM_MINUS_ONE, VM_ZERO,
};

constexpr ValueMask g_FrustumMask = {
    VM_ANY,  VM_ZERO, VM_ANY, VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,       VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ANY, VM_ANY,  VM_ZERO, VM_ZERO, VM_MINUS_ONE, VM_ZERO,
};

constexpr ValueMask g_LookAtMask = {
    VM_ANY,  VM_ZERO, VM_ZERO, VM_ANY, VM_ZERO, VM_ANY,  VM_ZERO, VM_ANY,
    VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};
//===----------------------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Scale>::isValid() const
{
	bool result = validateValues(g_ScaleMask, m_internalData[0].getMat4());

	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result    = result && Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]);
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
		if (Math::areElementsSame(vec)) // todo epsilonEQ would be probably better
		{ setInternalValue(glm::scale(vec)); }
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

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec4& vec) { return setValue(glm::vec3(vec)); }

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val, glm::ivec2 coords)
{
	if (!canSetValue(g_ScaleMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (hasSynergies() && coords.x == coords.y && coords.x != 3) // any float on a 3x3 diagonal, not the 4x4 bottom-right corner
	{ setInternalValue(glm::scale(glm::vec3(val))); }
	else
	{
		setInternalValue(val, coords);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}
void TransformImpl<ETransformType::Scale>::setDefaultUniformScale(float val)
{
	Transformation::setDefaultValue("scale", glm::vec3(val));
};


void TransformImpl<ETransformType::Scale>::onReset()
{

	// Do not enable synergies on reset, as it would switch to a uniform scale
	// m_hasSynergies = true; // PFx: JH When setting X value in non-uniform scale -> this switch to uniform scale (due to enable synergies)
	m_isLocked = true;

	auto scale = getDefaultValue("scale").getVec3();

	if (m_hasSynergies)
	{
		scale.y = scale.x;
		scale.z = scale.x;
		/////setDefaultUniformScale(scale.x);  // infinite recursion
		m_defaultValues.at("scale").setValue(scale);
	}

	setInternalValue(glm::scale(scale));
	notifySequence();
}

//===-- Euler rotation around X axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerX>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_RotateXMask, mat);

	float angle       = glm::atan(-mat[2][1], mat[2][2]); // glm::atan executes ::std::atan2
	auto  expectedMat = glm::eulerAngleX(angle);

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

	auto mat                = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies

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
			mat[1][2]         = clampedVal; // repair the setup without synergies
			mat[2][1]         = -clampedVal;
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
			mat[1][1]         = clampedVal;
			mat[2][2]         = clampedVal;
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
			mat[1][2]         = -clampedVal;
			mat[2][1]         = clampedVal; // repair the setup without synergies
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
	m_hasSynergies = true;
	m_isLocked     = true;

	setInternalValue(glm::eulerAngleX(getDefaultValue("rotation").getFloat()));
	notifySequence();
}

//===-- Euler rotation around Y axis --------------------------------------===//
// PF Important - cos returns angles <0, M_PI> only - we have to use sin to get the whole circle
ETransformState TransformImpl<ETransformType::EulerY>::isValid() const
{
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_RotateYMask, mat);

	float angle       = glm::atan(mat[2][0], mat[2][2]); // glm::atan executes ::std::atan2
	auto  expectedMat = glm::eulerAngleY(angle);

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
	{ ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."}; }

	auto mat                = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies

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
			mat[0][2]         = clampedVal; // repair the setup without synergies
			mat[2][0]         = -clampedVal;
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
			mat[0][0]         = clampedVal;
			mat[2][2]         = clampedVal;
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
			mat[0][2]         = -clampedVal;
			mat[2][0]         = clampedVal; // repair the setup without synergies
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
	//enableSynergies(); // if (hasSynergies) only - but EulerXYZ and Scale HAVE synergies
	m_hasSynergies = true;
	m_isLocked     = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 1.0f, 0.0f)));
	notifySequence();
}

//===-- Euler rotation around Z axis --------------------------------------===//

ETransformState TransformImpl<ETransformType::EulerZ>::isValid() const
{
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_RotateZMask, mat);

	float angle       = glm::atan(mat[0][1], mat[0][0]); // glm::atan executes ::std::atan2
	auto  expectedMat = glm::eulerAngleZ(angle);

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
	{ ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."}; }

	// PF: remembering the halfspace sign for each box to avoid jumps during interaction with rotation matrix

	auto mat                = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies, works in all 4x4 coordinates

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
			mat[0][1]         = clampedVal; // repair the setup without synergies
			mat[1][0]         = -clampedVal;
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
			mat[0][0]         = clampedVal;
			mat[1][1]         = clampedVal;
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
			mat[0][1]         = -clampedVal;
			mat[1][0]         = clampedVal; // repair the setup without synergies
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
	//enableSynergies(); // if (hasSynergies) only - but EulerXYZ and Scale HAVE synergies
	m_hasSynergies = true;
	m_isLocked     = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 0.0f, 1.0f)));
	notifySequence();
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

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val) { return setValue(glm::vec3(val)); }

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec3& vec)
{
	setInternalValue(glm::translate(vec));
	m_defaultValues.at("translate").setValue(vec);  // update Defaults
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

	// update Default Values according to the Transform matrix 
	if (coords.x == 3 && coords.y != 3)  // last T column
	{
		auto & mapPos = m_defaultValues.at("translation");
		glm::vec3 v      = mapPos.getVec3();
		v[coords.y]      = val;
		mapPos.setValue(v);
	}

	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Translation>::onReset()
{
	m_isLocked = true;

	setInternalValue(glm::translate(getDefaultValue("translation").getVec3()));
	notifySequence();
}

//===-- Axis angle rotation -----------------------------------------------===//

ETransformState TransformImpl<ETransformType::AxisAngle>::isValid() const
{ // \todo - test after working edit during unlock

	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_AxisAngleMask, mat);

	// det = +-1
	result = result && Math::eq(abs(glm::determinant(mat)), 1.0f); // math::eq

	// axis <> vec3(0) = the rotation vector is of non-zero length
	glm::vec3 ax = getDefaultValue("axis").getVec3();
	result       = result && glm::dot(ax, ax) != 0.0f;

	// expected matrix
	float angle       = getDefaultValue("rotation").getFloat();
	auto  expectedMat = glm::rotate(angle, ax);

	result = result && Math::eq(expectedMat, mat);

	// translation is not zero
	return ETransformState(result);

	//return ETransformState::Unknown;
}


void TransformImpl<ETransformType::AxisAngle>::onReset()
{
	m_isLocked = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), getDefaultValue("axis").getVec3()));
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
	// any linear transformation (3x3) may be a rotation
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_AxisAngleMask, mat);

	// Check, if the quaternion is of unit length?
	glm::quat quaternion = getDefaultValue("quat").getQuat();
	result               = result && glm::dot(quaternion, quaternion) == 1.0f;

	// \todo check the angle too

	return ETransformState(result);
	//return ETransformState::Unknown;
}

void TransformImpl<ETransformType::Quat>::onReset()
{
	//m_isLocked = true; quaternion is never locked
	//m_hasSynergies = true; ///////////////// \todo PF split reset and set

	setInternalValue(glm::toMat4(m_normalized));
	notifySequence();
}

const glm::quat& TransformImpl<ETransformType::Quat>::getNormalized() const { return m_normalized; };

ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::quat& q)
{
	m_initialQuat = q;
	m_normalized  = glm::normalize(q);
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
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_OrthoMask, mat);

	float left   = getDefaultValue("left").getFloat();
	float right  = getDefaultValue("right").getFloat();
	float bottom = getDefaultValue("bottom").getFloat();
	float top    = getDefaultValue("top").getFloat();
	float near   = getDefaultValue("near").getFloat();
	float far    = getDefaultValue("far").getFloat();

	// simple check of the frustum borders
	result = result && (left < right) && (bottom < top) && (near < far);

	// expected matrix
	auto expectedMat = glm::ortho(left, right, bottom, top, near, far);
	result           = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void TransformImpl<ETransformType::Ortho>::onReset()
{
	m_isLocked = true;
	//m_hasSynergies = true;  // symmetrical frustum

	setInternalValue(glm::ortho(getDefaultValue("left").getFloat(), getDefaultValue("right").getFloat(),
	                            getDefaultValue("bottom").getFloat(), getDefaultValue("top").getFloat(),
	                            getDefaultValue("near").getFloat(), getDefaultValue("far").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_OrthoMask, coords, val))
	{ return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an editable field."}; }
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

//===-- Perspective -------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Perspective>::isValid() const
{
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_PerspectiveMask, mat); //checks -1 in the last row too

	float fov    = getDefaultValue("fov").getFloat();
	float aspect = getDefaultValue("aspect").getFloat();
	float near   = getDefaultValue("zNear").getFloat();
	float far    = getDefaultValue("zFar").getFloat();

	result = result && (near != 0.0f);
	result = result && (far > near);
	result = result && (aspect > 0.0f);
	result = result && (fov > 0.0f);

	// expected matrix
	auto expectedMat = glm::perspective(fov, aspect, near, far);
	result           = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void TransformImpl<ETransformType::Perspective>::onReset()
{
	m_isLocked     = true;
	m_hasSynergies = true; // symmetrical frustum

	setInternalValue(glm::perspective(getDefaultValue("fov").getFloat(), getDefaultValue("aspect").getFloat(),
	                                  getDefaultValue("zNear").getFloat(), getDefaultValue("zFar").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_PerspectiveMask, coords, val))
	{ return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"}; }
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

//===-- Frustum ------------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::Frustum>::isValid() const
{
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_FrustumMask, mat); //checks -1 in the last row too

	float left   = getDefaultValue("left").getFloat();
	float right  = getDefaultValue("right").getFloat();
	float bottom = getDefaultValue("bottom").getFloat();
	float top    = getDefaultValue("top").getFloat();
	float near   = getDefaultValue("near").getFloat();
	float far    = getDefaultValue("far").getFloat();

	// simple check of the frustum borders
	result = result && (left < right) && (bottom < top) && (near < far);
	result = result && (near != 0.0f);

	// expected matrix
	auto expectedMat = glm::frustum(left, right, bottom, top, near, far);
	result           = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void TransformImpl<ETransformType::Frustum>::onReset()
{
	m_isLocked     = true;
	m_hasSynergies = true; // symmetrical frustum

	setInternalValue(glm::frustum(getDefaultValue("left").getFloat(), getDefaultValue("right").getFloat(),
	                              getDefaultValue("bottom").getFloat(), getDefaultValue("top").getFloat(),
	                              getDefaultValue("near").getFloat(), getDefaultValue("far").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setValue(float val, glm::ivec2 coords)
{
	if (!validateValue(g_FrustumMask, coords, val))
	{ return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"}; }
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

//===-- Look At -----------------------------------------------------------===//

ETransformState TransformImpl<ETransformType::LookAt>::isValid() const
{
	auto& mat    = m_internalData[0].getMat4();
	bool  result = validateValues(g_LookAtMask, mat); //checks -1 in the last row too

	// check, if center is different than eye, up-vector is not zero,
	// and up is not collinear with camera direction center - eye

	glm::vec3 eye    = getDefaultValue("eye").getVec3();
	glm::vec3 center = getDefaultValue("center").getVec3();
	glm::vec3 up     = getDefaultValue("up").getVec3();

	// expected matrix
	auto expectedMat = glm::lookAt(eye, center, up);
	result           = result && Math::eq(expectedMat, mat);

	// definition parameters
	glm::vec3 direction     = center - eye;
	float     directionSize = glm::dot(direction, direction);
	float     upSize        = glm::dot(up, up);
	if ((directionSize == 0.0f) || (upSize == 0.0f)) { result = false; }
	else
	{
		direction /= glm::sqrt(directionSize); //normalize
		up /= glm::sqrt(upSize);
		result = result && (glm::dot(direction, up) != 1.0f); // not parallel
	}

	return ETransformState(result);
}

void TransformImpl<ETransformType::LookAt>::onReset()
{
	m_isLocked = true;
	//m_hasSynergies = true; // no meaning now

	setInternalValue(glm::lookAt(getDefaultValue("eye").getVec3(), getDefaultValue("center").getVec3(),
	                             getDefaultValue("up").getVec3()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::LookAt>::setValue(float val, glm::ivec2 coords)
{
	if (isLocked()) { return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"}; }
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}
} // namespace Core
