/**
 * \file Core/GraphManager.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 10.12.2020
 *
 * Interface for the transformation and operator graph.
 */
#pragma once

#include <algorithm>

#include "Cycle.h"
#include "NodeImpl.h"
#include "Operations.h"
#include "Sequence.h"
#include "Transform.h"

namespace Builder
{
/**
 * Create new node.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
template <ENodeType T> FORCE_INLINE Ptr<Core::NodeBase> createNode()
{

	auto ret = std::make_shared<Core::NodeImpl<T>>();
	ret->create();
	ret->updateValues(0);
	return ret;
}

/**
 * Sequence has custom non virtual member functions, so shared_ptr is returned
 * instead of unique_ptr.
 */
Ptr<Core::Sequence> FORCE_INLINE createSequence()
{
	auto ret = std::make_shared<Core::Sequence>();
	ret->create();
	ret->updateValues(0);
	return ret;
}

template <typename T, typename... Args> Ptr<Core::NodeBase> FORCE_INLINE createTransform(Args&&... args)
{
	auto ret = std::make_shared<T>(std::forward<Args>(args)...);
	ret->create();
	ret->reset();
	return ret;
}
} // namespace Builder

namespace Core
{
class GraphManager
{
	/// References to created cycle nodes which needs to be updated.
	static std::vector<Ptr<Cycle>> m_cycles;

public:
  /**
   * Create Cycle
   */
  static Ptr<Core::Cycle> createCycle()
  {
    auto ret = std::make_shared<Core::Cycle>();
    ret->create();
    ret->updateValues(0);

		m_cycles.push_back(ret);

    return ret;
  }

	/**
	 * \param tick in ms
	 */
	static void update(double tick);

	static std::vector<Ptr<Core::Cycle>>& getCycles() { return m_cycles; }

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

	/// Plug first output pin of lhs to the first input pin of rhs.
	static ENodePlugResult plug(const Ptr<Core::NodeBase>& lhs, const Ptr<Core::NodeBase>& rhs);

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
	 */ /* surely not changing the pointer (just object that it points to - Nodebase in Workspacenode is const pointer -> so for calling this function pointers have to be const too) */
	static ENodePlugResult plug(const Ptr<Core::NodeBase>& leftNode, const Ptr<Core::NodeBase>& rightNode,
	                            unsigned parentOutputPinIndex, unsigned myInputPinIndex);

	static ENodePlugResult plugSequenceValueInput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node, unsigned nodeIndex = 0);
	static ENodePlugResult plugSequenceValueOutput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node, unsigned nodeIndex = 0);

	/// Unplug all inputs and outputs.
	static void unplugAll(Ptr<Core::NodeBase>& node);

	/**
	 * Unplug plugged node from given input pin of this node.
	 *
	 * \param index
	 */
	static void unplugInput(Ptr<Core::NodeBase> const & node, int index);

	/**
	 * Unplug all nodes connected to given output pin of this node.
	 *
	 * \param index
	 */
	static void unplugOutput(Ptr<Core::NodeBase>& node, int index);

	static Ptr<NodeBase> getParent(Ptr<Core::NodeBase>& node, size_t index = 0);
	static Ptr<NodeBase> getParent(Ptr<Core::Sequence> node, size_t index = 0);

	/**
	 * \return All nodes connected to given node inputs.
	 */
	static std::vector<Ptr<NodeBase>> getAllInputNodes(Ptr<Core::NodeBase>& node);

	/**
	 * \return All nodes plugged into given node output pins.
	 */
	static std::vector<Ptr<NodeBase>> getAllOutputNodes(Ptr<Core::NodeBase>& node);

	/**
	 * \return All nodes plugged into node input pin on given index.
	 */
	static std::vector<Ptr<NodeBase>> getOutputNodes(Ptr<Core::NodeBase>& node, size_t index);


};

class SequenceTree
{
	Ptr<Sequence> m_beginSequence;

public:
	class MatrixIterator
	{
		friend class SequenceTree;
		SequenceTree* m_tree;
		Ptr<Sequence> m_currentSequence;
		Ptr<NodeBase> m_currentMatrix;

	public:
		MatrixIterator(Ptr<Sequence>& sequence);
		MatrixIterator(Ptr<Sequence>& sequence, NodePtr node);

		/// Move iterator to root sequence.
		MatrixIterator& operator++();

		/// Move iterator to root sequence.
		MatrixIterator operator++(int);

		/// Move iterator towards to the leaf sequence.
		MatrixIterator& operator--();

		/// Move iterator towards to the leaf sequence.
		MatrixIterator operator--(int);

		Ptr<NodeBase> operator*() const;

		bool operator==(const MatrixIterator& rhs) const;
		bool operator!=(const MatrixIterator& rhs) const;

	private:
		/// Move to the next matrix.
		void advance();

		/// Move to the previous matrix.
		void withdraw();
	};

public:
	SequenceTree(Ptr<NodeBase> sequence);

	/**
	 * \return Iterator which points sequence.
	 */
	MatrixIterator begin();

	/**
	 * \return Iterator which points to the sequence root.
	 */
	MatrixIterator end();
};
} // namespace Core
