#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

using namespace Core;

Node::PinView Node::PinView::begin() const
{
	return Node::PinView(strategy, node, 0);
}

Node::PinView Node::PinView::end() const
{
	return Node::PinView(strategy, node, size());
}

size_t Node::PinView::size() const
{
	if (strategy == Node::PinView::EStrategy::Input)
		return node->m_operation->inputTypes.size();
	return node->m_operation->outputTypes.size();
}

bool Node::PinView::empty() const
{
	return size() == 0;
}

Node::PinView& Node::PinView::operator++()
{
	index++;

	return *this;
}

const Pin& Node::PinView::operator*()
{
	return (*this)[index];
}

bool Node::PinView::operator==(const PinView& view) const
{
	return index == view.index;
}

bool Node::PinView::operator!=(const PinView& view) const
{
	return !(*this == view);
}

Pin& Node::PinView::operator[](size_t i)
{
	if (strategy == Node::PinView::EStrategy::Input)
		return node->getIn(i);
	return node->getOut(i);
}

const Pin& Node::PinView::operator[](size_t i) const
{
	if (strategy == Node::PinView::EStrategy::Input)
		return node->getIn(i);
	return node->getOut(i);
}

Node::~Node()
{
	generator.returnId(m_id);
}

void Node::finalize()
{
	unplugAll();
}

void Node::init()
{
	m_id = generator.next();

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

	// \todo MH Ugly workaround for Model, Transforms and Screen node, which has no outputs.
	// \todo MH How to create nodes which have no outputs?
	if (m_operation->numberOfOutputs == 0)
	{
		if (m_operation->keyWord == "Model") m_internalData.emplace_back(EValueType::Ptr);
		else if (!m_operation->inputTypes.empty())
			m_internalData.emplace_back(m_operation->inputTypes[0]);
		else
			m_internalData.emplace_back(EValueType::Matrix);
	}
}

ID Node::getId() const { return m_id; }

void Node::pulse(size_t index)
{
	setInternalValue(true, index);
	setInternalValue(false, index);
}

bool Node::shouldPulse(size_t inputIndex, size_t outputIndex)
{
	auto	outputPinIndex = getIn(inputIndex).getParentPin()->getIndex();
	auto& storage				 = getIn(inputIndex).getStorage(outputPinIndex);

	if (getIn(inputIndex).isPluggedIn() && storage.isPulseTriggered()) { return true; }
	return false;
}

void Node::setDataMap(const Transform::DataMap* map)
{
	// PerspectiveProj;
	auto& validMaps = getValidDataMaps();
	auto	it = std::find_if(validMaps.begin(), validMaps.end(), [&](const Transform::DataMap* m) { return m == map; });

	if (it != validMaps.end()) m_currentMap = map;
}

void Node::spreadSignal()
{
	for (auto& operatorOutput : getOutputPinsRef())
	{
		for (auto* oct : operatorOutput.getOutComponents())
		{
			// I3T_DEBUG_LOG("Spreading signal from {} to {}:{}.", getSig(), oct->m_master->getSig(), oct->getSig());
			oct->m_master->receiveSignal(oct->getIndex());
		}
	}
}

void Node::spreadSignal(size_t outIndex)
{
	if (getOutputPinsRef().empty()) return;

	auto& outputPin = getOut(outIndex);

	for (auto* inPin : outputPin.getOutComponents())
		inPin->m_master->receiveSignal(outIndex);
}

void Node::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);

	/// \todo MH this call is unnecessary, but SpreadSignalTest.ValuesShouldBeSpreadThroughConnectedNodes fails.
	// spreadSignal();
}

bool Node::areInputsPlugged(int numInputs)
{
	Debug::Assert(m_inputs.size() >= static_cast<size_t>(numInputs), "Input pins subscript is out of range!");

	bool result = true;

	for (auto i = 0; i < numInputs; ++i) { result = result && m_inputs[i].isPluggedIn(); }

	return result;
}

bool Node::areAllInputsPlugged() { return areInputsPlugged(m_operation->numberOfInputs); }

ENodePlugResult Node::isPlugCorrect(Pin const* input, Pin const* output)
{
    /* \todo JH switch input and output if output is inputPin and input is outputPin here? I have to do it anyway ...*/
	auto* inp = input;
	if (!inp) return ENodePlugResult::Err_NonexistentPin;

	auto* out = output;
	if (!out) return ENodePlugResult::Err_NonexistentPin;

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
	auto toFind = inp->getOwner(); // INPUT

	// stack in vector - TOS is at the vector back.
	std::vector<Ptr<Node>> stack;

	// PUSH(output) insert element at end.
	stack.push_back(out->getOwner());

	while (!stack.empty())
	{
		// Return last element of mutable sequence.
		auto act = stack.back();
		stack.pop_back();

		if (act == toFind) return ENodePlugResult::Err_Loop;

		for (auto& pin : act->m_inputs)
		{
			if (pin.isPluggedIn())
			{
				Pin* ct = pin.m_input;
				stack.push_back(ct->getOwner());
			}
		}
	}

	/*
	  if (isOperatorPlugCorrectMod != NULL)
	    return isOperatorPlugCorrectMod(inp, out);
	*/

	return ENodePlugResult::Ok;
}

void Node::unplugAll()
{
	for (size_t i = 0L; i < m_inputs.size(); ++i) { unplugInput(i); }

	for (size_t i = 0L; i < m_outputs.size(); ++i) { unplugOutput(i); }
}

void Node::unplugInput(size_t index)
{
	Debug::Assert(m_inputs.size() > static_cast<size_t>(index),
								"The node's input pin that you want to unplug does not exists.");

	// auto* otherPin = m_inputs[index].m_input;
	auto	inputs	 = getInputPinsRef();
	auto* otherPin = inputs[index].m_input;

	if (otherPin)
	{
		// Erase pointer to my input pin in connected node outputs.
		auto& otherPinOutputs = otherPin->m_outputs;

		auto it = std::find(otherPinOutputs.begin(), otherPinOutputs.end(), &inputs[index]);
		if (it != otherPinOutputs.end())
		{
			/// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
			otherPinOutputs.erase(it);
		}
		else
			I3T_ABORT("Can't find pointer to input pin in other node outputs.");

		auto& myPin		= inputs[index];
		myPin.m_input = nullptr;
	}
}

void Node::unplugOutput(size_t index)
{
	Debug::Assert(m_outputs.size() > static_cast<size_t>(index),
								"The node's output pin that you want to unplug does not exists.");

	// auto& pin = m_outputs[index];
	auto& pin = getOut(index);

	// Set all connected nodes input as nullptr.
	for (const auto& otherPin : pin.m_outputs) otherPin->m_input = nullptr;

	pin.m_outputs.clear();
}
