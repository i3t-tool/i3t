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
	if (m_modelsNeedUpdate)
		updateModels();
	if (m_modelTransformsNeedUpdate)
		updateModelTransforms();
}

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

	// Create iterator for traversing sequence branch.
	// TransformChain always traverses "right to left" towards root.
	TransformChain st = TransformChain(beginSequence, beginCamera).ignoreCamera(false).skipEmptySequences(false);

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
			m_trackedSequences.back()->data.childrenIdxEnd = transformIdx + 1;

			if (transform.currentNode->getId() == sequenceID)
			{
				// The transform is the sequence, we already have tracking data for it
				m_trackedSequences.back()->data.index = transformIdx;
				m_trackedSequences.back()->dataIndex = transform.dataIndex;
				m_trackedTransforms.emplace_back(m_trackedSequences.back());
			}
			else
			{
				// Store tracking data for the transform
				TrackedNodeData transformData(this);
				transformData.index = transformIdx;
				transformData.space = transform.type;
				auto& trackedTransform = m_trackedTransforms.emplace_back(
				    std::make_shared<TrackedTransform>(transform.currentNode, std::move(transformData)));
				trackedTransform->dataIndex = transform.dataIndex;
			}
			transformIdx++;
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

	assert(m_trackedTransforms.size() > 0);

	if (Math::eq(0.0f, m_param))
		m_param = 0.0f;
	else if (Math::eq(1.0f, m_param))
		m_param = 1.0f;

	const int matricesCount = m_trackedTransforms.size();
	const float matStep = 1.0f / (float) m_trackedTransforms.size();
	const int matricesBefore = (int) (m_param * (float) matricesCount);
	m_fullMatricesCount = matricesBefore;

	float interpParam = fmod(m_param, matStep) / matStep;

	glm::mat4 matrix(1.0f);
	m_iViewMatrix = glm::identity<glm::mat4>();
	m_iProjMatrix = glm::identity<glm::mat4>();

	// Accumulate matrices up to the interpolation point
	for (int i = 0; i < matricesBefore; ++i)
	{
		auto& transform = m_trackedTransforms[i];
		transform->data.progress = 1.0f;
		transform->data.interpolating = false;
		transform->data.active = true;

		// TODO: Add special handling of left to right tracking

		// View and projection transformations have special handling (and are always right to left)
		if (!m_trackInWorldSpace && transform->data.space == TransformSpace::View)
		{
			m_iViewMatrix = transform->getMat() * m_iViewMatrix;
		}
		else if (!m_trackInWorldSpace && transform->data.space == TransformSpace::Projection)
		{
			m_iProjMatrix = transform->getMat() * m_iProjMatrix;
		}
		else
		{
			if (m_direction == TrackingDirection::LeftToRight)
				matrix = matrix * transform->getMat();
			else if (m_direction == TrackingDirection::RightToLeft)
				matrix = transform->getMat() * matrix;
		}
	}

	// Interpolate matrix at the "tracking cursor"
	{
		auto& trackedTransform = m_trackedTransforms[std::min(matricesBefore, matricesCount - 1)];
		trackedTransform->data.progress = matricesBefore == matricesCount ? 1.0f : interpParam;
		trackedTransform->data.interpolating = true;
		trackedTransform->data.active = true;
		Ptr<Node> coreNode = trackedTransform->node.lock();
		m_interpolatedTransformID = coreNode->getId();

		if (m_param > 0.f && m_param < 1.f)
		{
			glm::mat4 rhs = trackedTransform->getMat();
			glm::mat4 lhs(1.0f);

			// The tracked node can be a transformation which has an isRotation flag, but can it be an operator as well.
			// TODO: Detect rotation from an operator

			auto useQuat = false;
			if (auto coreTransform = std::dynamic_pointer_cast<Transform>(coreNode))
			{
				useQuat = coreTransform->properties()->isRotation;
			}

			glm::mat4 iMatrix = Math::lerp(lhs, rhs, interpParam, useQuat);

			if (!m_trackInWorldSpace && trackedTransform->data.space == TransformSpace::View)
			{
				// View transformations have special handling
				m_iViewMatrix = iMatrix * m_iViewMatrix;
			}
			else if (!m_trackInWorldSpace && trackedTransform->data.space == TransformSpace::Projection)
			{
				m_iProjMatrix = iMatrix * m_iProjMatrix;
			}
			else
			{
				if (m_direction == TrackingDirection::LeftToRight)
					matrix = matrix * iMatrix;
				else if (m_direction == TrackingDirection::RightToLeft)
					matrix = iMatrix * matrix;
			}
		}
		m_interpolatedMatrix = matrix;
	}

	// Finish iteration to the end to update progress values
	for (int i = matricesBefore + 1; i < matricesCount; ++i)
	{
		auto& transform = m_trackedTransforms[i];
		transform->data.progress = 0.0f;
		transform->data.interpolating = false;
		transform->data.active = false;
	}

	// Update tracked sequences that aren't transforms
	for (int i = 0; i < m_trackedSequences.size(); ++i)
	{
		auto& sequence = m_trackedSequences[i];

		if (sequence->data.index >= 0) // Sequences acting as transforms were already updated
			continue;

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
			}
			sequence->data.progress /= seqTransformCount;
		}
		else
		{
			sequence->data.progress = sequence->data.childrenIdxStart >= matricesBefore ? 0.f : 1.f;
			sequence->data.active = sequence->data.childrenIdxStart <= matricesBefore;
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

	assert(m_fullMatricesCount <= m_trackedTransforms.size());

	LOG_DEBUG("[TRACKING] Updated progress t:{} ({}/{} transforms in {} sequences{}).", m_param,
	          m_fullMatricesCount + 1, m_trackedTransforms.size(), m_trackedSequences.size(),
	          m_trackedCamera ? " + camera" : "");

	updateModelTransforms();
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
		transfrom->data.index = m_trackedTransforms.size() - 1 - transfrom->data.index;
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

Ptr<Sequence> MatrixTracker::getSequence() const
{
	if (m_beginSequence.expired())
	{
		return nullptr;
	}
	return m_beginSequence.lock();
}

ID MatrixTracker::getSequenceID() const
{
	if (m_beginSequence.expired())
	{
		return NIL_ID;
	}
	return m_beginSequence.lock()->getId();
}

std::vector<Ptr<Model>> MatrixTracker::getModels() const
{
	std::vector<Ptr<Model>> models;
	for (auto& model : m_models)
	{
		if (model->node.expired())
		{
			assert(false);
			continue;
		}
		auto modelPtr = model->node.lock()->as<Model>();
		models.push_back(modelPtr);
	}
	return models;
}

// std::vector<Ptr<Model>> MatrixTracker::getModels() const
// {
// 	std::vector<Ptr<Model>> result;
// 	for (const auto& proxy : m_models)
// 	{
// 		result.push_back(proxy->getModel());
// 	}
// 	return result;
// }

bool MatrixTracker::setParam(float param)
{
	m_param = glm::clamp(param, 0.0f, 1.0f);
	if (!isTracking())
		return false;
	updateProgress();
	return true;
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
int MatrixTracker::getTransformCount() const
{
	return m_trackedTransforms.size();
}
const std::vector<Ptr<MatrixTracker::TrackedTransform>>& MatrixTracker::getTrackedSequences() const
{
	return m_trackedSequences;
}
const std::vector<Ptr<MatrixTracker::TrackedTransform>>& MatrixTracker::getTrackedTransforms() const
{
	return m_trackedTransforms;
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
		node->m_trackingData->tracker->m_chainNeedsUpdate = true;
	}
}

void MatrixTracker::onNodeGraphChange(Node* node)
{
	if (node->m_trackingData == nullptr)
		return;
	if (node->m_trackingData->modelSubtree || node->getId() == node->m_trackingData->tracker->getSequenceID())
	{
		// The node is in the subtree of begin sequence, search for new models
		node->m_trackingData->tracker->m_modelsNeedUpdate = true;
	}
	else
	{
		node->m_trackingData->tracker->m_chainNeedsUpdate = true;
	}
}

std::string MatrixTracker::getDebugString()
{
	return fmt::format("Tracking {} transforms in {} sequences with {} models and {} sequences in the model subtree...",
	                   m_trackedTransforms.size(), m_trackedSequences.size(), m_models.size(),
	                   m_modelSequences.size()) +
	       fmt::format("\nProgress: {:.2f}, {}/{}, Direction: {}", m_param, m_fullMatricesCount,
	                   m_trackedTransforms.size(),
	                   m_direction == TrackingDirection::RightToLeft ? "RightToLeft" : "LeftToRight");
}

} // namespace Core
