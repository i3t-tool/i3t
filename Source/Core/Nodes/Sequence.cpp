/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Sequence.h"

#include "GraphManager.h"
#include "Tracking.h"

namespace Core
{
Ptr<Sequence> Builder::createSequence(MatrixTracker* tracker)
{
	auto ret = std::make_shared<Sequence>(tracker);
	ret->init();
	ret->updateValues(0);

	return ret;
}

//===-- Storage -----------------------------------------------------------===//
ValueSetResult Sequence::Storage::addMatrix(Ptr<Transform> matrix, size_t index) noexcept
{
	// insert transform to matrix array
	index = index >= m_matrices.size() ? m_matrices.size() : index;
	m_matrices.insert(m_matrices.begin() + index, matrix);

	// mark transform as used in a sequence
	matrix->as<Transform>()->setSequence(&m_sequence, index);

	return ValueSetResult{};
}

Ptr<Transform> Sequence::Storage::popMatrix(const int index)
{
	I3T_ASSERT(index < m_matrices.size(), "Sequence does not have as many matrices as you are expecting.");
	LOG_INFO("Popping matrix at {} from sequence {}.", index, m_sequence.getId());

	auto result = std::move(m_matrices.at(index));
	m_matrices.erase(m_matrices.begin() + index);

	result->nullSequence();

	return result;
}

void Sequence::Storage::swap(int from, int to)
{
	if (from > m_matrices.size() || to > m_matrices.size())
	{
		return;
	}

	std::swap(m_matrices[from], m_matrices[to]);

	m_sequence.updateValues(-1);
}

//===-- Sequence ----------------------------------------------------------===//

Sequence::Sequence(MatrixTracker* tracker) : Node(&g_sequence), m_storage(*this), m_tracker(tracker) {}

Sequence::~Sequence()
{
	stopTracking();
}

ValueSetResult Sequence::pushMatrix(Ptr<Transform> matrix) noexcept
{
	const auto result = pushMatrix(matrix, m_storage.m_matrices.size());
	updateValues(-1);

	return result;
}

ValueSetResult Sequence::pushMatrix(Ptr<Transform> matrix, size_t index) noexcept
{
	const auto result = m_storage.addMatrix(matrix, index);
	updateValues(-1);

	return result;
}

[[nodiscard]] Ptr<Transform> Sequence::popMatrix(const int index)
{
	const auto result = m_storage.popMatrix(index);
	result->m_activePart = 0.0f;
	updateValues(-1);

	return result;
}

void Sequence::swap(int from, int to)
{
	m_storage.swap(from, to);
	updateValues(-1);
}

void Sequence::updateValues(int inputIndex)
{
	glm::mat4 parentMatrix(1.0f);

	if (m_inputs[I3T_SEQ_IN_MUL].isPluggedIn())
	{
		// Get world transform from parent sequence.
		parentMatrix = getInput(I3T_SEQ_IN_MUL).data().getMat4();
	}

	if (m_inputs[I3T_SEQ_IN_MAT].isPluggedIn())
	{
		// read matrix from input
		auto newVal = m_inputs[I3T_SEQ_IN_MAT].data().getMat4();

		setInternalValue(parentMatrix * newVal, I3T_SEQ_OUT_MUL);
		setInternalValue(newVal, I3T_SEQ_OUT_MAT);
		setInternalValue(parentMatrix * newVal, I3T_SEQ_OUT_MOD);
	}
	else
	{
		// recompute matrix product
		auto product = getMatProduct(m_storage.m_matrices);

		// Set storage value.
		setInternalValue(parentMatrix * product, I3T_SEQ_OUT_MUL);
		setInternalValue(product, I3T_SEQ_OUT_MAT);
		setInternalValue(parentMatrix * product, I3T_SEQ_OUT_MOD);
	}

	// When sequence is inside a camera.
	notifyOwner();

	m_tracker->update();
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

//===-- Helpers -----------------------------------------------------------===//
Ptr<Sequence> getNonemptyParentSequence(Ptr<Sequence> sequence)
{
	auto parent = GraphManager::getParent(sequence, I3T_SEQ_IN_MUL);
	while (parent != nullptr)
	{
		const auto parentSequence = parent->as<Sequence>();
		const bool hasMatrixInput = parentSequence->getInput(I3T_SEQ_IN_MAT).isPluggedIn();
		if (!parentSequence->getMatrices().empty() || hasMatrixInput)
		{
			return parentSequence;
		}

		parent = GraphManager::getParent(parentSequence, I3T_SEQ_IN_MUL);
	}

	return nullptr;
}
} // namespace Core
