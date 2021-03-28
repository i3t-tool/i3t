#include "Transform.h"

#include "glm/gtx/rotate_vector.hpp"

#include "Logger/Logger.h"

using namespace Core;

ValueSetResult Scale::setValue(float val)
{
	return NodeBase::setValue(glm::vec3(val));
}

ValueSetResult Scale::setValue(const glm::vec3& vec)
{
	if (m_currentMap == Transform::g_UniformScale)
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

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Scale::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult Scale::setValue(const glm::mat4& mat)
{
	if (m_currentMap == Transform::g_UniformScale)
	{
		if (Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]))
		{
			setInternalValue(mat);
		}
		else
			return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation,
			                      "Given matrix does not represent uniform scale."};
	}
	else if (m_currentMap == Transform::g_Scale)
	{
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
	else if (m_currentMap == Transform::g_Free)
	{
		// Free transformation is set.
		setInternalValue(mat);
	}

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Scale::setValue(float val, glm::ivec2 coords)
{
	if (m_currentMap == Transform::g_Free)
	{
		// Free transformation is set.
		setInternalValue(val, coords);
	}

	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}

	if (m_currentMap == Transform::g_UniformScale)
	{
		setInternalValue(glm::scale(glm::vec3(val)));
	}

	if (m_currentMap == Transform::g_Scale)
	{
		setInternalValue(val, coords);
		spreadSignal();
	}

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void Scale::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::scale(m_initialScale));
}

void Scale::setDataMap(const Transform::DataMap& map)
{
	Debug::Assert(map == Transform::g_Free || map == Transform::g_Scale || map == Transform::g_UniformScale);
	NodeBase::setDataMap(map);
}

void Scale::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::scale(m_inputs[0].getStorage().getVec3()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}

//===-- Euler rotation around X axis --------------------------------------===//
ValueSetResult EulerRotX::setValue(float val)
{
	setInternalValue(glm::rotate(val, glm::vec3(1.0f, 0.0f, 0.0f)));

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
	if (m_currentMap == Transform::g_EulerX)
	{
		if (Transform::isMatValid(m_currentMap, mat))
		{
			/// \todo Rotation matrix values validation.
			Debug::Assert(false, "Not implemented yet.");
		}
	}
	else if (m_currentMap == Transform::g_Free)
	{
		setInternalValue(mat);
	}

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

	if (coords == glm::ivec2(1, 2))
	{
		// -sin(T)
		mat[1][2] = val;
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
		mat[2][1] = val;
		mat[1][2] = -val;

		auto cos = sqrt(1.0f - (val * val));
		mat[1][1] = cos;
		mat[2][2] = cos;
	}

	setInternalValue(mat);

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void EulerRotX::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(1.0f, 0.0f, 0.0f)));
}

void EulerRotX::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		float angle;

		angle = m_inputs[0].getStorage().getFloat();

		setInternalValue(glm::eulerAngleX(angle));
	}
	/*
	else
	{
	  setInternalValue(glm::eulerAngleX(0.0f));
	}
	 */
}

//===-- Euler rotation around Y axis --------------------------------------===//

ValueSetResult EulerRotY::setValue(float val)
{
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 1.0f, 0.0f)));
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

ValueSetResult EulerRotY::setValue(const glm::mat4&)
{
	Debug::Assert(false, "Not implemented yet.");
	return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Unsupported yet."};
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

	if (coords == glm::ivec2(0, 2))
	{
		// -sin(T)
		mat[0][2] = val;
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
		mat[2][0] = val;
		mat[0][2] = -val;

		auto cos = sqrt(1.0f - (val * val));
		mat[0][0] = cos;
		mat[2][2] = cos;
	}

	setInternalValue(mat);

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void EulerRotY::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void EulerRotY::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		float angle;

		angle = m_inputs[0].getStorage().getFloat();

		setInternalValue(glm::eulerAngleY(angle));
	}
	/*
	else
	{
	  setInternalValue(glm::eulerAngleY(0.0f));
	}
	 */
}

//===-- Euler rotation around Z axis --------------------------------------===//

