#include "Sequence.h"

#include "GraphManager.h"

using namespace Core;

void SequenceInternals::Storage::updateValues(int inputIndex)
{
  auto mat = getMatProduct(m_matrices);

  if (getInPin(0).isPluggedIn())
  {
		// Matrix input
    mat = getInPinRef(0).getStorage(0).getMat4();

		/*
		glm::mat4 mult(1.0f);

    auto parent = GraphManager::getParent(getPtr());
    if (parent)
      mult = parent->getData().getMat4();
      */
    //setInternalValue(mat, 1);
    //setInternalValue(mult * mat, 2);
	}
  setInternalValue(mat, 0);
  m_owner->as<Sequence>()->m_multiplier->updateValues(-1);
}

ValueSetResult SequenceInternals::Storage::addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
{
  GraphManager::unplugAll(matrix);

  index = index > m_matrices.size() ? m_matrices.size() : index;
  m_matrices.insert(m_matrices.begin() + index, matrix);

  matrix->as<Transformation>()->setSequence(m_owner, index);

  updateValues(-1);
	m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	// spreadSignal();

	// If sequence is sub-node of camera node.
  m_owner->as<Sequence>()->notifyParent();

  return ValueSetResult{};
}

Ptr<Transformation> SequenceInternals::Storage::popMatrix(const int index)
{
  Debug::Assert(m_matrices.size() > static_cast<size_t>(index),
                "Sequence does not have so many matrices as you are expecting.");

  auto result = std::move(m_matrices.at(index));
  m_matrices.erase(m_matrices.begin() + index);

  result->nullSequence();

  updateValues(-1);
  m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

	// spreadSignal();

  m_owner->as<Sequence>()->notifyParent();

  return result;
}

void SequenceInternals::Storage::swap(int from, int to)
{
  if (from > m_matrices.size() || to > m_matrices.size()) return;

  updateValues(-1);
  m_owner->as<Sequence>()->m_multiplier->updateValues(-1);

  // spreadSignal();

  m_owner->as<Sequence>()->notifyParent();

  std::swap(m_matrices[from], m_matrices[to]);
}

void SequenceInternals::Multiplier::updateValues(int inputIndex)
{
	auto product = m_owner->as<Sequence>()->m_storage->getData().getMat4();
  glm::mat4 mult(1.0f);

  if (m_owner->getInPin(0).isPluggedIn())
  {
    // auto mat = getMatProduct(m_owner->as<Sequence>()->getMatrices());
    auto parent = GraphManager::getParent(m_owner)->as<Sequence>();
    mult = parent->getData().getMat4();

    // setInternalValue(mat, 0);
    //setInternalValue(mat, 1);
    //setInternalValue(mult * mat, 2);
	}
	// Mul. output
	setInternalValue(mult * product, 0);

	// Model matrix
	setInternalValue(mult * product, 1);
}


Sequence::Sequence() : NodeBase(&g_sequence)
{
}

void Sequence::createComponents()
{
	m_storage = std::make_shared<SequenceInternals::Storage>();
	m_multiplier = std::make_shared<SequenceInternals::Multiplier>();

	m_storage->m_owner = getPtr();
  m_multiplier->m_owner = getPtr();

	m_storage->m_inputs.emplace_back(m_inputs[1]);
  setPinOwner(m_storage->getInPinRef(0), m_storage);

  m_storage->m_outputs.emplace_back(m_outputs[1]);
  setPinOwner(m_storage->getOutPinRef(0), m_storage);

  m_storage->m_internalData.emplace_back(EValueType::Matrix);


	m_multiplier->m_inputs.emplace_back(m_inputs[0]);
  setPinOwner(m_multiplier->getInPinRef(0), m_multiplier);

  m_multiplier->m_outputs.emplace_back(m_outputs[0]);
  setPinOwner(m_multiplier->getOutPinRef(0), m_multiplier);

  m_multiplier->m_outputs.emplace_back(m_outputs[2]);
  setPinOwner(m_multiplier->getOutPinRef(1), m_multiplier);

  m_multiplier->m_internalData.emplace_back(EValueType::MatrixMul);
  m_multiplier->m_internalData.emplace_back(EValueType::Matrix);

	m_inputs[0].m_master = m_multiplier;
	m_inputs[1].m_master = m_storage;

	m_outputs[0].m_master = m_multiplier;
	m_outputs[1].m_master = m_storage;
	m_outputs[2].m_master = m_multiplier;
}

DataStore& Sequence::getInternalData(size_t index)
{
	/*
  if (index == 0)
		return m_storage->getInternalData(index);
	else
		return m_multiplier->getInternalData(index - 1);
	 */
	if (index == 0)
  {
		return m_multiplier->getInternalData(0);
	}
	else if (index == 1)
  {
		return m_storage->getInternalData(0);
	}
	else if (index == 2)
  {
    return m_multiplier->getInternalData(1);
  }
}

void Sequence::updatePins()
{
  m_storage->m_inputs[0].m_input = m_inputs[1].m_input;
  m_multiplier->m_inputs[0].m_input = m_inputs[0].m_input;


	m_storage->m_outputs[0].m_outputs = m_outputs[1].m_outputs;
	resetInputPin(m_storage->m_outputs[0].m_outputs, &m_storage->m_outputs[0]);

	m_multiplier->m_outputs[0].m_outputs = m_outputs[0].m_outputs;
  resetInputPin(m_multiplier->m_outputs[0].m_outputs, &m_multiplier->m_outputs[0]);

	m_multiplier->m_outputs[1].m_outputs = m_outputs[2].m_outputs;
  resetInputPin(m_multiplier->m_outputs[1].m_outputs, &m_multiplier->m_outputs[1]);
}

void Sequence::resetInputPin(std::vector<Pin*>& outputsOfPin, Pin* newInput)
{
  std::for_each(outputsOfPin.begin(), outputsOfPin.end(), [newInput](Pin* p)
	              {
									p->m_input = newInput;
								});
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
