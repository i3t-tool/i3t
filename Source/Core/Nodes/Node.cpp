#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/LoggerInternal.h"

using namespace Core;

NodeBase::~NodeBase()
{
	unplugAll();
}

void NodeBase::init()
{
	m_id = IdGenerator::next();

	// Create input pins.
	for (int i = 0; i < m_operation->numberOfInputs; i++)
	{
		m_inputs.emplace_back(m_operation->inputTypes[i], true, getPtr(), i);
	}

	// Create output pins and data storage for each output.
	for (int i = 0; i < m_operation->numberOfOutputs; i++)
	{
		m_outputs.emplace_back(m_operation->outputTypes[i], false, getPtr(), i);
		m_internalData.emplace_back(m_operation->outputTypes[i]);
	}

	// \todo MH Ugly workaround for Model and Screen node, which has no outputs.
	if (m_operation->numberOfOutputs == 0)
	{
		m_internalData.emplace_back(m_operation->inputTypes[0]);
	}
}

ID NodeBase::getId() const
{
  return m_id;
}

void NodeBase::setPinOwner(Pin& pin, Ptr<NodeBase> node)
{
	pin.m_master = node;
}

void NodeBase::setDataMap(const Transform::DataMap* map)
{
  // PerspectiveProj;
	auto& validMaps = getValidDataMaps();
	auto it = std::find_if(validMaps.begin(), validMaps.end(),
      [&](const Transform::DataMap* m) { return m == map; });

	if (it != validMaps.end())
	  m_currentMap = map;
}

const std::vector<Pin>& NodeBase::getInputPins()
{
	return getInputPinsRef();
}

const std::vector<Pin>& NodeBase::getOutputPins()
{
	return getOutputPinsRef();
}

std::vector<Pin>& NodeBase::getInputPinsRef()
{
  return m_inputs;
}

std::vector<Pin>& NodeBase::getOutputPinsRef()
{
	return m_outputs;
}

void NodeBase::spreadSignal()
{
	for (auto& operatorOutput : m_outputs)
	{
		for (auto* oct : operatorOutput.getOutComponents())
		{
			oct->m_master->receiveSignal(oct->getIndex());
		}
	}
}

void NodeBase::spreadSignal(int outIndex)
{
	for (auto* oct : m_outputs[outIndex].getOutComponents())
	{
		oct->m_master->receiveSignal(oct->getIndex());
	}
}

void NodeBase::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);

  spreadSignal();
}

bool NodeBase::areInputsPlugged(int numInputs)
{
	Debug::Assert(m_inputs.size() >= static_cast<size_t>(numInputs), "Input pins subscript is out of range!");

	bool result = true;

	for (auto i = 0; i < numInputs; ++i)
	{
		result = result && m_inputs[i].isPluggedIn();
	}

	return result;
}

bool NodeBase::areAllInputsPlugged()
{
	return areInputsPlugged(m_operation->numberOfInputs);
}

ENodePlugResult NodeBase::isPlugCorrect(Pin const * input, Pin const * output)
{
  auto* inp = input;
  if (!inp)
    return ENodePlugResult::Err_NonexistentPin;

  auto* out = output;
  if (!out)
    return ENodePlugResult::Err_NonexistentPin;

  if (inp->m_valueType != out->m_valueType)
  {
    // Do the input and output data types match?
    return ENodePlugResult::Err_MismatchedPinTypes;
  }

  if (inp->m_isInput == out->m_isInput)
  {
    // Do the input and output kind match?
    return ENodePlugResult::Err_MismatchedPinKind;
  }

  if (inp->m_master == out->m_master)
  {
    // Not a circular edge?
    return ENodePlugResult::Err_Loopback;
  }

  // cycle detector
  auto toFind = inp->m_master; // INPUT

  // stack in vector - TOS is at the vector back.
  std::vector<Ptr<NodeBase>> stack;

  // PUSH(output) insert element at end.
  stack.push_back(out->m_master);

  while (!stack.empty())
  {
    // Return last element of mutable sequence.
    auto act = stack.back();
    stack.pop_back();

    if (act == toFind)
      return ENodePlugResult::Err_Loop;

    for (auto& pin : act->m_inputs)
    {
      if (pin.isPluggedIn())
      {
        Pin* ct = pin.m_input;
        stack.push_back(ct->m_master);
      }
    }
  }

  /*
    if (isOperatorPlugCorrectMod != NULL)
      return isOperatorPlugCorrectMod(inp, out);
  */

  return ENodePlugResult::Ok;
}

void NodeBase::unplugAll()
{
	for (size_t i = 0; i < m_inputs.size(); ++i)
	{
		unplugInput(i);
	}

	for (size_t i = 0; i < m_outputs.size(); ++i)
	{
		unplugOutput(i);
	}
}

void NodeBase::unplugInput(int index)
{
	Debug::Assert(m_inputs.size() > static_cast<size_t>(index),
	              "The node's input pin that you want to unplug does not exists.");

	auto* otherPin = m_inputs[index].m_input;

	if (otherPin)
	{
		// Erase pointer to my input pin in connected node outputs.
		auto& otherPinOutputs = otherPin->m_outputs;

		auto it = std::find(otherPinOutputs.begin(), otherPinOutputs.end(), &m_inputs[index]);
		if (it != otherPinOutputs.end())
		{
			/// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
			otherPinOutputs.erase(it);
		}
		else
		{
			Debug::Assert(false, "Can't find pointer to input pin in other node outputs.");
		}

		auto& myPin = m_inputs[index];
		myPin.m_input = nullptr;
	}
}

void NodeBase::unplugOutput(int index)
{
	Debug::Assert(m_outputs.size() > static_cast<size_t>(index),
	              "The node's output pin that you want to unplug does not exists.");

	auto& pin = m_outputs[index];

	// Set all connected nodes input as nullptr.
	for (const auto& otherPin : pin.m_outputs)
		otherPin->m_input = nullptr;

	pin.m_outputs.clear();
}

const DataStore& Pin::getStorage(unsigned int id)
{
  if (m_isInput)
  {
    // Debug::Assert(isPluggedIn(), "This input pin is not plugged to any output pin!");
    return m_input->m_master->getData(id);
  }
  else
  {
    return m_master->getData(id);
  }
}
