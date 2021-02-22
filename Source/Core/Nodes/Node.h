/**
 * \file Core/Node.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
 *
 * Code taken from (former) Source/operator.h, Source/operatorForm.h and Source/operatorNode.h
 * file which used to define Operator, base class for all boxes.
 */
#pragma once

#include <set>
#include <utility>
#include <vector>

#include "Core/Defs.h"
#include "Id.h"
#include "NodeData.h"
#include "Operations.h"

enum class ENodePlugResult
{
  Ok = 0,
  Err_MismatchedPinTypes,
  Err_Loopback,
  Err_NonexistentPin,
  Err_Loop,
};

enum class EValueSetResult
{
  Ok = 0,
  Err_ConstraintViolation
};

struct ValueSetResult
{
  const EValueSetResult m_status;
  const std::string m_message;

  ValueSetResult(EValueSetResult status, std::string message)
    : m_status(status), m_message(std::move(message)) {}
};

namespace Core
{
class Pin;

/**
 * Base class interface for all boxes.
 * \image html baseOperator.png
 */
class NodeBase
{
  friend class GraphManager;

protected:
  ID m_id{};

  /// Inputs of the box: Input tabs with glyphs.
  std::vector<Pin> m_inputs;

  /// Outputs of the box: output tabs with glyphs.
  std::vector<Pin> m_outputs;

  /// Values which node owns.
  std::vector<DataStore> m_initialData;
  std::vector<DataStore> m_internalData;

  Transform::DataMap m_initialMap{};
  Transform::DataMap m_currentMap{};

  /**
   * Operator node properties.
   */
  const Operation* m_operation = nullptr;

  /// \todo Is there values in NodeBase used?
  bool m_pulseOnPlug{};      ///< true for all operators except for operatorCycle. used in onOperatorPlugChange
  bool m_restrictedOutput{}; ///< Restrict the output update to restrictedOutputIndex (used by OperatorPlayerControll
                             ///< only)
  int m_restrictedOutputIndex{}; ///< Used in OperatorPlayerControll::updateValues(int inputIndex) only

public:
  NodeBase() = default;

  explicit NodeBase(const Operation* operation);

  /** Delete node and unplug its all inputs and outputs. */
  virtual ~NodeBase();

  [[nodiscard]] ID getId() const;

  //===-- Obtaining value functions. ----------------------------------------===//
  /**
   * Get node internal value
   *
   * \param index Index of the internal modifiable data field (e.g, 0 or 1 for two vectors).
   *              Value of field[0] is returned if this parameter omitted)
   */
  DataStore& getInternalData(unsigned index = 0)
  {
    Debug::Assert(!m_internalData.empty() && m_internalData.size() > index, "Desired data storage does not exist!");

    return m_internalData[index];
  }

  DataStore& getData() { return m_internalData[0]; }

  /**
   * Set a value of node.
   *
   * Sets value of the first float of DataStore. Derived types may override
   * default behaviour.
   *
   * \param val
   */
  virtual EValueSetResult setValue(float val)
  {
    m_internalData[0].setValue(val);
    return EValueSetResult::Ok;
  }

  virtual EValueSetResult setValue(const glm::vec3& vec)
  {
    m_internalData[0].setValue(vec);
    return EValueSetResult::Ok;
  }

  virtual EValueSetResult setValue(const glm::vec4& vec)
  {
    m_internalData[0].setValue(vec);
    return EValueSetResult::Ok;
  }

  virtual EValueSetResult setValue(const glm::mat4& mat)
  {
    if (m_currentMap == Transform::g_Free)
      m_internalData[0].setValue(mat);

    return EValueSetResult::Ok;
  }

  virtual EValueSetResult setValue(float val, glm::ivec2 cords)
  {
    Debug::Assert(false, "Unsupported operation!");
    return EValueSetResult::Err_ConstraintViolation;
  }


  virtual void reset() {}

  /**
   * Smart set function, used with constrained transformation for value checking.
   *
   * \param mask array of 16 chars.
   * \param mat
   */
  virtual EValueSetResult setValue(const glm::mat4& mat, const Transform::DataMap& map) { return EValueSetResult::Ok; }
  virtual void setDataMap(const Transform::DataMap& map) { m_currentMap = map; }

