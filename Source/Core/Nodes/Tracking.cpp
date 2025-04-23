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
#include "Sequence.h"

namespace Core
{
TransformChain::TransformChain(Ptr<Sequence> sequence, bool skipEmptySequences, bool ignoreCamera)
    : m_skipEmptySequences(skipEmptySequences), m_ignoreCamera(ignoreCamera)
{
	if (sequence == nullptr)
	{
		m_beginSequence = nullptr;
		return;
	}
	m_beginSequence = sequence;
}

TransformChain::TransformChainIterator TransformChain::begin()
{
	auto it = TransformChainIterator(m_beginSequence);
	it.m_skipEmptySequences = m_skipEmptySequences;
	it.m_ignoreCamera = m_ignoreCamera;
	it.m_tree = this;
	return it;
}

TransformChain::TransformChainIterator TransformChain::end()
{
	auto it = TransformChainIterator();
	it.m_skipEmptySequences = m_skipEmptySequences;
	it.m_ignoreCamera = m_ignoreCamera;
	it.m_tree = this;
	return it;
}

TransformChain::TransformChainIterator::TransformChainIterator(Ptr<Sequence> sequence)
{
	m_info.sequence = sequence;

	if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
	{
		// there is matrix output plugged into this sequence)
		m_info.isExternal = true;
		m_info.currentNode = sequence;
		m_info.dataIndex = I3T_SEQ_MAT;
	}
	else
	{
		// normal case
		m_info.currentNode = sequence->getMatrices().empty() ? nullptr : sequence->getMatrices().back();
		m_info.dataIndex = 0;
	}
}

TransformChain::TransformChainIterator::TransformChainIterator(Ptr<Sequence> sequence, Ptr<Node> node)
{
	m_info.sequence = sequence;
	m_info.currentNode = node;
}

Sequence* TransformChain::TransformChainIterator::getSequence() const
{
	return m_info.sequence.get();
}

std::vector<Ptr<Node>> TransformChain::TransformChainIterator::collect()
{
	std::vector<Ptr<Node>> result;
	auto it = *this;
	while (it != m_tree->end())
	{
		result.push_back(*it);
		++it;
	}
	return result;
}

std::pair<std::vector<Ptr<Node>>, std::vector<TransformInfo>> TransformChain::TransformChainIterator::collectWithInfo()
{
	std::vector<Ptr<Node>> matrices;
	std::vector<TransformInfo> info;
	auto it = *this;
	while (it != m_tree->end())
	{
		matrices.push_back(*it);
		info.push_back(it.m_info);
		++it;
	}
	return {matrices, info};
}

std::vector<TransformInfo> TransformChain::TransformChainIterator::collectInfo()
{
	std::vector<TransformInfo> info;
	auto it = *this;
	while (it != m_tree->end())
	{
		info.push_back(it.m_info);
		++it;
	}
	return info;
}

TransformChain::TransformChainIterator& TransformChain::TransformChainIterator::operator++()
{
	advance();
	return *this;
}

TransformChain::TransformChainIterator TransformChain::TransformChainIterator::operator++(int)
{
	advance();
	return *this;
}

TransformChain::TransformChainIterator TransformChain::TransformChainIterator::operator+(int a)
{
	for (int i = 0; i < a; ++i)
	{
		advance();
	}
	return *this;
}

// TransformChain::TransformChainIterator& TransformChain::TransformChainIterator::operator--()
// {
// 	withdraw();
// 	return *this;
// }
//
// TransformChain::TransformChainIterator TransformChain::TransformChainIterator::operator--(int)
// {
// 	withdraw();
// 	return *this;
// }

Ptr<Node> TransformChain::TransformChainIterator::operator*() const
{
	I3T_ASSERT(m_info.currentNode != nullptr, "Iterator is at the end!");
	return m_info.currentNode;
}

bool TransformChain::TransformChainIterator::operator==(const TransformChain::TransformChainIterator& rhs) const
{
	return m_info.currentNode == rhs.m_info.currentNode && m_info.sequence == rhs.m_info.sequence;
}

bool TransformChain::TransformChainIterator::operator!=(const TransformChain::TransformChainIterator& rhs) const
{
	return !(*this == rhs);
}

void TransformChain::TransformChainIterator::advance()
{
	// Check inner sequence state
	const auto& matrices = m_info.sequence->getMatrices();

	if (!m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
	{
		const auto it = std::find(matrices.begin(), matrices.end(), m_info.currentNode);
		int index = std::distance(matrices.begin(), it);
		assert(index >= 0);
		if (index != 0)
		{
			// Move left in the current sequence
			m_info.currentNode = matrices[--index];
			return;
		}
	}

	// No inner datra, search for a parent
	bool isCamera;
	Ptr<Node> parent = GraphManager::getParentSequenceOrCamera(m_info.sequence, isCamera, m_skipEmptySequences, true);
	if (!parent || (isCamera && m_ignoreCamera))
	{
		invalidate();
		return;
	}

	m_info.isExternal = false;
	m_info.dataIndex = 0;


	m_info.sequence = parent->as<Sequence>();

	// Check if the parent sequence is connected externally via matrix pin
	if (const auto matrixPluggedIntoParent = GraphManager::getParent(m_info.sequence, I3T_SEQ_IN_MAT))
	{
		m_info.isExternal = true;
		m_info.currentNode = m_info.sequence; // Set the sequence has the transform data source
		m_info.dataIndex = I3T_SEQ_MAT;
	}
	else
	{
		auto& parentMatrices = m_info.sequence->getMatrices();
		if (parentMatrices.empty())
			m_info.currentNode = nullptr;
		else
			m_info.currentNode = parentMatrices.back();
	}
}

// TODO: Move to graph manager if used again
// /// \pre parentSequence is direct or indirect parent of startSequence.
// /// \return Nonempty child sequence of parentSequence or nullptr if there is no such sequence.
// Ptr<Sequence> getNonemptyChildSequence(Ptr<Sequence> startSequence, Ptr<Sequence> parentSequence)
// {
// 	Ptr<Sequence> sequence = startSequence;
// 	Ptr<Sequence> prevSequence = nullptr;
//
// 	while (sequence != parentSequence)
// 	{
// 		if (!sequence->getMatrices().empty() || sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
// 		{
// 			prevSequence = sequence;
// 		}
// 		sequence = GraphManager::getParent(sequence->getPtr(), I3T_SEQ_IN_MUL)->as<Sequence>();
// 	}
//
// 	return prevSequence;
// }

// TODO: I commented out backwards iteration as it is not needed yet, might be rewritten later.
// void TransformChain::TransformChainIterator::withdraw()
// {
// 	bool hasPrevMatrix = true;
// 	auto prevNonEmptySequence = getNonemptyChildSequence(m_tree->m_beginSequence->getPtr()->as<Sequence>(),
// 	                                                     m_info.sequence->getPtr()->as<Sequence>());
//
// 	// Find index of current matrix in current sequence.
// 	const auto& matrices = m_info.sequence->getMatrices();
// 	const auto it = std::find(matrices.begin(), matrices.end(), m_info.currentNode);
// 	auto index = std::distance(matrices.begin(), it);
// 	if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
// 	{
// 		// The sequence has matrix input plugged in. We are at the beginning of the sequence.
// 		index = matrices.size() - 1;
// 	}
//
// 	if (index == matrices.size() - 1 && prevNonEmptySequence == nullptr)
// 	{
// 		hasPrevMatrix = false;
// 	}
//
// 	if (!hasPrevMatrix)
// 	{
// 		return;
// 	}
//
// 	m_info.isExternal = false;
// 	if (index == matrices.size() - 1)
// 	{
// 		const auto matrixPluggedIntoChild = GraphManager::getParent(prevNonEmptySequence, I3T_SEQ_IN_MAT);
//
// 		if (matrixPluggedIntoChild)
// 		{
// 			m_info.isExternal = true;
// 			m_info.currentNode = matrixPluggedIntoChild;
// 		}
// 		else
// 		{
// 			m_info.currentNode = prevNonEmptySequence->getMatrices().front();
// 		}
// 		m_info.sequence = prevNonEmptySequence;
// 	}
// 	else if (m_info.currentNode == nullptr)
// 	{
// 		// iterator is at the end
// 		m_info.currentNode = matrices.front();
// 	}
// 	else
// 	{
// 		m_info.currentNode = matrices[++index];
// 	}
// }

void TransformChain::TransformChainIterator::invalidate()
{
	m_info.currentNode = nullptr;
	m_info.sequence = nullptr;
}

//------------------------------------------------------------------------------------------------//

MatrixTracker::MatrixTracker(Ptr<Sequence> beginSequence, TrackingDirection direction)
    : m_direction(direction), m_beginSequence(beginSequence)
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
	m_modelSubtreeRoot = beginSequence->data(I3T_SEQ_MOD).getMat4();

	// Create iterator for traversing sequence branch.
	// TransformChain always traverses "right to left" towards root.
	TransformChain st(beginSequence, false, false);

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
		if (lastSequenceID == NIL_ID || sequenceID != lastSequenceID)
		{
			// Store tracking data for the enclosing sequence
			TrackedNodeData sequenceData(this);
			sequenceData.chain = true;
			sequenceData.seqIndex = m_trackedSequences.size();
			m_trackedSequences
			    .emplace_back(std::make_shared<TrackedSequence>(transform.sequence, std::move(sequenceData)))
			    ->transformIdxStart = transformIdx;
		}

		if (transform.currentNode != nullptr)
		{
			m_trackedSequences.back()->transformIdxEnd = transformIdx + 1;

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
				auto& trackedTransform = m_trackedTransforms.emplace_back(
				    std::make_shared<TrackedTransform>(transform.currentNode, std::move(transformData)));
				trackedTransform->dataIndex = transform.dataIndex;
			}
			transformIdx++;
		}
		else
		{
			// Empty sequence with no transforms
			m_trackedSequences.back()->transformIdxEnd = transformIdx;
		}
		assert(m_trackedSequences.back()->transformIdxStart >= 0);
		assert(m_trackedSequences.back()->transformIdxEnd >= 0);

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
	const float matFactor = 1.0f / (float) m_trackedTransforms.size();
	const int matricesBefore = (int) (m_param * (float) matricesCount);
	m_fullMatricesCount = matricesBefore;

