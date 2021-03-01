#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/LoggerInternal.h"

using namespace Core;

NodeBase::NodeBase(const Operation* operation)
		: m_operation(operation), m_pulseOnPlug(true), m_restrictedOutput(false), m_restrictedOutputIndex(0)
{
	m_id = IdGenerator::next();

	// Create input pins.
	for (int i = 0; i < m_operation->numberOfInputs; i++)
	{
		m_inputs.emplace_back(m_operation->inputTypes[i], true, this, i);
	}

	// Create output pins and data storage for each output.
	for (int i = 0; i < m_operation->numberOfOutputs; i++)
	{
		m_outputs.emplace_back(m_operation->outputTypes[i], false, this, i);
		m_internalData.emplace_back();
	}
}

NodeBase::~NodeBase()
{
	unplugAll();
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

void NodeBase::unplugAll()
{
	for (auto i = 0; i < m_inputs.size(); ++i)
	{
		unplugInput(i);
	}

	for (auto i = 0; i < m_outputs.size(); ++i)
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
