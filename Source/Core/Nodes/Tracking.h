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

#include "Iterators.h"

namespace Core
{
class Transform;
class Model;
class Sequence;
class Camera;

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
 * The struct contains common AND specific data for tracked Transforms, Sequences, Cameras and Models.
 */
struct TrackedNodeData
{
	friend class MatrixTracker;

	MatrixTracker* tracker;

	bool interpolating = false; ///< Whether the "tracking cursor" is within this node.
	float progress{0.f};        ///< Progress within this node

	TransformSpace space{TransformSpace::Model}; ///< What stage along the PVM matrix chain this transform represents.

	bool chain = false;        ///< Whether this node forms the top level of the tracked chain
	bool active = false;       ///< Whether this node is part of the final tracked interpolated matrix.
	bool modelSubtree = false; ///< Whether this node is part of the begin sequence subtree. Meaning it connects
	                           ///< to a tracked model but itself is NOT being tracked.
	bool isInCamera = false;

	UPtr<TrackedModelData> modelData = nullptr; ///< Extra model data held for models.

	int seqIndex = -1; ///< Sequence index within the chain, -1 in model subtree. Valid for sequences and transforms.
	int tIndex = -1;   ///< Transform index within the chain, >= 0 indicates transform, a sequence CAN be a transform.
	int mIndex = -1;   ///< Matrix index within the chain, a transform node can represent multiple matrices

	ID seqID{NIL_ID}; ///< Sequence node id
	ID tID{NIL_ID};   ///< Transform node id

	/**
	 * Index of the first contained sequence, transform or matrix.
	 * Cameras contain sequences.
	 * Sequences contain transforms, however, a sequence itself can BE a transform, in which case it holds indices
	 * of matrices, not transforms. The isSequenceTransform() can be used to check for such sequences, a sequence
	 * acts as a transform when it has external data plugged into it.
	 */
	int childrenIdxStart = -1;
	int childrenIdxEnd = -1; ///< Index of the last contained sequence, transform or matrix @see childrenIdxStart

	explicit TrackedNodeData(MatrixTracker* tracker) : tracker(tracker)
	{
		assert(tracker != nullptr);
	}

	/// Whether this node is both a sequence and a transform (eg. we're using the sequence data as transform)
	bool isSequenceTransform() const;

	/// Returns the number of children, what children are and if there are any depends on the particular node type.
	/// Cameras contain sequences, sequences contain transforms, transforms contain tracked matrix objects.
	int getChildCount() const;
};

enum class TrackingDirection
{
	LeftToRight = -1,
	RightToLeft = 1
};

/**
 * The main tracking object, responsible for tracking a single chain of sequences.
 *
 * This object assignes and manages TrackedNodeData objects accessible via Core::Node::getTrackingData().
 * Tracking data is assigned to all transformations, sequences, models and possibly cameras involved in the tracking
 * operation. Tracking data of models contains an additional TrackedModelData object containg the interpolated matrix
 * data. A null pointer indicates that no tracking operation is active.
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
	friend struct TrackedNodeData;

public:
	static float g_trackingViewportScalingFactorXY;
	static float g_trackingViewportScalingFactorZ;

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

	/**
	 * Internal tracked node wrapper representing a "transform", a node containing matrix data.
	 * Usually contains a single matrix but can contain multiple.
	 * @note Matrix or matrices of the transform are held by TrackedMatrix object(s) corresponding to the
	 * TrackedNodeData child indices.
	 */
	struct TrackedTransform : TrackedNode
	{
		TrackedTransform(const Ptr<Node>& node, TrackedNodeData&& data) : TrackedNode(node, std::move(data)) {}
		TrackedTransform(const TrackedTransform& other) = delete;
		TrackedTransform(TrackedTransform&& other) = delete;

		/// How many piecewise matrices this transform contains
		/// A transfrom is usually a single matrix, but in special cases we might want to decompose this matrix
		/// into multiple partial matrices and track them separately.
		int getMatrixCount() const
		{
			return data.getChildCount();
		}

