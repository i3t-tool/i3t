#include "Node.h"

#include "Core/Nodes/GraphManager.h"
#include "Logger/Logger.h"

using namespace Core;

NodeBase::~NodeBase()
{
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
