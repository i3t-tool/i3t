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
#include "Core/Nodes/Tracking.h"

#include "Core/Nodes/GraphManager.h"
#include "Iterators.h"

#include "Camera.h"
#include "Model.h"
#include "Sequence.h"
#include "Utils/ProjectionUtils.h"

namespace Core
{
MatrixTracker::MatrixTracker(Ptr<Sequence> beginSequence, TrackingDirection direction)
    : MatrixTracker(beginSequence, nullptr, direction)
{}
MatrixTracker::MatrixTracker(Ptr<Sequence> beginSequence, Ptr<Camera> beginCamera, TrackingDirection direction)
    : m_direction(direction), m_beginSequence(beginSequence), m_beginCamera(beginCamera)
{
	assert(beginSequence != nullptr);
	update();
}

MatrixTracker::~MatrixTracker()
{
	reset();
}

void MatrixTracker::update()
{
	if (!isTracking())
		return;
	if (m_chainNeedsUpdate)
		updateChain();
	if (m_progressNeedsUpdate)
		updateProgress();
	if (m_modelsNeedUpdate)
		updateModels();
	if (m_modelTransformsNeedUpdate)
		updateModelTransforms();
}

/// Regenerates tracking data for all nodes.
/// Transform, sequence and relevant child indicies are set. Matrix indices are set later in updateProgress().
/// The chain and isInCamera flags are set too, as well as the node space type.
void MatrixTracker::updateChain()
{
	if (!isTracking())
		return;

	clearTrackingData();

	Ptr<Sequence> beginSequence = m_beginSequence.lock();
	Ptr<Camera> beginCamera;
	if (!m_beginCamera.expired())
	{
		beginCamera = m_beginCamera.lock();
		m_modelSubtreeRoot = glm::identity<glm::mat4>();
	}
	else
	{
		m_modelSubtreeRoot = beginSequence->data(I3T_SEQ_MOD).getMat4();
	}

	// TODO: (DR) Camera tracking isn't supported for LeftToRight tracking
	//  It hasn't been thought through conceptually yet
	bool ignoreCamera = m_direction == TrackingDirection::LeftToRight;

	// Create iterator for traversing sequence branch.
	// TransformChain always traverses "right to left" towards root.
	TransformChain st = TransformChain(beginSequence, beginCamera).ignoreCamera(ignoreCamera).skipEmptySequences(false);

	// Perform the traversal
	auto transforms = st.begin().collectInfo();

	if (transforms.empty())
	{
		// Nothing to track
		reset();
		return;
	}

	// Process the results and save them into TrackingData entries
	ID lastSequenceID = NIL_ID;
	int transformIdx = 0;
	for (auto& transform : transforms)
	{
		ID sequenceID = transform.sequence->getId();

		// Store tracking data for the enclosing camera
		if (transform.camera != nullptr && m_trackedCamera == nullptr)
		{
			// No need for "lastCameraID" as camera will always be the last node in the chain
			TrackedNodeData cameraData(this);
			cameraData.chain = true;
			cameraData.childrenIdxStart = m_trackedSequences.size();
			cameraData.childrenIdxEnd = m_trackedSequences.size();
			m_trackedCamera = std::make_unique<TrackedNode>(transform.camera, std::move(cameraData));
		}

		// Store tracking data for the enclosing sequence
		if (lastSequenceID == NIL_ID || sequenceID != lastSequenceID)
		{
			TrackedNodeData sequenceData(this);
			sequenceData.seqID = sequenceID;
			sequenceData.chain = true;
			sequenceData.seqIndex = m_trackedSequences.size();
			sequenceData.space = transform.type;
			sequenceData.childrenIdxStart = transformIdx;
			sequenceData.childrenIdxEnd = transformIdx;
			sequenceData.isInCamera = m_trackedCamera != nullptr;
			m_trackedSequences.emplace_back(
			    std::make_shared<TrackedTransform>(transform.sequence, std::move(sequenceData)));

			// Assign to existing camera
			if (m_trackedSequences.back()->data.isInCamera)
			{
				m_trackedCamera->data.childrenIdxEnd = m_trackedSequences.size();
				assert(m_trackedCamera->data.childrenIdxStart >= 0);
				assert(m_trackedCamera->data.childrenIdxEnd >= 0);
			}
		}

		// Store tracking data for the transform
		if (transform.currentNode != nullptr)
		{
			if (transform.currentNode->getId() == sequenceID)
			{
				// The transform is the sequence, we already have tracking data for it
				auto& trackedSequence = m_trackedSequences.back();
				trackedSequence->data.tID = sequenceID; // tID == seqID indicates a "sequence transform"
				trackedSequence->data.tIndex = transformIdx++;
				trackedSequence->data.seqIndex = m_trackedSequences.size() - 1;
				trackedSequence->dataIndex = transform.dataIndex;
				m_trackedTransforms.emplace_back(trackedSequence);
			}
			else
			{
				// Store tracking data for the transform
				TrackedNodeData transformData(this);
				transformData.tID = transform.currentNode->getId();
				transformData.tIndex = transformIdx++;
				transformData.seqIndex = m_trackedSequences.size() - 1;
				transformData.space = transform.type;
				transformData.isInCamera = m_trackedCamera != nullptr;
				auto& trackedTransform = m_trackedTransforms.emplace_back(
				    std::make_shared<TrackedTransform>(transform.currentNode, std::move(transformData)));
				trackedTransform->dataIndex = transform.dataIndex;
			}
			m_trackedSequences.back()->data.childrenIdxEnd = transformIdx;
		}

		assert(m_trackedSequences.back()->data.childrenIdxStart >= 0);
		assert(m_trackedSequences.back()->data.childrenIdxEnd >= 0);

		lastSequenceID = transform.sequence->getId();
	}

	if (m_trackedTransforms.empty())
	{
		// Nothing to track
		reset();
		return;
	}

	// apply matrices right to left
	if (m_direction == TrackingDirection::LeftToRight)
	{
		reverseChain();
	}

	LOG_DEBUG("[TRACKING] Updated matrix chain, found {} transforms in {} sequences.", m_trackedTransforms.size(),
	          m_trackedSequences.size());

	m_chainNeedsUpdate = false;

	updateModels();
	updateProgress();
}

void MatrixTracker::updateProgress()
{
	if (!isTracking())
		return;

	// Update the time parameter
	m_param = m_newParam;

	assert(m_trackedTransforms.size() > 0);

	// I think there is no need to round? It prevents very low smooth tracking speeds (stuck at 1/0)
	// if (Math::eq(0.0f, m_param))
	// 	m_param = 0.0f;
	// else if (Math::eq(1.0f, m_param))
	// 	m_param = 1.0f;

	// NOTE: Number of tracked transforms does not necessarily determine to total number of interpolated matrices
	//  This is because the projection transform can be decomposed into multiple matrices
	//  Eg. individual transforms can represent more than a single matrix

	// Update transform data and construct the list of matrices.
	m_matrices.clear();
	for (auto& transform : m_trackedTransforms)
	{
		transform->data.mIndex = m_matrices.size();

		int createdMatrices = 0;

		// Special handling of the projection transform
		if (transform->data.space == TransformSpace::Projection)
		{
			createdMatrices = handleProjectionTransform(transform);
		}

		// Create matrix representing the transform
		if (createdMatrices == 0)
		{
			m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get()));
			createdMatrices++;
		}

