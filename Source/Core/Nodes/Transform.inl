#include "Transform.h"

#include "Logger/Logger.h"
#include "Utils/Format.h"

namespace Core
{
inline bool Transformation::isLocked() const
{
	return m_currentMap == &Transform::g_Free;
}

inline void Transformation::lock()
{
	m_currentMap = &Transform::g_AllLocked;
}

inline void Transformation::unlock()
{
	m_currentMap = &Transform::g_Free;
}

inline void Transformation::notifySequence()
{
	if (m_currentSequence)
	{
		m_currentSequence->updateValues(-1);
	}
}

//===----------------------------------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::Scale>::isValid() const
{
	bool result = Transform::cmp(&Transform::g_Scale, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]);
	}

	return ETransformState(result);
}

inline void TransformImpl<ETransformType::Scale>::lock()
{
	m_currentMap = &Transform::g_Scale;
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val)
{
	notifySequence();
	return NodeBase::setValue(glm::vec3(val));
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec3& vec)
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

inline ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setValue(const glm::mat4& mat)
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

inline ValueSetResult TransformImpl<ETransformType::Scale>::setValue(float val, glm::ivec2 coords)
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

inline void TransformImpl<ETransformType::Scale>::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::scale(m_initialScale));
}

inline Transformation::ValueMap TransformImpl<ETransformType::Scale>::getDefaultValues()
{
	return {{"scale", Utils::toString(m_initialScale)}};
}

inline float TransformImpl<ETransformType::Scale>::getX()
{
	return getInternalData().getMat4()[0][0];
}

inline float TransformImpl<ETransformType::Scale>::getY()
{
	return getInternalData().getMat4()[1][1];
}

inline float TransformImpl<ETransformType::Scale>::getZ()
{
	return getInternalData().getMat4()[2][2];
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setX(float v)
{
	return setValue(v, {0, 0});
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setY(float v)
{
	return setValue(v, {1, 1});
}

inline ValueSetResult TransformImpl<ETransformType::Scale>::setZ(float v)
{
	return setValue(v, {2, 2});
}

//===-- Euler rotation around X axis --------------------------------------===//

inline ETransformState TransformImpl<ETransformType::EulerX>::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerX, mat);;

	float angle = std::atan2(-mat[2][1], mat[2][2]);
	auto expectedMat = glm::eulerAngleX(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

inline void TransformImpl<ETransformType::EulerX>::lock()
{
	m_currentMap = &Transform::g_EulerX;
}

inline Transformation::ValueMap TransformImpl<ETransformType::EulerX>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

inline ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(1.0f, 0.0f, 0.0f)));

	notifySequence();
	return ValueSetResult{ValueSetResult::Status::Ok};
}

inline ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(const glm::mat4& mat)
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

inline ValueSetResult TransformImpl<ETransformType::EulerX>::setValue(float val, glm::ivec2 coords)
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

inline void TransformImpl<ETransformType::EulerX>::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::eulerAngleX(m_initialRot));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Y axis --------------------------------------===//
inline ETransformState TransformImpl<ETransformType::EulerY>::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerY, mat);

	float angle = std::asin(mat[2][0]);
	auto expectedMat = glm::eulerAngleY(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

inline void TransformImpl<ETransformType::EulerY>::lock()
{
	m_currentMap = &Transform::g_EulerY;
}

inline Transformation::ValueMap TransformImpl<ETransformType::EulerY>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

inline ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 1.0f, 0.0f)));
	notifySequence();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(const glm::mat4& mat)
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

inline ValueSetResult TransformImpl<ETransformType::EulerY>::setValue(float val, glm::ivec2 coords)
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

inline void TransformImpl<ETransformType::EulerY>::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_currentRot = m_initialRot;
}

//===-- Euler rotation around Z axis --------------------------------------===//

inline ETransformState TransformImpl<ETransformType::EulerZ>::isValid() const
{
	auto& mat = m_internalData[0].getMat4();
	bool result = Transform::isMatValid(&Transform::g_EulerZ, mat);

	float angle = glm::atan(mat[0][1], mat[0][0]);
	auto expectedMat = glm::eulerAngleZ(angle);

	result = result && Math::eq(expectedMat, mat);

	return ETransformState(result);
}

inline void TransformImpl<ETransformType::EulerZ>::lock()
{
	m_currentMap = &Transform::g_EulerZ;
}

