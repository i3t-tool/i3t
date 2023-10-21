/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 18.11.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <type_traits>
#include <variant>

#include "Core/Nodes/Node.h"

namespace Core
{
template <EOperatorType T> class Operator;

namespace Builder
{
/**
 * Create new operator.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
template <EOperatorType T> Ptr<Node> createOperator()
{
	auto ret = std::make_shared<::Core::Operator<T>>();
	ret->init();
	ret->updateValues(0);

	return ret;
}
} // namespace Builder

/**
 * Implementation of Node class.
 *
 * \tparam T Type of node to be created.
 */
template <EOperatorType T> class Operator : public Node
{
public:
	/**
	 * Creates an operator of given type (as template parameter).
	 *
	 * <b>DON'T</b> construct object directly, use Core::Builder::createOperator()
	 * function.
	 *
	 * Code taken from I3T v1 Operator<OperatorType>::Operator(...).
	 * Operator is no more associated with GUI, as used to be, so no input
	 * or output names are assigned here.
	 */
	Operator();
	~Operator() override = default;

	/**
	 * Implementation of virtual function Node::updateValues(int).
	 *
	 * Implementation differs for each template parameter. See end of the file
	 * for more details.
	 *
	 * \param inputIndex ???
	 */
	void updateValues(int inputIndex) override;
};

//===-- Member template function definitions.
//------------------------------===//

template <EOperatorType T> Operator<T>::Operator() : Node(&operations[static_cast<unsigned>(T)]) {}

//===-----------------------------------------------------------------------===//

template <EOperatorType T> void Operator<T>::updateValues(int inputIndex)
{
	I3T_ABORT("This function should be specialized!");
}

// inversion
template <> FORCE_INLINE void Operator<EOperatorType::Inversion>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::inverse(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// transpose
template <> FORCE_INLINE void Operator<EOperatorType::Transpose>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::transpose(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// determinant
template <> FORCE_INLINE void Operator<EOperatorType::Determinant>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::determinant(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(1.0f);
	}
}

// mat * mat
template <> FORCE_INLINE void Operator<EOperatorType::MatrixMulMatrix>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		const auto& mat1 = m_inputs[0].data().getMat4();
		const auto& mat2 = m_inputs[1].data().getMat4();
		setInternalValue(mat1 * mat2);
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getMat4());
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// MatrixAddMatrix
template <> FORCE_INLINE void Operator<EOperatorType::MatrixAddMatrix>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4() + m_inputs[1].data().getMat4());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getMat4());
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// MatrixMulVector
template <> FORCE_INLINE void Operator<EOperatorType::MatrixMulVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4() * m_inputs[1].data().getVec4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4());
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// VectorMulMatrix
template <> FORCE_INLINE void Operator<EOperatorType::VectorMulMatrix>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		glm::vec4 vec = m_inputs[0].data().getVec4();
		glm::mat4 mat = m_inputs[1].data().getMat4();
		setInternalValue(vec * mat);
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
	}
	else
	{
		// setInternalValue(glm::mat4());
		setInternalValue(glm::vec4()); // PF - should be vector, not matrix
	}
}

// FloatMulMatrix
template <> FORCE_INLINE void Operator<EOperatorType::MatrixMulFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		float num = m_inputs[0].data().getFloat();
		glm::mat4 mat = m_inputs[1].data().getMat4();
		setInternalValue(num * mat);
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat() * glm::mat4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getMat4());
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::VectorDotVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		auto vec1 = m_inputs[0].data().getVec4();
		auto vec2 = m_inputs[1].data().getVec4();
		setInternalValue(glm::dot(vec1, vec2));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// VectorAddVector
