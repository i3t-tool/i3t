/**
 * \file Core/Nodes/Sequence
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 20.12.2020
 */
#pragma once

#include "Core/Node.h"

namespace Core
{
/**
 * Sequence of matrices.
 */
class Sequence : public NodeBase
{
  using Matrix = NodeBase;

  std::vector<UPtr<Matrix>> m_matrices;

public:
  Sequence() : NodeBase(&operations[ENodeType::Sequence])
  {
    m_inputs.emplace_back(OpValueType::MATRIX, true, this); // mul. input
    m_inputs.emplace_back(OpValueType::MATRIX, true, this); // direct mat. input

    m_outputs.emplace_back(OpValueType::MATRIX, false, this); // mul. output
    m_outputs.emplace_back(OpValueType::MATRIX, false, this); // direct mat. output

    m_internalData.emplace_back();
    m_internalData.emplace_back();

    updateValues(0);
  };

  /**
   * Pass matrix to a sequence. Sequence takes ownership of matrix.
   *
   * \param matrix Matrix to transfer.
   * \param index New position of matrix.
   */
  FORCE_INLINE void addMatrix(UPtr<Matrix> matrix, const int index) noexcept
  {
    if (index > m_matrices.size())
      m_matrices.push_back(std::move(matrix));
    else
      m_matrices.insert(m_matrices.begin() + index, std::move(matrix));

    receiveSignal(0);
  };

  /**
   * Pop matrix from a sequence. Caller takes ownership of returned matrix.
   */
  [[nodiscard]] UPtr<Matrix> popMatrix(const int index)
  {
    I3T_DEBUG_ASSERT(m_matrices.size() > index, "Sequence does not have to many matrices.");

    return std::move(*m_matrices.erase(m_matrices.begin() + index));
  };

  void updateValues(int inputIndex) override
  {
    glm::mat4 result(1.0f);

    if (m_inputs[1].isPluggedIn())
    {
      // Matrix node is connected to direct matrix input.
      result = m_inputs[1].getStorage().getMat4();
    }
    else
    {
      for (const auto& mat : m_matrices)
      {
        result *= mat->getInternalData().getMat4();
      }
    }

    m_internalData[0].setValue(result);
    m_internalData[1].setValue(result);
  };
};
}


