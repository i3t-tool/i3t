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
	while ((parent = toSequence(GraphManager::getParent(cur->getPtr(), 0))) != nullptr)
	{
		cur = parent.get();
	}

	auto it = MatrixIterator(cur, nullptr);
	it.m_tree = this;

	return it;
}

SequenceTree::MatrixIterator::MatrixIterator(Sequence* sequence)
{
	m_currentSequence = sequence;

	auto inputOperator = GraphManager::getParent(m_currentSequence->getPtr(), I3T_SEQ_IN_MAT);
	if (inputOperator != nullptr)
	{
		// there is matrix output plugged into this sequence
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

Ptr<Node> SequenceTree::MatrixIterator::operator*() const
{
	I3T_ASSERT(m_currentMatrix != nullptr, "Iterator is at the end!");

	return m_currentMatrix;
}

bool SequenceTree::MatrixIterator::operator==(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix == rhs.m_currentMatrix && m_currentSequence == rhs.m_currentSequence;
}

bool SequenceTree::MatrixIterator::operator!=(const SequenceTree::MatrixIterator& rhs) const
{
	return !(*this == rhs);
}

void SequenceTree::MatrixIterator::advance()
{
	const auto parentSequence = GraphManager::getParent(m_currentSequence->getPtr());

	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	if (index == 0)
	{
		// We are at the beginning of the sequence. Go to the next sequence.

		// Check if current matrix is not first in the graph.
		if (parentSequence)
		{
			// Check if there is a node plugged into parent sequence matrix input.
			auto parentNode = GraphManager::getParent(parentSequence, I3T_SEQ_IN_MAT);
			if (parentNode)
			{
				m_currentSequence = parentSequence->as<Sequence>().get();
				m_currentMatrix = parentNode;

				return;
			}

			// Sequence is not the root, there is another parent sequence.
			m_currentSequence = parentSequence->as<Sequence>().get();
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
	// helper function
	const auto toSequenceFn = [](Ptr<Node> node) -> Sequence* {
		if (node == nullptr)
		{
			return nullptr;
		}

		return node->as<Sequence>().get();
	};

	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	// Not C++ standard compliant, but we need to be able to decrement end iterator.
	// m_currentMatrix == nullptr when iterator is at the end.
	if (m_currentMatrix == nullptr)
	{
		index = -1;
	}

	if (matrices.empty() || m_currentMatrix == matrices.back())
	{
		// Current matrix is last matrix in a sequence. Go to the previous sequence.

		auto prevSequence = m_tree->m_beginSequence;
		auto prevSequenceParent = m_tree->m_beginSequence;

		// Find previous sequence.
		// We must begin from the root sequence, because parent sequence may have multiple children.
		while ((prevSequenceParent = toSequenceFn(GraphManager::getParent(prevSequence->getPtr()))) !=
		       m_currentSequence)
		{
			prevSequence = prevSequenceParent;
		}

		// Check if there is a node plugged into parent sequence matrix input.
		auto parentNode = GraphManager::getParent(prevSequence->getPtr(), I3T_SEQ_IN_MAT);
		if (parentNode)
		{
			m_currentMatrix = parentNode;
			m_currentSequence = prevSequence;

			return;
		}

		m_currentSequence = prevSequence;
		if (m_currentSequence->getMatrices().empty())
		{
			m_currentMatrix = nullptr;
		}
		else
		{
			m_currentMatrix = m_currentSequence->getMatrices().front();
		}
	}
	else
	{
		m_currentMatrix = matrices[++index];
	}
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

MatrixTracker::MatrixTracker(Sequence* beginSequence, UPtr<IModelProxy> model)
    : m_model(std::move(model)), m_interpolatedMatrix(1.0f), m_beginSequence(beginSequence)
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
	auto st = SequenceTree(m_beginSequence->getPtr());

	int matricesCount = 0; // to the root
	{
		auto it = st.begin();

		// Get matrices count in total.
		while (it != st.end())
		{
			m_trackingProgress[(*it)->getId()] = 0.0f;
			setActivePart(*it, 0.0f);
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

	float matFactor = 1.0f / (float) matricesCount;
	int matricesBefore = (int) (m_param * (float) matricesCount);
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
			setActivePart(*it, 1.0f);
			result = (*it)->getData().getMat4() * result;
			++it;
		}

		if (!Math::eq(1.0f, m_param))
		{
			// Interpolate last matrix.
			const auto transform = *it;
			m_trackingProgress[transform->getId()] = interpParam;
			setActivePart(transform, interpParam);
			rhs = transform->getData().getMat4();

			auto useQuat = false;
			if (const auto reallyTransform = std::dynamic_pointer_cast<Transform>(transform))
			{
				useQuat = reallyTransform->properties()->isRotation;
			}
			result = Math::lerp(lhs, rhs, interpParam, useQuat) * result;

			m_interpolatedTransformID = transform->getId();
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
} // namespace Core