template <> FORCE_INLINE void Operator<EOperatorType::VectorAddVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4() + m_inputs[1].data().getVec4());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4());
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// VectorSubVector
template <> FORCE_INLINE void Operator<EOperatorType::VectorSubVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4() - m_inputs[1].data().getVec4());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4());
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// VectorMulFloat
template <> FORCE_INLINE void Operator<EOperatorType::VectorMulFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4() * m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4());
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// VectorPerspectiveDivision
template <> FORCE_INLINE void Operator<EOperatorType::VectorPerspectiveDivision>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		glm::vec4 in = m_inputs[0].data().getVec4();
		setInternalValue(glm::vec4(in.x / in.w, in.y / in.w, in.z / in.w, 1.0f));
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// NormalizeVector
template <> FORCE_INLINE void Operator<EOperatorType::NormalizeVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getVec4()));
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// MixVector
template <> FORCE_INLINE void Operator<EOperatorType::MixVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(
		    glm::mix(m_inputs[0].data().getVec4(), m_inputs[1].data().getVec4(), m_inputs[2].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec4());
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// Vector3CrossVector3
template <> FORCE_INLINE void Operator<EOperatorType::Vector3CrossVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::cross(m_inputs[0].data().getVec3(), m_inputs[1].data().getVec3()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::Vector3DotVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		auto vec1 = m_inputs[0].data().getVec3();
		auto vec2 = m_inputs[1].data().getVec3();

		setInternalValue(glm::dot(vec1, vec2));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// Vector3AddVector3
template <> FORCE_INLINE void Operator<EOperatorType::Vector3AddVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3() + m_inputs[1].data().getVec3());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// Vector3SubVector3
template <> FORCE_INLINE void Operator<EOperatorType::Vector3SubVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3() - m_inputs[1].data().getVec3());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// Vector3MulFloat
template <> FORCE_INLINE void Operator<EOperatorType::Vector3MulFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3() * m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// NormalizeVector3
template <> FORCE_INLINE void Operator<EOperatorType::NormalizeVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getVec3()));
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// Vector3Length
template <> FORCE_INLINE void Operator<EOperatorType::Vector3Length>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::length(m_inputs[0].data().getVec3()));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// ShowVector3
// Create the matrix rotating vector (1,0,0) to the input vector direction.
// Should be used with the VectorX model to show the inout vector.
template <> FORCE_INLINE void Operator<EOperatorType::ShowVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{

		// setInternalValue(glm::normalize(m_inputs[0].data().getVec3()));

		glm::mat4 m = glm::mat4(1.0f);

		glm::vec3 inNor = glm::normalize(m_inputs[0].data().getVec3());

		float angle = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), inNor));

		if (angle != 0)
		{
			glm::vec3 cross = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), inNor);

			if (glm::length2(cross) != 0)
				m = glm::rotate(m, angle, glm::normalize(cross));
			else
				m = glm::rotate(m, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		m[0] = glm::vec4(inNor, 0.0f);

		setInternalValue(m);
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// MixVector3
template <> FORCE_INLINE void Operator<EOperatorType::MixVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(
		    glm::mix(m_inputs[0].data().getVec3(), m_inputs[1].data().getVec3(), m_inputs[2].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// ConjQuat
template <> FORCE_INLINE void Operator<EOperatorType::ConjQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::conjugate(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// FloatVecToQuat
template <> FORCE_INLINE void Operator<EOperatorType::FloatVecToQuat>::updateValues(int inputIndex)
{
	// PF (1,0,0,0) if (nothing connected) w=1; else w=0;

	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::quat(m_inputs[0].data().getFloat(), m_inputs[1].data().getVec3()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::quat(m_inputs[0].data().getFloat(), glm::vec3()));
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::quat(0.0f, m_inputs[1].data().getVec3()));
	}
	else
		setInternalValue(glm::quat());
}

// AngleAxisToQuat
template <> FORCE_INLINE void Operator<EOperatorType::AngleAxisToQuat>::updateValues(int inputIndex)
{
	/*
\todo
if (m_inputs[0].isPluggedIn()) {
	m_inputs[0].setVisible(true);
	m_inputs[1]->setVisible(false);
}
else
if (m_inputs[1].isPluggedIn()) {
	m_inputs[1]->setVisible(true);
	m_inputs[0]->setVisible(false);
}
else {
	m_inputs[0]->setVisible(true);
	m_inputs[1]->setVisible(true);
}
 */

	// angle - is divided by 2 in glm::angleAxes()
	if (m_inputs[0].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(glm::angleAxis(m_inputs[0].data().getFloat(), glm::normalize(m_inputs[2].data().getVec3())));
	}
	else if (m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		// angle / 2  - given a halfAngle - multiply by 2, as needed by angleAxis

		setInternalValue(
		    glm::angleAxis(m_inputs[1].data().getFloat() * 2.0f, glm::normalize(m_inputs[2].data().getVec3())));
	}
	else if (m_inputs[2].isPluggedIn())
	{
		// setInternalValue(glm::quat(0.0f, m_inputs[2].data().getVec3()));
		setInternalValue(glm::angleAxis(0.0f, glm::normalize(m_inputs[2].data().getVec3()))); // bud zadat 1.0f, nebo
		                                                                                      // angleAxis(0.0f,...)
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// VecVecToQuat
template <> FORCE_INLINE void Operator<EOperatorType::VecVecToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(
		    glm::quat(glm::normalize(m_inputs[0].data().getVec3()), glm::normalize(m_inputs[1].data().getVec3())));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatToFloatVec
template <> FORCE_INLINE void Operator<EOperatorType::QuatToFloatVec>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		auto q = m_inputs[0].data().getQuat();
		setInternalValue(q.w);                         // w
		setInternalValue(glm::vec3(q.x, q.y, q.z), 1); //(x,y,z)
	}
	else
	{
		// a unit quaternion [1,(0,0,0)]
		setInternalValue(1.0f);                           // w
		setInternalValue(glm::vec3(0.0f, 0.0f, 0.0f), 1); //(x,y,z)
	}
}

// QuatToAngleAxis
template <> FORCE_INLINE void Operator<EOperatorType::QuatToAngleAxis>::updateValues(int inputIndex)
{
	// angle
	if (m_inputs[0].isPluggedIn())
	{

		// if (SetupForm::radians)
		// setInternalValue(glm::angle(m_inputs[0].data().getQuat())); else
		// setInternalValue(glm::degrees(glm::angle(m_inputs[0].data().getQuat())));

		setInternalValue(glm::angle(m_inputs[0].data().getQuat()));
		setInternalValue(glm::axis(m_inputs[0].data().getQuat()), 1);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(glm::vec3(), 1); // (0,0,1)
	}
}

// QuatToEuler
template <> FORCE_INLINE void Operator<EOperatorType::QuatToEuler>::updateValues(int inputIndex)
{
	// angle
	if (m_inputs[0].isPluggedIn())
	{

		// glm::vec3 e = glm::eulerAngles(m_inputs[0].data().getQuat());
		// normalization on input seem more error prune for a normal user
		glm::vec3 e = glm::eulerAngles(glm::normalize(m_inputs[0].data().getQuat()));

		setInternalValue(e.x);
		setInternalValue(e.y, 1);
		setInternalValue(e.z, 2);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(0.0f, 1);
		setInternalValue(0.0f, 2);
	}
}

// EulerToQuat
template <> FORCE_INLINE void Operator<EOperatorType::EulerToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{

		setInternalValue(glm::quat(
		    glm::vec3(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(), m_inputs[2].data().getFloat())));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatInverse
template <> FORCE_INLINE void Operator<EOperatorType::QuatInverse>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::inverse(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatSlerp
template <> FORCE_INLINE void Operator<EOperatorType::QuatSlerp>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		// setInternalValue(glm::mix(m_inputs[0].data().getQuat(),
		// m_inputs[1].data().getQuat(),
		// m_inputs[2].data().getFloat()));
		setInternalValue(
		    glm::slerp(m_inputs[0].data().getQuat(), m_inputs[1].data().getQuat(), m_inputs[2].data().getFloat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

/// PF: Long way version of Spherical linear interpolation of two quaternions.
/// The interpolation always take the long path and the rotation is performed at
/// constant speed.
///
/// @param x A quaternion
/// @param y A quaternion
/// @param a Interpolation factor. The interpolation is defined beyond the range
/// [0, 1].
/// @tparam T Value type used to build the quaternion. Supported: half, float or
/// double.
/// @see gtc_quaternion
template <typename T, glm::precision P>
GLM_FUNC_DECL glm::tquat<T, P> longWaySlerp(glm::tquat<T, P> const& x, glm::tquat<T, P> const& y, T a);

// Implementation - a copy of glm::slerp with a negated z
template <typename T, glm::precision P>
GLM_FUNC_QUALIFIER glm::tquat<T, P> longWaySlerp(glm::tquat<T, P> const& x, glm::tquat<T, P> const& y, T a)
{
	GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'longWaySlerp' only accept floating-point inputs");
	glm::qua<T, P> z = y;

	T cosTheta = dot(x, y);

	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To force the long way round interpolation, one quat must be negated if cosTheta > 0.
	if (cosTheta > static_cast<T>(0))
	{
		z = -y;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect
	// of sin(angle) becoming a zero denominator
	if (cosTheta > static_cast<T>(1) - glm::epsilon<T>())
	{
		// Linear interpolation
		return glm::qua<T, P>(glm::mix(x.w, z.w, a), glm::mix(x.x, z.x, a), glm::mix(x.y, z.y, a),
		                      glm::mix(x.z, z.z, a));
	}
	else
	{
		// Essential Mathematics, page 467
		T angle = acos(cosTheta);
		return (sin((static_cast<T>(1) - a) * angle) * x + sin(a * angle) * z) / sin(angle);
	}
}

// QuatLongWaySlerp
template <> FORCE_INLINE void Operator<EOperatorType::QuatLongWaySlerp>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		// setInternalValue(glm::mix(m_inputs[0].data().getQuat(),
		// m_inputs[1].data().getQuat(),
		// m_inputs[2].data().getFloat()));
		setInternalValue(
		    longWaySlerp(m_inputs[0].data().getQuat(), m_inputs[1].data().getQuat(), m_inputs[2].data().getFloat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatLerp
template <> FORCE_INLINE void Operator<EOperatorType::QuatLerp>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{

		float w =
		    glm::mix(m_inputs[0].data().getQuat().w, m_inputs[1].data().getQuat().w, m_inputs[2].data().getFloat());
		float x =
		    glm::mix(m_inputs[0].data().getQuat().x, m_inputs[1].data().getQuat().x, m_inputs[2].data().getFloat());
		float y =
		    glm::mix(m_inputs[0].data().getQuat().y, m_inputs[1].data().getQuat().y, m_inputs[2].data().getFloat());
		float z =
		    glm::mix(m_inputs[0].data().getQuat().z, m_inputs[1].data().getQuat().z, m_inputs[2].data().getFloat());

		setInternalValue(glm::quat(w, x, y, z));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// FloatMulQuat
template <> FORCE_INLINE void Operator<EOperatorType::FloatMulQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat() * m_inputs[1].data().getQuat());
	}
	else if (m_inputs[0].isPluggedIn())
	{ // predpoklada quat (1,0,0,0)
		setInternalValue(m_inputs[0].data().getFloat() * glm::quat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getQuat()); // PF new predpoklada float == 1
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatMulQuat
template <> FORCE_INLINE void Operator<EOperatorType::QuatMulQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getQuat() * m_inputs[1].data().getQuat());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getQuat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getQuat());
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatVecConjQuat = qvq*
template <> FORCE_INLINE void Operator<EOperatorType::QuatVecConjQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		// operator * is overloaded in glm - (quat * vec3) does the sandwich multiplication (quat vec3 quatConjugate)
		setInternalValue(m_inputs[1].data().getQuat() * m_inputs[0].data().getVec3()); // q * v   means   qvq*
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// QuatLength
template <> FORCE_INLINE void Operator<EOperatorType::QuatLength>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::length(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// ClampFloat
template <> FORCE_INLINE void Operator<EOperatorType::ClampFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(
		    glm::clamp(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(), m_inputs[2].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::clamp(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(), 1.0f));
	}
	else if (m_inputs[0].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(glm::clamp(m_inputs[0].data().getFloat(), 0.0f, m_inputs[2].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::clamp(m_inputs[0].data().getFloat(), 0.0f, 1.0f));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// FloatMulFloat
template <> FORCE_INLINE void Operator<EOperatorType::FloatMulFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat() * m_inputs[1].data().getFloat());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getFloat());
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// FloatDivFloat
template <> FORCE_INLINE void Operator<EOperatorType::FloatDivFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat() / m_inputs[1].data().getFloat());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(1.0f / m_inputs[1].data().getFloat());
	}
	else
	{
		setInternalValue(0.0f);
	}
}
// FloatAddFloat
template <> FORCE_INLINE void Operator<EOperatorType::FloatAddFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat() + m_inputs[1].data().getFloat());
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getFloat());
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// FloatPowFloat
template <> FORCE_INLINE void Operator<EOperatorType::FloatPowFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(pow(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getFloat());
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// MixFloat
template <> FORCE_INLINE void Operator<EOperatorType::MixFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		setInternalValue(
		    glm::mix(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(), m_inputs[2].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
	else if (m_inputs[1].isPluggedIn())
	{
		setInternalValue(m_inputs[1].data().getFloat());
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// FloatSinCos
template <> FORCE_INLINE void Operator<EOperatorType::FloatSinCos>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(sin(m_inputs[0].data().getFloat()));
		setInternalValue(cos(m_inputs[0].data().getFloat()), 1);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(1.0f, 1);
	}
}

// ASinACos
template <> FORCE_INLINE void Operator<EOperatorType::ASinACos>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(asin(m_inputs[0].data().getFloat()));
		setInternalValue(acos(m_inputs[0].data().getFloat()), 1);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(1.0f, 1);
	}
}

// Signum
template <> FORCE_INLINE void Operator<EOperatorType::Signum>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		float inputValue = m_inputs[0].data().getFloat();
		setInternalValue((inputValue > 0) ? 1.0f : ((inputValue < 0) ? -1.0f : 0.0f));
	}
	else
	{
		setInternalValue(0.0f);
	}
}

// MatrixToVectors
template <> FORCE_INLINE void Operator<EOperatorType::MatrixToVectors>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4()[0]);
		setInternalValue(m_inputs[0].data().getMat4()[1], 1);
		setInternalValue(m_inputs[0].data().getMat4()[2], 2);
		setInternalValue(m_inputs[0].data().getMat4()[3], 3);
	}
	else
	{
		setInternalValue(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		setInternalValue(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 1);
		setInternalValue(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), 2);
		setInternalValue(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 3);
	}
}

