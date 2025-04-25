/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "glm/matrix.hpp"

#include "Camera.h"
#include "Model.h"
#include "Sequence.h"

namespace Core
{
class Transform;

enum class TransformType
{
	Model,     // World space
	View,      // View space
	Projection // Clip/NDC space
	           // TODO: [T-VIEWPORT] Screen // Screen space
};

/// Data container for TransformChainIterator traversal metadata.
struct TransformInfo
{
	Ptr<Node> currentNode = nullptr;  ///< Node with matrix output, a transformation or the sequence itself.
	Ptr<Sequence> sequence = nullptr; ///< Sequence which contains the transform.
	Ptr<Camera> camera = nullptr;     ///< Optionally a camera which contains the sequence.

	bool isExternal = false; ///< Whether the sequence is supplied with external data, when true sequence == currentNode
	TransformType type = TransformType::Model;
	int dataIndex = 0; ///< Data index of currentNode holding the transform

	friend bool operator==(const TransformInfo& lhs, const TransformInfo& rhs)
	{
		return lhs.sequence == rhs.sequence && lhs.currentNode == rhs.currentNode && lhs.camera == rhs.camera &&
		       lhs.isExternal == rhs.isExternal && lhs.type == rhs.type && lhs.dataIndex == rhs.dataIndex;
	}
	friend bool operator!=(const TransformInfo& lhs, const TransformInfo& rhs)
	{
		return !(lhs == rhs);
	}
	std::string toString() const
	{
		return std::string() +                                                          //
		       "currentNode: " + (currentNode ? currentNode->getSignature() : "null") + //
		       " sequence: " + (sequence ? sequence->getSignature() : "null") +          //
		       " camera: " + (camera ? camera->getSignature() : "null") +                //
		       " isExternal: " + std::to_string(isExternal) +                            //
		       " type: " + std::to_string(static_cast<int>(type)) +                                  //
		       " dataIndex: " + std::to_string(dataIndex);
	}
};

/**
 * Iterable object representing a single chain of connected sequence nodes and their transform contents.
 * Iterates over individual transformations, contained within a sequence. The iterator returns the sequence itself when
 * it is plugged in externally using a matrix input.
 * @note Previously, the iterator returned the node connected to the sequence matrix input, rather than the sequence.
 */
class TransformChain
{
	Ptr<Sequence> m_beginSequence; ///< The sequence the chain begins from.
	Ptr<Camera> m_beginCamera;     ///< The camera the begin sequence is in, if any

	bool m_skipEmptySequences = true; ///< Excludes empty sequences from the chain.
	bool m_ignoreCamera = true;       ///< Excludes camera sequences from the chain.
	bool m_skipEmptyCamera = true;    ///< Excludes camera if its empty
public:
	/**
	 * Constructs a transform chain beginning at the specified sequence, ending at the scene graph root.
	 * The scene graph root can be the last connected sequence or a sequence within a connected camera.
	 * The starting sequence can itself be within a camera, but an enclosing camera must be specified.
	 * @param sequence The begin/starting sequence. The chain will begin at the last (right-most) transformation.
	 * @param camera The enclosing camera if the begin sequence is contained in one.
	 * @note Further regarding skipping empty sequences and/or camera can be enabled using dedicated methods after
	 * construction.
	 */
	explicit TransformChain(const Ptr<Sequence>& sequence, const Ptr<Camera>& camera = nullptr);

	/**
	 * Iterator for traversing the sequence / transform chain.
	 * Advances from a leaf to root (from "right" to "left"), optionally including empty sequences and camera sequences.
	 */
	class TransformChainIterator
	{
		using Iterator = TransformChainIterator;
		friend class TransformChain;
		TransformChain* m_tree{nullptr};

		/**
		 * Current state, identifies a matrix node (transformation / operator) and an enclosing
		 * sequence. When the enclosing sequence is null, the iterator is invalid. Non-null sequence but null current
		 * node indicates an empty sequence.
		 */
		TransformInfo m_info;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Ptr<Node>;
		using pointer = Ptr<Node>;
		using reference = Ptr<Node>&;

		bool m_skipEmptySequences = true;
		bool m_ignoreCamera = true;
		bool m_skipEmptyCamera = true;

