#include "Sequence.h"

#include "GraphManager.h"

using namespace Core;

//===-- Storage -----------------------------------------------------------===//

const Operation g_storageOp = {
		"SeqStor", "_", 1, {EValueType::Matrix}, 1, {EValueType::Matrix}
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

DataStore& Sequence::Storage::getInternalData(size_t index)
{
	// return m_owner->as<Sequence>()->getInternalData(index);
	return m_internalData[0];
}

void Sequence::Storage::updateValues(int inputIndex)
{
	auto mat = getMatProduct(m_matrices);

	/*
	 * \todo MH pin mismatch.
	if (inputIndex == I3T_SEQ_IN_MAT)
	{
		// Matrix is directly set from outside.
		mat = getIn(I3T_SEQ_IN_MAT).getStorage(I3T_SEQ_MAT).getMat4();
	}
	 */
	if (inputIndex != -1)
	{
		mat = getIn(I3T_SEQ_IN_MAT).getStorage(inputIndex).getMat4();
	}

	// Set storage value.
	setInternalValue(mat, I3T_SEQ_MAT);

	// Notify multiplier about change.
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
	"SeqMul", "_", 1, {EValueType::MatrixMul}, 2, {EValueType::MatrixMul, EValueType::Matrix}
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

DataStore& Sequence::Multiplier::getInternalData(size_t index)
{
	return m_owner->as<Sequence>()->getInternalData(index);
}

void Sequence::Multiplier::updateValues(int inputIndex)
{
	auto product = getData(I3T_SEQ_MAT).getMat4();
	glm::mat4 mult(1.0f);

	if (getIn(I3T_SEQ_IN_MUL).isPluggedIn())
	{
		mult = getIn(I3T_SEQ_IN_MUL).getStorage(I3T_SEQ_MUL).getMat4();
	}
	auto result = mult * product;

	// Mul. output
	setInternalValue(result, I3T_SEQ_MUL);

	// Model matrix
	setInternalValue(result, I3T_SEQ_MOD);
}

//===-- Sequence ----------------------------------------------------------===//

Sequence::Sequence() : NodeBase(&g_sequence)
{
}

Pin& Sequence::getIn(size_t i)
{
	assert(i < m_operation->inputTypes.size() && "Illegal index.");

	if (i == I3T_SEQ_IN_MAT) { return m_storage->m_inputs[0]; }
	return m_multiplier->m_inputs[i];
}

Pin& Sequence::getOut(size_t i)
{
	assert(i < m_operation->outputTypes.size() && "Illegal index.");

	if (i == I3T_SEQ_IN_MUL) { return m_multiplier->m_outputs[0]; }
	if (i == I3T_SEQ_IN_MAT) { return m_storage->m_outputs[0]; }
	else
	{
		// Model matrix output.
		return m_multiplier->m_outputs[1];
	}
}

void Sequence::createComponents()
{
	m_storage = std::make_shared<Sequence::Storage>();
	m_storage->m_id = getId();

	m_storage->m_inputs.emplace_back(EValueType::Matrix, true, m_storage->getPtr(), 1);
	m_storage->m_outputs.emplace_back(EValueType::Matrix, false, m_storage->getPtr(), 1);

	m_storage->m_internalData.emplace_back(EValueType::Matrix);

	m_storage->m_owner = getPtr();


	m_multiplier = std::make_shared<Sequence::Multiplier>();
	m_multiplier->m_id = getId();

	m_multiplier->m_inputs.emplace_back(EValueType::MatrixMul, true, m_multiplier->getPtr(), 0);

	m_multiplier->m_outputs.emplace_back(EValueType::MatrixMul, false, m_multiplier->getPtr(), 0);
	m_multiplier->m_outputs.emplace_back(EValueType::Matrix, false, m_multiplier->getPtr(), 2);

	m_multiplier->m_internalData.emplace_back(EValueType::MatrixMul);
	m_multiplier->m_internalData.emplace_back(EValueType::Matrix);

	m_multiplier->m_owner = getPtr();
}

DataStore& Sequence::getInternalData(size_t index)
{
	if (index == I3T_SEQ_MUL)
	{
		return m_multiplier->m_internalData[0];
	}
	else if (index == I3T_SEQ_MAT)
	{
		return m_storage->m_internalData[0];
	}
	else
	{
		return m_multiplier->m_internalData[1];
	}
}

void Sequence::updatePins()
{
	/*
	m_storage->m_inputs[0].m_input = m_inputs[1].m_input;
	m_multiplier->m_inputs[0].m_input = m_inputs[0].m_input;

	m_storage->m_outputs[0].m_outputs = m_outputs[1].m_outputs;
	resetInputPin(m_storage->m_outputs[0].m_outputs, &m_storage->m_outputs[0]);

	m_multiplier->m_outputs[0].m_outputs = m_outputs[0].m_outputs;
	resetInputPin(m_multiplier->m_outputs[0].m_outputs, &m_multiplier->m_outputs[0]);

	m_multiplier->m_outputs[1].m_outputs = m_outputs[2].m_outputs;
	resetInputPin(m_multiplier->m_outputs[1].m_outputs, &m_multiplier->m_outputs[1]);
	 */
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