// Vectors3ToMatrix
template <> FORCE_INLINE void Operator<EOperatorType::Vectors3ToMatrix>::updateValues(int inputIndex)
{
	glm::mat4 tmp = glm::mat4();

	if (m_inputs[0].isPluggedIn())
		tmp[0] = glm::vec4(m_inputs[0].data().getVec3(), 0.0f);
	if (m_inputs[1].isPluggedIn())
		tmp[1] = glm::vec4(m_inputs[1].data().getVec3(), 0.0f);
	if (m_inputs[2].isPluggedIn())
		tmp[2] = glm::vec4(m_inputs[2].data().getVec3(), 0.0f);
	if (m_inputs[3].isPluggedIn())
		tmp[3] = glm::vec4(m_inputs[3].data().getVec3(), 1.0f);

	setInternalValue(tmp);
}

// VectorsToMatrix
template <> FORCE_INLINE void Operator<EOperatorType::VectorsToMatrix>::updateValues(int inputIndex)
{
	glm::mat4 tmp = glm::mat4();

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getVec4();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getVec4();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getVec4();
	if (m_inputs[3].isPluggedIn())
		tmp[3] = m_inputs[3].data().getVec4();

	setInternalValue(tmp);
}

// MatrixToFloats
template <> FORCE_INLINE void Operator<EOperatorType::MatrixToFloats>::updateValues(int inputIndex)
{
	glm::mat4 tmp = glm::mat4();

	if (m_inputs[0].isPluggedIn())
	{
		tmp = m_inputs[0].data().getMat4();
	}

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			m_internalData[i * 4 + k].setValue(tmp[i][k]);
		}
	}
}

