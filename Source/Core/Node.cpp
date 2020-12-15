#include "Node.h"

#include "logger/Logger.h"

using namespace Core;

NodeBase::~NodeBase() {
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

void NodeBase::unplugAll()
{
  for (unsigned i = 0; i < m_inputs.size(); ++i)
  {
    unplugInput(i);
  }

  for (unsigned i = 0; i < m_outputs.size(); ++i)
  {
    unplugOutput(i);
  }
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

ENodePlugResult NodeBase::isOperatorPlugCorrect(Pin* sender, Pin* plugged)
{
  auto* inp = sender;
  if (!inp)
    return ENodePlugResult::Err_NonexistentPin;

  auto* out = plugged;
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

    if (act == toFind) return ENodePlugResult::Err_Loop;

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

ENodePlugResult NodeBase::plugToParent(UPtr<NodeBase>& toPlug, unsigned fromIndex, unsigned myIndex)
{
  I3T_DEBUG_ASSERT(m_inputs.size() > myIndex, "Desired input pin in this node with myIndex does not exists!")
  I3T_DEBUG_ASSERT(toPlug->m_outputs.size() > fromIndex, "Desired pin in other node with fromIndex does not exists!")

  auto result = isOperatorPlugCorrect(&m_inputs[myIndex], &toPlug->m_outputs[fromIndex]);
  if (result != ENodePlugResult::Ok)
    return result;

  // Insert to toPlug output pin outputs this operator input pin.
  toPlug->m_outputs[fromIndex].m_outputs.push_back(
      &(m_inputs[myIndex]));

  // Attach given operator output pin to this operator input pin.
  m_inputs[myIndex].m_input = &toPlug->m_outputs[fromIndex];

  return ENodePlugResult::Ok;
}

/*
void __Pin::unplug()
{
  if (m_isInput)
  {
    if (m_inComponent != nullptr)
    {
      for (auto iter = m_inComponent->m_outComponents.begin(); iter != m_inComponent->m_outComponents.end(); ++iter)
      {
        if (*iter == this)
        {
          /// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
          m_inComponent->m_outComponents.erase(iter);
          break;
        }
      }
      if (m_inComponent->onPlugChange) m_inComponent->onPlugChange(m_inComponent, nullptr, this);
      __Pin* oldIn = m_inComponent;
      m_inComponent = nullptr;
      if (onPlugChange) onPlugChange(this, nullptr, oldIn);
    }
  }
  else
  {
    auto it = m_outComponents.begin();

    while (it != m_outComponents.end())
    {
      __Pin* oc = *it;

      __Pin* oldOut = oc->m_inComponent;
      oc->m_inComponent = nullptr;
      it = m_outComponents.erase(it);

      if (oc->onPlugChange) oc->onPlugChange(oc, nullptr, oldOut);
    }

    if (onPlugChange) onPlugChange(this, nullptr, nullptr);
  }
}
 */

void NodeBase::unplugInput(int index)
{
  I3T_DEBUG_ASSERT(m_inputs.size() > index, "The node's input pin that you want to unplug does not exists.");

  auto* otherPin = m_inputs[index].m_input;

  if (otherPin)
  {
    // Erase pointer to my input pin in connected node outputs.
    auto& otherPinOutputs = otherPin->m_outputs;

    auto it = std::find(otherPinOutputs.begin(), otherPinOutputs.end(), &m_inputs[index]);
    if (it != otherPinOutputs.end())
    {
      otherPinOutputs.erase(it);
    }
    else
    {
      I3T_DEBUG_ASSERT(false, "Can't find pointer to input pin in other node outputs.");
    }

    auto& myPin = m_inputs[index];
    myPin.m_input = nullptr;
  }
}

void NodeBase::unplugOutput(int index)
{
  I3T_DEBUG_ASSERT(m_outputs.size() > index, "The node's output pin that you want to unplug does not exists.");

  auto& pin = m_outputs[index];

  // Set all connected nodes input as nullptr.
  for (const auto& otherPin : pin.m_outputs)
    otherPin->m_input = nullptr;

  pin.m_outputs.clear();
}
