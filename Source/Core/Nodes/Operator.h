/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>, Petr Felkel, felkel@fel.cvut.cz (bug fixes)
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
template <EOperatorType T>
class Operator;

namespace Builder
{
/**
 * Create new operator.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
template <EOperatorType T>
Ptr<Node> createOperator()
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
template <EOperatorType T>
class Operator : public Node
{
	using Super = Node;

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

	void updateValues(int inputIndex) override
	{
		if (updateValuesImpl(inputIndex))
		{
			// Trigger update callback on change
			Super::updateValues(inputIndex);
		}
	}

	/**
	 * Internal implementation of virtual function updateValues(int).
	 * Implementation differs for each template parameter. See end of the file
	 * for more details.
	 * @param inputIndex ???
	 * @return true if any internal value has changed, false otherwise
	 */
	bool updateValuesImpl(int inputIndex);
};

//===-- Member template function definitions.
//------------------------------===//

template <EOperatorType T>
Operator<T>::Operator() : Node(operations[static_cast<unsigned>(T)])
{}

//===-----------------------------------------------------------------------===//

template <EOperatorType T>
bool Operator<T>::updateValuesImpl(int inputIndex)
{
	I3T_ABORT("This function should be specialized!");
	return false;
}

// inversion
template <>
FORCE_INLINE bool Operator<EOperatorType::Inversion>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::inverse(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::mat4(1.0f));
	}
	return true;
}

// transpose
template <>
FORCE_INLINE bool Operator<EOperatorType::Transpose>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::transpose(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::mat4(1.0f));
	}
	return true;
}

// determinant
template <>
FORCE_INLINE bool Operator<EOperatorType::Determinant>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::determinant(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(1.0f);
	}
	return true;
}

// mat * mat
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixMulMatrix>::updateValuesImpl(int inputIndex)
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
		setInternalValue(glm::mat4(1.0f));
	}
	return true;
}

// MatrixAddMatrix - todo probably a never used operation
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixAddMatrix>::updateValuesImpl(int inputIndex)
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
		setInternalValue(glm::mat4(0.0f)); // todo The zero matrix or should it be an Identity?
	}
	return true;
}

// MatrixMulVector
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixMulVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorMulMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorMulMatrix>::updateValuesImpl(int inputIndex)
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
		setInternalValue(glm::vec4()); // PF - should be a vector, not a matrix
	}
	return true;
}

// MatrixMulFloat - todo probably a never used operation
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixMulFloat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
	{
		float num = m_inputs[0].data().getFloat();
		glm::mat4 mat = m_inputs[1].data().getMat4();
		setInternalValue(num * mat);
	}
	else if (m_inputs[0].isPluggedIn())
	{
		// todo A zero matrix or should it be an Identity?
		// Either a zero matrix
		setInternalValue(glm::mat4(0.0f));
		// or float * Identity?
		// setInternalValue(m_inputs[0].data().getFloat() * glm::mat4(1.0f));
	}
	else if (m_inputs[1].isPluggedIn())
	{
		// implicit float == 1.0f
		setInternalValue(m_inputs[1].data().getMat4());
	}
	else
	{
		setInternalValue(glm::mat4(0.0f)); // todo A zero matrix or an Identity?
	}
	return true;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::VectorDotVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorAddVector
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorAddVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorSubVector
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorSubVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorMulFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorMulFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorPerspectiveDivision
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorPerspectiveDivision>::updateValuesImpl(int inputIndex)
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
	return true;
}

// NormalizeVector
template <>
FORCE_INLINE bool Operator<EOperatorType::NormalizeVector>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getVec4()));
	}
	else
	{
		setInternalValue(glm::vec4());
	}
	return true;
}

// MixVector
template <>
FORCE_INLINE bool Operator<EOperatorType::MixVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Vector3CrossVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3CrossVector3>::updateValuesImpl(int inputIndex)
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
	return true;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3DotVector3>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Vector3AddVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3AddVector3>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Vector3SubVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3SubVector3>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Vector3MulFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3MulFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// NormalizeVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::NormalizeVector3>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getVec3()));
	}
	else
	{
		setInternalValue(glm::vec3());
	}
	return true;
}

// Vector3Length
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3Length>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::length(m_inputs[0].data().getVec3()));
	}
	else
	{
		setInternalValue(0.0f);
	}
	return true;
}