// FloatsToMatrix
template <> FORCE_INLINE void Operator<EOperatorType::FloatsToMatrix>::updateValues(int inputIndex)
{
	glm::mat4 tmp = glm::mat4();

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if (m_inputs[i * 4 + k].isPluggedIn())
				tmp[i][k] = m_inputs[i * 4 + k].data().getFloat();
		}
	}

	setInternalValue(tmp);
}

// MatrixToTR
template <> FORCE_INLINE void Operator<EOperatorType::MatrixToTR>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		glm::mat4 T = glm::mat4();
		T[3] = glm::vec4(glm::vec3(m_inputs[0].data().getMat4()[3]), 1.0f);
		setInternalValue(T);
		setInternalValue(glm::mat4(glm::mat3(m_inputs[0].data().getMat4())), 1);
	}
	else
	{
		setInternalValue(glm::mat4());
		setInternalValue(glm::mat4(), 1);
	}
}

// TRToMatrix
template <> FORCE_INLINE void Operator<EOperatorType::TRToMatrix>::updateValues(int inputIndex)
{
	glm::mat4 tmp = glm::mat4();

	if (m_inputs[0].isPluggedIn())
	{
		tmp[3] = glm::vec4(glm::vec3(m_inputs[0].data().getMat4()[3]), 1.0f);
	}

	if (m_inputs[1].isPluggedIn())
	{
		tmp[0] = glm::vec4(glm::vec3(m_inputs[1].data().getMat4()[0]), 0.0f);
		tmp[1] = glm::vec4(glm::vec3(m_inputs[1].data().getMat4()[1]), 0.0f);
		tmp[2] = glm::vec4(glm::vec3(m_inputs[1].data().getMat4()[2]), 0.0f);
	}

	setInternalValue(tmp);
}

