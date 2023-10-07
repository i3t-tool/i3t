#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

namespace Core
{
static IdGenerator generator;

void Node::finalize()
{
	unplugAll();
	triggerDeleteCallback(this);
}

void Node::init()
{
	m_id = generator.next();

	// Create input pins.
	for (auto i = 0; i < m_operation->inputTypes.size(); i++)
	{
		m_inputs.emplace_back(m_operation->inputTypes[i], true, getPtr(), i);
	}

	// Create output pins and data storage for each output.
	for (auto i = 0; i < m_operation->outputTypes.size(); i++)
	{
		m_outputs.emplace_back(m_operation->outputTypes[i], false, getPtr(), i);
		m_internalData.emplace_back(m_operation->outputTypes[i]);
	}

	// \todo MH Ugly workaround for Model, Transforms and Screen node, which has
	// no outputs. \todo MH How to create nodes which have no outputs?
	if (m_operation->outputTypes.empty())
	{
		if (!m_operation->inputTypes.empty())
		{
			m_internalData.emplace_back(m_operation->inputTypes[0]);
		}
		else
		{
			m_internalData.emplace_back(EValueType::Matrix);
		}
	}

	if (m_operation->isConstructor)
		for (int i = 0; i < m_operation->outputTypes.size(); i++)
			m_OperatorState.push_back(EValueState::Editable);
	else
		for (int i = 0; i < m_operation->outputTypes.size(); i++)
			m_OperatorState.push_back(EValueState::Locked);

	onInit();
}

void Node::appendChildNode(Ptr<Node> node)
{
	auto* self = getPtr().get();
	node->setOwner(self);
	m_children.push_back(node.get());
}

ENodePlugResult Node::plug(const Ptr<Node>& childNode, unsigned fromIndex, unsigned toIndex)
{
	I3T_ASSERT(childNode->getInputPins().size() > toIndex, "Node does not have input pin with given index!");
	I3T_ASSERT(getOutputPins().size() > fromIndex, "Node does not have output pin with given index!");

	auto& input = childNode->getInput(toIndex);
	auto& output = getOutput(fromIndex);

	const auto result = output.plug(input);

	if (result == ENodePlugResult::Ok)
	{
		for (auto& state : childNode->m_OperatorState)
		{
			state = EValueState::Locked;
		}

		if (this->getOutputPins()[fromIndex].ValueType != EValueType::Pulse)
		{
			this->spreadSignal(fromIndex);
		}

		triggerPlugCallback(this, childNode.get(), fromIndex, toIndex);
	}

	return result;
}

Ptr<Node> Node::getRootOwner()
{
	auto* me = this;
	auto* parent = m_owner;

	while (parent)
	{
		me = parent;
		parent = parent->m_owner;
	}

	return me->getPtr();
}

void Node::notifyOwner()
{
	if (m_owner)
	{
		m_owner->updateValues(-1);
		m_owner->notifyOwner();
	}
}

ID Node::getId() const
{
	return m_id;
}

EValueState Node::getState(size_t pinIndex)
{
	return m_OperatorState[pinIndex];
}

void Node::pulse(size_t index)
{
	setInternalValue(true, index);
	setInternalValue(false, index);
}

Data& Node::getInternalData(size_t index)
{
	I3T_ASSERT(index < m_internalData.size(), "Desired data storage does not exist!");

	return m_internalData[index];
}

Ptr<Node> Node::getOwner() const
{
	if (m_owner == nullptr)
	{
		return nullptr;
	}

	return m_owner->getPtr();
}

bool Node::shouldPulse(size_t inputIndex, size_t updatedInputIndex)
{
	if (updatedInputIndex == -1)
	{
		return false;
	}

	/*
	const auto& updatedInput = getInput(updatedInputIndex);

	if (!getInput(inputIndex).isPluggedIn() || updatedInput.ValueType != EValueType::Pulse)
	{
	    return false;
	}

	auto& parentStorage = updatedInput.data();
	if (parentStorage.isPulseTriggered())
	{
	    return true;
	}
	 */
	const auto testedInput = getInput(inputIndex);
	if (!testedInput.isPluggedIn())
	{
		return false;
	}

	const auto& parentData = testedInput.data();

	if (inputIndex == updatedInputIndex && parentData.isPulseTriggered())
	{
		return true;
	}

	return false;
}

void Node::spreadSignal()
{
	for (auto& operatorOutput : getOutputPins())
	{
		for (auto* oct : operatorOutput.getOutComponents())
		{
			// I3T_DEBUG_LOG("Spreading signal from {} to {}:{}.", getSig(),
			// oct->Owner.getSig(), oct->getSig());

			/// \todo MH Correct the owner of the pin.
			oct->getOwner()->receiveSignal(oct->Index);
		}
	}
}

void Node::spreadSignal(size_t outIndex)
{
	/// \todo MH This should not happen, remove it.
	if (getOutputPins().empty())
	{
		return;
	}

	auto& outputPin = getOutput(outIndex);

	for (auto* inPin : outputPin.getOutComponents())
	{
		inPin->getOwner()->receiveSignal(inPin->Index);
	}
}

void Node::receiveSignal(int inputIndex)
{
	updateValues(inputIndex);
	notifyOwner();
}

bool Node::areInputsPlugged(int numInputs)
{
	I3T_ASSERT(m_inputs.size() >= static_cast<size_t>(numInputs), "Input pins subscript is out of range!");

	bool result = true;

	for (auto i = 0; i < numInputs; ++i)
	{
		result = result && m_inputs[i].isPluggedIn();
	}

	return result;
}

bool Node::areAllInputsPlugged()
{
	return areInputsPlugged(m_operation->inputTypes.size());
}

bool Node::areAllInputsUnplugged() const
{
	for (auto& input : m_inputs)
	{
		if (input.isPluggedIn())
		{
			return false;
		}
	}

	return true;
}

ENodePlugResult Node::isPlugCorrect(const Pin& input, const Pin& output)
{
	return Pin::isPlugCorrect(input, output);
}

void Node::unplugAll()
{
	for (size_t i = 0L; i < m_inputs.size(); ++i)
	{
		unplugInput(i);
	}

	for (size_t i = 0L; i < m_outputs.size(); ++i)
	{
		unplugOutput(i);
	}
}

void Node::unplugInput(size_t index)
{
	I3T_ASSERT(m_inputs.size() > index, "The node's input pin that you want to unplug does not exist.");

	auto& input = m_inputs[index];

	if (!input.isPluggedIn())
	{
		// already unplugged
		return;
	}

	auto* otherPin = input.getParentPin();
	auto otherPinOwner = otherPin->getOwner().get();
	auto otherPinIndex = otherPin->Index;

	m_inputs[index].unplug();

	onUnplugInput(index);

	triggerUnplugCallback(otherPinOwner, this, otherPinIndex, index);
}

void Node::unplugOutput(size_t index)
{
	I3T_ASSERT(index < m_outputs.size(), "The node's output pin that you want to unplug does not exists.");

	m_outputs[index].unplug();
}

const DataMap* Node::getDataMap()
{
	static std::array<const unsigned char, 16> mapData = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	static DataMap map(mapData);

	return &map;
}

const DataMap& Node::getDataMapRef()
{
	static std::array<const unsigned char, 16> mapData = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	static DataMap map(mapData);

	return map;
}

void Node::updateValues(int inputIndex)
{
	triggerUpdateCallback(this);
}

void Node::addUpdateCallback(std::function<void(Node*)> callback)
{
	this->m_updateCallbacks.push_back(callback);
}
void Node::triggerUpdateCallback(Node* node)
{
	for (const auto& callback : m_updateCallbacks)
	{
		callback(node);
	}
}

void Node::addDeleteCallback(std::function<void(Node*)> callback)
{
	this->m_deleteCallbacks.push_back(callback);
}
void Node::triggerDeleteCallback(Node* node)
{
	for (const auto& callback : m_deleteCallbacks)
	{
		callback(node);
	}
}

void Node::addPlugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback)
{
	this->m_plugCallbacks.push_back(callback);
}
void Node::triggerPlugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex)
{
	for (const auto& callback : m_plugCallbacks)
	{
		callback(fromNode, toNode, fromIndex, toIndex);
	}
}

void Node::addUnplugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback)
{
	this->m_unplugCallbacks.push_back(callback);
}

void Node::triggerUnplugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex)
{
	for (const auto& callback : m_unplugCallbacks)
	{
		callback(fromNode, toNode, fromIndex, toIndex);
	}
}
} // namespace Core
