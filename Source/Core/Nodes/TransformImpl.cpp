#include "TransformImpl.h"

#include "Utils/Math.h"
#include <math.h>

// #include "pgr.h"
#include "Utils/Format.h"
#include <glm/gtx/matrix_interpolation.hpp> // AxisAngle
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp> // Euler angle rotations

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
    // symmetric perspective = inverse scale + tz + (-1)
    VM_ANY,  VM_ZERO, VM_ZERO, VM_ZERO, VM_ZERO, VM_ANY,  VM_ZERO,      VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,  VM_ZERO, VM_ZERO, VM_MINUS_ONE, VM_ZERO,
};

constexpr ValueMask g_FrustumMask = {
    VM_ANY,  VM_ZERO, VM_ANY, VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,       VM_ZERO,
    VM_ZERO, VM_ZERO, VM_ANY, VM_ANY,  VM_ZERO, VM_ZERO, VM_MINUS_ONE, VM_ZERO,
};

constexpr ValueMask g_LookAtMask = {
    // rotation + translation
    VM_ANY, VM_ANY, VM_ANY, VM_ANY, VM_ANY,  VM_ANY,  VM_ANY,  VM_ANY,
    VM_ANY, VM_ANY, VM_ANY, VM_ANY, VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
};

// constexpr ValueMask g_LookAtMask = { // scale + translation
//     VM_ANY,  VM_ZERO, VM_ZERO, VM_ANY,
//     VM_ZERO, VM_ANY,  VM_ZERO, VM_ANY,
//     VM_ZERO, VM_ZERO, VM_ANY,  VM_ANY,
//     VM_ZERO, VM_ZERO, VM_ZERO, VM_ONE,
// };

//===----------------------------------------------------------------------===//

/**
 * \brief Test scale matrix:
 * (1) only values on diagonal can differ from identity matrix
 * (2) synergies must have equal values on the diagonal
 * \return
 */
bool TransformImpl<ETransformType::Scale>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	bool result = validateValues(g_ScaleMask, m_internalData[0].getMat4());

	if (hasSynergies())
	{
		// matrix inner consistency - i.e., equal values on the diagonal
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]);

		auto& defaultValue = getDefaultValue("scale").getVec3();
		// auto& defaultValue = m_defaultValues["scale"];

		bool scaleResult = Math::eq(defaultValue[0], defaultValue[1]) && Math::eq(defaultValue[1], defaultValue[2]);

		// result = result && Math::eq(defaultValue[0], defaultValue[1]) &&
		//          Math::eq(defaultValue[1],defaultValue[2]);

		// debug print
		// std::cout << "mat[0][0] =" << mat[0][0] << std::endl;
		// std::cout << "mat[1][1] =" << mat[1][1] << std::endl;
		// std::cout << "mat[2][2] =" << mat[2][2] << std::endl;
		// std::cout << "   => Matrix synergies are " << (result ? "valid" : "wrong")
		//   << std::endl;

		// std::cout << "Default value scale = (" << defaultValue[0] << ", "
		//   << defaultValue[1] << ", " << defaultValue[2] << ") " << std::endl;

		// std::cout << "   => Default values synergies are " << (scaleResult ? "valid" : "wrong") << std::endl;

		result = result && scaleResult;
	}

	// consistency with defaults -omitted
	// consistency defaults <--> matrix   - omitted

	return result;
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val)
{
	// notifySequence();
	// return Node::setValue(glm::vec3(val));

	return setValue(glm::vec3(val));
}

ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec3& vec)
{
	if (hasSynergies() && !Math::areElementsSame(vec)) // todo epsilonEQ would be probably better
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Given vector does not have all three values equal."};
	}
	else
	{
		setInternalValue(glm::scale(vec));     // new matrix ONLY
		setDefaultValueNoUpdate("scale", vec); // update Defaults ONLY - no matrix
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
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}
	/// if (isLocked()) // can change the diagonal only - done by canSetValue()

	if (coords.x == coords.y && coords.x != 3) // any float on a 3x3 diagonal, not the 4x4 bottom-right corner
	{
		if (hasSynergies())
		{
			setInternalValue(val, glm::vec2(0, 0)); // change matrix diagonal only
			setInternalValue(val, glm::vec2(1, 1));
			setInternalValue(val, glm::vec2(2, 2));
			setDefaultValueNoUpdate("scale", glm::vec3(val)); // set default scale without matrix update
		}
		else
		{
			setInternalValue(val, coords);                   // single value in matrix
			auto scale = getDefaultValue("scale").getVec3(); // change single value on index [coords.x] in
			                                                 // default scale vector
			scale[coords.x] = val;                           // 0,1,2 as sx, sy, sz
			setDefaultValueNoUpdate("scale",
			                        scale); // set default scale without matrix update
		}
	}
	else // not on diagonal => do not change the default scale
	{
		setInternalValue(val, coords);
	}

	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}
void TransformImpl<ETransformType::Scale>::setDefaultUniformScale(float val)
{
	Transform::setDefaultValue("scale", glm::vec3(val));
};

void TransformImpl<ETransformType::Scale>::initDefaults()
{
	setDefaultValueNoUpdate("scale", glm::vec3{1.0f, 1.0f, 1.0f});
}

// void TransformImpl<ETransformType::Scale>::onResetMatrixFromDefaults()
void TransformImpl<ETransformType::Scale>::resetMatrixFromDefaults()
{

	// Do not enable synergies on reset, as it would switch to a uniform scale
	// m_hasSynergies = true; // PFx: JH When setting X value in non-uniform scale
	// -> this switch to uniform scale (due to enable synergies)
	m_isLocked = true;

	auto scale = getDefaultValue("scale").getVec3();


	// delete this check?
	if (m_hasSynergies && !Math::areElementsSame(scale))
	{
		// \todo - this should never happen - the synergies must be checked in
		// setValue and in setDefault!!!!!!!!
		scale.y = scale.x;
		scale.z = scale.x;
		/////setDefaultUniformScale(scale.x);  // infinite recursion
		getDefaultValueMut("scale").setValue(scale);
	}


	setInternalValue(glm::scale(scale));
	notifySequence();
}

//===-- Euler rotation around X axis --------------------------------------===//

