/**
 * \file
 * \brief Interface for the transformation and operator graph.
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 10.12.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
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
#include "Operations.h"
#include "Operator.h"
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

	template <typename T, typename... Args>
	static Ptr<Node> createCustomNode(Args&&... args)
	{
		auto ret = std::make_shared<T>(std::forward<Args>(args)...);
		ret->init();
		ret->updateValues(0);

		return ret;
	}

	template <EOperatorType T>
	static Ptr<Node> createNode()
	{
		return Builder::createOperator<T>();
	}

	template <ETransformType T>
	static Ptr<Transform> createTransform()
	{
		return Builder::createTransform<T>();
	}

	static Ptr<Sequence> createSequence();

	static CameraPtr createCamera();

	/**
	 * Create Cycle
	 */
	static Ptr<Core::Cycle> createCycle();

	static Ptr<Model> createModel();

	// Inline static helper methods to unify node type queries.
	// Type of nodes can be determined by inspecting their Operation keyword.
	// This is a string comparision which isn't ideal, alternative is to use dynamic_cast which a lot of the code does.
	// Either way having this check done in a method affords us some flexibility in the future.
	inline static bool isCamera(Node* node);
	inline static bool isModel(Node* node);
	inline static bool isSequence(Node* node);

	/**
	 * \param tick in seconds.
	 */
	static void update(double tick);

	[[nodiscard]] std::vector<Ptr<Core::Cycle>>& getCycles()
	{
		return m_cycles;
	}

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
	[[nodiscard]] static ENodePlugResult plug(const Ptr<Core::Node>& lhs, const Ptr<Core::Node>& rhs);

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
	[[nodiscard]] static ENodePlugResult plug(const Ptr<Node>& fromNode, const Ptr<Node>& toNode, unsigned fromIndex,
	                                          unsigned toIndex);

	[[nodiscard]] static ENodePlugResult plugSequenceValueInput(const Ptr<Node>& seq, const Ptr<Node>& node,
	                                                            unsigned nodeOutputIndex = 0);
	[[nodiscard]] static ENodePlugResult plugSequenceValueOutput(const Ptr<Node>& seq, const Ptr<Node>& node,
	                                                             unsigned nodeInputIndex = 0);

	/// Unplug all inputs and outputs.
	static void unplugAll(const Ptr<Node>& node);

	/**
	 * Unplug plugged node from given input pin of this node.
	 *
	 * \param index Index of the input pin
	 */
	static void unplugInput(const Ptr<Core::Node>& node, int index);

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
	static Ptr<Node> getParent(const Ptr<Node>& node, size_t index = 0);

	// TODO: Docs
	static Ptr<Node> getParentSequenceOrCamera(Ptr<Sequence> sequence, bool& isCamera, bool skipEmptySeq = false,
										bool skipEmptyCamera = false);

	/**
	 * \return All nodes connected to given node inputs.
	 */
	static std::vector<Ptr<Node>> getAllInputNodes(const Ptr<Node>& node);

	/**
	 * \return All nodes plugged into given node output pins.
	 */
	static std::vector<Ptr<Node>> getAllOutputNodes(Ptr<Node>& node);

	/**
	 * \return All nodes plugged into node input pin on given index.
	 */
	static std::vector<Ptr<Node>> getOutputNodes(const Ptr<Node>& node, size_t index);

	static const Operation& getOperation(const Pin* pin);

	static MatrixTracker* getTracker()
	{
		assert(s_self->m_tracker != nullptr);
		return s_self->m_tracker.get();
	}
	static bool isTracking();
	static bool isTrackingFromLeft();
	static void startTracking(Ptr<Sequence> beginSequence, TrackingDirection direction);
	static void stopTracking();

private:
	static GraphManager* s_self;

	UPtr<MatrixTracker> m_tracker = std::make_unique<MatrixTracker>(); ///< Should be non-null

	/// References to created cycle nodes which need to be regularly updated.
	std::vector<Ptr<Cycle>> m_cycles;
};

using gm = GraphManager;

//----------------------------------------------------------------------------//

inline CameraPtr GraphManager::createCamera()
{
	return Builder::createCamera();
}

inline Ptr<Core::Cycle> GraphManager::createCycle()
{
	auto ret = Builder::createCycle();

	s_self->m_cycles.push_back(ret);

	return ret;
}

inline bool GraphManager::isCamera(Node* node)
{
	return node->getOperation().keyWord == g_cameraProperties.keyWord;
}
inline bool GraphManager::isModel(Node* node)
{
	return node->getOperation().keyWord == g_modelProperties.keyWord;
}
inline bool GraphManager::isSequence(Node* node)
{
	return node->getOperation().keyWord == g_sequence.keyWord;
}
} // namespace Core