		TransformChainIterator() = default; ///< Empty "past the end" iterator
		TransformChainIterator(TransformChain* chain, const Ptr<Sequence>& sequence, const Ptr<Camera>& camera,
		                       const Ptr<Node>& node, bool mSkipEmptySequences, bool mIgnoreCamera,
		                       bool mSkipEmptyCamera);

		/// Advance the iterator. Move to the next matrix (to the root).
		/// Eg. advancing "right to left"
		void next();

		bool equals(const Iterator& b) const;

		/// Check whether the iterator is pointing at a valid element (eg. not the end of a container)
		bool valid() const;

		/// \returns Non-owned pointer to the current sequence.
		///     Never null.
		Sequence* getSequence() const;

		/// Return the current transform metadata object carrying detailed information about the current transform.
		const TransformInfo& transformInfo() const
		{
			return m_info;
		}

		/// \returns Non-owned pointer to the all matrices from start to the root,
		///     note that Ptr<Node> may points to operator with matrix output,
		///     not only to transformation.
		std::vector<Ptr<Node>> collect();
		std::pair<std::vector<Ptr<Node>>, std::vector<TransformInfo>> collectWithInfo();
		std::vector<TransformInfo> collectInfo();

		/// Get current matrix, can be a transformation or the sequence itself.
		/// @note You have to extract data from the node by yourself.
		Ptr<Node> operator*() const;

		// clang-format off
		Ptr<Node> operator->() const { return this->operator*(); }
		Iterator& operator++() { next(); return *this; }
		Iterator operator++(int) { TransformChainIterator tmp = *this; ++(*this); return tmp; }
		friend bool operator==(const Iterator& a, const Iterator& b) { return a.equals(b); }
		friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }
		// clang-format on

	private:
		// /// Move to the previous matrix (from the root).
		// ///
		// /// \todo Not implemented correctly for sequences with matrix input plugged.
		// void withdraw();

		/// Sets m_currentMatrix to nullptr.
		void invalidate();

		bool advanceWithinSequence();
	};

	/**
	 * \return Iterator which points to starting sequence and its last matrix.
	 */
	TransformChainIterator begin();

	/**
	 * \return Points to the root sequence and matrix is nullptr,
	 *     so it is not possible to decrement or dereference it (as any other STL iterator).
	 */
	TransformChainIterator end();

	TransformChain& skipEmptySequences(bool val = true)
	{
		m_skipEmptySequences = val;
		return *this;
	}
	TransformChain& ignoreCamera(bool val = true)
	{
		m_ignoreCamera = val;
		return *this;
	}
	TransformChain& skipEmptyCamera(bool val = true)
	{
		m_skipEmptyCamera = val;
		return *this;
	}
};

//----------------------------------------------------------------------------//

class MatrixTracker;

/// Data object holding extra information about a currently tracked model.
struct TrackedModelData
{
	glm::mat4 m_interpolatedMatrix{1.f}; ///< Model matrix of the tracked model (result of tracker interpolation).
	glm::mat4 m_referenceSpace{1.f};     ///< Reference space of the model, its "default" transformation.
	                                     ///< This is applicable when the begin sequence of tracking is not
	                                     ///< directly connected to a model.
};

/**
 * Tracking data for individual nodes involved in a tracking operation.
 * Instances are held and managed by a MatrixTracker, and referenced by Core nodes when they are tracked.
 */
struct TrackedNodeData
{
	friend class MatrixTracker;

	MatrixTracker* tracker;

	bool interpolating = false; ///< Whether the "tracking cursor" is within this node.
	float progress{0.f};        ///< Progress within this node

	bool chain = false;        ///< Whether this node forms the top level of the tracked chain
	bool active = false;       ///< Whether this node is part of the final tracked interpolated matrix.
	bool modelSubtree = false; ///< Whether this node is part of the begin sequence subtree. Meaning it connects
	                           ///< to a tracked model but itself is NOT being tracked.

	UPtr<TrackedModelData> modelData = nullptr; ///< Extra model data held for models.

	int seqIndex = -1; ///< Sequence index within the chain, -1 in model subtree. >= 0 indicates this is a sequence
	int index = -1;    ///< Transform index within the chain, >= 0 indicates transform, a sequence CAN be a transform