		[[nodiscard]] const glm::mat4& getMat() const
		{
			return node.lock()->data(dataIndex).getMat4();
		}

		int dataIndex = 0; ///< Index of the matrix data storage holding the transform
	};

	/**
	 * Internal helper object representing a single matrix. Usually represents the single matrix of a transform, in
	 * which case it simply retrieves the matrix of that transform's node. However it can carry its own data and
	 * represent one of multiple matrices in a single transform.
	 */
	struct TrackedMatrix
	{
		TrackedTransform* transform;
		glm::mat4 matrix{1.f}; ///< Matrix data, valid when useOwnData is true.

		// Temporary tracking information later forwarded to a parent transform.
		bool interpolating{false};
		float progress{0.f};
		bool active{false};

		// Projection metadata
		TransformSpace space;
		CameraCoordSystem coordinateSystem; ///< Relevant when TransformSpace is >= Projection
		// /// Indicates that space after applying this matrix should use the left handed coordinate system
		// bool useLHS = false;
		// NDCType ndcType = NDCType::OneToMinusOne;
		bool moveCameraOutOfNDC = false;
		float cameraNDCOffset = 0.f;

		/// Tracked matrix representing matrix data of a tracked transform.
		TrackedMatrix(TrackedTransform* transform) : transform(transform), space(transform->data.space) {}

		/// Tracked matrix holding an independent matrix
		TrackedMatrix(TrackedTransform* transform, const glm::mat4& matrix)
		    : transform(transform), matrix(matrix), useOwnData(true), space(transform->data.space)
		{}

		[[nodiscard]] glm::mat4 getMat() const
		{
			if (!useOwnData)
				return modify(transform->getMat());
			return modify(matrix);
		}

	private:
		bool useOwnData{false};

		/// Applies scene view specific modification of the given matrix
		glm::mat4 modify(const glm::mat4& matrix) const
		{
			if (space == TransformSpace::Screen)
			{
				glm::mat4 m = matrix;
				// Flip X as we are in emulated inverted Z
				m[3][0] = -1 * m[3][0];
				// Divide the whole viewport transform by a configurable scale to keep NDC and screen space roughly
				// similar in size
				float scalingFactorXY = 1.f / g_trackingViewportScalingFactorXY;
				float scalingFactorZ = 1.f / g_trackingViewportScalingFactorZ;
				m[0][0] *= scalingFactorXY;
				m[1][1] *= scalingFactorXY;
				m[3][0] *= scalingFactorXY;
				m[3][1] *= scalingFactorXY;
				m[2][2] *= scalingFactorZ;
				m[3][2] *= scalingFactorZ;
				return m;
			}
			return matrix;
		}
	};

private:
	float m_param = 0.0f; ///< Tracking time parameter (0..1),

	/// Interpolated matrix corresponding to the current time parameter.
	glm::mat4 m_interpolatedMatrix{1.0f};
	TrackedTransform* m_interpolatedTransform{nullptr}; ///< Reference to the internal interpolated transform object
	TrackedMatrix* m_interpolatedMatrixObject{nullptr}; ///< Reference to the internal interpolated matrix object
	ID m_interpolatedTransformID = NIL_ID;              ///< ID of the currently interpolated transformation / operator

	int m_matrixCount = 0; /// Total number of matrices
	/// Number of fully accumulated matrices, eg. matrices prior to the interpolated one.
	std::size_t m_fullMatricesCount = 0;

	/// Direction of tracking, the direction in which the multiplication of matrices is accumulated.
	/// Or in other other words how parentheses are put into the multiplication chain.
	/// With a matrix chain of A*B*C*D*E*F we can interpret this as (((((A*B)*C)*D)*E)*F) or (A*(B*(C*(D*(E*F)))))
	/// The result is the same but the intermediate results of the multiplication tell two different stories.
	TrackingDirection m_direction{TrackingDirection::RightToLeft};

