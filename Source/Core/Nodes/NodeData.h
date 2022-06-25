/**
 * \file Core/NodeData.h
 *
 * Declares the DataStore class which hold data used by node.
 */
#pragma once

#include <array>
#include <set>
#include <variant>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Utils/Math.h"

/// \todo MH - Will be removed
namespace Core::Transform
{
/// In column-major order.
class DataMap
{
	std::array<const unsigned char, 16> m_data;

public:
	DataMap(std::array<const unsigned char, 16> data) : m_data(data){};
	DataMap(const DataMap&) = delete;
	DataMap(DataMap&&)      = delete;
	DataMap& operator=(const DataMap&) = delete;
	DataMap& operator=(DataMap&&) = delete;

	unsigned char operator[](size_t i) const { return m_data[i]; };
};
} // namespace Core::Transform

/** An operator value type = type of the interconnection wire. */
enum class EValueType
{
	Pulse = 0,
	Float, ///< standard data type
	Vec3,
	Vec4,
	Matrix,
	Quat,
	MatrixMul, ///< connection of sequences in the scene graph - represents a matrix multiplication
	Screen,    ///< projection and camera view transformation
	Ptr,
};

namespace Core
{
/**
 * \brief Float value editable status: b1, b2 - b1 is editable bit, b2 is synergies bit
 *
 * \todo PF:- proc tohle neni po bitech?
 *    	SynergiesBit = 0x0001,		///< 001
 *    	EditableBit  = 0x0002,		///< 010
 *    	PossibleNextBit = 0x0004,	///< 100 ...
 */
enum class EValueState
{
	Editable    = 0x0002, ///< 10
	EditableSyn = 0x0003, ///< 11
	Locked      = 0x0000, ///< 00
	LockedSyn   = 0x0001, ///< 01
};
} // namespace Core

/**
 * \brief string name of the wire type
 * \param type Enum of the value of the wire
 * \return string name of the wire type
 */
const std::string& valueTypeToString(EValueType type);

/**
 * Representation of the interconnection wire value
 * (Shared piece of memory - union of all data types passed along the wire)
 * and of the value stored in the defaultValues map (such as fovy for Perspective)
 *
 * Old name was Transmitter in I3T v1.
 */
class DataStore
{
public:
	EValueType opValueType; ///< wire type, such as Float or 4x4 Matrix

protected:
	std::variant<bool, glm::mat4, glm::vec3, glm::vec4, glm::quat, float, void*> m_value;

public:
	/** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
	DataStore() : opValueType(EValueType::Matrix) { m_value = glm::mat4(1.0f); }

	explicit DataStore(EValueType valueType);

	[[nodiscard]] bool isPulseTriggered() const { return std::get<bool>(m_value); }

	[[nodiscard]] const glm::mat4& getMat4() const { return std::get<glm::mat4>(m_value); }
	[[nodiscard]] glm::mat4&       getMat4Ref() { return std::get<glm::mat4>(m_value); }

	[[nodiscard]] const glm::vec3& getVec3() const { return std::get<glm::vec3>(m_value); }
	[[nodiscard]] glm::vec3&       getVec3Ref() { return std::get<glm::vec3>(m_value); }

	[[nodiscard]] const glm::vec4& getVec4() const { return std::get<glm::vec4>(m_value); }

	[[nodiscard]] const glm::quat& getQuat() const { return std::get<glm::quat>(m_value); }

	[[nodiscard]] float getFloat() const { return std::get<float>(m_value); }

	[[nodiscard]] void* getPointer() const { return std::get<void*>(m_value); }

	template <typename T>
	void setValue(T&& val)
	{
		m_value = val;
	}
};

namespace Core
{
/**
 * \brief A synonym for DataStore - used for storage of DefaultValues[key]
 */
using Data = DataStore;
}
