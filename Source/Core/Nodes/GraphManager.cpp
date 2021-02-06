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
  NodeBase* toFind = inp->m_master; // INPUT

  // stack in vector - TOS is at the vector back.
  std::vector<NodeBase*> stack;

  // PUSH(output) insert element at end.
  stack.push_back(out->m_master);

  while (!stack.empty())
  {
    // Return last element of mutable sequence.
    NodeBase* act = stack.back();
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

ENodePlugResult GraphManager::plug(UPtr<Core::NodeBase>& leftNode, UPtr<Core::NodeBase>& rightNode, unsigned fromIndex,
                                           unsigned myIndex)
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

  return ENodePlugResult::Ok;
}

void GraphManager::unplugAll(UPtr<Core::NodeBase>& node)
{
  for (auto i = 0; i < node->m_inputs.size(); ++i)
  {
    unplugInput(node, i);
  }

  for (auto i = 0; i < node->m_outputs.size(); ++i)
  {
    unplugOutput(node, i);
  }
}

void GraphManager::unplugInput(UPtr<Core::NodeBase>& node, int index)
{
  Debug::Assert(node->m_inputs.size() > index, "The node's input pin that you want to unplug does not exists.");

  auto* otherPin = node->m_inputs[index].m_input;

  if (otherPin)
  {
    // Erase pointer to my input pin in connected node outputs.
    auto& otherPinOutputs = otherPin->m_outputs;

    auto it = std::find(otherPinOutputs.begin(), otherPinOutputs.end(), &node->m_inputs[index]);
    if (it != otherPinOutputs.end())
    {
      /// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
      otherPinOutputs.erase(it);
    }
    else
    {
      Debug::Assert(false, "Can't find pointer to input pin in other node outputs.");
    }

    auto& myPin = node->m_inputs[index];
    myPin.m_input = nullptr;
  }
}

void GraphManager::unplugOutput(UPtr<Core::NodeBase>& node, int index)
{
  Debug::Assert(node->m_outputs.size() > index, "The node's output pin that you want to unplug does not exists.");

  auto& pin = node->m_outputs[index];

  // Set all connected nodes input as nullptr.
  for (const auto& otherPin : pin.m_outputs)
    otherPin->m_input = nullptr;

  pin.m_outputs.clear();
}