	float interpParam = fmod(m_param, matFactor) / matFactor;

	glm::mat4 matrix(1.0f);

	// Accumulate matrices up to the interpolation point
	for (int i = 0; i < matricesBefore; ++i)
	{
		auto& transform = m_trackedTransforms[i];
		transform->data.progress = 1.0f;
		transform->data.interpolating = false;
		transform->data.active = true;

		if (m_direction == TrackingDirection::LeftToRight)
		{
			matrix = matrix * transform->getMat();
		}
		else if (m_direction == TrackingDirection::RightToLeft)
		{
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

			if (m_direction == TrackingDirection::LeftToRight)
			{
				matrix = matrix * Math::lerp(lhs, rhs, interpParam, useQuat);
			}
			else if (m_direction == TrackingDirection::RightToLeft)
			{
				matrix = Math::lerp(lhs, rhs, interpParam, useQuat) * matrix;
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

		int seqTransformCount = sequence->transformIdxEnd - sequence->transformIdxStart;
		assert(seqTransformCount >= 0);
		if (seqTransformCount > 0)
		{
			for (int j = sequence->transformIdxStart; j < sequence->transformIdxEnd; ++j)
			{
				auto& tTransform = m_trackedTransforms[j];
				sequence->data.interpolating |= tTransform->data.interpolating;
				sequence->data.progress += tTransform->data.progress;
				sequence->data.active |= tTransform->data.active;
			}
			sequence->data.progress /= sequence->transformIdxEnd - sequence->transformIdxStart;
		}
		else
		{
			sequence->data.progress = sequence->transformIdxStart >= matricesBefore ? 0.f : 1.f;
			sequence->data.active = sequence->transformIdxStart <= matricesBefore;
		}
	}

	assert(m_fullMatricesCount <= m_trackedTransforms.size());

	LOG_DEBUG("[TRACKING] Updated progress t:{} ({}/{} transforms in {} sequences).", m_param, m_fullMatricesCount + 1,
	          m_trackedTransforms.size(), m_trackedSequences.size());

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

	SequenceTree subTree(m_beginSequence.lock().get());
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
	LOG_DEBUG("[TRACKING] Updated models. Found {} models.", m_models.size());

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
		int start = sequence->transformIdxStart;
		sequence->transformIdxStart = m_trackedTransforms.size() - sequence->transformIdxEnd;
		sequence->transformIdxEnd = m_trackedTransforms.size() - start;
		sequence->data.seqIndex = m_trackedSequences.size() - 1 - sequence->data.seqIndex;
	}
	std::reverse(m_trackedSequences.begin(), m_trackedSequences.end());
}

void MatrixTracker::clearTrackingData()
{
	LOG_DEBUG("[TRACKING] Clearing tracking data for {} transforms and {} sequences.", m_trackedTransforms.size(),
	          m_trackedSequences.size());
	m_trackedSequences.clear();
	m_trackedTransforms.clear();
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
