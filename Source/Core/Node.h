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
#include <vector>

#include "Defs.h"
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

namespace Core
{
class Pin;

/**
 * Base class interface for all boxes.
 * \image html baseOperator.png
 */
class NodeBase
{
  template <ENodeType NodeType> friend class NodeImpl;

  /// Inputs of the box: Input tabs with glyphs.
  std::vector<Pin> m_inputs;

  /// Outputs of the box: output tabs with glyphs.
  std::vector<Pin> m_outputs;

  /// Values which node owns.
  std::vector<DataStore> m_internalData;

protected:
  /** Operator node properties. */
  const Operation* m_operation = nullptr;

  /// \todo Is there values in NodeBase used?
  bool m_pulseOnPlug;      ///< true for all operators except for operatorCycle. used in onOperatorPlugChange
  bool m_restrictedOutput; ///< Restrict the output update to restrictedOutputIndex (used by OperatorPlayerControll
                           ///< only)
  int m_restrictedOutputIndex; ///< Used in OperatorPlayerControll::updateValues(int inputIndex) only

public:
  /**
   * I3T v2 default Operator constructor.
   */
  explicit NodeBase(const Operation* operation)
      : m_operation(operation), m_pulseOnPlug(true), m_restrictedOutput(false), m_restrictedOutputIndex(0)
  {
  }

  /** Delete node and unplug its all inputs and outputs. */
  ~NodeBase();

  //===-- Obtaining value functions. ----------------------------------------===//
  /**
   * Get node internal value
   *
   * \param index Index of the internal modifiable data field (e.g, 0 or 1 for two vectors).
   *              Value of field[0] is returned if this parameter omitted)
   *
   */
  DataStore& getInternalData(unsigned index = 0)
  {
    I3T_DEBUG_ASSERT(m_internalData.size() && m_internalData.size() > index, "Desired data storage does not exist!");

    return m_internalData[index];
  }

  [[nodiscard]] const std::vector<Pin>& getInputPins() const;
  [[nodiscard]] const std::vector<Pin>& getOutputPins() const;
  //===----------------------------------------------------------------------===//

  //===-- Operator manipulation functions. ----------------------------------===//
  /**
   * Connect given node output pin to this operator input pin.
   *
   * Usage:
   * \code
   *    // Create nodes.
   *    auto vec1 = Builder::createNode<OperationType::Vector3>();
   *    auto vec2 = Builder::createNode<OperationType::Vector3>();
   *    auto dotNode = Builder::createNode<OperationType::Vector3DotVector3>();
   *
   *    // Plug vector nodes output to dot node inputs.
   *    dotNode->plug(vec1, 0, 0);
   *    dotNode->plug(vec2, 1, 0);
   * \endcode
   *
   * \param parentNode Reference to a unique pointer to a parent node to which \a parentOutputPinIndex this node
   *        should be connected to.
   * \param parentOutputPinIndex Index of the output pin of the parent node.
   * \param myInputPinIndex Index of input pin of this node.
   *
   * \return Result enum is returned from the function. \see ENodePlugResult.
   */
  ENodePlugResult plugToParent(UPtr<NodeBase>& parentNode, unsigned parentOutputPinIndex, unsigned myInputPinIndex);

  /// Unplug all inputs and outputs.
  void unplugAll();

  /**
   * Unplug plugged node from given input pin of this node.
   *
   * \param index
   */
  void unplugInput(int index);

  /**
   * Unplug all nodes connected to given output pin of this node.
   *
   * \param index
   */
  void unplugOutput(int index);

  /**
   * Is used to check before connecting to avoid cycles in the node graph.
   *
   * The function is used in plug() function.
   *
   * Algorithm described in panel Algoritmus 1 in [Folta, page 30]
   *
   * \param sender probably the input (the wire goes to its input)
   * \param pluged probably the output (the wire starts here)
   *
   * \todo Correct the parameter names. Rename: sender -> receiver or input, plugged -> output or sender
   */
  static ENodePlugResult isOperatorPlugCorrect(Pin* sender, Pin* plugged);
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
};

/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 */
class Pin
{
  template <ENodeType NodeType> friend class NodeImpl;
  friend class NodeBase;

  /// \todo Is pin index maybe unused?
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

  const OpValueType::type m_opValueType = OpValueType::type::PULSE;

public:
  Pin(OpValueType::type valueType, bool isInput, NodeBase* op)
      : m_opValueType(valueType), m_isInput(isInput), m_master(op)
  {
  }

  [[nodiscard]] int getIndex() const { return m_index; }

  [[nodiscard]] const std::vector<Pin*>& getOutComponents() const { return m_outputs; }

  /**
   * Query if input of this object is plugged to any parent output.
   *
   * \return True if plugged to parent, false if not.
   */
  [[nodiscard]] bool isPluggedIn() const { return m_input != nullptr; }

  /**
   * Get stored data based on pin type.
   *
   * \returns data storage owner by node connected to this input pin. If pin is output pin,
   *          it returns data storage of pin owner.
   */
  DataStore& getStorage(unsigned id = 0)
  {
    if (m_isInput)
    {
      I3T_DEBUG_ASSERT(isPluggedIn(), "This input pin is not plugged to any output pin!");
      return m_input->m_master->getInternalData(id);
    }
    else
    {
      return m_master->getInternalData(id);
    }
  }
};
} // namespace Core