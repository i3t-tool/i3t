/**
 * \file Core/Nodes/Sequence
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 20.12.2020
 */
#pragma once

#include "Transform.h"

namespace Core
{
/**
 * Sequence of matrices.
 */
class Sequence : public NodeBase
{
	using Matrix = NodeBase;

	std::vector<Ptr<Transformation>> m_matrices;

public:
	Sequence() : NodeBase(&g_sequence){};

	ValueSetResult addMatrix(Ptr<Transformation> matrix) noexcept { return addMatrix(matrix, m_matrices.size()); }

	/**
	 * Pass matrix to a sequence. Sequence takes ownership of matrix.
	 *
	 * \param matrix Matrix to transfer.
	 * \param index New position of matrix.
	 */
  ValueSetResult addMatrix(Ptr<Transformation> matrix, size_t index) noexcept;

	const std::vector<Ptr<Transformation>>& getMatrices() { return m_matrices; }

	/**
	 * \brief Get reference to matrix in a sequence at given position.
	 *
	 * <b>Be careful</b> not to access matrix via invalid reference after
	 * calling popMatrix.
	 *
	 * \param idx Index of matrix.
	 * \return Reference to matrix holt in m_matrices vector.
	 */
	[[nodiscard]] Ptr<Transformation>& getMatRef(size_t idx) { return m_matrices.at(idx); }

	/**
	 * Pop matrix from a sequence. Caller takes ownership of returned matrix.
	 */
	[[nodiscard]] Ptr<Transformation> popMatrix(const int index)
	{
		Debug::Assert(m_matrices.size() > static_cast<size_t>(index),
		              "Sequence does not have so many matrices as you are expecting.");

		auto result = std::move(m_matrices.at(index));
		m_matrices.erase(m_matrices.begin() + index);

		result->nullSequence();

    updateValues(0);
    spreadSignal();

		return result;
	};

	void swap(int from, int to);

	void updateValues(int inputIndex) override;

private:
	ENodePlugResult isPlugCorrect(Pin* input, Pin* output) override;
  void receiveSignal(int inputIndex) override;
};

FORCE_INLINE Ptr<Sequence> toSequence(Ptr<NodeBase> node)
{
	if (node == nullptr)
		return nullptr;
	return node->as<Sequence>();
}
} // namespace Core
