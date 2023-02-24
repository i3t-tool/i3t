#include "Core/Nodes/GraphManager.h"

#include <algorithm>

#include "Logger/Logger.h"

namespace Core
{
GraphManager* GraphManager::s_self = nullptr;

void GraphManager::init() { s_self = new GraphManager; }

void GraphManager::destroy()
{
	if (s_self != nullptr)
	{
		delete s_self;
	}
}

Ptr<Sequence> GraphManager::createSequence()
{
	// Temporary workaround:
	if (!s_self)
	{
		GraphManager::init();
	}

	return Builder::createSequence(&(s_self->m_tracker));
}

Ptr<Model> GraphManager::createModel() { return Builder::createModelNode(); }

ENodePlugResult GraphManager::isPlugCorrect(Pin const* input, Pin const* output)
{
	auto lhs = input->m_master;
	return lhs->isPlugCorrect(input, output);
}

ENodePlugResult GraphManager::isPlugCorrect(Pin& input, Pin& output) { return isPlugCorrect(&input, &output); }

ENodePlugResult GraphManager::plug(const Ptr<Core::NodeBase>& lhs, const Ptr<Core::NodeBase>& rhs)
{
	return GraphManager::plug(lhs, rhs, 0, 0);
}

ENodePlugResult GraphManager::plug(const Ptr<Core::NodeBase>& leftNode, const Ptr<Core::NodeBase>& rightNode,
                                   unsigned fromIndex, unsigned myIndex)
{
	I3T_ASSERT(rightNode->getInputPins().size() > myIndex && "Node does not have input pin with given index!");
	I3T_ASSERT(leftNode->getOutputPins().size() > fromIndex && "Node {} does not have output pin with given index!");

	auto result = isPlugCorrect(&rightNode->getIn(myIndex), &leftNode->getOut(fromIndex));
	if (result != ENodePlugResult::Ok)
		return result;

	auto& output = leftNode->getOut(fromIndex);
	auto& input = rightNode->getIn(myIndex);

	// Insert to toPlug output pin outputs this operator input pin.
	output.m_outputs.push_back(&input);

	// Attach given operator output pin to this operator input pin.
	input.m_input = &output;

	for (auto& state : rightNode->m_OperatorState)
		state = EValueState::Locked;

	if (leftNode->getOutputPinsRef()[fromIndex].getType() != EValueType::Pulse)
	{
		leftNode->spreadSignal(fromIndex);
	}

	return ENodePlugResult::Ok;
}

ENodePlugResult GraphManager::plugSequenceValueInput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node,
                                                     unsigned nodeIndex)
{
	return plug(node, seq, nodeIndex, 1);
}

ENodePlugResult GraphManager::plugSequenceValueOutput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node,
                                                      unsigned nodeIndex)
{
	return plug(seq, node, 1, nodeIndex);
}

void GraphManager::unplugAll(const Ptr<Core::NodeBase>& node) { node.get()->unplugAll(); }

void GraphManager::unplugInput(const Ptr<Core::NodeBase>& node, int index) { node.get()->unplugInput(index); }

void GraphManager::unplugOutput(Ptr<Core::NodeBase>& node, int index)
{
	node.get()->unplugOutput(index);
	// tryToDoSequenceProcedure(node);
}

std::vector<Ptr<NodeBase>> GraphManager::getAllInputNodes(const NodePtr& node)
{
	std::vector<Ptr<NodeBase>> result;
	size_t inputsCount = node->getInputPins().size();
	for (size_t i = 0; i < inputsCount; ++i)
	{
		auto parent = getParent(node);
		if (parent != nullptr)
			result.push_back(parent);
	}

	return result;
}

Ptr<NodeBase> GraphManager::getParent(const NodePtr& node, size_t index)
{
	auto pins = node->getInputPins();

	if (index > pins.size())
		return nullptr;

	if (pins.empty() || pins[index].m_input == nullptr)
	{
		return nullptr;
	}

	auto expected = pins[index].m_input->getOwner();

	if (expected->m_owner != nullptr)
	{
		return expected->m_owner;
	}
	else
	{
		return expected;
	}
}

std::vector<Ptr<NodeBase>> GraphManager::getAllOutputNodes(Ptr<Core::NodeBase>& node)
{
	std::vector<Ptr<NodeBase>> result;
	auto pins = node->getOutputPins();
	for (size_t i = 0; i < pins.size(); ++i)
	{
		auto pinOutputs = getOutputNodes(node, i);
		result.insert(result.end(), pinOutputs.begin(), pinOutputs.end());
	}

	return result;
}

std::vector<Ptr<NodeBase>> GraphManager::getOutputNodes(const Ptr<Core::NodeBase>& node, size_t index)
{
	I3T_ASSERT(node->getOutputPins().size() > index && "Out of range.");

	std::vector<Ptr<NodeBase>> result;
	auto pin = node->getOutputPins()[index];
	auto othersInputs = pin.getOutComponents();
	for (const auto& other : othersInputs)
		result.push_back(other->getOwner());

	return result;
}

void GraphManager::update(double tick)
{
	for (auto& cycle : s_self->m_cycles)
	{
		cycle->update(tick);
	}

	s_self->m_tracker.update();
}

const Operation* GraphManager::getOperation(const Pin* pin) { return pin->m_master->getOperation(); }

bool GraphManager::areFromSameNode(const Pin* lhs, const Pin* rhs) { return lhs->m_master == rhs->m_master; }

bool GraphManager::arePlugged(const Pin& input, const Pin& output)
{
	I3T_ASSERT(input.isInput() && "Given input pin is not input pin.");
	if (!input.isPluggedIn())
		return false;
	return input.getParentPin() == &output;
}

bool GraphManager::isTrackingEnabled()
{
	return s_self->m_tracker.getSequence() != nullptr;
}

void GraphManager::stopTracking()
{
	s_self->m_tracker = MatrixTracker{};
}
} // namespace Core