bool TransformImpl<ETransformType::EulerX>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateXMask, mat);

	// matrix inner consistency
	float angle = glm::atan(-mat[2][1], mat[2][2]); // glm::atan executes ::std::atan2
	float angle2 = glm::atan(mat[1][2], mat[1][1]); // glm::atan executes ::std::atan2
	// result       = result && Math::eq(angle, angle2);

	auto expectedMat = glm::eulerAngleX(angle);

	// consistency defaults <--> matrix
	float defaultAngle = getDefaultValue("rotation").getFloat();
	result = result && Math::eq(angle, defaultAngle);

	expectedMat = glm::eulerAngleX(defaultAngle);
	result = result && Math::eq(expectedMat, mat);

	return result;
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
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies

	if (hasSynergies())
	{
		////F Commented out
		/// 1) the return value is thrown away anyway
		///	2) should not fail for unlocked transforms
		// if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		// }

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f);

		if (coords == glm::ivec2(1, 2))
		{
			// -sin(T)
			mat[1][2] = clampedVal; // repair the setup without synergies
			mat[2][1] = -clampedVal;
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
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
				sin *= -1.0f; // allow negative sin values while changing cos - avoid
				              // jump in rotation
			mat[1][2] = sin;
			mat[2][1] = -sin;
		}
		else if (coords == glm::ivec2(2, 1))
		{
			// sin(T)
			mat[1][2] = -clampedVal;
			mat[2][1] = clampedVal; // repair the setup without synergies
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
			mat[1][1] = cos;
			mat[2][2] = cos;
		}

		float angle = glm::atan(-mat[2][1], mat[2][2]); // glm::atan executes ::std::atan2
		setDefaultValueNoUpdate("rotation", angle);
	}

	setInternalValue(mat);

	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerX>::initDefaults()
{
	setDefaultValueNoUpdate("rotation", 0.0f);
}

void TransformImpl<ETransformType::EulerX>::resetMatrixFromDefaults()
{
	m_hasSynergies = true;
	m_isLocked = true;

	setInternalValue(glm::eulerAngleX(getDefaultValue("rotation").getFloat()));
	notifySequence();
}

//===-- Euler rotation around Y axis --------------------------------------===//
// PF Important - cos returns angles <0, M_PI> only - we have to use sin to get
// the whole circle
bool TransformImpl<ETransformType::EulerY>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateYMask, mat);

	// matrix inner consistency
	float angle = glm::atan(mat[2][0], mat[2][2]);   // glm::atan executes ::std::atan2
	float angle2 = glm::atan(-mat[0][2], mat[0][0]); // glm::atan executes ::std::atan2
	result = result && Math::eq(angle, angle2);

	auto expectedMat = glm::eulerAngleY(angle);

	result = result && Math::eq(expectedMat, mat);

	// consistency defaults <--> matrix
	float defaultAngle = getDefaultValue("rotation").getFloat();
	result = result && Math::eq(angle, defaultAngle);

	expectedMat = glm::eulerAngleY(defaultAngle);
	result = result && Math::eq(expectedMat, mat);

	return result;
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
	// if (!validateValue(g_RotateYMask, coords, val))
	if (!canSetValue(g_RotateYMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies

	if (hasSynergies())
	{
		// PF Commented out - se the comment at EulerX
		// if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		// }

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f);

		if (coords == glm::ivec2(0, 2))
		{
			// -sin(T)
			mat[0][2] = clampedVal; // repair the setup without synergies
			mat[2][0] = -clampedVal;
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
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
				sin *= -1.0f; // allow negative sin values while changing cos - avoid
				              // jump in rotation
			mat[0][2] = -sin;
			mat[2][0] = sin;
		}
		else if (coords == glm::ivec2(2, 0))
		{
			// sin(T)
			mat[0][2] = -clampedVal;
			mat[2][0] = clampedVal; // repair the setup without synergies
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
			mat[0][0] = cos;
			mat[2][2] = cos;
		}

		float angle = glm::atan(mat[2][0], mat[2][2]); // glm::atan executes ::std::atan2
		setDefaultValueNoUpdate("rotation", angle);
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerY>::initDefaults()
{
	setDefaultValueNoUpdate("rotation", 0.0f);
}
void TransformImpl<ETransformType::EulerY>::resetMatrixFromDefaults()
{
	// enableSynergies(); // if (hasSynergies) only - but EulerXYZ and Scale HAVE
	// synergies
	m_hasSynergies = true;
	m_isLocked = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 1.0f, 0.0f)));
	notifySequence();
}

//===-- Euler rotation around Z axis --------------------------------------===//

bool TransformImpl<ETransformType::EulerZ>::isValid() const
{
	// check the basic matrix values 0, 1, -1, any
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_RotateZMask, mat);

	// matrix inner consistency
	float angle = glm::atan(mat[0][1], mat[0][0]);   // glm::atan executes ::std::atan2
	float angle2 = glm::atan(-mat[1][0], mat[1][1]); // glm::atan executes ::std::atan2
	result = result && Math::eq(angle, angle2);

	auto expectedMat = glm::eulerAngleZ(angle);

	result = result && Math::eq(expectedMat, mat);

	// consistency defaults <--> matrix
	float defaultAngle = getDefaultValue("rotation").getFloat();
	result = result && Math::eq(angle, defaultAngle);

	expectedMat = glm::eulerAngleZ(defaultAngle);
	result = result && Math::eq(expectedMat, mat);

	return result;
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
	// if (!validateValue(g_RotateZMask, coords, val))
	if (!canSetValue(g_RotateZMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	// PF: remembering the halfspace sign for each box to avoid jumps during
	// interaction with rotation matrix

	auto mat = getData().getMat4();
	mat[coords.x][coords.y] = val; // unlimited value, clamped if synergies, works
	                               // in all 4x4 coordinates

	if (hasSynergies())
	{
		// PF Commented out - se the comment at EulerX
		// if (!Math::withinInterval(val, -1.0f, 1.0f))
		//{
		//	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		//												"Value must be within [-1.0, 1.0] interval."};
		// }

		const float clampedVal = glm::clamp(val, -1.0f, 1.0f);

		if (coords == glm::ivec2(0, 1))
		{
			// -sin(T)
			mat[0][1] = clampedVal; // repair the setup without synergies
			mat[1][0] = -clampedVal;
			halfspaceSign.sin = glm::sign(clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
		else if (coords == glm::ivec2(0, 0) || coords == glm::ivec2(1, 1))
		{
			// cos(T)
			mat[0][0] = clampedVal;
			mat[1][1] = clampedVal;
			halfspaceSign.cos = glm::sign(clampedVal);

			auto sin = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.sin < 0.0f)
				sin *= -1.0f; // allow negative sin values while changing cos - avoid
				              // jump in rotation
			mat[0][1] = sin;
			mat[1][0] = -sin;
		}
		else if (coords == glm::ivec2(1, 0))
		{
			// sin(T)
			mat[0][1] = -clampedVal;
			mat[1][0] = clampedVal; // repair the setup without synergies
			halfspaceSign.sin = glm::sign(-clampedVal);

			auto cos = sqrt(1.0f - (clampedVal * clampedVal));
			if (halfspaceSign.cos < 0.0f)
				cos *= -1.0f; // allow negative cos values while changing sin - avoid
				              // jump in rotation
			mat[0][0] = cos;
			mat[1][1] = cos;
		}
		float angle = glm::atan(mat[0][1], mat[0][0]); // glm::atan executes ::std::atan2
		setDefaultValueNoUpdate("rotation", angle);
	}

	setInternalValue(mat);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::EulerZ>::initDefaults()
{
	setDefaultValueNoUpdate("rotation", 0.0f);
}

void TransformImpl<ETransformType::EulerZ>::resetMatrixFromDefaults()
{
	// enableSynergies(); // if (hasSynergies) only - but EulerXYZ and Scale HAVE
	// synergies
	m_hasSynergies = true;
	m_isLocked = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), glm::vec3(0.0f, 0.0f, 1.0f)));
	notifySequence();
}

