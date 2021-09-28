#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/NodeVisitor.h"
#include "Logger/Logger.h"
#include "Logger/LoggerInternal.h"

using namespace Core;

static IdGenerator generator;

NodeBase::PinView::Iterator::Iterator(Ptr<Strategy> strategy, Ptr<NodeBase> node, int index)
{
	m_strategy = strategy;
	m_node		 = node;
	m_index		 = index;
}

NodeBase::PinView::reference NodeBase::PinView::Iterator::operator*() const { return m_strategy->get(m_index); }

NodeBase::PinView::pointer NodeBase::PinView::Iterator::operator->() { return &m_strategy->get(m_index); }

NodeBase::PinView::Iterator& NodeBase::PinView::Iterator::operator++()
{
	m_index++;
	return *this;
}

NodeBase::PinView::Iterator NodeBase::PinView::Iterator::operator++(int)
{
	NodeBase::PinView::Iterator tmp = *this;
	++(*this);
	return tmp;
}

Pin& NodeBase::PinView::operator[](size_t i) const { return m_strategy->get(i); }

NodeBase::PinView::Iterator NodeBase::PinView::begin() const { return m_strategy->begin(); }

NodeBase::PinView::Iterator NodeBase::PinView::end() const { return m_strategy->end(); }

bool NodeBase::PinView::empty() const { return m_strategy->empty(); }

size_t NodeBase::PinView::size() const { return m_strategy->size(); }


Node::PinView::Iterator Node::InputStrategy::begin()
{
	if (empty()) { return NodeBase::PinView::Iterator(getThis(), m_node, -1); }
	return NodeBase::PinView::Iterator(getThis(), m_node, 0);
}

Node::PinView::Iterator Node::InputStrategy::end()
{
	if (m_node->m_operation->inputTypes.empty()) { return NodeBase::PinView::Iterator(getThis(), m_node, -1); }
	return NodeBase::PinView::Iterator(getThis(), m_node, static_cast<int>(size()));
}

bool Node::InputStrategy::empty() { return m_node->m_operation->inputTypes.empty(); }

size_t Node::InputStrategy::size() { return m_node->m_operation->inputTypes.size(); }

Pin& Node::InputStrategy::get(size_t index) { return m_node->getIn(index); }


Node::PinView::Iterator Node::OutputStrategy::begin()
{
	if (empty()) { return NodeBase::PinView::Iterator(getThis(), m_node, -1); }
	return NodeBase::PinView::Iterator(getThis(), m_node, 0);
}

Node::PinView::Iterator Node::OutputStrategy::end()
{
	if (empty()) { return NodeBase::PinView::Iterator(getThis(), m_node, -1); }
	return NodeBase::PinView::Iterator(getThis(), m_node, static_cast<int>(size()));
}

bool Node::OutputStrategy::empty() { return m_node->m_operation->outputTypes.empty(); }

size_t Node::OutputStrategy::size() { return m_node->m_operation->outputTypes.size(); }

Pin& Node::OutputStrategy::get(size_t index) { return m_node->getOut(index); }


NodeBase::~NodeBase()
{
	generator.returnId(m_id);
}

void NodeBase::finalize()
{
	unplugAll();
}

void NodeBase::init()
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

ID NodeBase::getId() const { return m_id; }

void NodeBase::pulse(size_t index)
{
	setInternalValue(true, index);
	setInternalValue(false, index);
}

bool NodeBase::shouldPulse(size_t inputIndex, size_t outputIndex)
{
	auto	outputPinIndex = getIn(inputIndex).getParentPin()->getIndex();
	auto& storage				 = getIn(inputIndex).getStorage(outputPinIndex);

	if (getIn(inputIndex).isPluggedIn() && storage.isPulseTriggered()) { return true; }
	return false;
}

void NodeBase::setDataMap(const Transform::DataMap* map)
{
	// PerspectiveProj;
	auto& validMaps = getValidDataMaps();
	auto	it = std::find_if(validMaps.begin(), validMaps.end(), [&](const Transform::DataMap* m) { return m == map; });

	if (it != validMaps.end()) m_currentMap = map;
}

const NodeBase::PinView NodeBase::getInputPins()
{
	return PinView(std::make_shared<InputStrategy>(getPtr()), getPtr());
}

const NodeBase::PinView NodeBase::getOutputPins()
{
	return PinView(std::make_shared<OutputStrategy>(getPtr()), getPtr());
}

NodeBase::PinView NodeBase::getInputPinsRef() { return PinView(std::make_shared<InputStrategy>(getPtr()), getPtr()); }

NodeBase::PinView NodeBase::getOutputPinsRef() { return PinView(std::make_shared<OutputStrategy>(getPtr()), getPtr()); }

void NodeBase::spreadSignal()
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

void NodeBase::spreadSignal(size_t outIndex)
{
	if (getOutputPinsRef().empty()) return;

	for (auto* inPin : getOutputPinsRef()[outIndex].getOutComponents())
	{
		// I3T_DEBUG_LOG("Spreading signal from {}:{} to {}:{}.", getSig(), outIndex, inPin->m_master->getSig(), inPin->getSig());
		inPin->m_master->receiveSignal(outIndex);
	}
}

void NodeBase::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);

	/// \todo MH this call is unnecessary, but SpreadSignalTest.ValuesShouldBeSpreadThroughConnectedNodes fails.
	// spreadSignal();
}

bool NodeBase::areInputsPlugged(int numInputs)
{
	Debug::Assert(m_inputs.size() >= static_cast<size_t>(numInputs), "Input pins subscript is out of range!");

	bool result = true;

	for (auto i = 0; i < numInputs; ++i) { result = result && m_inputs[i].isPluggedIn(); }

	return result;
}

bool NodeBase::areAllInputsPlugged() { return areInputsPlugged(m_operation->numberOfInputs); }

void NodeBase::accept(NodeVisitor& visitor) { visitor.visit(getPtr()); }

ENodePlugResult NodeBase::isPlugCorrect(Pin const* input, Pin const* output)
{
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
	std::vector<Ptr<NodeBase>> stack;

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

void NodeBase::unplugAll()
{
	for (size_t i = 0L; i < m_inputs.size(); ++i) { unplugInput(i); }

	for (size_t i = 0L; i < m_outputs.size(); ++i) { unplugOutput(i); }
}

void NodeBase::unplugInput(size_t index)
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
		{
			Debug::Assert(false, "Can't find pointer to input pin in other node outputs.");
		}

		auto& myPin		= inputs[index];
		myPin.m_input = nullptr;
	}
}

void NodeBase::unplugOutput(size_t index)
{
	Debug::Assert(m_outputs.size() > static_cast<size_t>(index),
								"The node's output pin that you want to unplug does not exists.");

	// auto& pin = m_outputs[index];
	auto& pin = getOut(index);

	// Set all connected nodes input as nullptr.
	for (const auto& otherPin : pin.m_outputs) otherPin->m_input = nullptr;

	pin.m_outputs.clear();
}


Pin::Pin(EValueType valueType, bool isInput, Ptr<NodeBase> owner, int index) :
		m_valueType(valueType), m_isInput(isInput), m_master(owner.get()), m_index(index)
{
	m_id = generator.next();
}

Pin::~Pin()
{
	generator.returnId(m_id);
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