	/// The sequence that started this tracking operation.
	/// On sequence destruction, the tracker is destroyed too.
	WPtr<Sequence> m_beginSequence;

	/// If begin sequence is inside a camera a refernce to it must be provided in order for the tracker to function
	/// properly. In other words, the search for the parent camera is deferred to the user.
	WPtr<Camera> m_beginCamera;

	/// List of all individual tracked transformation nodes, contains one or more TrackedMatrix objects.
	/// A transform can be a transformation node or a sequence. They usually contain a single matrix, but a transform
	/// can be decomposed by the tracker into multiple matrices, hence transforms reference potentially multiple
	/// TrackedMatrix objects, which are the objects the tracker actually works with during interpolation.
	std::vector<Ptr<TrackedTransform>> m_trackedTransforms;

	/// List of all tracked matrices, objects actually containing the matrix data. Owned by a single transform.
	std::vector<UPtr<TrackedMatrix>> m_matrices;

	/// List of all sequences involved in the tracking operation with supplemental tracking data for each.
	/// Sequences always contain one or more transforms. A sequence can be its own transform when connected externally.
	std::vector<Ptr<TrackedTransform>> m_trackedSequences;

	/// The tracked camera if applicable, only one since a camera always ends the chain (has no matrix mul input)
	UPtr<TrackedNode> m_trackedCamera;

	/// Tracked models, managed and updated by the tracker itself based on the sequence chain.
	std::vector<UPtr<TrackedNode>> m_models;

	/// Sequences forming the subtree of the begin sequence. These sequences are NOT tracked but do carry tracking
	/// data indicating they are a part of the operation.
	std::vector<UPtr<TrackedNode>> m_modelSequences;

	/// Model transform from scene graph root -> begin sequence (its world transform)
	glm::mat4 m_modelSubtreeRoot{1.f};

public:
	// When false, view and projection (and optionally viewport) transformations are excluded from the tracked matrix
	// accumulation/interpolation and instead are saved into the m_iViewMatrix and m_iProjMatrix (and m_iViewportMatrix)
	// fields. These fields can be then applied globally to the whole world to visualize their local spaces. When
	// enabled, view and projection (and viewport) matrices are accumulated and interpolated like any other world space
	// matrix.
	bool m_trackInWorldSpace{false};

	// For starters, the NDC z-coordinate range is inverted in OpenGL, so at the very least we want to flip the third
	// row of the matrix to prevent "flipping" of the space along the z axis during interpolation.
	bool m_decomposeProjection{true};         // TODO: Docs, Maybe rename to "Raw tracking" in GUI
	bool m_decomposePerspectiveShirley{true}; // TODO: Docs
	bool m_decomposePerspectiveBrown{false};  // TODO: Docs

	/// Interpolated view matrix, relevant during camera tracking when m_trackInWorldSpace is false.
	glm::mat4 m_iViewMatrix{1.0f};
	/// Interpolated projection matrix, relevant during camera tracking when m_trackInWorldSpace is false.
	glm::mat4 m_iProjMatrix{1.0f};
	/// Interpolated viewport matrix, relevant during camera tracking when m_trackInWorldSpace is false.
	glm::mat4 m_iViewportMatrix{1.0f};

private:
	float m_newParam = 0.0f; ///< Newly set tracking time parameter last set by setProgress().

	bool m_chainNeedsUpdate = true; ///< Whether the tracked matrix chain needs update (due to graph structure changes).
	bool m_modelsNeedUpdate = true; ///< Whether the tracked model list needs to be updated (due to a structural change)
	bool m_progressNeedsUpdate = true;       ///< Whether tracked matrices need an update, called on matrix value change
	bool m_modelTransformsNeedUpdate = true; ///< Whether the tracked model transforms need to be updated

public:
	/// Constructs an empty / inactive tracker.
	MatrixTracker() = default;
	MatrixTracker(Ptr<Sequence> beginSequence, TrackingDirection direction);
	MatrixTracker(Ptr<Sequence> beginSequence, Ptr<Camera> beginCamera, TrackingDirection direction);
	~MatrixTracker();