	explicit TrackedNodeData(MatrixTracker* tracker) : tracker(tracker)
	{
		assert(tracker != nullptr);
	}
};

enum class TrackingDirection
{
	LeftToRight = -1,
	RightToLeft = 1
};

// TODO: (DR) Implement camera tracking
/**
 * The main tracking object, responsible for tracking a single chain of sequences.
 *
 * This object assignes and manages TrackedNodeData objects accessible via Core::Node::getTrackingData().
 * Tracking data is assigned to all transformations, sequences and possibly operators involved in a tracking operation.
 * Models don't get tracking data assigned but they instead have a special TrackedModel reference. In both cases a null
 * pointer indicates that no tracking operation is active.
 *
 * The MatrixTracker does not rebuild the sequence chain every frame, but reacts to value updates in any nodes that
 * have tracking data assigned (so transformations, sequences and possibly their external operators).
 * This behavior is implemented directly in Core::Node.
 *
 * Tracking operation consists of the "chain" and "model subtree".
 * The chain is a set of sequences as well as their transforms (transformations or operators) from the root of the scene
 * graph to the begin sequence. The begin sequence marks the end of this chain.
 * The subtree rooted in the begin sequence, referred to as "model subtree", is NOT tracked, as that would require
 * the tracking "cursor" to split into multiple cursors. Instead this subtree is traversed to find all models,
 * which are then tracked as if they were connected directly to the begin sequence.
 *
 * The nodes in the model subtree are considered part of the tracking operation and do carry tracking data with a flag
 * marking them as model subtree nodes.
 *
 * All involved nodes notify the tracker of any value or structure changes, which causes the entire chain and/or subtree
 * to be rebuilt.
 */
class MatrixTracker final
{
	/**
	 * Internal wrapper of core nodes for managing tracking data for each node involved in the tracking operation.
	 * Sets and unsets the node's tracking data pointer using RAII. Is NOT copyable.
	 */
	struct TrackedNode
	{
		std::weak_ptr<Node> node;
		TrackedNodeData data;

		TrackedNode(const Ptr<Node>& node, TrackedNodeData&& data) : data(std::move(data))
		{
			assert(node != nullptr);
			node->m_trackingData = &this->data;
			this->node = node;
		}
		virtual ~TrackedNode()
		{
			if (auto ptr = node.lock())
			{
				if (ptr->m_trackingData == &data)
					ptr->m_trackingData = nullptr;
			}
		}
		TrackedNode(const TrackedNode& other) = delete;
		TrackedNode(TrackedNode&& other) = delete;
		TrackedNode& operator=(TrackedNode other) = delete;
	};

	struct TrackedTransform : TrackedNode
	{
		TrackedTransform(const Ptr<Node>& node, TrackedNodeData&& data) : TrackedNode(node, std::move(data)) {}
		TrackedTransform(const TrackedTransform& other) = delete;
		TrackedTransform(TrackedTransform&& other) = delete;

		int dataIndex = 0; ///< Index of the matrix data storage holding the transform

		[[nodiscard]] glm::mat4 getMat() const
		{
			return node.lock()->data(dataIndex).getMat4();
		}
	};

	struct TrackedSequence : TrackedTransform
	{
		TrackedSequence(const Ptr<Node>& node, TrackedNodeData&& data) : TrackedTransform(node, std::move(data)) {}
		TrackedSequence(const TrackedSequence& other) = delete;
		TrackedSequence(TrackedSequence&& other) = delete;

		int transformIdxStart = -1; ///< Index of the first contained transform
		int transformIdxEnd = -1;   ///< Index of the last contained transform
	};

	float m_param = 0.0f; ///< Tracking time parameter (0..1),

	/// Interpolated matrix corresponding to the current time parameter.
	glm::mat4 m_interpolatedMatrix{1.0f};

	/// Number of fully accumulated matrices, eg. matrices prior to the interpolated one.
	std::size_t m_fullMatricesCount = 0;
	ID m_interpolatedTransformID = NIL_ID; ///< ID of the currently interpolated transformation / operator

