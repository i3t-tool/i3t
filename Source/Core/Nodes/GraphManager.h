/**
 * \file Core/GraphManager.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 10.12.2020
 *
 * Interface for the transformation and operator graph.
 */
#pragma once

#include <algorithm>

#include "Camera.h"
#include "Cycle.h"
#include "NodeData.h"
#include "NodeImpl.h"
#include "Operations.h"
#include "Sequence.h"
#include "Transform.h"
#include "Node.h"


namespace Core
{
namespace Builder
{
	constexpr inline bool isEditableOperatorType(ENodeType type)
	{
		return type == ENodeType::FloatToFloat || type == ENodeType::Vector3ToVector3 ||
				type == ENodeType::Vector4ToVector4 || type == ENodeType::MatrixToMatrix;
	}

	/**
 * Create new node.
 *
 * \tparam T Operation type from OperationType enum.
 * \return Unique pointer to newly created logic operator.
 */
	template <ENodeType T>
	FORCE_INLINE Ptr<NodeBase> createNode()
	{
		// \todo MH
		bool shouldUnlockAllValues = isEditableOperatorType(T);
		auto ret = std::make_shared<NodeImpl<T>>();
		ret->init();
		if (shouldUnlockAllValues) ret->setDataMap(&Transform::g_Free);

		ret->updateValues(0);
		return ret;
	}

	Ptr<Core::Sequence> FORCE_INLINE createSequence()
	{
		auto ret = std::make_shared<Core::Sequence>();
		ret->init();
		ret->createComponents();
		ret->updateValues(0);
		return ret;
	}

	template <typename T, typename... Args>
	Ptr<T> FORCE_INLINE createTransform(Args&&... args)
	{
		static_assert(std::is_base_of_v<Transformation, T>, "T is not derived from Transformation class.");
		auto ret = std::make_shared<T>(std::forward<Args>(args)...);
		ret->init();
		ret->reset();
		return ret;
	}
} // namespace Builder

class GraphManager
{
	/// References to created cycle nodes which need to be regularly updated.
	static std::vector<Ptr<Cycle>> m_cycles;

public:
	template <ENodeType T>
	static Ptr<NodeBase> createNode()
	{
		return Builder::createNode<T>();
	}

	template <typename T, typename... Args>
	static Ptr<T> createTransform(Args&&... args)
	{
		return Builder::createTransform<T>(std::forward<Args>(args)...);
	}

	static CameraPtr createCamera();

	/**
	 * Create Cycle
	 */
	static Ptr<Core::Cycle> createCycle();

	/**
	 * \param tick in seconds.
	 */
	static void update(double tick);

	[[nodiscard]] static std::vector<Ptr<Core::Cycle>>& getCycles() { return m_cycles; }

	/**
	 * Is used to check before connecting to avoid cycles in the node graph.
	 *
	 * The function is used in plug() function.
	 *
	 * Algorithm described in panel Algoritmus 1 in [Folta, page 30]
	 *
	 * \param input Pin of right node.
	 * \param output Pin of left node.
	 */
	static ENodePlugResult isPlugCorrect(Pin const* input, Pin const* output);

	/// Plug first output pin of lhs to the first input pin of rhs.
	[[nodiscard]] static ENodePlugResult plug(const Ptr<Core::NodeBase>& lhs, const Ptr<Core::NodeBase>& rhs);

	/**
	 * Connect given node output pin to this operator input pin.
	 *
	 * Usage:
	 * \code
	 *    // Create nodes.
	 *    auto vec1    = Core::Builder::createNode<OperationType::Vector3>();
	 *    auto vec2    = Core::Builder::createNode<OperationType::Vector3>();
	 *    auto dotNode = Core::Builder::createNode<OperationType::Vector3DotVector3>();
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
	 /* \todo JH what about plug(pin, pin) ? Now I just have to read info from Core for pass it back to Core... */
	[[nodiscard]] static ENodePlugResult plug(const NodePtr& leftNode, const NodePtr& rightNode,
																						unsigned parentOutputPinIndex, unsigned myInputPinIndex);

