/**
 * \file Core/Nodes/Sequence
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 20.12.2020
 */
#pragma once

#include "Node.h"

namespace Core
{
/**
 * Sequence of matrices.
 */
class Sequence : public NodeBase
{
	using Matrix = NodeBase;

	std::vector<Ptr<Matrix>> m_matrices;

public:
	Sequence() : NodeBase(&g_sequence){};

	void addMatrix(Ptr<Matrix> matrix) noexcept { addMatrix(matrix, m_matrices.size()); }

	/**
	 * Pass matrix to a sequence. Sequence takes ownership of matrix.
	 *
	 * \param matrix Matrix to transfer.
	 * \param index New position of matrix.
	 */
	void addMatrix(Ptr<Matrix> matrix, size_t index) noexcept;;

	std::vector<Ptr<Matrix>>& getMatrices() { return m_matrices; }

	/**
	 * \brief Get reference to matrix in a sequence at given position.
	 *
	 * <b>Be careful</b> not to access matrix via invalid reference after
	 * calling popMatrix.
	 *
	 * \param idx Index of matrix.
	 * \return Reference to matrix holt in m_matrices vector.
	 */
	[[nodiscard]] Ptr<Matrix>& getMatRef(size_t idx) { return m_matrices.at(idx); }

	/**
	 * Pop matrix from a sequence. Caller takes ownership of returned matrix.
	 */
	[[nodiscard]] Ptr<Matrix> popMatrix(const int index)
	{
		Debug::Assert(m_matrices.size() > static_cast<size_t>(index),
		              "Sequence does not have so many matrices as you are expecting.");

		auto result = std::move(m_matrices.at(index));
		m_matrices.erase(m_matrices.begin() + index);

    updateValues(0);
    spreadSignal();

		return result;
	};

	void updateValues(int inputIndex) override;

private:
	ENodePlugResult isPlugCorrect(Pin* input, Pin* output) override;
  void receiveSignal(int inputIndex) override;
};

FORCE_INLINE Ptr<Sequence> toSequence(Ptr<NodeBase> node)
{
	if (node == nullptr)
		return nullptr;

	Debug::Assert(node->getOperation()->keyWord == g_sequence.keyWord, "Given node is not a sequence!");
	return std::dynamic_pointer_cast<Sequence>(node);
}
} // namespace Core
