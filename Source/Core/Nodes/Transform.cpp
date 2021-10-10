#include "Transform.h"

#include "Logger/Logger.h"
#include "Utils/Format.h"

using namespace Core;

bool Transformation::isLocked() const
{
	return m_currentMap == &Transform::g_Free;
}

void Transformation::lock()
{
	m_currentMap = &Transform::g_AllLocked;
}

void Transformation::unlock()
{
	m_currentMap = &Transform::g_Free;
}

void Transformation::notifySequence()
{
	if (m_currentSequence)
	{
		m_currentSequence->updateValues(-1);
	}
}

//===----------------------------------------------------------------------===//
ETransformState Scale::isValid() const
{
	bool result = Transform::cmp(&Transform::g_Scale, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]);
	}

	return ETransformState(result);
}

void Scale::lock()
{
	m_currentMap = &Transform::g_Scale;
}

ValueSetResult Scale::setValue(float val)
{
	notifySequence();
	return NodeBase::setValue(glm::vec3(val));
}

ValueSetResult Scale::setValue(const glm::vec3& vec)
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

ValueSetResult Scale::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult Scale::setValue(const glm::mat4& mat)
{
	if (m_currentMap == &Transform::g_Scale)
	{
		if (hasSynergies())
		{
			if (Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]))
			{
				setInternalValue(mat);
			}
			else
				return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
															"Given matrix does not represent uniform scale."};
		}
		if (Transform::cmp(m_currentMap, mat))
		{
			setInternalValue(mat);
		}
		else
		{
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
			                      "Given matrix does not represent scale."};
		}
	}
	else if (m_currentMap == &Transform::g_Free)
	{
		// Free transformation is set.
		setInternalValue(mat);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Scale::setValue(float val, glm::ivec2 coords)
{
	if (m_currentMap == &Transform::g_Free)
	{
		// Free transformation is set.
		setInternalValue(val, coords);
	}

	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (m_currentMap == &Transform::g_Scale)
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

void Scale::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::scale(m_initialScale));
}

Transformation::ValueMap Scale::getDefaultValues()
{
	return {{"scale", Utils::toString(m_initialScale)}};
}

float Scale::getX()
{
	return getInternalData().getMat4()[0][0];
}

float Scale::getY()
{
	return getInternalData().getMat4()[1][1];
}

float Scale::getZ()
{
	return getInternalData().getMat4()[2][2];
}

ValueSetResult Scale::setX(float v)
{
	return setValue(v, {0, 0});
}

ValueSetResult Scale::setY(float v)
{
	return setValue(v, {1, 1});
}

ValueSetResult Scale::setZ(float v)
{
	return setValue(v, {2, 2});
}

//===-- Euler rotation around X axis --------------------------------------===//
ETransformState EulerRotX::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerX, mat);;

	float angle = std::atan2(-mat[2][1], mat[2][2]);
	auto expectedMat = glm::eulerAngleX(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void EulerRotX::lock()
{
	m_currentMap = &Transform::g_EulerX;
}

Transformation::ValueMap EulerRotX::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult EulerRotX::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(1.0f, 0.0f, 0.0f)));

	notifySequence();
	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotX::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotX::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotX::setValue(const glm::mat4& mat)
{
	if (m_currentMap == &Transform::g_EulerX)
	{
		if (Transform::isMatValid(m_currentMap, mat))
		{
			float angleRad = std::atan2(-mat[2][1], mat[2][2]);

			bool isValid = Math::eq(glm::eulerAngleX(angleRad), mat);

			if (isValid)
			{
				m_currentRot = angleRad;
				setInternalValue(mat);
			}
			else
			{
				return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
				                      "Given matrix is not Euler rot. around X axis."};
			}
		}
	}
	else if (m_currentMap == &Transform::g_Free)
	{
		setInternalValue(mat);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotX::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
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

void EulerRotX::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::eulerAngleX(m_initialRot));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Y axis --------------------------------------===//
ETransformState EulerRotY::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerY, mat);

	float angle = std::asin(mat[2][0]);
	auto expectedMat = glm::eulerAngleY(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void EulerRotY::lock()
{
    m_currentMap = &Transform::g_EulerY;
}

Transformation::ValueMap EulerRotY::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult EulerRotY::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 1.0f, 0.0f)));
	notifySequence();
	return ValueSetResult{};
}