inline Transformation::ValueMap TransformImpl<ETransformType::EulerZ>::getDefaultValues()
{
	return {{"rotation", Utils::toString(m_initialRot)}};
}

inline ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(float val)
{
	m_currentRot = val;
	setInternalValue(glm::rotate(val, glm::vec3(0.0f, 0.0f, 1.0f)));
	notifySequence();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(const glm::vec3& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(const glm::vec4& val)
{
	return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation for rotation matrix"};
}

inline ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(const glm::mat4& mat)
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

inline ValueSetResult TransformImpl<ETransformType::EulerZ>::setValue(float val, glm::ivec2 coords)
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

inline void TransformImpl<ETransformType::EulerZ>::reset()
{
	setDataMap(m_initialMap);
	setInternalValue(glm::rotate(m_initialRot, glm::vec3(0.0f, 0.0f, 1.0f)));
	m_currentRot = m_initialRot;
}

//===-- Translation -------------------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::Translation>::isValid() const
{
	bool result = Transform::isMatValid(&Transform::g_Translate, m_internalData[0].getMat4());
	if (hasSynergies())
	{
		auto& mat = m_internalData[0].getMat4();
		result = result && Math::eq(mat[3][0], mat[3][1]) && Math::eq(mat[3][1], mat[3][2]);
	}

	return ETransformState(result);
}

inline void TransformImpl<ETransformType::Translation>::lock()
{
	m_currentMap = &Transform::g_Translate;
}

inline Transformation::ValueMap TransformImpl<ETransformType::Translation>::getDefaultValues()
{
	return {{"translation", Utils::toString(m_initialTrans)}};
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val)
{
	return setValue(glm::vec3(val));
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec3& vec)
{
	setInternalValue(glm::translate(vec));
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::vec4& vec)
{
	return setValue(glm::vec3(vec));
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setValue(const glm::mat4& mat)
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

inline ValueSetResult TransformImpl<ETransformType::Translation>::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

inline void TransformImpl<ETransformType::Translation>::reset()
{
	setDataMap(m_initialMap);
	setValue(glm::translate(m_initialTrans));
}

inline float TransformImpl<ETransformType::Translation>::getX()
{
	return getInternalData().getMat4()[3][0];
}

inline float TransformImpl<ETransformType::Translation>::getY()
{
	return getInternalData().getMat4()[3][1];
}

inline float TransformImpl<ETransformType::Translation>::getZ()
{
	return getInternalData().getMat4()[3][2];
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setX(float v)
{
	return setValue(v, {3, 0});
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setY(float v)
{
	return setValue(v, {3, 1});
}

inline ValueSetResult TransformImpl<ETransformType::Translation>::setZ(float v)
{
	return setValue(v, {3, 2});
}

//===-- Axis angle rotation -----------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::AxisAngle>::isValid() const
{
	return ETransformState::Unknown;
}

inline Transformation::ValueMap TransformImpl<ETransformType::AxisAngle>::getDefaultValues()
{
	return {{"axis", Utils::toString(m_initialAxis)}, {"rotation", Utils::toString(m_initialRads)}};
}

inline void TransformImpl<ETransformType::AxisAngle>::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::rotate(m_initialRads, m_initialAxis));
}

inline ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(float rads)
{
	return setRot(rads);
}

inline ValueSetResult TransformImpl<ETransformType::AxisAngle>::setValue(const glm::vec3& axis)
{
	return setAxis(axis);
}

inline ValueSetResult TransformImpl<ETransformType::AxisAngle>::setRot(float rads)
{
	m_initialRads = rads;
	setInternalValue(glm::rotate(rads, m_initialAxis));
	return ValueSetResult();
}

inline ValueSetResult TransformImpl<ETransformType::AxisAngle>::setAxis(const glm::vec3& axis)
{
	m_initialAxis = axis;
	setInternalValue(glm::rotate(m_initialRads, axis));
	return ValueSetResult();
}

//===-- Quaternion rotation -----------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::Quat>::isValid() const
{
	return ETransformState::Unknown;
}

inline Transformation::ValueMap TransformImpl<ETransformType::Quat>::getDefaultValues()
{
	return {{"quat", Utils::toString(m_initialQuat)}};
}

inline void TransformImpl<ETransformType::Quat>::reset()
{
	notifySequence();
	setInternalValue(glm::toMat4(m_normalized));
}

inline const glm::quat& TransformImpl<ETransformType::Quat>::getNormalized() const
{
	return m_normalized;
};

inline ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::quat& q)
{
	m_initialQuat = q;
	m_normalized = glm::normalize(q);
	setInternalValue(glm::toMat4(m_normalized));
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Quat>::setValue(const glm::vec4& vec)
{
	glm::quat q(vec);
	return setValue(glm::quat(q));
}

//===-- Orthographic projection -------------------------------------------===//

ETransformState TransformImpl<ETransformType::Ortho>::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Ortho, m_internalData[0].getMat4()));
}

