#include "Sequence.h"

#include "GraphManager.h"
#include "Tracking.h"

using namespace Core;

Ptr<Sequence> Builder::createSequence(MatrixTracker* tracker)
{
	auto ret = std::make_shared<Sequence>(tracker);
	ret->init();
	ret->createComponents();
	ret->updateValues(0);

	return ret;
}

//===-- Storage -----------------------------------------------------------===//

const Operation g_storageOp = {"SeqStor", "_", 1, {EValueType::Matrix}, 1, {EValueType::Matrix}};

Sequence::Storage::Storage()
    : // 		Node(&g_storageOp)
      Node(&g_sequence)
{
}

Pin& Sequence::Storage::getIn(size_t i) { return m_owner->getIn(i); }

Pin& Sequence::Storage::getOut(size_t i) { return m_owner->getOut(i); }

/// \todo MH Check if this function should be used.
DataStore& Sequence::Storage::getInternalData(size_t index)
{
	return m_owner->as<Sequence>()->getInternalData(index);
	// return m_internalData[index];
}

void Sequence::Storage::updateValues(int inputIndex)
{
	auto in0 = getIn(I3T_SEQ_MAT);

	/*
	 * \todo MH pin mismatch.
	if (inputIndex == I3T_SEQ_IN_MAT)
	{
	    // Matrix is directly set from outside.
	    mat = getIn(I3T_SEQ_IN_MAT).getStorage(I3T_SEQ_MAT).getMat4();
	}
	 */

	if (in0.isPluggedIn())
	{
		// Read value from outside.
		// in0.data().setValue(in0.);
		auto newVal = in0.data().getMat4();
		setInternalValue(newVal, I3T_SEQ_MAT);
	}
	else
	{
		auto mat = getMatProduct(m_matrices);

		if (inputIndex != -1)
		{
			mat = getIn(I3T_SEQ_IN_MAT).data().getMat4();
		}

		// Set storage value.
		setInternalValue(mat, I3T_SEQ_MAT);
	}

	// Notify multiplier about change.
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);
}

ValueSetResult Sequence::Storage::addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
{
	/// @todo test this
	// insert transform to matrix array
	index = index >= m_matrices.size() ? m_matrices.size() : index;
	m_matrices.insert(m_matrices.begin() + index, matrix);

	// mark transform used in sequence
	matrix->as<Transformation>()->setSequence(m_owner, index);

	auto seq = m_owner->as<Sequence>();

	updateValues(-1);

	// Update world transform
	seq->m_multiplier->updateValues(-1);

	// If sequence is sub-node of camera node.
	seq->notifyOwner();

	return ValueSetResult{};
}

Ptr<Transformation> Sequence::Storage::popMatrix(const int index)
{
	I3T_ASSERT(m_matrices.size() > static_cast<size_t>(index),
	           "Sequence does not have so many matrices as you are expecting.");

	auto result = std::move(m_matrices.at(index));
	m_matrices.erase(m_matrices.begin() + index);

	result->nullSequence();

	updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	m_owner->as<Sequence>()->notifyOwner();

	return result;
}

void Sequence::Storage::swap(int from, int to)
{
	if (from > m_matrices.size() || to > m_matrices.size())
		return;

	updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	m_owner->as<Sequence>()->notifyOwner();

	std::swap(m_matrices[from], m_matrices[to]);
}

//===-- Multiplier --------------------------------------------------------===//

const Operation g_multiplerOp = {
    "SeqMul", "_", 1, {EValueType::MatrixMul}, 2, {EValueType::MatrixMul, EValueType::Matrix}};

Sequence::Multiplier::Multiplier()
    : // 		Node(&g_multiplerOp)
      Node(&g_sequence)
{
}

Pin& Sequence::Multiplier::getIn(size_t i) { return m_owner->getIn(i); }

Pin& Sequence::Multiplier::getOut(size_t i) { return m_owner->getOut(i); }

DataStore& Sequence::Multiplier::getInternalData(size_t index)
{
	return m_owner->as<Sequence>()->getInternalData(index);
}

void Sequence::Multiplier::updateValues(int inputIndex)
{
	auto product = m_owner->as<Sequence>()->getData(I3T_SEQ_MAT).getMat4();
	glm::mat4 mult(1.0f);

	if (getIn(I3T_SEQ_IN_MUL).isPluggedIn())
	{
		// Get world transform from parent sequence.
		mult = getIn(I3T_SEQ_IN_MUL).getStorage(I3T_SEQ_MUL).getMat4();
	}
	auto result = mult * product;

	// Mul. output
	setInternalValue(result, I3T_SEQ_MUL);

	// Model matrix
	setInternalValue(result, I3T_SEQ_MOD);

	notifyOwner();
}