//===-- Translation -------------------------------------------------------===//

bool TransformImpl<ETransformType::Translation>::isValid() const
{
	bool result = validateValues(g_TranslateMask, m_internalData[0].getMat4());
	// PF - err - translate has no synergies
	// if (hasSynergies())
	//{
	//	auto& mat = m_internalData[0].getMat4();
	//	result = result && Math::eq(mat[3][0], mat[3][1]) && Math::eq(mat[3][1],
	// mat[3][2]);
	//}

	return result;
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val)
{
	return setValue(glm::vec3(val)); // sets the defaults
}

ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec3& vec)
{

	setInternalValue(glm::translate(vec));
	// m_defaultValues.at("translation").setValue(vec);  // update Defaults and
	// Matrix

	setDefaultValueNoUpdate("translation", vec); // update Defaults ONLY
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
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);

	// update Default Values according to the Transform matrix

	if (coords.x == 3 && coords.y != 3) // last T column
	{
		auto& mapPos = getDefaultValueMut("translation");
		glm::vec3 v = mapPos.getVec3();
		v[coords.y] = val;
		mapPos.setValue(v); // direct set of the default value
	}

	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void TransformImpl<ETransformType::Translation>::initDefaults()
{
	setDefaultValueNoUpdate("translation", glm::vec3{0.0f, 0.0f, 0.0f});
}

void TransformImpl<ETransformType::Translation>::resetMatrixFromDefaults()
{
	m_isLocked = true;

	setInternalValue(glm::translate(getDefaultValue("translation").getVec3()));
	notifySequence();
}

//===-- Axis angle rotation -----------------------------------------------===//

bool TransformImpl<ETransformType::AxisAngle>::isValid() const
{ // \todo - test after working edit during unlock
	// check the basic matrix values 0, 1, -1, any
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_AxisAngleMask, mat);

	// matrix inner consistency
	// det = +-1
	result = result && Math::eq(abs(glm::determinant(mat)), 1.0f); // math::eq

	// consistency of defaults
	// axis <> vec3(0) = the rotation vector is of non-zero length
	glm::vec3 ax = getDefaultValue("axis").getVec3();
	result = result && glm::dot(ax, ax) != 0.0f;

	// consistency defaults <--> matrix
	// expected matrix
	float angle = getDefaultValue("rotation").getFloat();
	auto expectedMat = glm::rotate(angle, ax);

	result = result && Math::eq(expectedMat, mat);

	// translation is not zero
	return result;
}

void TransformImpl<ETransformType::AxisAngle>::initDefaults()
{
	setDefaultValueNoUpdate("axis", glm::vec3{0.0f, 1.0f, 0.0f});
	setDefaultValueNoUpdate("rotation", 0.0f);
}

