/**
 * \file Core/NodeData.h
 *
 * Declares the DataStore class which hold data used by node.
 */
#pragma once

#include <set>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

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
  OpValue value;   ///< transmitted data (union of all data types passed along the wire)
  EValueType opValueType; ///< wire type, such as FLOAT or MATRIX

public:
  /** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
  DataStore() : opValueType(EValueType::Matrix) { value.matrix = glm::mat4(); }

  DataStore(EValueType valueType) : opValueType(valueType)
  {
    switch (valueType)
    {
    case EValueType::Screen:
      setValue((void*) nullptr);
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

  glm::mat4& getMat4() { return value.matrix; }

  glm::vec3& getVec3() { return value.vector3; }

  glm::vec4& getVec4() { return value.vector4; }

  glm::quat& getQuat() { return value.quat; }

  float& getFloat() { return value.fValue; }

  OpValue* getValue() { return &value; }

  void setValue(OpValue value) { this->value = value; }

  void*& getPointer() { return value.pointer; }

  void setValue(glm::mat4 mat) { value.matrix = mat; }

  void setValue(glm::vec3 vec) { value.vector3 = vec; }

  void setValue(glm::vec4 vec) { value.vector4 = vec; }

  void setValue(glm::quat q) { value.quat = q; }

  void setValue(float f) { value.fValue = f; }

  void setValue(void* p) { value.pointer = p; }
};