// ShowVector3
// Create the matrix rotating vector (1,0,0) to the input vector direction.
// Should be used with the VectorX model to show the input vector.
template <>
FORCE_INLINE bool Operator<EOperatorType::ShowVector3>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{

		// setInternalValue(glm::normalize(m_inputs[0].data().getVec3()));

		glm::mat4 m = glm::mat4(1.0f);

		glm::vec3 inNormalized = glm::normalize(m_inputs[0].data().getVec3());

		float angle = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), inNormalized));

		if (angle != 0)
		{
			glm::vec3 cross = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), inNormalized);

			if (glm::length2(cross) != 0)
				m = glm::rotate(m, angle, glm::normalize(cross));
			else
				m = glm::rotate(m, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		m[0] = glm::vec4(inNormalized, 0.0f);

		setInternalValue(m);
	}
	else
	{
		setInternalValue(glm::mat4(1.0f));
	}
	return true;
}

// MixVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::MixVector3>::updateValuesImpl(int inputIndex)
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
	return true;
}

// ConjQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::ConjQuat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::conjugate(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
	return true;
}

// FloatVecToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatVecToQuat>::updateValuesImpl(int inputIndex)
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
	{
		setInternalValue(glm::quat());
	}
	return true;
}

// AngleAxisToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::AngleAxisToQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VecVecToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::VecVecToQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatToFloatVec
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToFloatVec>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatToAngleAxis
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToAngleAxis>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatToEuler
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToEuler>::updateValuesImpl(int inputIndex)
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
	return true;
}

// EulerToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::EulerToQuat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{

		setInternalValue(glm::quat(
		    glm::vec3(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(), m_inputs[2].data().getFloat())));
	}
	else
	{
		setInternalValue(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	}
	return true;
}

// QuatInverse
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatInverse>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::inverse(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat()); // (0,0,0,0)
	}
	return true;
}

// QuatSlerp
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatSlerp>::updateValuesImpl(int inputIndex)
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
	return true;
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
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatLongWaySlerp>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatLerp
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatLerp>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatMulQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatMulQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatMulQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatMulQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatVecConjQuat = qvq*
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatVecConjQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatLength
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatLength>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::length(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(0.0f);
	}
	return true;
}

// ClampFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::ClampFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatMulFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatMulFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatDivFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatDivFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}
// FloatAddFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatAddFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatPowFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatPowFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// MixFloat
template <>
FORCE_INLINE bool Operator<EOperatorType::MixFloat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatSinCos
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatSinCos>::updateValuesImpl(int inputIndex)
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
	return true;
}

// ASinACos
template <>
FORCE_INLINE bool Operator<EOperatorType::ASinACos>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Signum
template <>
FORCE_INLINE bool Operator<EOperatorType::Signum>::updateValuesImpl(int inputIndex)
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
	return true;
}

// MatrixToVectors
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixToVectors>::updateValuesImpl(int inputIndex)
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
	return true;
}

// Vectors3ToMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::Vectors3ToMatrix>::updateValuesImpl(int inputIndex)
{
	glm::mat4 tmp = glm::mat4(1.0f); // Identity is probably more useful for graphics

	if (m_inputs[0].isPluggedIn())
		tmp[0] = glm::vec4(m_inputs[0].data().getVec3(), 0.0f);
	if (m_inputs[1].isPluggedIn())
		tmp[1] = glm::vec4(m_inputs[1].data().getVec3(), 0.0f);
	if (m_inputs[2].isPluggedIn())
		tmp[2] = glm::vec4(m_inputs[2].data().getVec3(), 0.0f);
	if (m_inputs[3].isPluggedIn())
		tmp[3] = glm::vec4(m_inputs[3].data().getVec3(), 1.0f);

	setInternalValue(tmp);
	return true;
}

// VectorsToMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorsToMatrix>::updateValuesImpl(int inputIndex)
{
	glm::mat4 tmp = glm::mat4(1.0f); // Identity is probably more useful for graphics

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getVec4();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getVec4();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getVec4();
	if (m_inputs[3].isPluggedIn())
		tmp[3] = m_inputs[3].data().getVec4();

	setInternalValue(tmp);
	return true;
}

// MatrixToFloats
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixToFloats>::updateValuesImpl(int inputIndex)
{
	glm::mat4 tmp = glm::mat4(0.0f);

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
	return true;
}

// FloatsToMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatsToMatrix>::updateValuesImpl(int inputIndex)
{
	glm::mat4 tmp = glm::mat4(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if (m_inputs[i * 4 + k].isPluggedIn())
				tmp[i][k] = m_inputs[i * 4 + k].data().getFloat();
		}
	}

	setInternalValue(tmp);
	return true;
}

