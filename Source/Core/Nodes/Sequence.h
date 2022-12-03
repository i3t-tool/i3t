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

constexpr size_t I3T_SEQ_IN_MUL = 0; // owned by multiplier
constexpr size_t I3T_SEQ_IN_MAT = 1; // owned by storage

constexpr size_t I3T_SEQ_OUT_MUL = 0; // owned by multiplier
constexpr size_t I3T_SEQ_OUT_MAT = 1; // owned by storage
constexpr size_t I3T_SEQ_OUT_MOD = 2; // owned by multiplier

constexpr size_t I3T_SEQ_MUL = 0;
constexpr size_t I3T_SEQ_MAT = 1; // local transform
constexpr size_t I3T_SEQ_MOD = 2; // world transform

class Sequence;

namespace Builder
{
Ptr<Sequence> createSequence();
} // namespace Builder

/**
 * Sequence of matrices.
 *
 * +---------------------------+
 * |               <M> 0 mul   |
 * | 0 mul <M>     <S> 1 mat   |
 * | 1 mat <S>     <M> 2 model |
 * +---------------------------+
 */
class Sequence : public Node
{
	friend class GraphManager;

	using Matrix = NodeBase;
	using SequencePins = std::vector<Pin>;

	friend class Multiplier;
	friend class Storage;

	/** Structure for storing transform matrices. */
	class Storage : public Node
	{
		friend class Core::Sequence;
		friend class Multiplier;

		Matrices m_matrices;

	public:
		Storage();

		Ptr<Node> clone() override
		{
			I3T_ABORT("Don't clone this class!");

			return nullptr;
		}

		Pin& getIn(size_t i) override;
		Pin& getOut(size_t i) override;
		DataStore& getInternalData(size_t index = 0) override;

		ValueSetResult addMatrix(Ptr<Transformation> matrix) noexcept { return addMatrix(matrix, 0); };
		ValueSetResult addMatrix(Ptr<Transformation> matrix, size_t index) noexcept;
		Ptr<Transformation> popMatrix(const int index);
		void swap(int from, int to);

		/**
		 * Updates local transform.
		 * @param inputIndex
		 */
		void updateValues(int inputIndex) override;
	};

	/** Structure which represents sequences multiplication. */
	class Multiplier : public Node
	{
		friend class Core::Sequence;

	public:
		Multiplier();

		Ptr<Node> clone() override
		{
			I3T_ABORT("Don't clone this class!");

			return nullptr;
		}

		Pin& getIn(size_t i) override;
		Pin& getOut(size_t i) override;
		DataStore& getInternalData(size_t index = 0) override;

		/**
		 * Updates mul. output and world transform.
		 * @param inputIndex
		 */
		void updateValues(int inputIndex) override;
	};

	Ptr<Storage> m_storage;
	Ptr<Multiplier> m_multiplier;

public:
	Sequence();

	Ptr<Node> clone() override;

	Pin& getIn(size_t i) override;
	Pin& getOut(size_t i) override;

	void createComponents();

	ValueSetResult addMatrix(Ptr<Transformation> matrix) noexcept
	{
		return addMatrix(matrix, m_storage->m_matrices.size());
	}

	/**
	 * Pass matrix to a sequence. Sequence takes ownership of matrix.
	 *
	 * \param matrix Matrix to transfer.
	 * \param index New position of matrix.
	 */
	ValueSetResult addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
	{
		return m_storage->addMatrix(matrix, index);
	}

	DataStore& getInternalData(size_t index = 0) override;

	const Matrices& getMatrices() const { return m_storage->m_matrices; }

	/**
	 * \brief Get reference to matrix in a sequence at given position.
	 *
	 * <b>Be careful</b> not to access matrix via invalid reference after
	 * calling popMatrix.
	 *
	 * \param idx Index of matrix.
	 * \return Reference to matrix holt in m_matrices vector.
	 */
	[[nodiscard]] Ptr<Transformation>& getMatRef(size_t idx) { return m_storage->m_matrices.at(idx); }

	/**
	 * Pop matrix from a sequence. Caller takes ownership of returned matrix.
	 */
	[[nodiscard]] Ptr<Transformation> popMatrix(const int index) { return m_storage->popMatrix(index); }

	void swap(int from, int to) { return m_storage->swap(from, to); }

	void updateValues(int inputIndex) override;

private:
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
	auto* op = p->getOperation();
	auto* expected = &g_sequence;
	return p->getOperation() == &g_sequence;
}
} // namespace Core
