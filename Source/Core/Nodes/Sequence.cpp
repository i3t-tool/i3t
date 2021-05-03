#include "Sequence.h"

#include "GraphManager.h"

using namespace Core;

void SequenceInternals::Storage::updateValues(int inputIndex)
{
	if (m_owner->getInPin(1).isPluggedIn())
  {
		// Matrix input
		m_owner->updateValues();
	}
}

void SequenceInternals::Multiplier::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {

	}
}

Sequence::Sequence() : NodeBase(&g_sequence)
{
}

ValueSetResult Sequence::addMatrix(Ptr<Transformation> matrix, size_t index) noexcept
{
	GraphManager::unplugAll(matrix);

	index = index > m_matrices.size() ? m_matrices.size() : index;
  m_matrices.insert(m_matrices.begin() + index, matrix);

  matrix->as<Transformation>()->setSequence(getPtr(), index);

  updateValues(0);
	spreadSignal();
  notifyParent();

	return ValueSetResult{};
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
  if (index == 0)
		return m_storage->getInternalData(index);
	else
		return m_multiplier->getInternalData(index - 1);
}

Ptr<Transformation> Sequence::popMatrix(const int index)
{
  Debug::Assert(m_matrices.size() > static_cast<size_t>(index),
                "Sequence does not have so many matrices as you are expecting.");

  auto result = std::move(m_matrices.at(index));
  m_matrices.erase(m_matrices.begin() + index);

  result->nullSequence();

  updateValues(0);
  spreadSignal();
  notifyParent();

  return result;
}

void Sequence::swap(int from, int to)
{
  if (from > m_matrices.size() || to > m_matrices.size()) return;

  updateValues(0);
  spreadSignal();
  notifyParent();

  std::swap(m_matrices[from], m_matrices[to]);
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
	if (getInPin(0).isPluggedIn() && !getInPin(1).isPluggedIn())
  {
		auto r = getMatProduct(getMatrices());
    auto parent = GraphManager::getParent(getPtr())->as<Sequence>();

		/// \todo MH output index information gets lost when storage and multiplier are combined.
		setInternalValue(r, 0);
    setInternalValue(r, 1);
		setInternalValue(parent->getData().getMat4() * r, 2);
  }
	if (getInPin(1).isPluggedIn())
  {
		auto forcedMat = getInPinRef(1).getStorage(0).getMat4();

		glm::mat4 parentMat(1.0f);

		auto parent = GraphManager::getParent(getPtr());
		if (parent)
      parentMat = parent->getData().getMat4();

		setInternalValue(forcedMat, 0);
		setInternalValue(forcedMat, 1);
    setInternalValue(parentMat * forcedMat, 2);
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
