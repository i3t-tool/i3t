/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Core/Nodes/Tracking.h"

#include "Core/Nodes/GraphManager.h"

namespace Core
{
SequenceTree::SequenceTree(Ptr<Node> sequence)
{
	if (sequence == nullptr)
	{
		m_beginSequence = nullptr;
		return;
	}

	m_beginSequence = sequence->as<Sequence>().get();
}

SequenceTree::MatrixIterator SequenceTree::begin()
{
	auto it = MatrixIterator(m_beginSequence);
	it.m_tree = this;

	return it;
}

SequenceTree::MatrixIterator SequenceTree::end()
{
	auto cur = m_beginSequence;
	Ptr<Sequence> parent;
	while ((parent = getNonemptyParentSequence(cur->getPtr()->as<Sequence>())))
	{
		cur = parent.get();
	}

	auto it = MatrixIterator(cur, nullptr);
	it.m_tree = this;

	return it;
}

SequenceTree::MatrixIterator::MatrixIterator(Sequence* sequence)
{
	m_info.sequence = sequence;

	auto inputOperator = GraphManager::getParent(m_info.sequence->getPtr(), I3T_SEQ_IN_MAT);
	if (inputOperator != nullptr)
	{
		// there is matrix output plugged into this sequence)
		m_info.isExternal = true;
		m_currentMatrix = inputOperator;
	}
	else
	{
		// normal case
		m_currentMatrix = sequence->getMatrices().empty() ? nullptr : sequence->getMatrices().back();
	}
}

SequenceTree::MatrixIterator::MatrixIterator(Sequence* sequence, Ptr<Node> node)
{
	m_info.sequence = sequence;
	m_currentMatrix = node;
}

Sequence* SequenceTree::MatrixIterator::getSequence() const
{
	return m_info.sequence;
}

std::vector<Ptr<Node>> SequenceTree::MatrixIterator::collect()
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

std::pair<std::vector<Ptr<Node>>, std::vector<TransformInfo>> SequenceTree::MatrixIterator::collectWithInfo()
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

SequenceTree::MatrixIterator& SequenceTree::MatrixIterator::operator++()
{
	advance();

	return *this;
}

SequenceTree::MatrixIterator SequenceTree::MatrixIterator::operator++(int)
{
	advance();

	return *this;
}

SequenceTree::MatrixIterator SequenceTree::MatrixIterator::operator+(int a)
{
	for (int i = 0; i < a; ++i)
	{
		advance();
	}

	return *this;
}

SequenceTree::MatrixIterator& SequenceTree::MatrixIterator::operator--()
{
	withdraw();

	return *this;
}

SequenceTree::MatrixIterator SequenceTree::MatrixIterator::operator--(int)
{
	withdraw();

	return *this;
}

Ptr<Node> SequenceTree::MatrixIterator::operator*() const
{
	I3T_ASSERT(m_currentMatrix != nullptr, "Iterator is at the end!");

	return m_currentMatrix;
}

bool SequenceTree::MatrixIterator::operator==(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix == rhs.m_currentMatrix && m_info.sequence == rhs.m_info.sequence;
}

bool SequenceTree::MatrixIterator::operator!=(const SequenceTree::MatrixIterator& rhs) const
{
	return !(*this == rhs);
}

void SequenceTree::MatrixIterator::advance()
{
	bool hasNext = true;
	const auto parentNonEmptySequence = getNonemptyParentSequence(m_info.sequence->getPtr()->as<Sequence>());

	if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn() && !parentNonEmptySequence)
	{
		hasNext = false;
	}

