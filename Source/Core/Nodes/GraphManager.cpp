#include "Core/Nodes/GraphManager.h"

using namespace Core;

ENodePlugResult GraphManager::isPlugCorrect(Pin* input, Pin* output)
{
	auto* inp = input;
	if (!inp)
		return ENodePlugResult::Err_NonexistentPin;

	auto* out = output;
	if (!out)
		return ENodePlugResult::Err_NonexistentPin;

	if (inp->m_opValueType != out->m_opValueType)
	{
		// Do the input and output data types match?
		return ENodePlugResult::Err_MismatchedPinTypes;
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

ENodePlugResult GraphManager::plug(const Ptr<Core::NodeBase>& leftNode, const Ptr<Core::NodeBase>& rightNode,
                                   unsigned fromIndex, unsigned myIndex)
{
	Debug::Assert(rightNode->m_inputs.size() > myIndex, "Desired input pin in this node with myIndex does not exists!");
	Debug::Assert(leftNode->m_outputs.size() > fromIndex, "Desired pin in other node with fromIndex does not exists!");

	auto result = isPlugCorrect(&rightNode->m_inputs[myIndex], &leftNode->m_outputs[fromIndex]);
	if (result != ENodePlugResult::Ok)
		return result;

	// Insert to toPlug output pin outputs this operator input pin.
	leftNode->m_outputs[fromIndex].m_outputs.push_back(&(rightNode->m_inputs[myIndex]));

	// Attach given operator output pin to this operator input pin.
	rightNode->m_inputs[myIndex].m_input = &leftNode->m_outputs[fromIndex];

	leftNode->spreadSignal();

	return ENodePlugResult::Ok;
}

void GraphManager::unplugAll(Ptr<Core::NodeBase>& node)
{
	node.get()->unplugAll();
}

void GraphManager::unplugInput(Ptr<Core::NodeBase>& node, int index)
{
	node.get()->unplugInput(index);
}

void GraphManager::unplugOutput(Ptr<Core::NodeBase>& node, int index)
{
	node.get()->unplugOutput(index);
}

Ptr<NodeBase> GraphManager::getParent(Ptr<NodeBase>& node, size_t index)
{
	auto pins = node->getInputPins();
	if (pins.empty() || pins[index].m_input == nullptr)
	{
        return nullptr;
    }
    return pins[index].m_input->m_master;
}

std::vector<Ptr<NodeBase>> GraphManager::getAllInputNodes(Ptr<Core::NodeBase>& node)
{
  std::vector<Ptr<NodeBase>> result;
	size_t inputsCount = node->getInputPins().size();
	for (auto i = 0; i < inputsCount; ++i)
  {
		auto parent = getParent(node);
		if (parent != nullptr)
		  result.push_back(parent);
	}

	return result;
}

std::vector<Ptr<NodeBase>> GraphManager::getAllOutputNodes(Ptr<Core::NodeBase>& node)
{
  std::vector<Ptr<NodeBase>> result;
	auto pins = node->getOutputPins();
	for (auto i = 0; i < pins.size(); ++i)
  {
    auto pinOutputs = getOutputNodes(node, i);
		result.insert(result.end(), pinOutputs.begin(), pinOutputs.end());
	}

	return result;
}

std::vector<Ptr<NodeBase>> GraphManager::getOutputNodes(Ptr<Core::NodeBase>& node, size_t index)
{
	Debug::Assert(node->getOutputPins().size() > index, "Out of range.");

  std::vector<Ptr<NodeBase>> result;
  auto pin = node->getOutputPins()[index];
  auto othersInputs = pin.getOutComponents();
  for (const auto& other : othersInputs)
    result.push_back(other->m_master);

	return result;
}
