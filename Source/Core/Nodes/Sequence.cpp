#include "Sequence.h"

#include "GraphManager.h"
#include "Tracking.h"

using namespace Core;

Ptr<Sequence> Builder::createSequence(MatrixTracker* tracker)
{
	auto ret = std::make_shared<Sequence>(tracker);
	ret->init();
	ret->updateValues(0);

	return ret;
}

//===-- Storage -----------------------------------------------------------===//

const Operation g_storageOp = {"SeqStor", "_", 1, {EValueType::Matrix}, 1, {EValueType::Matrix}};

Sequence::Storage::Storage()
    : Node(&g_storageOp)
{
}

void Sequence::Storage::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		// read matrix from input
		auto newVal = m_inputs[0].data().getMat4();
		setInternalValue(newVal, 0);
	}
	else
	{
		// recompute matrix product
		auto mat = getMatProduct(m_matrices);

		/*
		if (inputIndex != -1)
		{
			mat = getInput(I3T_SEQ_IN_MAT).data().getMat4();
		}
		 */

		// Set storage value.
		setInternalValue(mat, 0);
	}

	// Notify multiplier about change.
	auto sequence = m_owner->as<Sequence>();
	sequence->m_multiplier->updateValues(-1);
	sequence->m_tracker->update();
}

ValueSetResult Sequence::Storage::addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
{
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
	I3T_ASSERT(index < m_matrices.size(),
	           "Sequence does not have as many matrices as you are expecting.");
	LOG_INFO("Popping matrix at {} from sequence {}.", index, getId());

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
    : Node(&g_multiplerOp)
{
}

void Sequence::Multiplier::updateValues(int inputIndex)
{
	glm::mat4 mult(1.0f);

	if (getInput(I3T_SEQ_IN_MUL).isPluggedIn())
	{
		// Get world transform from parent sequence.
		mult = getInput(I3T_SEQ_IN_MUL).data().getMat4();
	}

	auto storageNode = m_owner->as<Sequence>()->m_storage;
	auto product = storageNode->getData(0).getMat4();
	auto result = mult * product;

	// Mul. output
	setInternalValue(result, 0);

	// Model matrix
	setInternalValue(result, 1);
}

//===-- Sequence ----------------------------------------------------------===//

Sequence::Sequence(MatrixTracker* tracker) : NodeBase(&g_sequence) { m_tracker = tracker; }

Sequence::~Sequence() { stopTracking(); }

void Sequence::onInit()
{
	m_storage = Builder::createNode<Sequence::Storage>(this);
	m_inputs[I3T_SEQ_IN_MAT].mapTo(&m_storage->m_inputs[0]);
	m_outputs[I3T_SEQ_OUT_MAT].mapTo(&m_storage->m_outputs[0]);

	m_multiplier = Builder::createNode<Sequence::Multiplier>(this);
	m_inputs[I3T_SEQ_IN_MUL].mapTo(&m_multiplier->m_inputs[0]);
	m_outputs[I3T_SEQ_OUT_MUL].mapTo(&m_multiplier->m_outputs[0]);
	m_outputs[I3T_SEQ_OUT_MOD].mapTo(&m_multiplier->m_outputs[1]);
}

Ptr<Node> Sequence::clone() { return Builder::createSequence(m_tracker); }

[[nodiscard]] Ptr<Transformation> Sequence::popMatrix(const int index)
{
	auto result = m_storage->popMatrix(index);
	result->m_activePart = 0.0f;

	return result;
}

void Sequence::updateValues(int inputIndex)
{
	if (inputIndex == -1)
	{
		// value of sequence changed
		m_storage->updateValues(-1);
		m_tracker->update();
	}
}

MatrixTracker* Sequence::startTracking(UPtr<IModelProxy> modelProxy)
{
	*m_tracker = MatrixTracker(this, std::move(modelProxy));

	return m_tracker;
}

void Sequence::stopTracking()
{
	// Called in destructor, cannot use shared_from_this() here.
	if (m_tracker->getSequenceID() == getId())
	{
		*m_tracker = MatrixTracker();
	}
}

void Sequence::addPlugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback)
{
	m_multiplier->addPlugCallback(callback);
	Node::addPlugCallback(callback);
}

void Sequence::addUnplugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback)
{
	m_multiplier->addUnplugCallback(callback);
	Node::addUnplugCallback(callback);
}