		assert(createdMatrices > 0 && "Tracked transform must always contain a matrix");

		// Configure child matrix indices of the transform
		transform->data.childrenIdxStart = transform->data.mIndex;
		transform->data.childrenIdxEnd = transform->data.mIndex + createdMatrices;
	}
	m_matrixCount = m_matrices.size();
	assert(m_matrixCount > 0);

	// Step through the matrices
	const float matStep = 1.0f / (float) m_matrixCount;
	float matrixParam = m_param * (float) m_matrixCount;
	m_fullMatricesCount = (int) matrixParam;
	float interpParam = fmod(m_param, matStep) / matStep;
	int matricesBefore = m_fullMatricesCount;
	if (matricesBefore > 0 && matricesBefore == matrixParam)
	{
		// On a "border" between two matrices
		interpParam = 1.0f;
		matricesBefore -= 1; // The last "full" matrix will be the interpolated one with param of 1.0f
	}

	glm::mat4 accMatrix(1.0f);
	m_iViewMatrix = glm::identity<glm::mat4>();
	m_iProjMatrix = glm::identity<glm::mat4>();

	// Accumulate matrices up to the interpolation point
	for (int i = 0; i < matricesBefore; ++i)
	{
		auto& matrix = m_matrices[i];
		matrix->progress = 1.0f;
		matrix->interpolating = false;
		matrix->active = true;

		accumulateMatrix(accMatrix, *matrix->transform, matrix->getMat());
	}

	// Interpolate matrix at the "tracking cursor"
	{
		int interpIdx = matricesBefore;
		auto& matrix = m_matrices[interpIdx];
		matrix->progress = interpParam;
		matrix->interpolating = true;
		matrix->active = true;

		Ptr<Node> coreNode = matrix->transform->node.lock();
		m_interpolatedTransformID = coreNode->getId();

		if (interpParam == 1.f)
		{
			accumulateMatrix(accMatrix, *matrix->transform, matrix->getMat());
		}
		else if (interpParam > 0.f && interpParam < 1.f)
		{
			glm::mat4 rhs = matrix->getMat();
			glm::mat4 lhs(1.0f);

			// The tracked node can be a transformation which has an isRotation flag, but can it be an operator as well.
			// TODO: Detect rotation from an operator

			auto useQuat = false;
			if (auto coreTransform = std::dynamic_pointer_cast<Transform>(coreNode))
				useQuat = coreTransform->properties()->isRotation;

			glm::mat4 iMatrix = Math::lerp(lhs, rhs, interpParam, useQuat);

			accumulateMatrix(accMatrix, *matrix->transform, iMatrix);
		}
		m_interpolatedMatrix = accMatrix;
		m_interpolatedMatrixObject = matrix.get();
	}

	// Finish iteration to the end to update progress values
	for (int i = matricesBefore + 1; i < m_matrixCount; ++i)
	{
		auto& matrix = m_matrices[i];
		matrix->progress = 0.0f;
		matrix->interpolating = false;
		matrix->active = false;
	}

	// Update transforms with the accumulated individual matrix data
	for (int i = 0; i < m_trackedTransforms.size(); ++i)
	{
		auto& transform = m_trackedTransforms[i];
		transform->data.interpolating = false;
		transform->data.progress = 0.f;
		transform->data.active = false;

		int matrixCount = transform->getMatrixCount();
		assert(matrixCount >= 1);
		for (int j = transform->data.childrenIdxStart; j < transform->data.childrenIdxEnd; ++j)
		{
			auto& matrix = m_matrices[j];
			transform->data.interpolating |= matrix->interpolating;
			transform->data.progress += matrix->progress;
			transform->data.active |= matrix->active;
		}
		transform->data.progress /= matrixCount;

		if (transform->data.interpolating)
			m_interpolatedTransform = transform.get();
	}

	// Update tracked sequences that aren't transforms
	int mIdx = 0;
	for (int i = 0; i < m_trackedSequences.size(); ++i)
	{
		auto& sequence = m_trackedSequences[i];

		sequence->data.mIndex = mIdx;

		if (sequence->data.tIndex >= 0)
		{
			mIdx++; // Sequences acting as transforms were already updated
			continue;
		}


		sequence->data.interpolating = false;
		sequence->data.progress = 0.f;
		sequence->data.active = false;

		int seqTransformCount = sequence->data.childrenIdxEnd - sequence->data.childrenIdxStart;
		assert(seqTransformCount >= 0);
		if (seqTransformCount > 0)
		{
			for (int j = sequence->data.childrenIdxStart; j < sequence->data.childrenIdxEnd; ++j)
			{
				auto& tTransform = m_trackedTransforms[j];
				sequence->data.interpolating |= tTransform->data.interpolating;
				sequence->data.progress += tTransform->data.progress;
				sequence->data.active |= tTransform->data.active;
				mIdx += tTransform->getMatrixCount();
			}
			sequence->data.progress /= seqTransformCount;
		}
		else
		{
			// Empty sequence
			sequence->data.progress = sequence->data.mIndex <= matricesBefore ? 1.f : 0.f;
			sequence->data.active = sequence->data.mIndex <= matricesBefore;
		}
	}

	// Update tracked camera
	if (m_trackedCamera)
	{
		auto& data = m_trackedCamera->data;
		data.interpolating = false;
		data.progress = 0.f;
		data.active = false;

		int cameraSeqCount = data.childrenIdxEnd - data.childrenIdxStart;
		assert(cameraSeqCount >= 1 && "Camera should always have at least one sequence");

		for (int j = data.childrenIdxStart; j < data.childrenIdxEnd; ++j)
		{
			auto& tSeq = m_trackedSequences[j];
			data.interpolating |= tSeq->data.interpolating;
			data.progress += tSeq->data.progress;
			data.active |= tSeq->data.active;
		}
		data.progress /= cameraSeqCount;
	}

	assert(m_fullMatricesCount <= m_matrixCount);

	LOG_DEBUG("[TRACKING] Updated progress t:{} ({}/{} matrices in {} transforms inside {} sequences{}).", m_param,
	          m_fullMatricesCount + 1, m_matrixCount, m_trackedTransforms.size(), m_trackedSequences.size(),
	          m_trackedCamera ? " + camera" : "");

	m_progressNeedsUpdate = false;

	updateModelTransforms();
}

