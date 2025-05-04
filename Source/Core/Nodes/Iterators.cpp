#include "Iterators.h"

#include "Camera.h"
#include "GraphManager.h"
#include "Model.h"

namespace Core
{
TransformChain::TransformChain(const Ptr<Sequence>& sequence, const Ptr<Camera>& camera)
    : m_beginSequence(sequence), m_beginCamera(camera)

{}

TransformChain::TransformChainIterator TransformChain::begin()
{
	auto it = TransformChainIterator(this, m_beginSequence, m_beginCamera, nullptr, m_skipEmptySequences,
	                                 m_ignoreCamera, m_skipEmptyCamera);
	return it;
}

TransformChain::TransformChainIterator TransformChain::end()
{
	// NOTE: Would need to pass all relevant data (seq/camera/flags) if the iterator could go backwards
	return TransformChainIterator();
}

TransformChain::TransformChainIterator::TransformChainIterator(TransformChain* chain, const Ptr<Sequence>& sequence,
                                                               const Ptr<Camera>& camera, const Ptr<Node>& node,
                                                               bool skipEmptySequences, bool ignoreCamera,
                                                               bool skipEmptyCamera)
    : m_tree(chain), m_skipEmptySequences(skipEmptySequences), m_ignoreCamera(ignoreCamera),
      m_skipEmptyCamera(skipEmptyCamera)
{
	// Cannot ignore camera if we begin in one.
	if (m_ignoreCamera && camera != nullptr)
	{
		return; // Return as invalid iterator
	}
	if (camera != nullptr && m_skipEmptyCamera)
	{
		if (camera->isEmpty())
			return;
	}

	m_info.camera = camera;
	m_info.sequence = sequence;
	m_info.currentNode = node;

	if (m_info.camera != nullptr)
	{
		// TODO: [T-VIEWPORT]
		auto seqs = {m_info.camera->getView(), m_info.camera->getProj()};
		auto types = {TransformSpace::View, TransformSpace::Projection};
		auto typeIt = types.begin();
		for (auto seqIt = seqs.begin(); seqIt != seqs.end(); ++seqIt, ++typeIt)
		{
			if (*seqIt == m_info.sequence)
			{
				m_info.type = *typeIt;
				break;
			}
		}
	}

	if (m_info.currentNode == nullptr)
	{
		if (m_info.sequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn())
		{
			// there is matrix output plugged into this sequence
			m_info.isExternal = true;
			m_info.currentNode = sequence;
			m_info.dataIndex = I3T_SEQ_MAT;
		}
		else
		{
			if (sequence->getMatrices().empty())
			{
				if (sequence->getMatrices().empty() && m_skipEmptySequences)
					this->next(); // Advance immediately
			}
			else
			{
				m_info.currentNode = sequence->getMatrices().back();
			}
		}
	}
}

bool TransformChain::TransformChainIterator::equals(const Iterator& other) const
{
	return m_info.sequence == other.m_info.sequence && m_info.currentNode == other.m_info.currentNode;
}
bool TransformChain::TransformChainIterator::valid() const
{
	return m_info.sequence != nullptr;
}

bool TransformChain::TransformChainIterator::advanceWithinSequence()
{
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
			return true;
		}
	}
	return false;
}

void TransformChain::TransformChainIterator::next()
{
	// Check inner sequence state
	if (advanceWithinSequence())
		return;

	// No inner data, search for the next sequence
	Ptr<Camera> nextCamera;
	Ptr<Sequence> nextSequence;
	if (m_info.camera != nullptr)
	{
		nextCamera = m_info.camera;
		// Advance within a camera
		// TODO: [T-VIEWPORT] Viewport sequence
		if (m_info.sequence == m_info.camera->getView() && (!m_skipEmptySequences || !m_info.sequence->isEmpty()))
		{
			nextSequence = m_info.camera->getProj();
			m_info.type = TransformSpace::Projection;
		}
		else
		{
			// End of camera, end of chain (unless we add matrix mul input to camera)
			invalidate();
			return;
		}
	}

	if (nextSequence == nullptr)
	{
		// Advance in the graph
		bool isCamera;
		Ptr<Node> parent =
		    GraphManager::getParentSequenceOrCamera(m_info.sequence, isCamera, m_skipEmptySequences, m_skipEmptyCamera);
		if (!parent || (isCamera && m_ignoreCamera))
		{
			invalidate();
			return;
		}
		if (isCamera)
		{
			nextCamera = std::static_pointer_cast<Camera>(parent);
			nextSequence = nextCamera->getView();
			m_info.type = TransformSpace::View;
		}
		else
		{
			nextSequence = std::static_pointer_cast<Sequence>(parent);
			m_info.type = TransformSpace::Model;
		}
	}

	m_info.camera = nextCamera;
	m_info.sequence = nextSequence;

	// Find the initial data source for the new sequence
	m_info.isExternal = false;
	m_info.dataIndex = 0;

	// Check if the next sequence is connected externally via matrix pin
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
	I3T_ASSERT(valid(), "Cannot dereference past the end TransformChainIterator!");
	return m_info.currentNode;
}

void TransformChain::TransformChainIterator::invalidate()
{
	m_info = TransformInfo();
}

} // namespace Core