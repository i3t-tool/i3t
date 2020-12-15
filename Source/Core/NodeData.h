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
struct OpValueType
{
  enum type
  {
    PULSE = 0,
    ///< pink		(1.0, 0.0, 1.0), purely abstract - used just to provoke the connected operator to perfom some activity
    FLOAT,
    ///< green		(0.0, 0.7, 0.0) standard data type
    VEC3,
    ///< blue		(0.0, 0.2, 1.0)
    VEC4,
    ///< brown		(0.4, 0.3, 0.0)
    MATRIX,
    ///< red		(1.0, 0.7, 0.0)
    QUAT,
    ///< orange		(1.0, 0.1, 0.0)
    MATRIX_MUL,
    ///< white		(1.0, 1.0, 1.0) connection of sequences in the scene graph - represents a matrix multiplication
    SCREEN ///< turqouise	(0.0, 1.0, 1.0) projection and camera view transformation
  };
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
  int opValueType; ///< wire type, such as FLOAT or MATRIX

public:
  /** Default constructor constructs a signal of type OpValueType::MATRIX and undefined value (a unit matrix) */
  // Transmitter() : opValueType(0) { //PFQ: 0 je pulse a value matrix??? - pulse nema typ, tak inicializace default
  // matice Transmitter() : opValueType(OpValueType::MATRIX), value(glm::mat4()) {
  DataStore() : opValueType(OpValueType::MATRIX) { value.matrix = glm::mat4(); }

  /**
      Constructor

      \param	_opValueType	Type of the operation value.
   */
  DataStore(const int _opValueType) : opValueType(_opValueType)
  {
    // Transmitter(int _opValueType) : opValueType(_opValueType), value(glm::mat4()) {

    switch (_opValueType)
    {
    case OpValueType::SCREEN:
      setValue((void*)NULL);
      break;
    case OpValueType::FLOAT:
      setValue(0.0f);
      break;
    case OpValueType::VEC3:
      setValue(glm::vec3());
      break;
    case OpValueType::VEC4:
      setValue(glm::vec4());
      break;
    case OpValueType::QUAT:
      setValue(glm::quat());
      break;
    default: // MATRIX, MATRIX_MUL, PULSE
      // setValue(glm::mat4()); //  set moved to initialiation
      break;
    }
  }

  int getOpValType() const { return opValueType; }

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