void MatrixTracker::accumulateMatrix(glm::mat4& accMatrix, const TrackedTransform& trackedTransform,
                                     const glm::mat4& transformMatrix)
{
	// View and projection transformations have special handling (and are always right to left)
	if (!m_trackInWorldSpace && trackedTransform.data.space == TransformSpace::View)
	{
		// View transformations have special handling
		m_iViewMatrix = transformMatrix * m_iViewMatrix;
	}
	else if (!m_trackInWorldSpace && trackedTransform.data.space == TransformSpace::Projection)
	{
		m_iProjMatrix = transformMatrix * m_iProjMatrix;
	}
	else
	{
		if (m_direction == TrackingDirection::LeftToRight)
			accMatrix = accMatrix * transformMatrix;
		else if (m_direction == TrackingDirection::RightToLeft)
			accMatrix = transformMatrix * accMatrix;
	}
}

void MatrixTracker::updateModelTransforms()
{
	for (UPtr<TrackedNode>& model : m_models)
	{
		if (model->node.expired())
		{
			LOG_ERROR("[TRACKING] Cannot update model transform of an expired model!");
			assert(false);
			continue;
		}
		assert(model->data.modelData != nullptr);
		auto* modelNode = model->node.lock()->asRaw<Model>();
		model->data.modelData->m_referenceSpace = glm::inverse(m_modelSubtreeRoot) * modelNode->m_modelMatrix;
		model->data.modelData->m_interpolatedMatrix = m_interpolatedMatrix * model->data.modelData->m_referenceSpace;
	}
	m_modelTransformsNeedUpdate = false;
	LOG_DEBUG("[TRACKING] Updated transforms for {} models.", m_models.size());
}

