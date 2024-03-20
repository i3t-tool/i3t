/**
 * \file
 * \brief Declares the DataStore class which hold data used by node.
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <array>
#include <set>
#include <variant>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Utils/Math.h"
#include "Utils/Variant.h"

namespace Core
{
/// In column-major order.
class DataMap
{
	std::array<const unsigned char, 16> m_data;

public:
	DataMap(std::array<const unsigned char, 16> data) : m_data(data){};
	DataMap(const DataMap&) = delete;
	DataMap(DataMap&&) = delete;
	DataMap& operator=(const DataMap&) = delete;
	DataMap& operator=(DataMap&&) = delete;

	unsigned char operator[](size_t i) const
	{
		return m_data[i];
	};
};

/**
 * \brief An operator value type = type of the interconnection wire.
 * The string names are defined in Operations.h, defaultIoNames
 */
enum class EValueType
{
	Pulse = 0,
	Float, ///< standard data type
	Vec3,
	Vec4,
	Matrix,
	Quat,
	MatrixMul, ///< connection of sequences in the scene graph - represents a
	           ///< matrix multiplication - stores a matrix
	Screen,    ///< projection and camera view transformation
	Ptr,
};

/**
 * \brief Float value editable status: b1, b2 - b1 is editable bit, b2 is
 * synergies bit
 *
 * \todo PF:- proc tohle neni po bitech?
 *    	SynergiesBit = 0x0001,		///< 001
 *    	EditableBit  = 0x0002,		///< 010
 *    	PossibleNextBit = 0x0004,	///< 100 ...
 */
enum class EValueState
{
	Editable = 0x0002,    ///< 10
	EditableSyn = 0x0003, ///< 11
	Locked = 0x0000,      ///< 00
	LockedSyn = 0x0001,   ///< 01
};

/**
 * Representation of the interconnection wire value
 * (Shared piece of memory - union of all data types passed along the wire)
 * and of the value stored in the defaultValues map (such as fovy for
 * Perspective)
 *
 * Old name was Transmitter in I3T v1.
 */
class Data
{
public:
	EValueType opValueType; ///< wire type, such as Float or 4x4 Matrix

	using Storage =
	    std::variant<bool, glm::mat4, std::pair<glm::mat4, glm::mat4>, glm::vec3, glm::vec4, glm::quat, float, void*>;

public:
	/** Default constructor constructs a signal of type OpValueType::MATRIX and
	 * with an undefined value (a unit matrix) */
	Data() : opValueType(EValueType::Matrix)
	{
		m_value = glm::mat4(1.0f);
	}

	/**
	 * \brief Explicit constructors from given data values
	 */
	///@{
	explicit Data(const bool val) : opValueType(EValueType::Pulse)
	{
		m_value = val;
	}

	explicit Data(float val) : opValueType(EValueType::Float)
	{
		m_value = val;
	}

	explicit Data(const glm::vec3& val) : opValueType(EValueType::Vec3)
	{
		m_value = val;
	}

	explicit Data(const glm::vec4& val) : opValueType(EValueType::Vec4)
	{
		m_value = val;
	}

	explicit Data(const glm::mat4& val) : opValueType(EValueType::Matrix)
	{
		m_value = val;
	}

	explicit Data(const glm::quat val) : opValueType(EValueType::Quat)
	{
		m_value = val;
	}
	explicit Data(const std::pair<glm::mat4, glm::mat4> val) : opValueType(EValueType::Screen)
	{
		m_value = val;
	}
	///@}

	/** Constructor of given signal type with an undefined value (identity) */
	explicit Data(EValueType valueType);

	[[nodiscard]] bool isPulseTriggered() const
	{
		return std::get<bool>(m_value);
	}

	[[nodiscard]] const glm::mat4& getMat4() const
	{
		return std::get<glm::mat4>(m_value);
	}
	[[nodiscard]] glm::mat4& getMat4Ref()
	{
		return std::get<glm::mat4>(m_value);
	}

	[[nodiscard]] const std::pair<glm::mat4, glm::mat4>& getScreen() const
	{
		return std::get<std::pair<glm::mat4, glm::mat4>>(m_value);
	}

	[[nodiscard]] const glm::vec3& getVec3() const
	{
		return std::get<glm::vec3>(m_value);
	}

	[[nodiscard]] const glm::vec4& getVec4() const
	{
		return std::get<glm::vec4>(m_value);
	}

	[[nodiscard]] const glm::quat& getQuat() const
	{
		return std::get<glm::quat>(m_value);
	}

	[[nodiscard]] float getFloat() const
	{
		return std::get<float>(m_value);
	}

	//

	/// \todo MH Refactor
	template <typename T> std::optional<std::remove_reference_t<T>> getValue() const;

	//

	template <typename T> void setValue(T&& val)
	{
		m_value = val;
	}

	void setValue(const Data& other)
	{
		*this = other;
	}

	std::size_t index() const
	{
		return m_value.index();
	}

private:
	Storage m_value;
};

template <typename T> std::optional<std::remove_reference_t<T>> Data::getValue() const
{
	if (index() != variant_index<Storage, std::remove_reference_t<T>>())
	{
		return std::nullopt;
	}

	return std::get<std::remove_reference_t<T>>(m_value);
}
} // namespace Core