// MatrixToTR
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixToTR>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		glm::mat4 T = glm::mat4(1.0f);
		T[3] = glm::vec4(glm::vec3(m_inputs[0].data().getMat4()[3]), 1.0f);
		setInternalValue(T);
		setInternalValue(glm::mat4(glm::mat3(m_inputs[0].data().getMat4())), 1);
	}
	else
	{
		setInternalValue(glm::mat4(1.0f));
		setInternalValue(glm::mat4(1.0f), 1);
	}
	return true;
}

// TRToMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::TRToMatrix>::updateValuesImpl(int inputIndex)
{
	glm::mat4 tmp = glm::mat4(1.0f);

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
	return true;
}

// MatrixToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixToQuat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::quat_cast(m_inputs[0].data().getMat4()));
	}
	else
	{
		setInternalValue(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	}
	return true;
}

// QuatToMatrix
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToMatrix>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::mat4_cast(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::mat4(1.0f));
	}
	return true;
}

// VectorToFloats
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorToFloats>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatsToVector
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatsToVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// VectorTVector3ToFloatsoFloats
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3ToFloats>::updateValuesImpl(int inputIndex)
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
	return true;
}

// FloatsToVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatsToVector3>::updateValuesImpl(int inputIndex)
{
	glm::vec3 tmp = glm::vec3();

	if (m_inputs[0].isPluggedIn())
		tmp[0] = m_inputs[0].data().getFloat();
	if (m_inputs[1].isPluggedIn())
		tmp[1] = m_inputs[1].data().getFloat();
	if (m_inputs[2].isPluggedIn())
		tmp[2] = m_inputs[2].data().getFloat();

	setInternalValue(tmp);
	return true;
}

// VectorToVector3
template <>
FORCE_INLINE bool Operator<EOperatorType::VectorToVector3>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::vec3(m_inputs[0].data().getVec4()));
	}
	else
	{
		setInternalValue(glm::vec3());
	}
	return true;
}

// Vector3ToVector
template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3ToVector>::updateValuesImpl(int inputIndex)
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
	return true;
}

// QuatToFloats
template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToFloats>::updateValuesImpl(int inputIndex)
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
		setInternalValue(0.0f);
		setInternalValue(0.0f, 1);
		setInternalValue(0.0f, 2);
		setInternalValue(1.0f, 3); // PF - to return a unit quaternion
	}
	return true;
}

// FloatsToQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::FloatsToQuat>::updateValuesImpl(int inputIndex)
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
	return true;
}

// NormalizeQuat
template <>
FORCE_INLINE bool Operator<EOperatorType::NormalizeQuat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::normalize(m_inputs[0].data().getQuat()));
	}
	else
	{
		setInternalValue(glm::quat());
	}
	return true;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::FloatToFloat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getFloat());
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::Vector3ToVector3>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec3());
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::Vector4ToVector4>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getVec4());
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::QuatToQuat>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getQuat());
		return false;
	}
	return true;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MatrixToMatrix>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(m_inputs[0].data().getMat4());
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeTranslation>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::translate(m_inputs[0].data().getVec3()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeEulerX>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(1.0f, 0.0f, 0.0f)));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeEulerY>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(0.0f, 1.0f, 0.0f)));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeEulerZ>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), glm::vec3(0.0f, 0.0f, 1.0f)));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeScale>::updateValuesImpl(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		setInternalValue(glm::scale(m_inputs[0].data().getVec3()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeAxisAngle>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::rotate(m_inputs[0].data().getFloat(), m_inputs[1].data().getVec3()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeOrtho>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::ortho(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                            m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat(),
		                            m_inputs[4].data().getFloat(), m_inputs[5].data().getFloat()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakePerspective>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::perspective(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                                  m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeFrustum>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(glm::frustum(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
		                              m_inputs[2].data().getFloat(), m_inputs[3].data().getFloat(),
		                              m_inputs[4].data().getFloat(), m_inputs[5].data().getFloat()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::MakeLookAt>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(
		    glm::lookAt(m_inputs[0].data().getVec3(), m_inputs[1].data().getVec3(), m_inputs[2].data().getVec3()));
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::Screen>::updateValuesImpl(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		setInternalValue(m_inputs[inputIndex].data().getScreen());
		return true;
	}
	return false;
}

template <>
FORCE_INLINE bool Operator<EOperatorType::PulseToPulse>::updateValuesImpl(int inputIndex)
{
	if (m_outputs[0].isPluggedIn())
	{
		pulse(0);
		return true;
	}
	return false;
}
} // namespace Core
