/**
 * \file Core/Nodes/Sequence
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 20.12.2020
 */
#pragma once

#include "Transform.h"

namespace Core
{
using Matrices = std::vector<Ptr<Transformation>>;

namespace SequenceInternals
{
class Storage : public Node
{
  friend class Core::Sequence;

public:
  Storage() : Node(nullptr) {}

	void updateValues(int inputIndex) override;
};


class Multiplier : public Node
{
  friend class Core::Sequence;

public:
	Multiplier() : Node(nullptr) {}

	void updateValues(int inputIndex) override;
};
}


/**
 * Sequence of matrices.
 */
class Sequence : public NodeBase
{
	friend class GraphManager;
	using Matrix = NodeBase;

  Matrices m_matrices;
	NodePtr m_parent = nullptr; ///< Node which owns the sequence.

  Ptr<SequenceInternals::Storage> m_storage;
  Ptr<SequenceInternals::Multiplier> m_multiplier;

public:
	Sequence();

	ValueSetResult addMatrix(Ptr<Transformation> matrix) noexcept { return addMatrix(matrix, m_matrices.size()); }

	/**
	 * Pass matrix to a sequence. Sequence takes ownership of matrix.
	 *
	 * \param matrix Matrix to transfer.
	 * \param index New position of matrix.
	 */
  ValueSetResult addMatrix(Ptr<Transformation> matrix, size_t index) noexcept;

	void createComponents();

  DataStore& getInternalData(size_t index = 0) override;

	const Matrices& getMatrices() { return m_matrices; }

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
	[[nodiscard]] Ptr<Transformation> popMatrix(const int index);

	void swap(int from, int to);

	/**
	 * Keep storage and multipliers pins at same state as sequences pins are.
	 */
	void updatePins();
	void resetInputPin(std::vector<Pin*>& outputsOfPin, Pin* newInput);

	void updateValues(int inputIndex) override;

private:
	void notifyParent();
  void receiveSignal(int inputIndex) override;
};

FORCE_INLINE Ptr<Sequence> toSequence(Ptr<NodeBase> node)
{
	if (node == nullptr)
		return nullptr;
	return node->as<Sequence>();
}

FORCE_INLINE glm::mat4 getMatProduct(const std::vector<Ptr<Transformation>>& matrices)
{
  glm::mat4 result(1.0f);
  for (const auto& mat : matrices)
    result *= mat->getData().getMat4();
  return result;
}

using SequencePtr = Ptr<Sequence>;

FORCE_INLINE bool isSequence(const NodePtr& p)
{
	return p->getOperation() == &g_sequence;
}
} // namespace Core