  [[nodiscard]] const std::vector<Pin>& getInputPins() const;
  [[nodiscard]] const std::vector<Pin>& getOutputPins() const;
  //===----------------------------------------------------------------------===//

  //===-- Values updating functions. ----------------------------------------===//
  /**
   * Computes new values of outputs based on inputs.
   *
   * Encodes the box function by updating the output values for a
   * given \a inputIndex - fired by receiveSignal().
   *
   * \todo Is it correct also for multiple edges? If the edges from one module are connected to more than one input,
   * and the implementation of updateValues uses the \a inputIndex, only subset of outputs may be updated.
   * PF: This method is intended for complex modules to allow for optimization.
   * May be replaced by updateValues() or implemented in such a way. Do such optimizable modules exist?
   *
   * \param	inputIndex Index of the modified input.
   */
  virtual void updateValues(int inputIndex) = 0;

  /// Spread signal to all outputs.
  /// \todo Does not use operators for calling each follower just once
  void spreadSignal();

  /// Spread signal to the selected output \a outIndex only.
  void spreadSignal(int outIndex);

  /**
   * Implements the operator reaction to the change of its \a inputIndex input.
   *
   * 1. Forces the operator to recompute its outputs based on the changed input \a
   *    inputIndex byc calling updateValues() and <BR>
   * 2. Spread signal to the connected children by spreadSignal().
   *
   * This method is called by the spreadSignal() of the parent box in the
   * scene graph. The computation may be restricted by internal bool value
   * \a restrictedOutput to a single previously defined output index \a restrictedOutputIndex.
   *
   * \param	inputIndex	Index of the input that was changed and that forces the operator to recompute its outputs.
   */
  void receiveSignal(int inputIndex);
  //===----------------------------------------------------------------------===//

private:
  /// \todo unplug* are internal functions.
  void _unplugAll();
  void _unplugInput(int index);
  void _unplugOutput(int index);
};

/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 */
class Pin
{
  friend class GraphManager;
  // template <ENodeType NodeType> friend class NodeImpl;
  friend class NodeBase;

  ID m_id;

  /// Index within a node.
  int m_index = -1;

  /// Pin type.
  const bool m_isInput;

  /// Owner of the pin.
  NodeBase* m_master;

  /**
   * The box can have a single parent. Therefore, just a single input component
   * (a single connected wire) to output tab of the parent node).
   */
  Pin* m_input = nullptr;

  /**
   * Child boxes in the scene graph (coming out to the right).
   * A pointer to input pins of boxes connected to this box output.
   */
  std::vector<Pin*> m_outputs;

  const EValueType m_opValueType = EValueType::Pulse;

public:
  Pin(EValueType valueType, bool isInput, NodeBase* op, int index)
      : m_opValueType(valueType), m_isInput(isInput), m_master(op), m_index(index)
  {
    m_id = IdGenerator::next();
  }

  [[nodiscard]] ID getId() const { return m_id; }

  [[nodiscard]] int getIndex() const { return m_index; }

  [[nodiscard]] const Pin* getParentPin() const
  {
    if (m_isInput)
    {
      Debug::Assert(isPluggedIn(), "This input pin is not plugged to any output pin!");
      return m_input;
    }
    else
    {
      Debug::Assert(false, "Output pin can not has parent pin!");
      return nullptr;
    }
  }

  [[nodiscard]] const std::vector<Pin*>& getOutComponents() const { return m_outputs; }

  /**
   * Get stored data based on pin type.
   *
   * \returns data storage owner by node connected to this input pin. If pin is output pin,
   *          it returns data storage of pin owner.
   */
  [[nodiscard]] DataStore& getStorage(unsigned id = 0)
  {
    if (m_isInput)
    {
      Debug::Assert(isPluggedIn(), "This input pin is not plugged to any output pin!");
      return m_input->m_master->getInternalData(id);
    }
    else
    {
      return m_master->getInternalData(id);
    }
  }

  [[nodiscard]] EValueType getType() const { return m_opValueType; }

  /**
   * Query if input of this object is plugged to any parent output.
   *
   * \return True if plugged to parent, false if not.
   */
  [[nodiscard]] bool isPluggedIn() const { return m_input != nullptr; }
};
} // namespace Core
