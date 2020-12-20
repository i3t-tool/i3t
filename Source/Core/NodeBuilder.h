#pragma once

#include "Node.h"
#include "NodeImpl.h"
#include "Nodes/Sequence.h"
#include "Operations.h"

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
  I3T_DEBUG_ASSERT(T != ENodeType::Sequence, "Use createSequence instead.");

  return std::make_unique<Core::NodeImpl<T>>();
}

/**
 * Sequence has custom non virtual member functions, so shared_ptr is returned
 * instead of unique_ptr.
 */
FORCE_INLINE Ptr<Core::Sequence> createSequence()
{
  return std::make_shared<Core::Sequence>();
}
} // namespace Builder