//===-- Sequence ----------------------------------------------------------===//

Sequence::Sequence(MatrixTracker* tracker) : NodeBase(&g_sequence)
{
	m_tracker = tracker;
}

Sequence::~Sequence()
{
	stopTracking();
}

Ptr<Node> Sequence::clone()
{
	return Builder::createSequence(m_tracker);
}

Pin& Sequence::getIn(size_t i)
{
	I3T_ASSERT(i < g_sequence.inputTypes.size(), "Illegal index.");

	if (i == I3T_SEQ_IN_MAT)
		return m_storage->m_inputs[i];
	return m_multiplier->m_inputs[i];
}

Pin& Sequence::getOut(size_t i)
{
	I3T_ASSERT(i < g_sequence.outputTypes.size(), "Illegal index.");

	if (i == I3T_SEQ_OUT_MUL || i == I3T_SEQ_OUT_MOD)
		return m_multiplier->m_outputs[i];
	else
	{
		// Model matrix output.
		return m_storage->m_outputs[i];
	}
}

void Sequence::createComponents()
{
	m_storage = std::make_shared<Sequence::Storage>();
	m_storage->m_id = getId();

	m_storage->m_inputs.emplace_back(EValueType::MatrixMul, true, m_storage->getPtr(), 0);
	m_storage->m_inputs.emplace_back(EValueType::Matrix, true, m_storage->getPtr(), 1);
	m_storage->m_outputs.emplace_back(EValueType::MatrixMul, false, m_storage->getPtr(), 0);
	m_storage->m_outputs.emplace_back(EValueType::Matrix, false, m_storage->getPtr(), 1);
	m_storage->m_outputs.emplace_back(EValueType::Matrix, false, m_storage->getPtr(), 2);

	m_storage->m_internalData.emplace_back(EValueType::MatrixMul);
	m_storage->m_internalData.emplace_back(EValueType::Matrix);
	m_storage->m_internalData.emplace_back(EValueType::Matrix);

	m_storage->m_owner = getPtr();

	m_multiplier = std::make_shared<Sequence::Multiplier>();
	m_multiplier->m_id = getId();

	m_multiplier->m_inputs.emplace_back(EValueType::MatrixMul, true, m_multiplier->getPtr(), 0);
	m_multiplier->m_inputs.emplace_back(EValueType::Matrix, true, m_multiplier->getPtr(), 1);

	m_multiplier->m_outputs.emplace_back(EValueType::MatrixMul, false, m_multiplier->getPtr(), 0);
	m_multiplier->m_outputs.emplace_back(EValueType::Matrix, false, m_multiplier->getPtr(), 1);
	m_multiplier->m_outputs.emplace_back(EValueType::Matrix, false, m_multiplier->getPtr(), 2);

	m_multiplier->m_internalData.emplace_back(EValueType::MatrixMul);
	m_multiplier->m_internalData.emplace_back(EValueType::Matrix);
	m_multiplier->m_internalData.emplace_back(EValueType::Matrix);

	m_multiplier->m_owner = getPtr();
}

DataStore& Sequence::getInternalData(size_t index)
{
	if (index == I3T_SEQ_MUL || index == I3T_SEQ_MOD)
		return m_multiplier->m_internalData[index];
	else
	{
		// index == I3T_SEQ_MAT
		return m_storage->m_internalData[index];
	}
}

[[nodiscard]] Ptr<Transformation> Sequence::popMatrix(const int index)
{
	auto result = m_storage->popMatrix(index);
	result->m_activePart = 0.0f;

	return result;
}

MatrixTracker* Sequence::startTracking(UPtr<IModelProxy> modelProxy)
{
	*m_tracker = MatrixTracker(shared_from_this()->as<Sequence>(), std::move(modelProxy));

	return m_tracker;
}

void Sequence::stopTracking()
{
	if (m_tracker->getSequence() == shared_from_this()->as<Sequence>())
	{
		*m_tracker = MatrixTracker();
	}
}

void Sequence::updateValues(int inputIndex)
{
	if (inputIndex == -1)
	{
		// Got update from nested transform.
		m_storage->updateValues(inputIndex);
		m_multiplier->updateValues(inputIndex);
	}

	m_tracker->update();

	if (m_owner)
	{
		notifyOwner();
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