	/// Set the tracker's progress parameter, a value from 0 to 1.
	/// The change will only take effect after the next MatrixTracker::update() call.
	/// Meaning subsequent getProgress() calls will return the old value until update.
	void setProgress(float param);

	/// Returns the current processed time parameter of the tracker. The value might change after a call to update().
	float getProgress() const
	{
		return m_param;
	}

	bool isTracking() const;

	TrackingDirection getDirection() const;
	bool isTrackingFromLeft() const;

	void reverseDirection();

	/// Updates the state of the tracker, reacts to any changes induced by other method calls.
	/// This method is meant to be run frequently and does lazy evaluation based on internal flags.
	void update();

	Ptr<Sequence> getSequence() const; ///< Returns the begin sequence
	Ptr<Camera> getCamera() const;     ///< Returns the begin camera (if provided)
	ID getSequenceID() const;          ///< Returns the begin sequence ID
	ID getCameraID() const;            ///< Returns the begin camera ID (if provided)

	/// Result of the tracking operation, returns the complete accumulated and interpolated matrix.
	const glm::mat4& getInterpolatedMatrix() const
	{
		return m_interpolatedMatrix;
	}
	/// Reference to the internal interpolated matrix object
	const TrackedMatrix* getInterpolatedMatrixObject() const
	{
		return m_interpolatedMatrixObject;
	}
	/// Core ID of the currently interpolated transform
	ID getInterpolatedTransformID() const
	{
		return m_interpolatedTransformID;
	}
	/// Reference to the internal interpolated transform object
	const TrackedTransform* getInterpolatedTransform() const
	{
		return m_interpolatedTransform;
	}
	unsigned getFullMatricesCount() const
	{
		return m_fullMatricesCount;
	}
	int getTransformCount() const; ///< Returns the number of tracked transform nodes.
	/// Returns the number of tracked matrices / transformations. Greater or equal to transform count.
	int getMatrixCount() const;

	/// Returns an internal list of tracked sequence objects. Use with care and DO NOT MODIFY.
	const std::vector<Ptr<TrackedTransform>>& getTrackedSequences() const;
	/// Returns an internal list of tracked transform objects. Use with care and DO NOT MODIFY.
	const std::vector<Ptr<TrackedTransform>>& getTrackedTransforms() const;
	/// Returns an internal list of tracked model objects. Use with care and DO NOT MODIFY.
	const std::vector<UPtr<TrackedNode>>& getTrackedModels() const;
	/// Returns an internal list of matrix objects. Use with care and DO NOT MODIFY.
	const std::vector<UPtr<TrackedMatrix>>& getMatrices() const;
	const UPtr<TrackedNode>& getTrackedCamera() const;

	/// Alerts the tracker that a node has been destroyed. Called in Core::Node::finalize().
	/// @note MatrixTracker::update() must be called for changes to take effect.
	static void onNodeDestroy(Node* node);

	/// Alerts the tracker that a tracked node has been updated. Called in Core::Node::onUpdate().
	/// @note MatrixTracker::update() must be called for changes to take effect.
	static void onNodeUpdate(Node* node);

	/// Alerts the tracker of a structure change in the tracked nodes. Called in Core::Node::onPlug().
	/// @note MatrixTracker::update() must be called for changes to take effect.
	static void onNodeGraphChange(Node* node);

	void requestChainUpdate();
	void requestProgressUpdate();

	std::string getDebugString();

protected:
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

private:
	/// Handle smart decomposition of a projection transform
	/// This method must initialize and add at least one TrackedMatrix object to the m_matrices list.
	/// @return Number of added matrices
	int handleProjectionTransform(const Ptr<TrackedTransform>& transform);

	int handleViewportTransform(const Ptr<TrackedTransform>& transform);

	void accumulateMatrix(glm::mat4& accMatrix, const TrackedTransform& trackedTransform,
	                      const glm::mat4& transformMatrix);
};
} // namespace Core
