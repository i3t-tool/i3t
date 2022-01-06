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
	DataMap(DataMap&&) = delete;
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

inline const std::string& valueTypeToString(EValueType type)
{
	static const std::string names[] = {
			"Pulse",
			"Float", ///< standard data type
			"Vec3",
			"Vec4",
			"Matrix",
			"Quat",
			"MatrixMul", ///< connection of sequences in the scene graph - represents a matrix multiplication
			"Screen",    ///< projection and camera view transformation
			"Ptr",
	};
	return names[static_cast<int>(type)];
}

/**
 * Representation of the interconnection wire value
 * (Shared piece of memory - union of all data types passed along the wire)
 *
 * Old name was Transmitter in I3T v1.
 */
class DataStore
{
protected:
	std::variant<bool, glm::mat4, glm::vec3, glm::vec4, glm::quat, float, void*> m_value;
	EValueType opValueType; ///< wire type, such as FLOAT or MATRIX

public:
	/** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
	DataStore() : opValueType(EValueType::Matrix) { m_value = glm::mat4(1.0f); }

	explicit DataStore(EValueType valueType) : opValueType(valueType)
	{
		switch (valueType)
		{
		case EValueType::Pulse:
			setValue(false);
			break;
		case EValueType::Ptr:
		case EValueType::Screen:
			// setValue((void*)nullptr);
			setValue(glm::mat4(1.0f));
			break;
		case EValueType::Float:
			setValue(0.0f);
			break;
		case EValueType::Vec3:
			setValue(glm::vec3());
			break;
		case EValueType::Vec4:
			setValue(glm::vec4());
			break;
		case EValueType::Quat:
			setValue(glm::quat());
			break;
		case EValueType::Matrix:
		case EValueType::MatrixMul:
			setValue(glm::mat4(1.0f));
		default:
			break;
		}
	}

	[[nodiscard]] EValueType getOpValType() const { return opValueType; }
 	[[nodiscard]] bool isPulseTriggered() const { return std::get<bool>(m_value); }
	[[nodiscard]] const glm::mat4& getMat4() const { return std::get<glm::mat4>(m_value); }
	[[nodiscard]] glm::mat4& getMat4Ref() { return std::get<glm::mat4>(m_value); }
	[[nodiscard]] const glm::vec3& getVec3() const { return std::get<glm::vec3>(m_value); }
	[[nodiscard]] glm::vec3& getVec3Ref() { return std::get<glm::vec3>(m_value); }
	[[nodiscard]] const glm::vec4& getVec4() const { return std::get<glm::vec4>(m_value); }

	[[nodiscard]] const glm::quat& getQuat() const { return std::get<glm::quat>(m_value); }
	[[nodiscard]] float getFloat() const { return std::get<float>(m_value); }
	[[nodiscard]] void* getPointer() const { return std::get<void*>(m_value); }

	template <typename T> void setValue(T&& val) { m_value = val; }
};
