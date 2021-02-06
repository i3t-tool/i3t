/**
 * \file Core/GraphManager.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 10.12.2020
 *
 * Interface for the transformation and operator graph.
 */
#pragma once

#include "Core/Nodes/NodeImpl.h"
#include "Core/Nodes/Operations.h"
#include "Sequence.h"
#include <algorithm>

namespace Builder
{
/**
 * Create new node.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
template <ENodeType T> FORCE_INLINE UPtr<Core::NodeBase> createNode()
{
  Debug::Assert(T != ENodeType::Sequence, "Use createSequence instead.");

  return std::make_unique<Core::NodeImpl<T>>();
}

/**
 * Sequence has custom non virtual member functions, so shared_ptr is returned
 * instead of unique_ptr.
 */
Ptr<Core::Sequence> FORCE_INLINE createSequence()
{
  return std::make_shared<Core::Sequence>();
}

template <typename T, typename... Args>
Ptr<Core::NodeBase> FORCE_INLINE createTransform(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace Builder

namespace Core
{
class GraphManager
{
public:
  /**
   * Is used to check before connecting to avoid cycles in the node graph.
   *
   * The function is used in plug() function.
   *
   * Algorithm described in panel Algoritmus 1 in [Folta, page 30]
   *
   * \param input
   * \param output
   */
  static ENodePlugResult isPlugCorrect(Pin* input, Pin* output);

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
   *    GraphManager::plug(vec1, dotNode, 0, 0);
   *    GraphManager::plug(vec2, dotNode, 1, 0);
   * \endcode
   *
   * \param parentNode Reference to a unique pointer to a parent node to which \a parentOutputPinIndex this node
   *        should be connected to.
   * \param rightNode node which should be connected to right node output.
   * \param parentOutputPinIndex Index of the output pin of the parent node.
   * \param myInputPinIndex Index of input pin of this node.
   *
   * \return Result enum is returned from the function. \see ENodePlugResult.
   */
  static ENodePlugResult plug(UPtr<Core::NodeBase>& leftNode, UPtr<Core::NodeBase>& rightNode, unsigned parentOutputPinIndex, unsigned myInputPinIndex);

  /// Unplug all inputs and outputs.
  static void unplugAll(UPtr<Core::NodeBase>& node);

  /**
   * Unplug plugged node from given input pin of this node.
   *
   * \param index
   */
  static void unplugInput(UPtr<Core::NodeBase>& node, int index);

  /**
   * Unplug all nodes connected to given output pin of this node.
   *
   * \param index
   */
  static void unplugOutput(UPtr<Core::NodeBase>& node, int index);
};
}
