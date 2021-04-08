#include "Sequence.h"

#include "GraphManager.h"

using namespace Core;

void Sequence::addMatrix(Ptr<Sequence::Matrix> matrix, size_t index) noexcept
{
	Debug::Assert(isTransform(matrix), "Parameter 'matrix' is not transform node.");

	GraphManager::unplugAll(matrix);
	matrix->as<Transformation>()->m_currentSequence = getPtr();

	if (index > m_matrices.size())
		m_matrices.push_back(std::move(matrix));
	else
		m_matrices.insert(m_matrices.begin() + index, std::move(matrix));

	updateValues(0);
	spreadSignal();
}

void Sequence::updateValues(int inputIndex)
{
	glm::mat4 result(1.0f);

	if (m_inputs[1].isPluggedIn())
	{
		// Matrix node is connected to direct matrix input.
		result = m_inputs[1].getStorage().getMat4();
	}
	else
	{
		for (const auto& mat : m_matrices)
		{
			result *= mat->getData().getMat4();
		}
	}

	m_internalData[0].setValue(result);
	m_internalData[1].setValue(result);
}

ENodePlugResult Sequence::isPlugCorrect(Pin* input, Pin* output)
{
	auto usualCheckResult = NodeBase::isPlugCorrect(input, output);
	bool areBothDifferentSequences = GraphManager::getOperation(input) == GraphManager::getOperation(output) &&
	                                 !GraphManager::areFromSameNode(input, output);
	if (usualCheckResult == ENodePlugResult::Err_Loop && areBothDifferentSequences)
	{
		// Two sequences can have a loop.
		auto lhs = output->getOwner();
		auto rhs = input->getOwner();

		if (gm::arePlugged(lhs->getInPin(1), rhs->getOutPin(1)))
		{
			return ENodePlugResult::Err_Loop;
		}
		return ENodePlugResult::Ok;
	}

	return ENodePlugResult::Ok;

	// return NodeBase::isPlugCorrect(input, output);
}

void Sequence::receiveSignal(int inputIndex)
{
	updateValues(0);

	// Do not spread signal from matrix output when got signal from mul input.
	if (inputIndex == 0)
  {
    spreadSignal(0);
	}
	else
  {
		spreadSignal(1);
	}
}