	[[nodiscard]] static ENodePlugResult plugSequenceValueInput(const NodePtr& seq, const NodePtr& node,
																															unsigned nodeOutputIndex = 0);
	[[nodiscard]] static ENodePlugResult plugSequenceValueOutput(const NodePtr& seq, const NodePtr& node,
																															 unsigned nodeInputIndex = 0);

	/// Unplug all inputs and outputs.
	static void unplugAll(const NodePtr& node);

	/**
	 * Unplug plugged node from given input pin of this node.
	 *
	 * \param index Index of the input pin
	 */
	static void unplugInput(const Ptr<Core::NodeBase>& node, int index);

	/**
	 * Unplug all nodes connected to given output pin of this node.
	 *
	 * \param node \todo Why single node here?
	 * \param index Index of the output pin
	 */
	static void unplugOutput(Ptr<Core::NodeBase>& node, int index);

	/**
	 * \param index input pin index.
	 */
	static Ptr<NodeBase> getParent(const NodePtr& node, size_t index = 0);

	/**
	 * \return All nodes connected to given node inputs.
	 */
	static std::vector<NodePtr> getAllInputNodes(const NodePtr& node);

	/**
	 * \return All nodes plugged into given node output pins.
	 */
	static std::vector<Ptr<NodeBase>> getAllOutputNodes(NodePtr& node);

	/**
	 * \return All nodes plugged into node input pin on given index.
	 */
	static std::vector<Ptr<NodeBase>> getOutputNodes(const NodePtr& node, size_t index);

	static const Operation* getOperation(const Pin* pin);
	static bool							areFromSameNode(const Pin* lhs, const Pin* rhs);
	static bool							arePlugged(const Pin& input, const Pin& output);
};

using gm = GraphManager;

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
		explicit MatrixIterator(Ptr<Sequence>& sequence);
		MatrixIterator(Ptr<Sequence>& sequence, NodePtr node);

		MatrixIterator(const MatrixIterator& mt);

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
		/// Move to the next matrix (to the root).
		void advance();

		/// Move to the previous matrix (from the root).
		void withdraw();
	};

public:
	explicit SequenceTree(Ptr<NodeBase> sequence);

	/**
	 * \return Iterator which points to the sequence.
	 */
	MatrixIterator begin();

	/**
	 * \return Iterator which points to the sequence root.
	 */
	MatrixIterator end();
};


class MatrixTracker
{
	glm::mat4		m_interpolatedMatrix;
	float				m_param			 = 0.0f;
	bool				m_isReversed = false;
	SequencePtr m_beginSequence;

public:
	explicit MatrixTracker(const SequencePtr& beginSequence) : m_interpolatedMatrix(1.0f), m_beginSequence(beginSequence)
	{}

	const glm::mat4& getInterpolatedMatrix() { return m_interpolatedMatrix; }

	float getParam() const { return m_param; }

	void setMode(bool reversed) { m_isReversed = reversed; }

	/**
	 * Set interpolation parameter and calculate interpolated matrix product.
	 *
	 * \param param from -1.0f to 1.0f. Use 0-1 param for tracking from right to left.
	 *
	 * \warning Call the function only on parameter change. Note that it is
	 * necessary to change parameter on sequence unplug or on matrix remove
	 * or add.
	 */
	void setParam(float param);

private:
	void track();
};


inline CameraPtr GraphManager::createCamera()
{
	auto ret = std::make_shared<Core::Camera>();
	ret->init();
	ret->m_proj->m_parent = ret;
	ret->m_view->m_parent = ret;
	ret->updateValues(0);

	return ret;
}

inline Ptr<Core::Cycle> GraphManager::createCycle()
{
	auto ret = std::make_shared<Core::Cycle>();
	ret->setDataMap(&Transform::g_Free);
	ret->init();
	ret->updateValues(-1);
	ret->stopAndReset();

	m_cycles.push_back(ret);

	return ret;
}
} // namespace Core