	/// Direction of tracking, the direction in which the multiplication of matrices is accumulated.
	/// Or in other other words how parentheses are put into the multiplication chain.
	/// With a matrix chain of A*B*C*D*E*F we can interpret this as (((((A*B)*C)*D)*E)*F) or (A*(B*(C*(D*(E*F)))))
	/// The result is the same but the intermediate results of the multiplication tell two different stories.
	TrackingDirection m_direction{TrackingDirection::RightToLeft};

	/// The sequence that started this tracking operation.
	/// On sequence destruction, the tracker is destroyed too.
	WPtr<Sequence> m_beginSequence;

	/// List of all sequences involved in the tracking operation with supplemental tracking data for each.
	std::vector<Ptr<TrackedSequence>> m_trackedSequences;

	/// List of all individual tracked transformation nodes, containing the matrix data.
	std::vector<Ptr<TrackedTransform>> m_trackedTransforms;

	/// Tracked models, managed and updated by the tracker itself based on the sequence chain.
	std::vector<UPtr<TrackedNode>> m_models;

	/// Sequences forming the subtree of the begin sequence. These sequences are NOT tracked but do carry tracking
	/// data indicating they are a part of the operation.
	std::vector<UPtr<TrackedNode>> m_modelSequences;

	/// Model transform from scene graph root -> begin sequence (its world transform)
	glm::mat4 m_modelSubtreeRoot{1.f};

	bool m_chainNeedsUpdate = true; ///< Whether the tracked matrix chain needs update
	bool m_modelsNeedUpdate = true; ///< Whether the tracked model list needs to be updated (due to a structural change)
	bool m_modelTransformsNeedUpdate = true; ///< Whether the tracked model transforms need to be updated

public:
	/// Constructs an empty / inactive tracker.
	MatrixTracker() = default;
	MatrixTracker(Ptr<Sequence> beginSequence, TrackingDirection direction);
	~MatrixTracker();

	bool setParam(float param);
	float getParam() const
	{
		return m_param;
	}

	bool isTracking() const;

	TrackingDirection getDirection() const;
	bool isTrackingFromLeft() const;

	// TODO: Implement
	void reverseDirection();

	void update();

	Ptr<Sequence> getSequence() const;
	ID getSequenceID() const;

	const glm::mat4& getInterpolatedMatrix() const
	{
		return m_interpolatedMatrix;
	}
	ID getInterpolatedTransformID() const
	{
		return m_interpolatedTransformID;
	}
	unsigned getFullMatricesCount() const
	{
		return m_fullMatricesCount;
	}
	int getTransformCount() const; ///< Returns the number of tracked matrices / transformations.

	std::vector<Ptr<Model>> getModels() const; ///< Returns a list of tracked model nodes.

	/// Alerts the tracker that a node has been destroyed. Called in Core::Node::finalize().
	static void onNodeDestroy(Node* node);

	/// Alerts the tracker that a tracked node has been updated. Called in Core::Node::onUpdate().
	static void onNodeUpdate(Node* node);

	/// Alerts the tracker of a structure change in the tracked nodes. Called in Core::Node::onPlug().
	static void onNodeGraphChange(Node* node);

	std::string getDebugString();

private:
	/// Traverses the graph from the begin sequence, updating the sequence/matrix chain.
	/// @note Should be called when the matrices along the chain change, be it due to change of structure or any value.
	void updateChain();

	/// Updates the interpolated matrix and progress values.
	/// Traverses the current chain, accumulating matrices and updating tracking data.
	/// @note Should be called on chain update and when the tracking time parameter changes.
	void updateProgress();

	/// Updates tracked model transforms based on the current tracking state.
	/// Reference space of models is derived from model and begin sequence data (no traversal is needed)
	/// @note Should be called if any matrix along the chain and model subtree change.
	void updateModelTransforms();

	/// Updates the tracked model list by traversing the subtree rooted in the begin sequence to find connected models.
	/// @note Should be called if the structure of the begin sequence (model) subtree changes.
	void updateModels();

	/// Reverses the order of tracked transforms and sequences.
	void reverseChain();

	void clearModels();
	void clearTrackingData();

	/// Reset the matrix tracker to empty / inactive state
	void reset();
};
} // namespace Core