// MatrixToQuat
template <> FORCE_INLINE void Operator<EOperatorType::MatrixToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::quat_cast(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

// QuatToMatrix
template <> FORCE_INLINE void Operator<EOperatorType::QuatToMatrix>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::mat4_cast(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::mat4());
	}
}

// VectorToFloats
template <> FORCE_INLINE void Operator<EOperatorType::VectorToFloats>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4()[0]);
		setInternalValue(m_inputs[0].data().getVec4()[1], 1);
		setInternalValue(m_inputs[0].data().getVec4()[2], 2);
		setInternalValue(m_inputs[0].data().getVec4()[3], 3);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(0.0f, 1);
		setInternalValue(0.0f, 2);
		setInternalValue(0.0f, 3);
	}
}

// FloatsToVector
template <> FORCE_INLINE void Operator<EOperatorType::FloatsToVector>::updateValues(int inputIndex)
{
	glm::vec4 tmp = glm::vec4();

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getFloat();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getFloat();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getFloat();
	if (m_inputs[3].isPluggedIn())
		tmp[3] = m_inputs[3].data().getFloat();

	setInternalValue(tmp);
}

// VectorTVector3ToFloatsoFloats
template <> FORCE_INLINE void Operator<EOperatorType::Vector3ToFloats>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3()[0]);
		setInternalValue(m_inputs[0].data().getVec3()[1], 1);
		setInternalValue(m_inputs[0].data().getVec3()[2], 2);
	}
	else
	{
		setInternalValue(0.0f);
		setInternalValue(0.0f, 1);
		setInternalValue(0.0f, 2);
	}
}