ValueSetResult EulerRotY::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotY::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotY::setValue(const glm::mat4& mat)
{
	/// \todo MH Validation is not always correct.

	if (m_currentMap == &Transform::g_Free)
		setInternalValue(mat);
	else if (m_currentMap == &Transform::g_EulerY)
	{
		float angleRad = std::asin(mat[2][0]);
		m_currentRot = angleRad;
		auto eulerY = glm::eulerAngleY(angleRad);

		if (!Math::eq(eulerY, mat))
		{
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an Euler rot around Y axis."};
		}
		setInternalValue(mat);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotY::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
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

void EulerRotY::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Z axis --------------------------------------===//
ETransformState EulerRotZ::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerZ, mat);

	float angle = glm::atan(mat[0][1], mat[0][0]);
	auto expectedMat = glm::eulerAngleZ(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

void EulerRotZ::lock()
{
    m_currentMap = &Transform::g_EulerZ;
}

Transformation::ValueMap EulerRotZ::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

ValueSetResult EulerRotZ::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 0.0f, 1.0f)));
	notifySequence();
	return ValueSetResult{};
}

ValueSetResult EulerRotZ::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotZ::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

ValueSetResult EulerRotZ::setValue(const glm::mat4& mat)
{
	if (m_currentMap == &Transform::g_Free)
		setInternalValue(mat);
	else if (m_currentMap == &Transform::g_EulerZ)
	{
		float angle = glm::atan(mat[0][1], mat[0][0]);
		auto eulerZ = glm::eulerAngleZ(angle);
		if (!Math::eq(eulerZ, mat))
		{
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an Euler rot around Z axis."};
		}
		m_currentRot = angle;
		setInternalValue(mat);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotZ::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
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

void EulerRotZ::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 0.0f, 1.0f)));
	m_currentRot = m_initialRot;
}

//===-- Translation -------------------------------------------------------===//
ETransformState Translation::isValid() const
{
	bool result = Transform::isMatValid(&Transform::g_Translate, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[3][0], mat[3][1]) && Math::eq(mat[3][1], mat[3][2]);
	}

	return ETransformState(result);
}

void Translation::lock()
{
	m_currentMap = &Transform::g_Translate;
}

Transformation::ValueMap Translation::getDefaultValues()
{
	return {{"translation", Utils::toString(m_initialTrans)}};
}

ValueSetResult Translation::setValue(float val)
{
	return setValue(glm::vec3(val));
}

ValueSetResult Translation::setValue(const glm::vec3& vec)
{
	setInternalValue(glm::translate(vec));
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Translation::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult Translation::setValue(const glm::mat4& mat)
{
	if (m_currentMap == &Transform::g_Translate)
	{
		if (Transform::isMatValid(m_currentMap, mat))
		{
			setInternalValue(mat);
		}
		else
		{
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
			                      "Cannot set given matrix, because it not represents translation."};
		}
	}
	else if (m_currentMap == &Transform::g_Free)
	{
		setInternalValue(mat);
	}
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Translation::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void Translation::reset()
{
	setDataMap(m_initialMap);
	setValue(glm::translate(m_initialTrans));
}

float Translation::getX()
{
	return getInternalData().getMat4()[3][0];
}

float Translation::getY()
{
	return getInternalData().getMat4()[3][1];
}

float Translation::getZ()
{
	return getInternalData().getMat4()[3][2];
}

ValueSetResult Translation::setX(float v)
{
	return setValue(v, {3, 0});
}

ValueSetResult Translation::setY(float v)
{
	return setValue(v, {3, 1});
}

ValueSetResult Translation::setZ(float v)
{
	return setValue(v, {3, 2});
}

//===-- Axis angle rotation -----------------------------------------------===//
ETransformState AxisAngleRot::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap AxisAngleRot::getDefaultValues()
{
	return {{"axis", Utils::toString(m_initialAxis)}, {"rotation", Utils::toString(m_initialRads)}};
}

void AxisAngleRot::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::rotate(m_initialRads, m_initialAxis));
}

ValueSetResult AxisAngleRot::setValue(float rads)
{
	return setRot(rads);
}

ValueSetResult AxisAngleRot::setValue(const glm::vec3& axis)
{
	return setAxis(axis);
}

ValueSetResult AxisAngleRot::setRot(float rads)
{
	m_initialRads = rads;
	setInternalValue(glm::rotate(rads, m_initialAxis));
	return ValueSetResult();
}

