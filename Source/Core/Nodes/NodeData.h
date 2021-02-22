/**
 * \file Core/NodeData.h
 *
 * Declares the DataStore class which hold data used by node.
 */
#pragma once

#include <array>
#include <set>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Utils/Math.h"

namespace Core::Transform
{
typedef std::array<unsigned char, 16> DataMap;
}

namespace Core
{
FORCE_INLINE bool coordsAreValid(const glm::ivec2& coords, const Transform::DataMap& map)
{
  int x = coords[0];
  int y = coords[1];

  return map[4 * x + y] != 255 && map[4 * x + y] != 1;
}
}

namespace Core::Transform
{
static constexpr DataMap g_Free = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
    13, 14, 15, 16
};

static constexpr DataMap g_Scale = {
    1, 0, 0, 0,
    0, 2, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 255,
};

static constexpr DataMap g_UniformScale = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 255,
};

static constexpr DataMap g_EulerX = {
    255, 0, 0, 0, 0, 1, 2, 0, 0, 3, 1, 0, 0, 0, 0, 255,
};

static constexpr DataMap g_EulerY = {1, 0, 2, 0, 0, 255, 0, 0, 3, 0, 1, 0, 0, 0, 0, 255};

static constexpr DataMap g_EulerZ = {1, 2, 0, 0, 3, 1, 0, 0, 0, 0, 255, 0, 0, 0, 0, 255};

static constexpr DataMap g_Translate = {
    0, 0, 0, 1,
    0, 0, 0, 2,
    0, 0, 0, 3,
    0, 0, 0, 255,
};

/**
 * Return whether DataMaps are same.
 */
FORCE_INLINE bool eq(const DataMap& lhs, const DataMap& rhs)
{
  return std::memcmp(&lhs[0], &rhs[0], 16) == 0;
}

/**
 * Compare data map with matrix..
 * \todo Zeroes only are compared.
 */
FORCE_INLINE bool cmp(const DataMap& map, const glm::mat4& mat)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      switch (map[4 * i + j])
      {
      case 0:
        if (!Math::eq(mat[i][j], 0.0f) || !Math::eq(mat[i][j], -0.0f))
        {
          return false;
        }
      case 255:
        if (!Math::eq(mat[i][j], 255.0f))
        {
          return false;
        }
      }
    }
  }

  return true;
}

FORCE_INLINE bool isMatValid(const DataMap& map, const glm::mat4& mat)
{
  return cmp(map, mat);
}
} // namespace Core::TransformMap

/** An operator value type = type of the interconnection wire. */
enum class EValueType
{
  /**
   * purely abstract - used just to provoke the connected operator to perfom some activity
   */
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
 * An operator value = union used as a value passed between the boxes in the scene.
 */
union OpValue
{
  glm::mat4 matrix;
  glm::vec3 vector3;
  glm::vec4 vector4;
  glm::quat quat;

  float fValue;
  void* pointer;
};

/**
 * Representation of the interconnection wire value
 * (Shared piece of memory - union of all data types passed along the wire)
 *
 * Old name is Transmitter in I3T v1.
 */
class DataStore
{
protected:
  OpValue value;          ///< transmitted data (union of all data types passed along the wire)
  EValueType opValueType; ///< wire type, such as FLOAT or MATRIX

public:
  /** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
  DataStore() : opValueType(EValueType::Matrix) { value.matrix = glm::mat4(); }

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
    default: // MATRIX, MATRIX_MUL, PULSE
      // setValue(glm::mat4()); //  set moved to initialiation
      break;
    }
  }

  [[nodiscard]] EValueType getOpValType() const { return opValueType; }
  [[nodiscard]] const glm::mat4& getMat4() const { return value.matrix; }
  [[nodiscard]] glm::mat4& getMat4Ref() { return value.matrix; }
  [[nodiscard]] const glm::vec3& getVec3() const { return value.vector3; }
  [[nodiscard]] const glm::vec4& getVec4() const { return value.vector4; }
  [[nodiscard]] const glm::quat& getQuat() const { return value.quat; }
  [[nodiscard]] float getFloat() const { return value.fValue; }
  OpValue* getValue() { return &value; }
  void*& getPointer() { return value.pointer; }

  void setValue(OpValue value) { this->value = value; }
  void setValue(const glm::mat4& mat) { value.matrix = mat; }
  void setValue(const glm::vec3& vec) { value.vector3 = vec; }
  void setValue(const glm::vec4& vec) { value.vector4 = vec; }
  void setValue(const glm::quat& q) { value.quat = q; }
  void setValue(float f) { value.fValue = f; }
  void setValue(void* p) { value.pointer = p; }
};