void TransformImpl<ETransformType::AxisAngle>::resetMatrixFromDefaults()
{
	m_isLocked = true;

	setInternalValue(glm::rotate(getDefaultValue("rotation").getFloat(), getDefaultValue("axis").getVec3()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(const glm::mat4& mat)
{
	ValueSetResult result;

	// Axis angle cannot set the value individually for each coordinate in the matrix as the 3x3 rotation matrix has to
	// be evaluated at once, piecewise evaluation would fail as the matrix would be invalid at times (not orthonormal,
	// skewed etc.)

	// Check if the 3x3 rotation matrix can be edited ((DR) it should always be though?)
	for (int c = 0; c < 3; ++c)
	{
		for (int r = 0; r < 3; ++r)
		{
			if (!canSetValue(g_AxisAngleMask, glm::ivec2(c, r), mat[c][r]))
			{
				return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
				                      "Cannot set value at given coordinates."};
			}
		}
	}

	glm::mat4 oldValue = getInternalData().getMat4();
	float oldAngle = getDefaultValue("rotation").getFloat();
	glm::vec3 oldAxis = getDefaultValue("axis").getVec3();

	// Extract current axis angle from new matrix
	glm::vec3 axis;
	float angle;
	glm::axisAngle(mat, axis, angle);
	// Recreate the matrix using the extracted axis and angle to reduce imprecision from float math
	glm::mat4 newMat = glm::rotate(angle, axis);

	// Set the new matrix
	setInternalValue(newMat);

	// Update default values
	setDefaultValueNoUpdate("rotation", angle);
	setDefaultValueNoUpdate("axis", axis);

	// Check validity
	if (!isValid())
	{
		// Revert changes
		setInternalValue(oldValue);
		setDefaultValueNoUpdate("rotation", oldAngle);
		setDefaultValueNoUpdate("axis", oldAxis);
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "AxisAngle synergy not valid!"};
	}

	notifySequence();
	return ValueSetResult{ValueSetResult::Status::Ok};
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

// #define PF_DEBUG

//===-- Quaternion rotation -----------------------------------------------===//
// todo - what should isValid without synergies return ofr |q| != 1?
// most probably a corrupted quaternion
bool TransformImpl<ETransformType::Quat>::isValid() const
{
#ifdef PF_DEBUG
	std::cerr << "Entering isValid() ===============" << std::endl;
#endif


	// check the basic matrix values 0, 1, -1, any
	// any linear transformation (3x3) may be a rotation
	auto& mat = m_internalData[0].getMat4();
	// auto& mat = getInternalData().getMat4();
	// auto& mat = data().getMat4();
	bool result = validateValues(g_AxisAngleMask, mat); // 3x3 rotational matrix

#ifdef PF_DEBUG
	if (!result)
	{
		std::cerr << "validateValues" << std::endl;
		std::cerr << Utils::toString(mat, true) << std::endl;
	}
#endif


	// matrix inner consistency
	// result = result && Math::eq(glm::determinant(mat), 1.0f); // det == 1
	// result = result && Math::eq(glm::determinant(mat), 1.0f, Math::FACTOR_FOR_DETERMINANT); // det == 1
	result = result && Math::eqToOne(glm::determinant(mat)); // det == 1

#ifdef PF_DEBUG
	if (!result)
	{
		std::cerr << std::endl;
		std::cerr << "determinant " << glm::determinant(mat) << " != 1" << std::endl;
		std::cerr << Utils::toString(mat, true) << std::endl;
	}
#endif

	// consistency of defaults
	// 1) Check, if the default quaternion is of unit length?
	const glm::quat quaternion = getDefaultValue("quat").getQuat();

	result = result && Math::isNormalized(quaternion);
#ifdef PF_DEBUG
	if (!result)
	{
		std::cerr << "quat NOT normalized" << std::endl;
		std::cerr << Utils::toString(quaternion) << std::endl;
		return result;
	}
#endif


	// 2) normalized helper
	result = result && Math::eq(m_normalized, glm::normalize(quaternion), Math::FACTOR_SIMILAR);
#ifdef PF_DEBUG
	if (!result)
	{
		std::cerr << "m_normalized != normalized(quaternion)" << std::endl;
		std::cerr << Utils::toString(mat, true) << std::endl;
		return result;
	}
#endif


	// \todo check the angle too...
	// consistency defaults <--> matrix
	// matrix from a non-unit quaternion would also be wrong (non-unity of quat is
	// tested above)
	result = result && Math::eq(mat, glm::toMat4(quaternion));
#ifdef PF_DEBUG
	if (!result)
	{
		std::cerr << "stored mat != mat(quaternion)" << std::endl;
		std::cerr << Utils::toString(mat, true) << std::endl;
	}
#endif

	return result;
}

void TransformImpl<ETransformType::Quat>::initDefaults()
{
	setDefaultValueNoUpdate("quat", glm::quat{1.0f, 0.0f, 0.0f, 0.0f});
	m_normalized = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
	enableSynergies();
}

void TransformImpl<ETransformType::Quat>::resetMatrixFromDefaults()
{
	m_isLocked = true;

	// todo - remove from here and unify the setters - setValue() and setDefaultValueNoUpdate()
	m_normalized = glm::normalize(getQuat());

	setInternalValue(glm::toMat4(m_normalized)); // matrix
	notifySequence();
}

const glm::quat& TransformImpl<ETransformType::Quat>::getQuat() const
{
	// todo možná spíš nechat return getDefaultValue("quat").getQuat();, aby to
	// skokem nemìnilo obsah if (hasSynergies()) 	return m_normalized;
	//  else
	return getDefaultValue("quat").getQuat();
};

const glm::quat& TransformImpl<ETransformType::Quat>::getNormalizedQuat() const
{
	return m_normalized;
};

ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::quat& q)
{
	////m_initialQuat = q;
	m_normalized = glm::normalize(q);

	if (hasSynergies())
		setDefaultValueNoUpdate("quat", m_normalized); // normalized
	else
		setDefaultValueNoUpdate("quat", q); // not normalized

	setInternalValue(glm::toMat4(m_normalized)); // matrix

	return ValueSetResult{};
}

// ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::vec4& vec)
//{
//	// glm::quat q(vec);
//	// return setValue(glm::quat(q));
//	return setValue(glm::quat(vec)); // todo - is wrong - replace with (w,x,y,z)
// }

ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::mat4& mat)
{
	return setValue(glm::quat_cast(mat));
}

void TransformImpl<ETransformType::Quat>::setDefaultValueWithSynergies(const std::string& name, Core::Data&& val)
{
	constexpr float HYSTERESIS_ONE = 1.2f;
	assert(hasSynergies());                      // called from setValues after this check
	assert(val.opValueType == EValueType::Quat); // \todo future - add float for w,x,y,z

	// std::cout << "Pretizena virtualni funkce pro Quat" << std::endl;

	if (name != "quat")
		return;

	const glm::quat& oldVal = getDefaultValue("quat").getQuat();
	const glm::quat newVal = val.getQuat();

	glm::quat correctedNewVal = glm::normalize(newVal); // replaces 0000 with 1000

	if (abs(newVal.x) > HYSTERESIS_ONE)
	{
		correctedNewVal.x = glm::clamp(newVal.x, -1.0f, 1.0f);
		correctedNewVal.y = 0.0f;
		correctedNewVal.z = 0.0f;
		correctedNewVal.w = 0.0f;
	}
	else if (abs(newVal.y) > HYSTERESIS_ONE)
	{
		correctedNewVal.y = glm::clamp(newVal.y, -1.0f, 1.0f);
		correctedNewVal.x = 0.0f;
		correctedNewVal.z = 0.0f;
		correctedNewVal.w = 0.0f;
	}
	else if (abs(newVal.z) > HYSTERESIS_ONE)
	{
		correctedNewVal.z = glm::clamp(newVal.z, -1.0f, 1.0f);
		correctedNewVal.x = 0.0f;
		correctedNewVal.y = 0.0f;
		correctedNewVal.w = 0.0f;
	}
	else if (abs(newVal.w) > HYSTERESIS_ONE)
	{
		correctedNewVal.w = glm::clamp(newVal.w, -1.0f, 1.0f);
		correctedNewVal.x = 0.0f;
		correctedNewVal.y = 0.0f;
		correctedNewVal.z = 0.0f;
	}

	// Process the synergies
	m_normalized = glm::normalize(correctedNewVal); /// todo do it per partes

	// setDefaultValueNoUpdate(name, newVal);
	setDefaultValueNoUpdate(name, m_normalized);
}