// FloatsToVector3
template <> FORCE_INLINE void Operator<EOperatorType::FloatsToVector3>::updateValues(int inputIndex)
{
	glm::vec3 tmp = glm::vec3();

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getFloat();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getFloat();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getFloat();

	setInternalValue(tmp);
}

// VectorToVector3
template <> FORCE_INLINE void Operator<EOperatorType::VectorToVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::vec3(m_inputs[0].data().getVec4()));
	}
	else
	{
		setInternalValue(glm::vec3());
	}
}

// Vector3ToVector
template <> FORCE_INLINE void Operator<EOperatorType::Vector3ToVector>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		setInternalValue(glm::vec4(m_inputs[0].data().getVec3(), m_inputs[1].data().getFloat()));
	}
	else if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::vec4(m_inputs[0].data().getVec3(),
		                           0.0f)); // PF implicitne pro vektor se
		                                   // w=0
	}
	else
	{
		setInternalValue(glm::vec4());
	}
}

// QuatToFloats
template <> FORCE_INLINE void Operator<EOperatorType::QuatToFloats>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getQuat()[0]);
		setInternalValue(m_inputs[0].data().getQuat()[1], 1);
		setInternalValue(m_inputs[0].data().getQuat()[2], 2);
		setInternalValue(m_inputs[0].data().getQuat()[3], 3);
	}
	else
	{
		setInternalValue(1.0f); // PF - to return a unit quaternion
		setInternalValue(0.0f, 1);
		setInternalValue(0.0f, 2);
		setInternalValue(0.0f, 3);
	}
}