void MatrixTracker::updateModels()
{
	if (!isTracking())
		return;

	clearModels();

	auto* beginSequence = m_beginSequence.lock()->asRaw<Node>();
	if (beginSequence->getTrackingData()->isInCamera)
	{
		// SequenceTree search must start from the camera itself when begin sequence is inside one.
		assert(m_trackedCamera != nullptr);
		beginSequence = m_trackedCamera->node.lock().get();
	}

	SequenceTree subTree(beginSequence);
	auto it = subTree.begin();
	++it; // Skip root
	for (; it != subTree.end(); ++it)
	{
		Node& node = *it;
		if (GraphManager::isModel(&node))
		{
			TrackedNodeData data(this);
			data.modelSubtree = true;
			data.modelData = std::make_unique<TrackedModelData>();
			m_models.emplace_back(std::make_unique<TrackedNode>(node.getPtr(), std::move(data)));
		}
		else
		{
			assert(GraphManager::isSequence(&node));
			TrackedNodeData data(this);
			data.modelSubtree = true;
			m_modelSequences.emplace_back(std::make_unique<TrackedNode>(node.getPtr(), std::move(data)));
		}
	}
	m_modelsNeedUpdate = false;
	LOG_DEBUG("[TRACKING] Updated models. {} models found.", m_models.size());

	updateModelTransforms();
}