ValueSetResult EulerRotZ::setValue(float val)
{
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 0.0f, 1.0f)));
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

ValueSetResult EulerRotZ::setValue(const glm::mat4&)
{
	Debug::Assert(false, "Not implemented yet.");
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

	if (coords == glm::ivec2(0, 1))
	{
		// -sin(T)
		mat[0][1] = val;
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
		mat[1][0] = val;
		mat[0][1] = -val;

		auto cos = sqrt(1.0f - (val * val));
		mat[0][0] = cos;
		mat[1][1] = cos;
	}

	setInternalValue(mat);

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void EulerRotZ::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 0.0f, 1.0f)));
}

void EulerRotZ::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		float angle;

		angle = m_inputs[0].getStorage().getFloat();

		setInternalValue(glm::eulerAngleZ(angle));
	}
	/*
	else
	{
	  setInternalValue(glm::eulerAngleZ(0.0f));
	}
	 */
}

//===-- Euler rotation around Z axis --------------------------------------===//
ValueSetResult Translation::setValue(float val)
{
	return setValue(glm::vec3(val));
}

ValueSetResult Translation::setValue(const glm::vec3& vec)
{
	setInternalValue(glm::translate(vec));

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Translation::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

ValueSetResult Translation::setValue(const glm::mat4& mat)
{
	if (m_currentMap == Transform::g_Translate)
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
	else if (m_currentMap == Transform::g_Free)
	{
		setInternalValue(mat);
	}

	return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Translation::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);
	return ValueSetResult{ValueSetResult::Status::Ok};
}


void Translation::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::translate(m_initialTrans));
}

void Translation::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::translate(m_inputs[0].getStorage().getVec3()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4(1.0f));
	}
	 */
}

void AxisAngleRot::reset()
{
	m_currentMap = m_initialMap;
	setInternalValue(glm::rotate(m_initialRads, m_initialAxis));
}

void AxisAngleRot::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getVec3()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}

void OrthoProj::reset()
{
	m_currentMap = m_initialMap;
	setInternalValue(glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult OrthoProj::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an editable field."};
	}
	setInternalValue(val, coords);

	return ValueSetResult{ValueSetResult::Status::Ok};
}

void OrthoProj::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
	    m_inputs[3].isPluggedIn() && m_inputs[4].isPluggedIn() && m_inputs[5].isPluggedIn())
	{
		setInternalValue(glm::ortho(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
		                            m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat(),
		                            m_inputs[4].getStorage().getFloat(), m_inputs[5].getStorage().getFloat()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}

void PerspectiveProj::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
	    m_inputs[3].isPluggedIn())
	{
		setInternalValue(glm::perspective(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
		                                  m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}

void PerspectiveProj::reset()
{
	m_currentMap = m_initialMap;
	setInternalValue(glm::perspective(m_initialFOW, m_initialAspect, m_initialZNear, m_initialZFar));
}

ValueSetResult PerspectiveProj::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);

	return ValueSetResult{};
}

void Frustum::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
	    m_inputs[3].isPluggedIn() && m_inputs[4].isPluggedIn() && m_inputs[5].isPluggedIn())
	{
		setInternalValue(glm::frustum(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
		                              m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat(),
		                              m_inputs[4].getStorage().getFloat(), m_inputs[5].getStorage().getFloat()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}

void Frustum::reset()
{
	m_currentMap = m_initialMap;
	setInternalValue(glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

ValueSetResult Frustum::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);

	return ValueSetResult{};
}

void LookAt::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(glm::lookAt(m_inputs[0].getStorage().getVec3(), m_inputs[1].getStorage().getVec3(),
		                             m_inputs[2].getStorage().getVec3()));
	}
	/*
	else
	{
	  setInternalValue(glm::mat4());
	}
	 */
}
void LookAt::reset()
{
	m_currentMap = m_initialMap;
	setInternalValue(glm::lookAt(m_initialEye, m_initialCenter, m_initialUp));
}

ValueSetResult LookAt::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);

	return ValueSetResult{};
}
