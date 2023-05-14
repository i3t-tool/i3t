/**
 * \file Core/GraphManager.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 10.12.2020
 *
 * Interface for the transformation and operator graph.
 */
#pragma once

#include <algorithm>
#include <vector>

#include "Core/Defs.h"

#include "Camera.h"
#include "Cycle.h"
#include "Model.h"
#include "Node.h"
#include "NodeData.h"
#include "NodeImpl.h"
#include "Operations.h"
#include "Pin.h"
#include "Sequence.h"
#include "Tracking.h"
#include "TransformImpl.h"

namespace Core
{
using ID = std::int64_t;

/**
 * You must manage GraphManager lifecycle.
 * \code
 * GraphManager::init();
 *
 * // interaction with GraphManager
 *
 * GraphManager::destroy();
 * \endcode
 */
class GraphManager
{
public:
	static void init();
	static void destroy();

	template <ENodeType T> static Ptr<NodeBase> createNode() { return Builder::createOperator<T>(); }

	template <ETransformType T> static Ptr<Transformation> createTransform() { return Builder::createTransform<T>(); }

	static Ptr<Sequence> createSequence();

	static CameraPtr createCamera();

	/**
	 * Create Cycle
	 */
	static Ptr<Core::Cycle> createCycle();

	static Ptr<Model> createModel();

	/**
	 * \param tick in seconds.
	 */
	static void update(double tick);

	[[nodiscard]] std::vector<Ptr<Core::Cycle>>& getCycles() { return m_cycles; }

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
	static ENodePlugResult isPlugCorrect(const Pin& input, const Pin& output);

	/// Plug first output pin of lhs to the first input pin of rhs.
	[[nodiscard]] static ENodePlugResult plug(const Ptr<Core::NodeBase>& lhs, const Ptr<Core::NodeBase>& rhs);

	/**
	 * Connect given node output pin to this operator input pin.
	 *
	 * Usage:
	 * \code
	 *    // Create nodes.
	 *    auto vec1    = Core::Builder::createOperator<OperationType::Vector3>();
	 *    auto vec2    = Core::Builder::createOperator<OperationType::Vector3>();
	 *    auto dotNode =
	 * Core::Builder::createOperator<OperationType::Vector3DotVector3>();
	 *
	 *    // Plug vector nodes output to dot node inputs.
	 *    GraphManager::plug(vec1, dotNode, 0, 0);
	 *    GraphManager::plug(vec2, dotNode, 1, 0);
	 * \endcode
	 *
	 * \param parentNode Reference to a unique pointer to a parent node to which
	 * \a parentOutputPinIndex this node should be connected to. \param rightNode
	 * node which should be connected to right node output. \param
	 * parentOutputPinIndex Index of the output pin of the parent node. \param
	 * myInputPinIndex Index of input pin of this node.
	 *
	 * \return Result enum is returned from the function. \see ENodePlugResult.
	 */
	[[nodiscard]] static ENodePlugResult plug(const NodePtr& fromNode, const NodePtr& toNode, unsigned fromIndex,
	                                          unsigned toIndex);

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
	 * Returns parent node of given node (the topmost one).
	 *
	 * For example for a node plugged into a sequence node, the real parent
	 * would be the multiplier node, but the sequence node would be returned.
	 *
	 * \param index input pin index.
	 *
	 * \todo Move me to NodeUtils.
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

	static bool isTrackingEnabled();
	static void stopTracking();

private:
	static GraphManager* s_self;

	MatrixTracker m_tracker;

	/// References to created cycle nodes which need to be regularly updated.
	std::vector<Ptr<Cycle>> m_cycles;
};

using gm = GraphManager;

//----------------------------------------------------------------------------//

inline CameraPtr GraphManager::createCamera() { return Builder::createCamera(); }

inline Ptr<Core::Cycle> GraphManager::createCycle()
{
	auto ret = Builder::createCycle();

	s_self->m_cycles.push_back(ret);

	return ret;
}
} // namespace Core
