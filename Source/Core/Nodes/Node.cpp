#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/LoggerInternal.h"

using namespace Core;

NodeBase::~NodeBase()
{
	unplugAll();
}

void NodeBase::create()
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
		m_internalData.emplace_back();
	}

	// Ugly workaround for Model node, which has no outputs.
	if (m_operation->numberOfOutputs == 0)
	{
		m_internalData.emplace_back();
	}
}

const std::vector<Pin>& NodeBase::getInputPins() const
{
	return m_inputs;
}

const std::vector<Pin>& NodeBase::getOutputPins() const
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
		// for each wire connected to the outIndex output (for each OperatorCurveTab oct):
		oct->m_master->receiveSignal(oct->getIndex());
	}
}

void NodeBase::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);

	if (m_restrictedOutput)
		spreadSignal(m_restrictedOutputIndex);
	else
		spreadSignal();
}

bool NodeBase::arePluggedIn(int numInputs)
{
	Debug::Assert(m_inputs.size() <= static_cast<size_t>(numInputs), "Input pins subscript is out of range!");

	bool result = true;

	for (size_t i = 0; i < numInputs; ++i)
  {
		result = result && m_inputs[i].isPluggedIn();
	}

	return result;
}

bool NodeBase::areAllPluggedIn()
{
	return arePluggedIn(m_operation->numberOfInputs);
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
	Debug::Assert(m_inputs.size() > index, "The node's input pin that you want to unplug does not exists.");

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
	Debug::Assert(m_outputs.size() > index, "The node's output pin that you want to unplug does not exists.");

	auto& pin = m_outputs[index];

	// Set all connected nodes input as nullptr.
	for (const auto& otherPin : pin.m_outputs)
		otherPin->m_input = nullptr;

	pin.m_outputs.clear();
}

ID NodeBase::getId() const
{
	return m_id;
}