void MatrixTracker::reverseChain()
{
	for (auto& transfrom : m_trackedTransforms)
	{
		transfrom->data.tIndex = m_trackedTransforms.size() - 1 - transfrom->data.tIndex;
		transfrom->data.seqIndex = m_trackedSequences.size() - 1 - transfrom->data.seqIndex;
	}
	std::reverse(m_trackedTransforms.begin(), m_trackedTransforms.end());

	for (auto& sequence : m_trackedSequences)
	{
		int start = sequence->data.childrenIdxStart;
		sequence->data.childrenIdxStart = m_trackedTransforms.size() - sequence->data.childrenIdxEnd;
		sequence->data.childrenIdxEnd = m_trackedTransforms.size() - start;
		sequence->data.seqIndex = m_trackedSequences.size() - 1 - sequence->data.seqIndex;
	}
	std::reverse(m_trackedSequences.begin(), m_trackedSequences.end());

	if (m_trackedCamera)
	{
		int start = m_trackedCamera->data.childrenIdxStart;
		m_trackedCamera->data.childrenIdxStart = m_trackedSequences.size() - m_trackedCamera->data.childrenIdxEnd;
		m_trackedCamera->data.childrenIdxEnd = m_trackedSequences.size() - start;
	}
}

void MatrixTracker::clearTrackingData()
{
	LOG_DEBUG("[TRACKING] Clearing tracking data for {} transforms and {} sequences.", m_trackedTransforms.size(),
	          m_trackedSequences.size());
	m_trackedSequences.clear();
	m_trackedTransforms.clear();
	m_trackedCamera.reset();
	m_chainNeedsUpdate = true;
	m_interpolatedTransform = nullptr;
	m_interpolatedTransformID = NIL_ID;
	m_interpolatedMatrix = glm::identity<glm::mat4>();
	m_interpolatedMatrixObject = nullptr;
}

void MatrixTracker::clearModels()
{
	LOG_DEBUG("[TRACKING] Clearing {} tracked models and {} model subtree sequences.", m_models.size(),
	          m_modelSequences.size());
	m_modelSequences.clear();
	m_models.clear();
	m_modelsNeedUpdate = true;
}

void MatrixTracker::reset()
{
	m_beginSequence.reset();
	clearTrackingData();
	clearModels();
}