	const auto& matrices = m_info.sequence->getMatrices();
	const auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);
	if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
	{
		// The sequence has matrix input plugged in. We are at the end of the sequence.
		index = 0;
	}

	if (index == 0 && !parentNonEmptySequence)
	{
		hasNext = false;
	}

	if (!hasNext)
	{
		invalidate();
		return;
	}

	m_info.isExternal = false;
	if (index == 0)
	{
		const auto matrixPluggedIntoParent = GraphManager::getParent(parentNonEmptySequence, I3T_SEQ_IN_MAT);

		// We are at the beginning of the sequence. Go to the next sequence.
		if (matrixPluggedIntoParent)
		{
			m_info.isExternal = true;
			m_currentMatrix = matrixPluggedIntoParent;
		}
		else
		{
			m_currentMatrix = parentNonEmptySequence->getMatrices().back();
		}
		m_info.sequence = parentNonEmptySequence.get();
	}
	else
	{
		m_currentMatrix = matrices[--index];
	}
}

/// \pre parentSequence is direct or indirect parent of startSequence.
/// \return Nonempty child sequence of parentSequence or nullptr if there is no such sequence.
Ptr<Sequence> getNonemptyChildSequence(Ptr<Sequence> startSequence, Ptr<Sequence> parentSequence)
{
	Ptr<Sequence> sequence = startSequence;
	Ptr<Sequence> prevSequence = nullptr;

	while (sequence != parentSequence)
	{
		if (!sequence->getMatrices().empty() || sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
		{
			prevSequence = sequence;
		}
		sequence = GraphManager::getParent(sequence->getPtr(), I3T_SEQ_IN_MUL)->as<Sequence>();
	}

	return prevSequence;
}

void SequenceTree::MatrixIterator::withdraw()
{
	bool hasPrevMatrix = true;
	auto prevNonEmptySequence = getNonemptyChildSequence(m_tree->m_beginSequence->getPtr()->as<Sequence>(),
	                                                     m_info.sequence->getPtr()->as<Sequence>());

	// Find index of current matrix in current sequence.
	const auto& matrices = m_info.sequence->getMatrices();
	const auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);
	if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
	{
		// The sequence has matrix input plugged in. We are at the beginning of the sequence.
		index = matrices.size() - 1;
	}

	if (index == matrices.size() - 1 && prevNonEmptySequence == nullptr)
	{
		hasPrevMatrix = false;
	}

	if (!hasPrevMatrix)
	{
		return;
	}

	m_info.isExternal = false;
	if (index == matrices.size() - 1)
	{
		const auto matrixPluggedIntoChild = GraphManager::getParent(prevNonEmptySequence, I3T_SEQ_IN_MAT);

		if (matrixPluggedIntoChild)
		{
			m_info.isExternal = true;
			m_currentMatrix = matrixPluggedIntoChild;
		}
		else
		{
			m_currentMatrix = prevNonEmptySequence->getMatrices().front();
		}
		m_info.sequence = prevNonEmptySequence.get();
	}
	else if (m_currentMatrix == nullptr)
	{
		// iterator is at the end
		m_currentMatrix = matrices.front();
	}
	else
	{
		m_currentMatrix = matrices[++index];
	}
}

void SequenceTree::MatrixIterator::invalidate()
{
	m_currentMatrix = nullptr;
}

//------------------------------------------------------------------------------------------------//

void setActivePart(Ptr<Node> node, float value)
{
	const auto maybeTransform = std::dynamic_pointer_cast<Transform>(node);
	if (maybeTransform)
	{
		maybeTransform->m_activePart = value;
	}
}

MatrixTracker::MatrixTracker(Sequence* beginSequence, TrackingDirection direction,
                             std::vector<UPtr<IModelProxy>> models)
    : m_models(std::move(models)), m_direction(direction), m_beginSequence(beginSequence)
{
	update();
}

void MatrixTracker::update()
{
	track();
}

Ptr<Sequence> MatrixTracker::getSequence() const
{
	if (m_beginSequence == nullptr)
	{
		return nullptr;
	}

	return m_beginSequence->getPtr()->as<Sequence>();
}

ID MatrixTracker::getSequenceID() const
{
	if (m_beginSequence == nullptr)
	{
		return NIL_ID;
	}

	return m_beginSequence->getId();
}