inline void TransformImpl<ETransformType::Ortho>::lock()
{
	m_currentMap = &Transform::g_Ortho;
}

inline Transformation::ValueMap TransformImpl<ETransformType::Ortho>::getDefaultValues()
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

inline void TransformImpl<ETransformType::Ortho>::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Not an editable field."};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{ValueSetResult::Status::Ok};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Ortho>::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Perspective -------------------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::Perspective>::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Perspective, m_internalData[0].getMat4()));;
}

inline void TransformImpl<ETransformType::Perspective>::lock()
{
	m_currentMap = &Transform::g_Perspective;
}

inline Transformation::ValueMap TransformImpl<ETransformType::Perspective>::getDefaultValues()
{
	return {
			{"fov", Utils::toString(m_initialFOW)},
			{"aspect", Utils::toString(m_initialAspect)},
			{"zNear", Utils::toString(m_initialZNear)},
			{"zFar", Utils::toString(m_initialZFar)},
	};
}

inline ValueSetResult TransformImpl<ETransformType::Perspective>::setFOW(float v)
{
	m_initialFOW = v;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Perspective>::setAspect(float v)
{
	m_initialAspect = v;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Perspective>::setZNear(float v)
{
	m_initialZNear = v;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Perspective>::setZFar(float v)
{
	m_initialZFar = v;
	reset();
	return ValueSetResult{};
}

inline void TransformImpl<ETransformType::Perspective>::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::perspective(m_initialFOW, m_initialAspect, m_initialZNear, m_initialZFar));
}

inline ValueSetResult TransformImpl<ETransformType::Perspective>::setValue(float val, glm::ivec2 coords)
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

inline ETransformState TransformImpl<ETransformType::Frustum>::isValid() const
{
	return ETransformState(Transform::isMatValid(&Transform::g_Frustum, m_internalData[0].getMat4()));
}

inline void TransformImpl<ETransformType::Frustum>::lock()
{
	m_currentMap = &Transform::g_Frustum;
}

inline Transformation::ValueMap TransformImpl<ETransformType::Frustum>::getDefaultValues()
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

inline void TransformImpl<ETransformType::Frustum>::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far));
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setLeft(float val)
{
	m_left = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setRight(float val)
{
	m_right = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setBottom(float val)
{
	m_bottom = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setTop(float val)
{
	m_top = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setNear(float val)
{
	m_near = val;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::Frustum>::setFar(float val)
{
	m_far = val;
	reset();
	return ValueSetResult{};
}

//===-- Look At -----------------------------------------------------------===//

inline ETransformState TransformImpl<ETransformType::LookAt>::isValid() const
{
	return ETransformState::Unknown;
}

inline Transformation::ValueMap TransformImpl<ETransformType::LookAt>::getDefaultValues()
{
	return {
			{"eye", Utils::toString(m_initialEye)},
			{"center", Utils::toString(m_initialCenter)},
			{"up", Utils::toString(m_initialUp)},
	};
}

inline void TransformImpl<ETransformType::LookAt>::reset()
{
	m_currentMap = m_initialMap;
	notifySequence();
	setInternalValue(glm::lookAt(m_initialEye, m_initialCenter, m_initialUp));
}

inline ValueSetResult TransformImpl<ETransformType::LookAt>::setValue(float val, glm::ivec2 coords)
{
	if (!coordsAreValid(coords, m_currentMap))
	{
		return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}
	setInternalValue(val, coords);
	notifySequence();

	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::LookAt>::setEye(const glm::vec3& eye)
{
	m_initialEye = eye;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::LookAt>::setCenter(const glm::vec3& center)
{
	m_initialCenter = center;
	reset();
	return ValueSetResult{};
}

inline ValueSetResult TransformImpl<ETransformType::LookAt>::setUp(const glm::vec3& up)
{
	m_initialUp = up;
	reset();
	return ValueSetResult{};
}
}
