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

namespace Core::Transform
{
/// In column-major order.
class DataMap
{
  std::array<const unsigned char, 16> m_data;

public:
	DataMap(std::array<const unsigned char, 16> data) : m_data(data) {};
	//DataMap(const DataMap&) = delete;
	//DataMap(DataMap&&) = delete;
  //DataMap& operator=(const DataMap&) = delete;
  //DataMap& operator=(DataMap&&) = delete;

  unsigned char operator[](size_t i) const { return m_data[i]; };
};
} // namespace Core::Transform

namespace Core
{
FORCE_INLINE bool coordsAreValid(const glm::ivec2& coords, const Transform::DataMap* mapRef)
{
	int x = coords[0];
	int y = coords[1];

	auto& map = *mapRef;

	return map[4 * x + y] != 255 && map[4 * x + y] != 0;
}
} // namespace Core

namespace Core::Transform
{
static constexpr uint8_t ZERO = 0;
static constexpr uint8_t ONE = 255;

inline const DataMap g_Free({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

inline const DataMap g_Scale({1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 255});

inline const DataMap g_UniformScale({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 255});

inline const DataMap g_EulerX({255, 0, 0, 0, 0, 1, 2, 0, 0, 3, 1, 0, 0, 0, 0, 255});

inline const DataMap g_EulerY({1, 0, 2, 0, 0, 255, 0, 0, 3, 0, 1, 0, 0, 0, 0, 255});

inline const DataMap g_EulerZ({1, 2, 0, 0, 3, 1, 0, 0, 0, 0, 255, 0, 0, 0, 0, 255});

inline const DataMap g_Translate({
		255, 0, 0, 0, 0, 255, 0, 0, 0, 0, 255, 0, 1, 2, 3, 255,
});

inline const DataMap g_AllLocked({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

inline const DataMap g_Ortho({
		1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 4, 5, 6, 255,
});

inline const DataMap g_Frustum({1, 0, 0, 0, 0, 2, 0, 0, 3, 4, 5, 6, 0, 0, 7, 0});

inline const DataMap g_Perspective({
		1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 4, 0, 0, 5, 0,
});

/**
 * Return whether DataMaps are same.
 */
FORCE_INLINE bool eq(const DataMap& lhs, const DataMap& rhs)
{
	return &lhs == &rhs;
}

/**
 * Compare data map with matrix..
 */
bool cmp(const DataMap* map, const glm::mat4& mat);

FORCE_INLINE bool isMatValid(const DataMap* map, const glm::mat4& mat)
{
	return cmp(map, mat);
}
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
	Screen     ///< projection and camera view transformation
};

/**
 * Representation of the interconnection wire value
 * (Shared piece of memory - union of all data types passed along the wire)
 *
 * Old name was Transmitter in I3T v1.
 */
class DataStore
{
protected:
	std::variant<glm::mat4, glm::vec3, glm::vec4, glm::quat, float, void*> m_value;
	EValueType opValueType; ///< wire type, such as FLOAT or MATRIX

public:
	/** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
	DataStore() : opValueType(EValueType::Matrix) { m_value = glm::mat4(1.0f); }

	DataStore(EValueType valueType) : opValueType(valueType)
	{
		switch (valueType)
		{
		case EValueType::Screen:
			setValue((void*)nullptr);
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
			setValue(glm::mat4(1.0f));
		default:
			break;
		}
	}

	[[nodiscard]] EValueType getOpValType() const { return opValueType; }
	[[nodiscard]] const glm::mat4& getMat4() const { return std::get<glm::mat4>(m_value); }
	[[nodiscard]] glm::mat4& getMat4Ref() { return std::get<glm::mat4>(m_value); }
	[[nodiscard]] const glm::vec3& getVec3() const { return std::get<glm::vec3>(m_value); }
	[[nodiscard]] glm::vec3& getVec3Ref() { return std::get<glm::vec3>(m_value); }
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