ValueSetResult AxisAngleRot::setAxis(const glm::vec3& axis)
{
	m_initialAxis = axis;
	setInternalValue(glm::rotate(m_initialRads, axis));
	return ValueSetResult();
}

//===-- Quaternion rotation -----------------------------------------------===//
ETransformState QuatRot::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap QuatRot::getDefaultValues()
{
	return {{"quat", Utils::toString(m_initialQuat)}};
}

void QuatRot::reset()
{
	notifySequence();
	setInternalValue(glm::toMat4(m_normalized));
}

const glm::quat& QuatRot::getNormalized() const
{
	return m_normalized;
};

ValueSetResult QuatRot::setValue(const glm::quat& q)
{
	m_initialQuat = q;
	m_normalized = glm::normalize(q);
	setInternalValue(glm::toMat4(m_normalized));
	return ValueSetResult{};
}

ValueSetResult QuatRot::setValue(const glm::vec4& vec)
{
	glm::quat q(vec);
	return setValue(glm::quat(q));
}

//===-- Orthographic projection -------------------------------------------===//
ETransformState OrthoProj::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Ortho, m_internalData[0].getMat4()));
}

void OrthoProj::lock()
{
    m_currentMap = &Transform::g_Ortho;
}

Transformation::ValueMap OrthoProj::getDefaultValues()
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

void OrthoProj::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult OrthoProj::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an editable field."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult OrthoProj::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult OrthoProj::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult OrthoProj::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult OrthoProj::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult OrthoProj::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult OrthoProj::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Perspective -------------------------------------------------------===//
ETransformState PerspectiveProj::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Perspective, m_internalData[0].getMat4()));;
}

void PerspectiveProj::lock()
{
    m_currentMap = &Transform::g_Perspective;
}

Transformation::ValueMap PerspectiveProj::getDefaultValues()
{
	return {
			{"fov", Utils::toString(m_initialFOW)},
			{"aspect", Utils::toString(m_initialAspect)},
			{"zNear", Utils::toString(m_initialZNear)},
			{"zFar", Utils::toString(m_initialZFar)},
	};
}

ValueSetResult PerspectiveProj::setFOW(float v)
{
	m_initialFOW = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult PerspectiveProj::setAspect(float v)
{
	m_initialAspect = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult PerspectiveProj::setZNear(float v)
{
	m_initialZNear = v;
	reset();
	return ValueSetResult{};
}

ValueSetResult PerspectiveProj::setZFar(float v)
{
	m_initialZFar = v;
	reset();
	return ValueSetResult{};
}

void PerspectiveProj::reset()
{
    m_currentMap = m_initialMap;
    notifySequence();
    setInternalValue(glm::perspective(m_initialFOW, m_initialAspect, m_initialZNear, m_initialZFar));
}

ValueSetResult PerspectiveProj::setValue(float val, glm::ivec2 coords)
{
    if (!coordsAreValid(coords, m_currentMap))
    {
        return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
    }
    setInternalValue(val, coords);
    notifySequence();

    return ValueSetResult{};
}

//===-- Frusum ------------------------------------------------------------===//
ETransformState Frustum::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Frustum, m_internalData[0].getMat4()));
}

void Frustum::lock()
{
    m_currentMap = &Transform::g_Frustum;
}

Transformation::ValueMap Frustum::getDefaultValues()
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

void Frustum::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult Frustum::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

ValueSetResult Frustum::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult Frustum::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult Frustum::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult Frustum::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult Frustum::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

ValueSetResult Frustum::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Look At -----------------------------------------------------------===//

ETransformState LookAt::isValid() const
{
	return ETransformState::Unknown;
}

Transformation::ValueMap LookAt::getDefaultValues()
{
	return {
			{"eye", Utils::toString(m_initialEye)},
			{"center", Utils::toString(m_initialCenter)},
			{"up", Utils::toString(m_initialUp)},
	};
}

void LookAt::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::lookAt(m_initialEye, m_initialCenter, m_initialUp));
}

ValueSetResult LookAt::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

ValueSetResult LookAt::setEye(const glm::vec3& eye)
{
	m_initialEye = eye;
	reset();
	return ValueSetResult{};
}

ValueSetResult LookAt::setCenter(const glm::vec3& center)
{
	m_initialCenter = center;
	reset();
	return ValueSetResult{};
}

ValueSetResult LookAt::setUp(const glm::vec3& up)
{
	m_initialUp = up;
	reset();
	return ValueSetResult{};
}
