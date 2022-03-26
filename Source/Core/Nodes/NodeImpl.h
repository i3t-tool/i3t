/**
 * \file Core/NodeImpl.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
 */
#pragma once

#include <type_traits>
#include <variant>

#include "Core/Nodes/Node.h"

#include "World/GameObject.h"

namespace Core
{
template <ENodeType T> class NodeImpl;

namespace Builder
{
	/**
	 * Create new operator.
	 *
	 * \tparam T Operation type from OperationType enum.
	 * \return Unique pointer to newly created logic operator.
	 */
	template <ENodeType T>
	Ptr<NodeBase> createNode()
	{
		auto ret = std::make_shared<NodeImpl<T>>();
		ret->init();

		ret->updateValues(0);
		return ret;
	}
}

/**
 * Implementation of NodeBase class.
 *
 * \tparam T Type of node to be created.
 */
template <ENodeType T> class NodeImpl : public Node
{
public:
	/**
	 * Creates an operator of given type (as template parameter).
	 *
	 * <b>DON'T</b> construct object directly, use Core::Builder::createNode() function.
	 *
	 * Code taken from I3T v1 NodeImpl<OperatorType>::NodeImpl(...).
	 * Operator is no more associated with GUI, as used to be, so no input
	 * or output names are assigned here.
	 */
	NodeImpl();
	~NodeImpl() override = default;

	Ptr<Node> clone() override
	{
		auto node = Builder::createNode<T>();

		if (m_operation->isConstructor)
		{
			for (auto i = 0L; i < m_internalData.size(); ++i)
			{
				node->getInternalData() = m_internalData[i];
			}
		}

		return node;
	}

	/**
	 * Implementation of virtual function NodeBase::updateValues(int).
	 *
	 * Implementation differs for each template parameter. See end of the file
	 * for more details.
	 *
	 * \param inputIndex ???
	 */
	void updateValues(int inputIndex) override;
};

//===-- Member template function definitions. ------------------------------===//

template <ENodeType T> NodeImpl<T>::NodeImpl() : NodeBase(&operations[static_cast<unsigned>(T)])
{
}

//===-----------------------------------------------------------------------===//

template <ENodeType T> void NodeImpl<T>::updateValues(int inputIndex)
{
}
} // namespace Core
