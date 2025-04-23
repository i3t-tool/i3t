/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 20.12.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Tracking.h"
#include "Transform.h"

namespace Core
{
using Matrices = std::vector<Ptr<Transform>>;

constexpr size_t I3T_SEQ_IN_MUL = 0; // owned by multiplier
constexpr size_t I3T_SEQ_IN_MAT = 1; // owned by storage

constexpr size_t I3T_SEQ_OUT_MUL = 0; // owned by multiplier
constexpr size_t I3T_SEQ_OUT_MAT = 1; // owned by storage, local transform
constexpr size_t I3T_SEQ_OUT_MOD = 2; // owned by multiplier, world transform

// FIXME: Couldn't the OUT vars be used instead? Do pin indices ever not match internal data?
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
	friend class MatrixTracker;

	using Matrix = Node;

	friend class Storage;

	/** Structure for storing transform matrices. */
	class Storage
	{
		friend class Core::Sequence;

	public:
		Storage(Sequence& sequence) : m_sequence(sequence) {}

		SetValueResult addMatrix(Ptr<Transform> matrix) noexcept
		{
			return addMatrix(matrix, 0);
		};
		SetValueResult addMatrix(Ptr<Transform> matrix, size_t index) noexcept;
		Ptr<Transform> popMatrix(const int index);
		void swap(int from, int to);

	private:
		std::vector<Ptr<Transform>> m_matrices;

		Sequence& m_sequence;
	};

public:
	Sequence();

	/**
	 * Push \p matrix to the end of the sequence.
	 * @param matrix
	 * @return
	 */
	SetValueResult pushMatrix(Ptr<Transform> matrix) noexcept;

	/**
	 * Pass matrix to a sequence. Sequence takes ownership of matrix.
	 *
	 * \param matrix Matrix to transfer.
	 * \param index New position of matrix.
	 */
	SetValueResult pushMatrix(Ptr<Transform> matrix, size_t index) noexcept;

	const Matrices& getMatrices() const
	{
		return m_storage.m_matrices;
	}

	/**
	 * \brief Get reference to matrix in a sequence at given position.
	 *
	 * <b>Be careful</b> not to access matrix via invalid reference after
	 * calling popMatrix.
	 *
	 * \param idx Index of matrix.
	 * \return Reference to matrix holt in m_matrices vector.
	 */
	[[nodiscard]] Ptr<Transform>& getMatRef(size_t idx)
	{
		return m_storage.m_matrices.at(idx);
	}

	/**
	 * Pop matrix from a sequence. Caller takes ownership of returned matrix.
	 */
	[[nodiscard]] Ptr<Transform> popMatrix(const int index);

	void swap(int from, int to);

	void updateValues(int inputIndex) override;

	/// Whether the sequence holds no matrix data (no transformations and isn't connected externally)
	bool isEmpty();

private:
	Storage m_storage;
};

FORCE_INLINE glm::mat4 getMatProduct(const std::vector<Ptr<Transform>>& matrices)
{
	glm::mat4 result(1.0f);
	for (const auto& mat : matrices)
		result *= mat->data().getMat4();
	return result;
}
} // namespace Core
