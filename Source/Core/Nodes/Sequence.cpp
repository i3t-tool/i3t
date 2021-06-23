#include "Sequence.h"

#include "GraphManager.h"

using namespace Core;

//===-- Storage -----------------------------------------------------------===//

const Operation g_storageOp = {
		"_", "_", 1, {EValueType::Matrix}, 1, {EValueType::Matrix}
};

Sequence::Storage::Storage() :
		Node(&g_storageOp)
{
}

Pin& Sequence::Storage::getIn(size_t i)
{
	return m_owner->getIn(i);
}

Pin& Sequence::Storage::getOut(size_t i)
{
	return m_owner->getOut(i);
}

void Sequence::Storage::updateValues(int inputIndex)
{
	auto mat = getMatProduct(m_matrices);

	if (getInPin(0).isPluggedIn())
	{
		// Matrix input
		mat = getInPinRef(0).getStorage(0).getMat4();
	}
	setInternalValue(mat, 0);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);
}

ValueSetResult Sequence::Storage::addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
{
	auto* currentMap = matrix->getDataMap();
	GraphManager::unplugAll(matrix);
	matrix->setDataMap(currentMap);

	index = index > m_matrices.size() ? m_matrices.size() : index;
	m_matrices.insert(m_matrices.begin() + index, matrix);

	matrix->as<Transformation>()->setSequence(m_owner, index);

	updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	// If sequence is sub-node of camera node.
	m_owner->as<Sequence>()->notifyParent();

	return ValueSetResult{};
}

Ptr<Transformation> Sequence::Storage::popMatrix(const int index)
{
	Debug::Assert(m_matrices.size() > static_cast<size_t>(index),
	              "Sequence does not have so many matrices as you are expecting.");

	auto result = std::move(m_matrices.at(index));
	m_matrices.erase(m_matrices.begin() + index);

	result->nullSequence();

	updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	m_owner->as<Sequence>()->notifyParent();

	return result;
}

void Sequence::Storage::swap(int from, int to)
{
	if (from > m_matrices.size() || to > m_matrices.size())
		return;

	updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	m_owner->as<Sequence>()->notifyParent();

	std::swap(m_matrices[from], m_matrices[to]);
}

//===-- Multiplier --------------------------------------------------------===//

const Operation g_multiplerOp = {
	"_", "_", 1, {EValueType::MatrixMul}, 2, {EValueType::MatrixMul, EValueType::Matrix}
};

Sequence::Multiplier::Multiplier() :
		Node(&g_multiplerOp)
{}

Pin& Sequence::Multiplier::getIn(size_t i)
{
	return m_owner->getIn(i);
}

Pin& Sequence::Multiplier::getOut(size_t i)
{
	return m_owner->getOut(i);
}

void Sequence::Multiplier::updateValues(int inputIndex)
{
	auto product = m_owner->as<Sequence>()->m_storage->getData().getMat4();
	glm::mat4 mult(1.0f);

	if (m_owner->getInPin(0).isPluggedIn())
	{
		auto parent = GraphManager::getParent(m_owner)->as<Sequence>();
		mult = parent->getData().getMat4();
	}
	// Mul. output
	setInternalValue(mult * product, 0);

	// Model matrix
	setInternalValue(mult * product, 1);
}

//===-- Sequence ----------------------------------------------------------===//

Sequence::Sequence() : NodeBase(&g_sequence)
{
}

Pin& Sequence::getIn(size_t i)
{
	assert(i < m_operation->inputTypes.size() && "Illegal index.");

	if (i == 1)
	{
		return m_storage->m_inputs[0];
	}
	return m_multiplier->m_inputs[i];
}

Pin& Sequence::getOut(size_t i)
{
	assert(i < m_operation->outputTypes.size() && "Illegal index.");

	if (i == 2)
	{
		return m_storage->m_outputs[0];
	}
	return m_multiplier->m_outputs[i];
}

void Sequence::createComponents()
{
	m_storage = std::make_shared<Sequence::Storage>();
	m_storage->init();

	/*
	m_storage->m_inputs.emplace_back(EValueType::Matrix, true, m_storage->getPtr(), 0);
	m_storage->m_outputs.emplace_back(EValueType::Matrix, false, m_storage->getPtr(), 0);

	m_storage->m_internalData.emplace_back(EValueType::Matrix);
	 */

	m_storage->m_owner = getPtr();


	m_multiplier = std::make_shared<Sequence::Multiplier>();
	m_multiplier->init();

	/*
	m_multiplier->m_inputs.emplace_back(EValueType::MatrixMul, true, m_multiplier->getPtr(), 0);

	m_multiplier->m_outputs.emplace_back(EValueType::MatrixMul, false, m_multiplier->getPtr(), 0);
	m_multiplier->m_outputs.emplace_back(EValueType::Matrix, false, m_multiplier->getPtr(), 1);

	m_multiplier->m_internalData.emplace_back(EValueType::MatrixMul);
	m_multiplier->m_internalData.emplace_back(EValueType::Matrix);
	 */

	m_multiplier->m_owner = getPtr();
}

DataStore& Sequence::getInternalData(size_t index)
{
	if (index == 0)
	{
		return m_multiplier->getInternalData(0);
	}
	else if (index == 1)
	{
		return m_storage->getInternalData(0);
	}
	else if (index == 2)
	{
		return m_multiplier->getInternalData(1);
	}
}

void Sequence::updatePins()
{
	m_storage->m_inputs[0].m_input = m_inputs[1].m_input;
	m_multiplier->m_inputs[0].m_input = m_inputs[0].m_input;

	m_storage->m_outputs[0].m_outputs = m_outputs[1].m_outputs;
	resetInputPin(m_storage->m_outputs[0].m_outputs, &m_storage->m_outputs[0]);

	m_multiplier->m_outputs[0].m_outputs = m_outputs[0].m_outputs;
	resetInputPin(m_multiplier->m_outputs[0].m_outputs, &m_multiplier->m_outputs[0]);

	m_multiplier->m_outputs[1].m_outputs = m_outputs[2].m_outputs;
	resetInputPin(m_multiplier->m_outputs[1].m_outputs, &m_multiplier->m_outputs[1]);
}

void Sequence::resetInputPin(std::vector<Pin*>& outputsOfPin, Pin* newInput)
{
	std::for_each(outputsOfPin.begin(), outputsOfPin.end(), [newInput](Pin* p) { p->m_input = newInput; });
}

void Sequence::updateValues(int inputIndex)
{
	if (inputIndex == -1)
	{
		// Got update from nested transform.
		m_storage->updateValues(inputIndex);
		m_multiplier->updateValues(inputIndex);
	}
}

void Sequence::notifyParent()
{
	if (m_parent)
	{
		m_parent->spreadSignal();
		m_parent->updateValues(0);
	}
}

void Sequence::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);

	/// \todo MH check this expression.
	spreadSignal(0);
	if (inputIndex != 0)
	{
		spreadSignal(1);
	}
}