// FloatsToQuat
template <> FORCE_INLINE void Operator<EOperatorType::FloatsToQuat>::updateValues(int inputIndex)
{
	glm::quat tmp = glm::quat(); // (0,0,0,0) is the default value after construction

	// setup of w=0 is therefore redundant
	// if (m_inputs[0].isPluggedIn() || m_inputs[1].isPluggedIn() || m_inputs[2].isPluggedIn())
	//	tmp[3] = 0.0f;

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getFloat();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getFloat();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getFloat();
	if (m_inputs[3].isPluggedIn())
		tmp[3] = m_inputs[3].data().getFloat(); // w

	setInternalValue(tmp);
}

// NormalizeQuat
template <> FORCE_INLINE void Operator<EOperatorType::NormalizeQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::FloatToFloat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::Vector3ToVector3>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::Vector4ToVector4>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::QuatToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getQuat());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MatrixToMatrix>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeTranslation>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::translate(m_inputs[0].data().getVec3()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeEulerX>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeEulerY>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeEulerZ>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeScale>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::scale(m_inputs[0].data().getVec3()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeAxisAngle>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), m_inputs[1].data().getVec3()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeOrtho>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::ortho(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                            m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat(),
		                            m_inputs[4].data().getFloat(), m_inputs[5].data().getFloat()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakePerspective>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::perspective(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                                  m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeFrustum>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::frustum(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                              m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat(),
		                              m_inputs[4].data().getFloat(), m_inputs[5].data().getFloat()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::MakeLookAt>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(
		    glm::lookAt(m_inputs[0].data().getVec3(), m_inputs[1].data().getVec3(), m_inputs[2].data().getVec3()));
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::Screen>::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(m_inputs[inputIndex].data().getScreen());
	}
}

template <> FORCE_INLINE void Operator<EOperatorType::Pulse>::updateValues(int inputIndex)
{
	if (m_outputs[0].isPluggedIn())
	{
		pulse(0);
	}
}
} // namespace Core