//===-- Orthographic projection -------------------------------------------===//

bool TransformImpl<ETransformType::Ortho>::isValid() const
{
	// rough matrix check
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_OrthoMask, mat);

	const float left = getDefaultValue("left").getFloat();
	const float right = getDefaultValue("right").getFloat();
	const float bottom = getDefaultValue("bottom").getFloat();
	const float top = getDefaultValue("top").getFloat();
	const float near = getDefaultValue("near").getFloat();
	const float far = getDefaultValue("far").getFloat();

	// check the defaults: simple check of the frustum borders
	result = result && (left < right) && (bottom < top) && (near < far);

	// matrix match the defaults
	constexpr float ROUGH_EPSILON = 0.001f;
	const auto expectedMat = glm::ortho(left, right, bottom, top, near, far);
	result = result && Math::eq(expectedMat, mat, ROUGH_EPSILON);

	return result;
}

void TransformImpl<ETransformType::Ortho>::initDefaults()
{
	setDefaultValueNoUpdate("left", -5.0f);
	setDefaultValueNoUpdate("right", 5.0f);
	setDefaultValueNoUpdate("bottom", -5.0f);
	setDefaultValueNoUpdate("top", 5.0f);
	setDefaultValueNoUpdate("near", 1.0f);
	setDefaultValueNoUpdate("far", 10.0f);
}


