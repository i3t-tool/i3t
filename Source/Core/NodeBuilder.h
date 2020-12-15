#pragma once

#include "Node.h"
#include "NodeImpl.h"
#include "Operations.h"

namespace Builder
{
/**
 * Create new node.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
template <ENodeType T> UPtr<Core::NodeBase> createNode()
{
  return std::make_unique<Core::NodeImpl<T>>();
}
} // namespace Builder