int MatrixTracker::handleProjectionTransform(const Ptr<TrackedTransform>& transform)
{
	assert(m_direction != TrackingDirection::LeftToRight && "Left to right camera tracking isn't supported!");
	assert(transform->data.space == TransformSpace::Projection);
	assert(m_trackedCamera != nullptr);

	if (!m_decomposeProjection)
	{
		// Apply defaults
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get()));
		m_matrices.back()->useLHS = false;
		m_matrices.back()->ndcType = NDCType::MinusOneToOne;
		m_matrices.back()->moveCameraOutOfNDC = true;
		m_matrices.back()->cameraNDCOffset = 3.f;
		return 1;
	}

	// If this is the ONLY projection transform along the chain (that isn't the sequence itself),
	// we will decompose it into multiple separate transforms to better visualize the transformation.

	auto& parentSequence = m_trackedSequences[transform->data.seqIndex];
	if (!transform->data.isSequenceTransform() &&
	    parentSequence->data.childrenIdxEnd - parentSequence->data.childrenIdxStart > 1)
		return 0; // More than one perspective transform

	// Right now, we simply assume it is a
	// standard OpenGL perspective or ortho projection.

	// TODO: Different types of decomposition depending on the matrix type, add some kind of a hinting
	//  option that indicates what type of matrix it is (opengl? directx? vulkan?, ortho/persp, can inspect the node)

	glm::mat4 transformMat = transform->getMat();

	bool isPerspective = transformMat[2][3] != 0.f;

	if (isPerspective && m_decomposePerspectiveBrown)
	{
		auto [ortho1, persp1, persp2, ortho2, neg] = ProjectionUtils::decomposePerspectiveBrown(transformMat);
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), ortho1));
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), persp1));
		m_matrices.back()->moveCameraOutOfNDC = true;
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), persp2));
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), ortho2));
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), neg));
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.back()->ndcType = NDCType::MinusOneToOne;
		m_matrices.back()->useLHS = true;
		return 5;
	}
	if (isPerspective && m_decomposePerspectiveShirley)
	{
		auto [persp, ortho, neg] = ProjectionUtils::decomposePerspectiveShirley(transformMat);
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), persp));
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), ortho));
		m_matrices.back()->moveCameraOutOfNDC = true;
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), neg));
		m_matrices.back()->ndcType = NDCType::MinusOneToOne;
		m_matrices.back()->useLHS = true;
		m_matrices.back()->cameraNDCOffset = 3.f;
		return 3;
	}
	{
		auto [neg, proj] = ProjectionUtils::constructZFlippedProjection(transformMat);
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), proj));
		m_matrices.back()->moveCameraOutOfNDC = true;
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.emplace_back(std::make_unique<TrackedMatrix>(transform.get(), neg));
		m_matrices.back()->cameraNDCOffset = 3.f;
		m_matrices.back()->useLHS = true;
		m_matrices.back()->ndcType = NDCType::MinusOneToOne;
		return 2;
	}
}

Ptr<Sequence> MatrixTracker::getSequence() const
{
	if (m_beginSequence.expired())
	{
		return nullptr;
	}
	return m_beginSequence.lock();
}
Ptr<Camera> MatrixTracker::getCamera() const
{
	if (m_beginCamera.expired())
	{
		return nullptr;
	}
	return m_beginCamera.lock();
}
ID MatrixTracker::getSequenceID() const
{
	if (auto seq = getSequence())
		return seq->getId();
	return NIL_ID;
}
ID MatrixTracker::getCameraID() const
{
	if (auto cam = getCamera())
		return cam->getId();
	return NIL_ID;
}

const std::vector<UPtr<MatrixTracker::TrackedNode>>& MatrixTracker::getTrackedModels() const
{
	return m_models;
}

void MatrixTracker::setProgress(float param)
{
	m_newParam = glm::clamp(param, 0.0f, 1.0f);
	m_progressNeedsUpdate = true;
}

bool MatrixTracker::isTracking() const
{
	return !m_beginSequence.expired();
}
TrackingDirection MatrixTracker::getDirection() const
{
	return m_direction;
}
bool MatrixTracker::isTrackingFromLeft() const
{
	return isTracking() && getDirection() == TrackingDirection::LeftToRight;
}

void MatrixTracker::reverseDirection()
{
	m_direction =
	    m_direction == TrackingDirection::RightToLeft ? TrackingDirection::LeftToRight : TrackingDirection::RightToLeft;
	m_chainNeedsUpdate = true;
}