std::vector<Ptr<Model>> MatrixTracker::getModels() const
{
	std::vector<Ptr<Model>> result;
	for (const auto& proxy : m_models)
	{
		result.push_back(proxy->getModel());
	}

	return result;
}

bool MatrixTracker::setParam(float param)
{
	m_param = glm::clamp(param, 0.0f, 1.0f);

	if (!m_beginSequence)
	{
		return false;
	}

	track();

	return true;
}

void MatrixTracker::track()
{
	if (!m_beginSequence)
	{
		// uninitialized
		return;
	}

	m_state.reset();

	// Create iterator for traversing sequence branch.
	SequenceTree st(m_beginSequence->getPtr());

	auto [matrices, info] = st.begin().collectWithInfo();
	for (size_t i = 0; i < matrices.size(); ++i)
	{
		m_state.transformInfo[matrices[i]->getId()] = info[i];
	}

	for (const auto& matrix : matrices)
	{
		m_state.trackingProgress[matrix->getId()] = 0.0f;
		setActivePart(matrix, 0.0f);
	}

	if (matrices.empty())
	{
		// Empty sequence cannot be tracked, end tracking.
		stop();
		return;
	}

	// apply matrices right to left

	if (m_direction == TrackingDirection::LeftToRight)
	{
		std::reverse(matrices.begin(), matrices.end());
	}
	else if (m_direction == TrackingDirection::RightToLeft)
	{}

	const float matFactor = 1.0f / (float) matrices.size();
	const auto matricesBefore = (int) (m_param * (float) matrices.size());
	m_state.fullMatricesCount = matricesBefore;

	float interpParam = fmod(m_param, matFactor) / matFactor;

	handleEdgeCases(interpParam, matrices, info);

	glm::mat4 matrix(1.0f);

	for (int i = 0; i < matricesBefore; ++i)
	{
		const auto& transform = matrices[i];
		m_state.trackingProgress[transform->getId()] = 1.0f;
		setActivePart(transform, 1.0f);

		if (m_direction == TrackingDirection::LeftToRight)
		{
			matrix = matrix * transform->data().getMat4();
		}
		else if (m_direction == TrackingDirection::RightToLeft)
		{
			matrix = transform->data().getMat4() * matrix;
		}
	}

	if (0.0f < m_param && m_param < 1.0f)
	{
		// interpolate matrix
		glm::mat4 rhs;
		glm::mat4 lhs(1.0f);

		const auto maybeTransform = matrices[matricesBefore];
		m_state.trackingProgress[maybeTransform->getId()] = interpParam;
		setActivePart(maybeTransform, interpParam);
		rhs = maybeTransform->data().getMat4();

		auto useQuat = false;
		if (auto transform = std::dynamic_pointer_cast<Transform>(maybeTransform))
		{
			useQuat = transform->properties()->isRotation;
		}

		if (m_direction == TrackingDirection::LeftToRight)
		{
			matrix = matrix * Math::lerp(lhs, rhs, interpParam, useQuat);
		}
		else if (m_direction == TrackingDirection::RightToLeft)
		{
			matrix = Math::lerp(lhs, rhs, interpParam, useQuat) * matrix;
		}

		m_state.interpolatedTransformID = maybeTransform->getId();
	}

	m_state.interpolatedMatrix = matrix;

	setModelTransform();
}

void MatrixTracker::handleEdgeCases(float& interpParam, const std::vector<Ptr<Node>>& matrices,
                                    const std::vector<TransformInfo>& info)
{
	if (Math::eq(0.0f, m_param))
	{
		interpParam = 0.0f;

		m_state.interpolatedTransformID = matrices.front()->getId();
	}
	else if (Math::eq(1.0f, m_param))
	{
		interpParam = 1.0f;

		m_state.interpolatedTransformID = matrices.back()->getId();
	}
}

void MatrixTracker::stop()
{
	m_beginSequence = nullptr;
	m_models.clear();
}

void MatrixTracker::setModelTransform()
{
	for (const auto& model : m_models)
	{
		model->update(m_state.interpolatedMatrix);
	}
}
} // namespace Core