void TransformImpl<ETransformType::Ortho>::setDefaultValueWithSynergies(const std::string& name, Core::Data&& val)
{
	assert(hasSynergies()); // called from setValues after this check
	assert(val.opValueType == EValueType::Float);

	std::cout << "Pretizena virtualni funkce pro Frustum" << std::endl;

	float newVal = val.getFloat();
	setDefaultValueNoUpdate(name, newVal);

	// Process the synergies
	if (name == "left")
	{
		setDefaultValueNoUpdate("right", -newVal);
	}
	else if (name == "right")
	{
		setDefaultValueNoUpdate("left", -newVal);
	}
	else if (name == "top")
	{
		setDefaultValueNoUpdate("bottom", -newVal);
	}
	else if (name == "bottom")
	{
		setDefaultValueNoUpdate("top", -newVal);
	}
	// "near" and "far" are not updated
}
void TransformImpl<ETransformType::Ortho>::resetMatrixFromDefaults()
{
	m_isLocked = true;

	setInternalValue(glm::ortho(getDefaultValue("left").getFloat(), getDefaultValue("right").getFloat(),
	                            getDefaultValue("bottom").getFloat(), getDefaultValue("top").getFloat(),
	                            getDefaultValue("near").getFloat(), getDefaultValue("far").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Ortho>::setValue(float val, glm::ivec2 coords)
{
	if (!canSetValue(g_OrthoMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	setInternalValue(val, coords);
	notifySequence();

	// update the defaults to match the perspective matrix
	// m[0,0] = 2/(R-L)      => w = R-L, newR = newC * w, newL = (newC-1) * w
	// m[1,1] = 2/(T-B)      =>
	// m[2,2] = -2/(f-n)
	// m[3,0] = -(R+L)/(R-L)
	// m[3,1] = -(T+B)/(T-B)
	// m[3,2] = -(f+n)/(f-n)
	// m[3,3] = 1
	// F=cotan(0.5*fovy) = 1 / tan(0.5*fovy)   => FOVY = 2*atan(1/F]

	auto& mat = m_internalData[0].getMat4();

	// ROW 0
	if (coords == glm::ivec2(0, 0)) // fix B, changing A
	                                // --- changing scale => changing width
	{
		const auto left = getDefaultValue("left").getFloat();
		const auto right = getDefaultValue("right").getFloat();

		const auto width = getDefaultValue("right").getFloat() - getDefaultValue("left").getFloat();
		const auto newA = mat[0][0];
		if (newA != 0.0f)
		{
			const auto newL = (2.0f * left / (width * newA));
			const auto newR = (2.0f * right / (width * newA));
			setDefaultValueNoUpdate("left", newL);  // newL =
			setDefaultValueNoUpdate("right", newR); // newR =
		}
	}

	if (coords == glm::ivec2(3, 0)) // fix A, changing B
	{
		const auto left = getDefaultValue("left").getFloat();
		const auto right = getDefaultValue("right").getFloat();

		const auto width = getDefaultValue("right").getFloat() - getDefaultValue("left").getFloat();
		const auto newB = mat[3][0];

		const auto newL = (width / 2.0f) * (-1.0f - newB); // newL = (R-L)/2 * (-1-newB)
		const auto newR = (width / 2.0f) * (1.0f - newB);  // newR = (R-L)/2 * (+1-newB)
		;
		// const auto newR = newL + width;
		setDefaultValueNoUpdate("left", newL);  // newL =
		setDefaultValueNoUpdate("right", newR); // newR =
	}

	// ROW 1
	if (coords == glm::ivec2(1, 1)) // fix B, changing A
	                                // --- changing scale => changing height
	{
		const auto bottom = getDefaultValue("bottom").getFloat();
		const auto top = getDefaultValue("top").getFloat();

		const auto height = getDefaultValue("top").getFloat() - getDefaultValue("bottom").getFloat();
		const auto newA = mat[1][1];
		if (newA != 0.0f)
		{
			const auto newL = (2.0f * bottom / (height * newA));
			const auto newR = (2.0f * top / (height * newA));
			setDefaultValueNoUpdate("bottom", newL); // newL =
			setDefaultValueNoUpdate("top", newR);    // newR =
		}
	}

	if (coords == glm::ivec2(3, 1)) // fix A, changing B
	{
		const auto bottom = getDefaultValue("bottom").getFloat();
		const auto top = getDefaultValue("top").getFloat();

		const auto height = getDefaultValue("top").getFloat() - getDefaultValue("bottom").getFloat();
		const auto newB = mat[3][1];

		const auto newL = (height / 2.0f) * (-newB - 1.0f); // newL = (R-L)/2 * (newB-1)
		// const auto newR = newL + height;
		const auto newR = newL + height;
		setDefaultValueNoUpdate("bottom", newL); // newL =
		setDefaultValueNoUpdate("top", newR);    // newR =
	}

	// ROW 2
	else if (coords == glm::ivec2(2, 2)) // Z-scale:   A -> A', B = B'
	{
		const auto near = getDefaultValue("near").getFloat();
		const auto far = getDefaultValue("far").getFloat();

		const auto depth = getDefaultValue("far").getFloat() - getDefaultValue("near").getFloat();
		const auto newA = mat[2][2];
		if (newA != 0.0f)
		{
			const auto newNear = (-2.0f * near / (depth * newA));
			const auto newFar = (-2.0f * far / (depth * newA));

			setDefaultValueNoUpdate("near", newNear);
			setDefaultValueNoUpdate("far", newFar);
		}
	}
	else if (coords == glm::ivec2(3, 2)) // Z-offset:   A=A', B -> B'
	{
		const auto depth = getDefaultValue("far").getFloat() - getDefaultValue("near").getFloat();
		const auto newB = mat[3][2];

		auto newNear = (-depth / 2.0f) * (1.0f + newB); // -1/2 * (f-n) *(1 + newB)
		auto newFar = (depth / 2.0f) * (1.0f - newB);   //  1/2 * (f-n) *(1 - newB)
		setDefaultValueNoUpdate("near", newNear);
		setDefaultValueNoUpdate("far", newFar);
	}
	///--------------------------to

	return ValueSetResult{ValueSetResult::Status::Ok};
}

//===-- Perspective -------------------------------------------------------===//

bool TransformImpl<ETransformType::Perspective>::isValid() const
{
	// has no synergies, it is symmetrical
	// update of far and near has problems with the precision -r rough epsilon
	// introduced

	// check the basic matrix values 0, 1, -1, any
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_PerspectiveMask, mat); // checks 0,0,-1,0 in the last row

	// matrix inner consistency - nonzero scale and z-translate
	result = result && (mat[0][0] * mat[1][1] * mat[2][2] * mat[3][2] != 0.0f);

	// consistency of defaults
	float fovy = getDefaultValue("fovy").getFloat();
	float aspect = getDefaultValue("aspect").getFloat();
	float near = getDefaultValue("near").getFloat();
	float far = getDefaultValue("far").getFloat();

	// result = result && (near != 0.0f);
	result = result && (near > 0.0f); // stronger condition than  near != 0
	result = result && (far > near);
	result = result && (aspect > 0.0f);
	result = result && (fovy > 0.0f);

	// consistency defaults <--> matrix
	constexpr float roughEpsilon = 0.001f;
	const auto expectedMat = glm::perspective(fovy, aspect, near, far);
	result = result && Math::eq(expectedMat, mat, roughEpsilon);

	return result;
}

void TransformImpl<ETransformType::Perspective>::initDefaults()
{
	setDefaultValueNoUpdate("fovy", glm::radians(70.0f));
	setDefaultValueNoUpdate("aspect", 1.33f);
	setDefaultValueNoUpdate("near", 1.0f);
	setDefaultValueNoUpdate("far", 10.0f); //  update matrix
}

void TransformImpl<ETransformType::Perspective>::resetMatrixFromDefaults()
{
	m_isLocked = true;
	// m_hasSynergies = true; // perspective has a symmetrical frustum from
	// definition \todo We must decide

	setInternalValue(glm::perspective(getDefaultValue("fovy").getFloat(), getDefaultValue("aspect").getFloat(),
	                                  getDefaultValue("near").getFloat(), getDefaultValue("far").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Perspective>::setValue(float val, glm::ivec2 coords)
{
	if (!canSetValue(g_PerspectiveMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	setInternalValue(val, coords);
	notifySequence();

	// update the defaults to match the perspective matrix
	// m[0,0] = F/aspect                  => newAspect = (F / newM[0][0]),   FOVY
	// = const. m[1,1] = F                         => newAspect = newM[1,1] /
	// m[0,0], FOVY = 2*atan(1/F] m[2,2] = -(f+n)/(f-n) = A m[3,2] = -(2fn)/(f-n)
	// = B m[2,3] = -1 F=cotan(0.5*fovy) = 1 / tan(0.5*fovy)   => FOVY =
	// 2*atan(1/F]

	auto& mat = m_internalData[0].getMat4();

	if (coords == glm::ivec2(0, 0)) // symmetric scale x
	{
		auto newAspect = mat[1][1] / mat[0][0]; // newAspect = (F / newM[0][0])
		setDefaultValueNoUpdate("aspect", newAspect);
	}
	else if (coords == glm::ivec2(1, 1)) // symmetric scale y
	{
		auto newAspect = mat[1][1] / mat[0][0]; // newAspect = (F / newM[0][0])
		setDefaultValueNoUpdate("aspect", newAspect);

		auto newFovy = 2.0f * atan(1.0f / mat[1][1]);
		setDefaultValueNoUpdate("fovy", newFovy);
	}
	else if (coords == glm::ivec2(2, 2)) // Perspective scale Z
	{
		float newNear = 1.0f, newFar = 10.0f;
		if (val != 1.0f)
			newNear = mat[3][2] / (mat[2][2] - 1.0f); // B / (newA - 1)
		if (val != -1.0f)
			newFar = mat[3][2] / (mat[2][2] + 1.0f); // B / (newA + 1)

		// more stable variant??
		// auto near    = getDefaultValue("near").getFloat();
		// auto far     = getDefaultValue("far").getFloat();
		// auto distance = far - near;
		// auto newA     = mat[2][2];

		// auto newNear = 2.0f * far * near / (distance * (newA - 1.0f));
		// auto newFar  = 2.0f * far * near / (distance * (newA + 1.0f));

		// std::cerr << "mat22: " << mat[2][2] << "\t";
		// std::cerr << "mat32: " << mat[3][2] << std::endl;

		// std::cerr << "Near: " << getDefaultValue("near").getFloat() << " -> " <<
		// newNear << "\t"; std::cerr << "Far : " <<
		// getDefaultValue("far").getFloat() << " -> " << newFar << std::endl;
		setDefaultValueNoUpdate("near", newNear);
		setDefaultValueNoUpdate("far", newFar);
	}
	else if (coords == glm::ivec2(3, 2)) // Perspective offset Z
	{
		// Problems:
		// mat[3][2] == 0    => newNear = newFar
		// mat[2][2] == -1   => infinity newFar
		// mat[2][2] ==  1   => infinity newNear
		float newNear = 1.0f, newFar = 10.0f;
		if (val != 1.0f)
			newNear = mat[3][2] / (mat[2][2] - 1.0f); // newB / (A - 1)
		if (val != -1.0f)
			newFar = mat[3][2] / (mat[2][2] + 1.0f); // newB / (A + 1)
		setDefaultValueNoUpdate("near", newNear);
		setDefaultValueNoUpdate("far", newFar);
	}

	return ValueSetResult{};
}

//===-- Frustum
//------------------------------------------------------------===//

bool TransformImpl<ETransformType::Frustum>::isValid() const
{
	// synergies force the frustum to be symmetrical (moving of left will update
	// the right, etc) update of far and near has problems with the precision -r
	// rough epsilon introduced

	// rough matrix check
	auto& mat = m_internalData[0].getMat4();
	bool result = validateValues(g_FrustumMask, mat); // checks -1 in the last row too

	// nonzero scale and z-translate
	result = result && (mat[0][0] * mat[1][1] * mat[2][2] * mat[3][2] != 0.0f);

	float left = getDefaultValue("left").getFloat();
	float right = getDefaultValue("right").getFloat();
	float bottom = getDefaultValue("bottom").getFloat();
	float top = getDefaultValue("top").getFloat();
	float near = getDefaultValue("near").getFloat();
	float far = getDefaultValue("far").getFloat();

	// check the defaults: simple check of the frustum borders
	result = result && (left < right) && (bottom < top) && (near < far);
	result = result && (near != 0.0f);

	// matrix match the defaults
	constexpr float ROUGH_EPSILON = 0.001f;
	const auto expectedMat = glm::frustum(left, right, bottom, top, near, far);
	result = result && Math::eq(expectedMat, mat, ROUGH_EPSILON);

	return result;
}

void TransformImpl<ETransformType::Frustum>::initDefaults()
{
	setDefaultValueNoUpdate("left", -1.0f);
	setDefaultValueNoUpdate("right", 1.0f);
	setDefaultValueNoUpdate("bottom", -1.0f);
	setDefaultValueNoUpdate("top", 1.0f);
	setDefaultValueNoUpdate("near", 1.0f);
	setDefaultValueNoUpdate("far", 10.0f);
}

void TransformImpl<ETransformType::Frustum>::setDefaultValueWithSynergies(const std::string& name, Core::Data&& val)
{
	assert(hasSynergies()); // called from setValues after this check
	assert(val.opValueType == EValueType::Float);

	std::cout << "Pretizena virtualni funkce pro Frustum" << std::endl;

	float newVal = val.getFloat();
	setDefaultValueNoUpdate(name, newVal);

	// Process the synergies
	if (name == "left")
	{
		setDefaultValueNoUpdate("right", -newVal);
	}
	else if (name == "right")
	{
		setDefaultValueNoUpdate("left", -newVal);
	}
	else if (name == "top")
	{
		setDefaultValueNoUpdate("bottom", -newVal);
	}
	else if (name == "bottom")
	{
		setDefaultValueNoUpdate("top", -newVal);
	}
	// "near" and "far" are not updated
}

// template <typename T>
// void TransformImpl<ETransformType::Frustum>::setDefaultValueWithSynergies(const std::string& name, T&& val)
// {
//	getDefaultValueMut(name).setValue(val);
// }

// void TransformImpl<ETransformType::Frustum>::setDefaultValueWithSynergies(const std::string& name, float val)
//{
//	// getDefaultValueMut(name).setValue(val); // defaults
//	setDefaultValueNoUpdate(name, val);
// };


void TransformImpl<ETransformType::Frustum>::resetMatrixFromDefaults()
{
	m_isLocked = true;

	setInternalValue(glm::frustum(getDefaultValue("left").getFloat(), getDefaultValue("right").getFloat(),
	                              getDefaultValue("bottom").getFloat(), getDefaultValue("top").getFloat(),
	                              getDefaultValue("near").getFloat(), getDefaultValue("far").getFloat()));
	notifySequence();
}

ValueSetResult TransformImpl<ETransformType::Frustum>::setValue(float val, glm::ivec2 coords)
{
	if (!canSetValue(g_FrustumMask, coords, val))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
		                      "Cannot set value on given coordinates."};
	}

	setInternalValue(val, coords);
	notifySequence();

	//////--------------------- from

	// update the defaults to match the perspective matrix
	// m[0,0] = 2n/(R-L)                  => w = R-L, newR = newC * w, newL =
	// (newC-1) * w m[1,1] = 2n/(T-B)                  => m[2,0] = (R+L)/(R-L)

	// m[2,1] = (T+B)/(T-B)
	// m[2,2] = -(f+n)/(f-n) = A    (-1 for infinity f)
	// m[3,2] = -(2fn)/(f-n) = B    (-2n for infinity f)
	// m[2,3] = -1
	// F=cotan(0.5*fovy) = 1 / tan(0.5*fovy)   => FOVY = 2*atan(1/F]

	auto& mat = m_internalData[0].getMat4();

	// ROW 0 - X
	if (coords == glm::ivec2(0, 0))
	{
		const auto near = getDefaultValue("near").getFloat();
		const auto left = getDefaultValue("left").getFloat();
		const auto right = getDefaultValue("right").getFloat();

		const auto sum = getDefaultValue("right").getFloat() + getDefaultValue("left").getFloat();
		const auto newA = mat[0][0];

		if (newA != 0.0f)
		{
			const auto newL = (sum * newA - 2.0f * near) / (2.0f * newA); // newL =
			const auto newR = sum - newL;

			setDefaultValueNoUpdate("left", newL);  // newL =
			setDefaultValueNoUpdate("right", newR); // newR =
		}
	}
	else if (coords == glm::ivec2(2, 0))
	{
		const auto w = getDefaultValue("right").getFloat() - getDefaultValue("left").getFloat();

		const auto newL = (mat[2][0] - 1.0f) * w / 2.0f; // newL = (newC-1) * w / 2

		setDefaultValueNoUpdate("left", newL);      // newL = (newC-1) * w / 2
		setDefaultValueNoUpdate("right", newL + w); // newR = newL + w
	}

	// ROW 1 - Y
	else if (coords == glm::ivec2(1, 1)) // Changing scale in y axis (A),
	                                     // offset is fixed (B)
	{
		const auto near = getDefaultValue("near").getFloat();
		const auto bottom = getDefaultValue("bottom").getFloat();
		const auto top = getDefaultValue("top").getFloat();

		const auto sum = getDefaultValue("top").getFloat() + getDefaultValue("bottom").getFloat();
		const auto newA = mat[1][1];

		if (newA != 0.0f)
		{
			const auto newBot = (sum * newA - 2.0f * near) / (2.0f * newA); // newL =
			const auto newTop = sum - newBot;

			setDefaultValueNoUpdate("bottom", newBot); // newL =
			setDefaultValueNoUpdate("top", newTop);    // newR =
		}
	}
	else if (coords == glm::ivec2(2,
	                              1)) // Changing offset in y axis (B), scale is fixed (A)
	{
		const auto h = getDefaultValue("top").getFloat() - getDefaultValue("bottom").getFloat();

		const auto newBot = (mat[2][1] - 1.0f) * h / 2.0f; // newL = (newC-1) * w / 2

		setDefaultValueNoUpdate("bottom", newBot);  // newL = (newC-1) * w / 2
		setDefaultValueNoUpdate("top", newBot + h); // newR = newL + w
	}

	// ****************************** from
	// ROW 2 - Z
	else if (coords == glm::ivec2(2, 2)) // Changing scale in Z-axis (A),
	                                     // offset is fixed (B)
	{
		float newNear = 1.0f, newFar = 10.0f;
		if (val != 1.0f)
			newNear = mat[3][2] / (mat[2][2] - 1.0f); // B / (newA - 1)
		if (val != -1.0f)
			newFar = mat[3][2] / (mat[2][2] + 1.0f); // B / (newA + 1)

		setDefaultValueNoUpdate("near", newNear);
		setDefaultValueNoUpdate("far", newFar);
	}
	// ****************************** to

	else if (coords == glm::ivec2(3, 2)) // Changing offset in Z-axis (B),
	                                     // scale is fixed (A)
	{
		float newNear = 1.0f, newFar = 10.0f;
		if (val != 1.0f)
			newNear = mat[3][2] / (mat[2][2] - 1.0f); // newB / (A - 1)
		if (val != -1.0f)
			newFar = mat[3][2] / (mat[2][2] + 1.0f); // newB / (A + 1)
		setDefaultValueNoUpdate("near", newNear);
		setDefaultValueNoUpdate("far", newFar);
	}

	return ValueSetResult{};
}

//===-- Look At -----------------------------------------------------------===//
#define MY_ASSERT(condition, message)                                                                                  \
	if (!(condition))                                                                                                  \
	{                                                                                                                  \
		std::cerr << "Line: " << message << std::endl;                                                                 \
	}

bool TransformImpl<ETransformType::LookAt>::isValid() const
{
	// todo DEBUG matrix comparisons

	auto& mat = m_internalData[0].getMat4();
	auto matL = glm::mat3(mat);       // linear 3x3 part
	auto matT = glm::transpose(matL); // linear 3x3 part - rows to columns

	// check the basic matrix values 0, 1, -1, any
	// check (0,0,0,1) in the last row only
	bool result = validateValues(g_LookAtMask, mat); // rotation + translation

	MY_ASSERT(result, "LookAt::isValid(): validateValues(g_LookAtMask, mat) failed"); // todo remove

	// matrix inner consistency
	// check the Linear part - must have normalized rows }
	result = result && Math::eq(glm::determinant(mat), 1.0f); // linearly independent
	result = result && Math::eq(glm::length2(matT[0]), 1.0f); // unit camera axes
	result = result && Math::eq(glm::length2(matT[1]), 1.0f); // - rows in linear part
	result = result && Math::eq(glm::length2(matT[2]), 1.0f);

	MY_ASSERT(result, "LookAt::isValid(): Orthonormality tests failed"); // todo remove

	// consistency of the defaults
	glm::vec3 eye = getDefaultValue("eye").getVec3();
	glm::vec3 center = getDefaultValue("center").getVec3();
	glm::vec3 up = getDefaultValue("up").getVec3();

	glm::vec3 direction = center - eye;
	float directionSize = glm::dot(direction, direction);
	float upSize = glm::dot(up, up);

	// check, if center is different than eye or up-vector is not zero,
	if ((directionSize == 0.0f) || (upSize == 0.0f))
	{
		result = false;
	}
	else
	{
		direction /= glm::sqrt(directionSize); // normalize
		up /= glm::sqrt(upSize);

		// and up is not collinear with camera direction center - eye
		result = result && (glm::dot(direction, up) != 1.0f); // not parallel
	}

	MY_ASSERT(result, "LookAt::isValid(): Direction or up vector tests failed"); // todo remove

	// consistency defaults <--> matrix
	//  expected matrix - this is a more rigid test
	//  - good for resetMatrixFromDefaults
	//  - does not allow to edit the matrix in Full mode
	//  todo LookAt: use the rigid test or not?
	auto expectedMat = glm::lookAt(eye, center, up);
	result = result && Math::eq(expectedMat, mat);

	MY_ASSERT(result, "LookAt::isValid(): Matrix comparison failed"); // todo remove

	return result;
}

void TransformImpl<ETransformType::LookAt>::initDefaults()
{
	setDefaultValueNoUpdate("eye", glm::vec3{0.0, 0.0, 10.0});
	setDefaultValueNoUpdate("center", glm::vec3{0.0, 0.0, 0.0});
	setDefaultValueNoUpdate("up", glm::vec3{0.0, 1.0, 0.0});
}
void TransformImpl<ETransformType::LookAt>::resetMatrixFromDefaults()
{
	m_isLocked = true;
	// m_hasSynergies = true; // no meaning now

	setInternalValue(glm::lookAt(getDefaultValue("eye").getVec3(), getDefaultValue("center").getVec3(),
	                             getDefaultValue("up").getVec3()));
	notifySequence();
}

// Same as in Transform, no synergies or matrix to defaults
// ValueSetResult TransformImpl<ETransformType::LookAt>::setValue(float val,
// glm::ivec2 coords)
//{
//	if (isLocked()) { return
// ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid
// position!"}; } 	setInternalValue(val, coords); 	notifySequence();
//
//	return ValueSetResult{};
//}
} // namespace Core