int MatrixTracker::getTransformCount() const
{
	return m_trackedTransforms.size();
}
int MatrixTracker::getMatrixCount() const
{
	return m_matrixCount;
}
const std::vector<Ptr<MatrixTracker::TrackedTransform>>& MatrixTracker::getTrackedSequences() const
{
	return m_trackedSequences;
}
const std::vector<Ptr<MatrixTracker::TrackedTransform>>& MatrixTracker::getTrackedTransforms() const
{
	return m_trackedTransforms;
}
const std::vector<UPtr<MatrixTracker::TrackedMatrix>>& MatrixTracker::getMatrices() const
{
	return m_matrices;
}
const UPtr<MatrixTracker::TrackedNode>& MatrixTracker::getTrackedCamera() const
{
	return m_trackedCamera;
}

void MatrixTracker::onNodeDestroy(Node* node)
{
	if (node->m_trackingData == nullptr)
		return;

	// If the begin sequence is destroyed, the tracking is ended.
	MatrixTracker* tracker = node->m_trackingData->tracker;
	if (tracker->m_beginSequence.expired() || node->getId() == tracker->m_beginSequence.lock()->getId())
	{
		tracker->reset();
		return;
	}

	if (node->m_trackingData->modelSubtree)
		tracker->m_modelsNeedUpdate = true; // Node off chain, in the model subtree, update model reference spaces
	else
		tracker->m_chainNeedsUpdate = true; // Node on chain, rebuild
}
void MatrixTracker::onNodeUpdate(Node* node)
{
	if (node->m_trackingData == nullptr)
		return;
	if (node->m_trackingData->modelSubtree)
	{
		// The node is in the subtree of begin sequence, not on the chain, we only update the model reference space
		node->m_trackingData->tracker->m_modelTransformsNeedUpdate = true;
	}
	else
	{
		// The node is on the tracked chain, need to reaccumulate matrices with the updated values
		node->m_trackingData->tracker->m_progressNeedsUpdate = true;
	}
}

void MatrixTracker::onNodeGraphChange(Node* node)
{
	if (node->m_trackingData == nullptr)
	{
		// Core node plug/unplug callbacks aren't called for both nodes so we always update chain for unknown nodes.
		if (GraphManager::isTracking())
			GraphManager::getTracker()->m_chainNeedsUpdate = true;
		return;
	}

	if (node->m_trackingData->modelSubtree || node->getId() == node->m_trackingData->tracker->getSequenceID())
	{
		// The node is in the subtree of begin sequence (or it is the begin sequence), search for new models
		node->m_trackingData->tracker->m_modelsNeedUpdate = true;
	}
	node->m_trackingData->tracker->m_chainNeedsUpdate |= !node->m_trackingData->modelSubtree;
}
void MatrixTracker::requestChainUpdate()
{
	m_chainNeedsUpdate = true;
}
void MatrixTracker::requestProgressUpdate()
{
	m_progressNeedsUpdate = true;
}

std::string MatrixTracker::getDebugString()
{
	return fmt::format("Tracking {} matrices in {} transforms inside {} sequences with {} models and {} sequences in "
	                   "the model subtree...",
	                   m_matrixCount, getTransformCount(), m_trackedSequences.size(), m_models.size(),
	                   m_modelSequences.size()) +

	       fmt::format("\nProgress: {:.2f}, {}/{}, Direction: {}", m_param, m_fullMatricesCount, m_matrixCount,
	                   m_direction == TrackingDirection::RightToLeft ? "RightToLeft" : "LeftToRight");
}

bool TrackedNodeData::isSequenceTransform() const
{
	if (seqID == NIL_ID || tID == NIL_ID)
		return false;
	return seqID == tID;
}

int TrackedNodeData::getChildCount() const
{
	return childrenIdxEnd - childrenIdxStart;
}

} // namespace Core
