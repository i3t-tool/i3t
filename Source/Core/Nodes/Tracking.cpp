#include "Core/Nodes/Tracking.h"

#include "Core/Nodes/GraphManager.h"

namespace Core
{
SequenceTree::SequenceTree(Ptr<NodeBase> sequence) { m_beginSequence = toSequence(sequence); }

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
	while ((parent = toSequence(GraphManager::getParent(cur, 0))) != nullptr)
	{
		cur = parent;
	}

	auto it = MatrixIterator(cur, nullptr);
	it.m_tree = this;

	return it;
}

SequenceTree::MatrixIterator::MatrixIterator(Ptr<Sequence>& sequence)
{
	m_currentSequence = sequence;
	m_currentMatrix = sequence->getMatrices().empty() ? nullptr : sequence->getMatrices().back();
}

SequenceTree::MatrixIterator::MatrixIterator(Ptr<Sequence>& sequence, NodePtr node)
{
	m_currentSequence = sequence;
	m_currentMatrix = node;
}

SequenceTree::MatrixIterator::MatrixIterator(const SequenceTree::MatrixIterator& mt)
{
	m_tree = mt.m_tree;
	m_currentSequence = mt.m_currentSequence;
	m_currentMatrix = mt.m_currentMatrix;
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

Ptr<Transformation> SequenceTree::MatrixIterator::operator*() const
{
	I3T_ASSERT(m_currentMatrix != nullptr && "Iterator is at the end!");

	return m_currentMatrix->as<Transformation>();
}

bool SequenceTree::MatrixIterator::operator==(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix == rhs.m_currentMatrix;
}

bool SequenceTree::MatrixIterator::operator!=(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix != rhs.m_currentMatrix;
}

void SequenceTree::MatrixIterator::advance()
{
	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	/// \todo MH handle case when matrix is not in a sequence.

	if (index == 0)
	{
		auto parent = GraphManager::getParent(m_currentSequence);

		// Check if current matrix is not first in the graph.
		if (parent)
		{
			// Sequence is not the root, there is another parent sequence.
			m_currentSequence = parent->as<Sequence>();
			if (!m_currentSequence->getMatrices().empty())
			{
				m_currentMatrix = m_currentSequence->getMatrices().back();
			}
			else
			{
				m_currentMatrix = nullptr;
			}
		}
		else
		{
			m_currentMatrix = nullptr;
		}
	}
	else
	{
		m_currentMatrix = matrices[--index];
	}
}

void SequenceTree::MatrixIterator::withdraw()
{
	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	if (m_currentMatrix == nullptr)
		index = -1;

	/// \todo MH handle case when matrix is not in a sequence.

	if (m_currentMatrix == matrices.back())
	{
		// Current matrix is last matrix in a sequence. Go to the previous sequence.
		auto prev = m_tree->m_beginSequence;
		auto prevsParent = m_tree->m_beginSequence;
		while ((prevsParent = toSequence(GraphManager::getParent(prev))) != m_currentSequence)
		{
			prev = prevsParent;
		}
		m_currentSequence = prev;
		m_currentMatrix = m_currentSequence->getMatrices().front();
	}
	else
	{
		m_currentMatrix = matrices[++index];
	}
}

//------------------------------------------------------------------------------------------------//

MatrixTracker::MatrixTracker(Ptr<Sequence> beginSequence, UPtr<IModelProxy> model)
    : m_model(std::move(model)), m_interpolatedMatrix(1.0f), m_beginSequence(beginSequence)
{
}

void MatrixTracker::update()
{
	track();
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

	m_trackingProgress.clear();

	// Create iterator for traversing sequence branch.
	auto st = SequenceTree(m_beginSequence);

	int matricesCount = 0; // to the root
	{
		auto it = st.begin();

		// Get matrices count in total.
		while (it != st.end())
		{
			m_trackingProgress[(*it)->getId()] = 0.0f;
			(*it)->m_activePart = 0.0f;
			++it;
			++matricesCount;
		}
	}

	if (matricesCount == 0)
	{
		// Empty sequence cannot be tracked, end tracking.
		stop();
		return;
	}

	// Iterator now points to the sequences root.

	float matFactor = 1.0f / (float)matricesCount;
	int matricesBefore = (int)(m_param * (float)matricesCount);
	m_fullMatricesCount = matricesBefore;

	float interpParam = fmod(m_param, matFactor) / matFactor;
	// Handle special cases
	if (Math::eq(0.0f, m_param))
	{
		interpParam = 0.0f;
		m_interpolatedTransformID = (*st.begin())->getId();
	}
	else if (Math::eq(1.0f, m_param))
	{
		interpParam = 1.0f;
		m_interpolatedTransformID = (*(st.begin() + (matricesCount - 1)))->getId();
	}

	glm::mat4 result(1.0f);
	glm::mat4 rhs;
	glm::mat4 lhs(1.0f);

	{
		auto it = st.begin();
		for (int i = 0; i < m_fullMatricesCount; ++i)
		{
			m_trackingProgress[(*it)->getId()] = 1.0f;
			(*it)->m_activePart = 1.0f;
			result = (*it)->getData().getMat4() * result;
			++it;
		}

		if (!Math::eq(1.0f, m_param))
		{
			// Interpolate last matrix.
			m_trackingProgress[(*it)->getId()] = interpParam;
			(*it)->m_activePart = interpParam;
			rhs = (*it)->getData().getMat4();
			result = Math::lerp(lhs, rhs, interpParam, isRot(*it)) * result;
			m_interpolatedTransformID = (*it)->getId();
		}
	}

	m_interpolatedMatrix = result;

	setTransform();
}

void MatrixTracker::stop()
{
	m_beginSequence = nullptr;
	m_model = nullptr;
}

void MatrixTracker::setTransform()
{
	// m_model->getModel()->setValue(m_interpolatedMatrix);
	m_model->update(m_interpolatedMatrix);
}
}
