#include "Core/Nodes/GraphManager.h"

#include <algorithm>

using namespace Core;

std::vector<Ptr<Cycle>> GraphManager::m_cycles;

void tryToDoSequenceProcedure(Ptr<Node> node)
{
	if (isSequence(node))
		node->as<Sequence>()->updatePins();
}

ENodePlugResult GraphManager::isPlugCorrect(Pin const * input, Pin const * output)
{
	auto lhs = input->m_master;
  return lhs->isPlugCorrect(input, output);
}

ENodePlugResult GraphManager::plug(const Ptr<Core::NodeBase>& lhs, const Ptr<Core::NodeBase>& rhs)
{
	return GraphManager::plug(lhs, rhs, 0, 0);
}

ENodePlugResult GraphManager::plug(const Ptr<Core::NodeBase>& leftNode, const Ptr<Core::NodeBase>& rightNode,
                                   unsigned fromIndex, unsigned myIndex)
{
	Debug::Assert(rightNode->getInputPins().size() > myIndex,
	              "Node {} does not have input pin with index {}!", rightNode->getSig(), myIndex);
	Debug::Assert(leftNode->getOutputPins().size() > fromIndex,
	              "Node {} does not have output pin with index {}!", leftNode->getSig(), fromIndex);

	auto result = isPlugCorrect(&rightNode->getInPin(myIndex), &leftNode->getOutPin(fromIndex));
	if (result != ENodePlugResult::Ok)
		return result;

	// Insert to toPlug output pin outputs this operator input pin.
	leftNode->getOutputPinsRef()[fromIndex].m_outputs.push_back(&(rightNode->getInputPinsRef()[myIndex]));

	// Attach given operator output pin to this operator input pin.
	rightNode->getInputPinsRef()[myIndex].m_input = &leftNode->getOutputPinsRef()[fromIndex];

  if (isSequence(leftNode))
  {
    leftNode->as<Sequence>()->updatePins();
  }
  if (isSequence(rightNode))
  {
    rightNode->as<Sequence>()->updatePins();
  }

	leftNode->spreadSignal();

	rightNode->setDataMap(&Transform::g_AllLocked);

	return ENodePlugResult::Ok;
}

ENodePlugResult GraphManager::plugSequenceValueInput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node, unsigned nodeIndex)
{
  return plug(node, seq, nodeIndex, 1);
}

ENodePlugResult GraphManager::plugSequenceValueOutput(const Ptr<Core::NodeBase>& seq, const Ptr<Core::NodeBase>& node, unsigned nodeIndex)
{
  return plug(seq, node, 1, nodeIndex);
}

void GraphManager::unplugAll(const Ptr<Core::NodeBase>& node)
{
  node.get()->unplugAll();
	node->setDataMap(&Transform::g_Free);
  tryToDoSequenceProcedure(node);
}

void GraphManager::unplugInput(const Ptr<Core::NodeBase>& node, int index)
{
	node.get()->unplugInput(index);
  tryToDoSequenceProcedure(node);
	if (getAllInputNodes(node).empty())
    node->setDataMap(node->m_initialMap);
}

void GraphManager::unplugOutput(Ptr<Core::NodeBase>& node, int index)
{
	node.get()->unplugOutput(index);
  tryToDoSequenceProcedure(node);
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

	auto expected = pins[index].m_input->m_master;

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
	Debug::Assert(node->getOutputPins().size() > index, "Out of range.");

	std::vector<Ptr<NodeBase>> result;
	auto pin = node->getOutputPins()[index];
	auto othersInputs = pin.getOutComponents();
	for (const auto& other : othersInputs)
		result.push_back(other->m_master);

	return result;
}

void GraphManager::update(double tick)
{
	for (auto& cycle : m_cycles)
		cycle->update(tick);
}

const Operation* GraphManager::getOperation(const Pin* pin)
{
	return pin->m_master->getOperation();
}

bool GraphManager::areFromSameNode(const Pin* lhs, const Pin* rhs)
{
	return lhs->m_master == rhs->m_master;
}

bool GraphManager::arePlugged(const Pin& input, const Pin& output)
{
	Debug::Assert(input.isInput(), "Given input pin is not input pin.");
	if (!input.isPluggedIn()) return false;
	return input.getParentPin() == &output;
}

SequenceTree::SequenceTree(Ptr<NodeBase> sequence)
{
	m_beginSequence = toSequence(sequence);
}

SequenceTree::MatrixIterator SequenceTree::begin()
{
	auto it = MatrixIterator(m_beginSequence);
	it.m_tree = this;
	return it;
}

SequenceTree::MatrixIterator SequenceTree::end()
{
	auto cur = m_beginSequence;
	Ptr<Sequence> parent;
	while ((parent = toSequence(GraphManager::getParent(cur, 0))) != nullptr)
	{
		cur = parent;
	}

	auto it = MatrixIterator(cur, nullptr);
	it.m_tree = this;
	return it;
}

SequenceTree::MatrixIterator::MatrixIterator(Ptr<Sequence>& sequence)
{
	m_currentSequence = sequence;
	m_currentMatrix = sequence->getMatrices().empty() ? nullptr : sequence->getMatrices().back();
}

SequenceTree::MatrixIterator::MatrixIterator(Ptr<Sequence>& sequence, NodePtr node)
{
	m_currentSequence = sequence;
	m_currentMatrix = node;
}

SequenceTree::MatrixIterator& SequenceTree::MatrixIterator::operator++()
{
	advance();
	return *this;
}

SequenceTree::MatrixIterator SequenceTree::MatrixIterator::operator++(int)
{
	advance();
	return *this;
}

SequenceTree::MatrixIterator& SequenceTree::MatrixIterator::operator--()
{
	withdraw();
	return *this;
}

SequenceTree::MatrixIterator SequenceTree::MatrixIterator::operator--(int)
{
	withdraw();
	return *this;
}

Ptr<NodeBase> SequenceTree::MatrixIterator::operator*() const
{
	Debug::Assert(m_currentMatrix != nullptr, "Iterator is at the end!");
	return m_currentMatrix;
}

bool SequenceTree::MatrixIterator::operator==(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix == rhs.m_currentMatrix;
}

bool SequenceTree::MatrixIterator::operator!=(const SequenceTree::MatrixIterator& rhs) const
{
	return m_currentMatrix != rhs.m_currentMatrix;
}

void SequenceTree::MatrixIterator::advance()
{
	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	/// \todo MH handle case when matrix is not in a sequence.

	if (index == 0)
	{
		auto parent = GraphManager::getParent(m_currentSequence);

		// Check if current matrix is not first in the graph.
		if (parent)
		{
			// Sequence is not the root, there is another parent sequence.
			m_currentSequence = toSequence(parent);
			m_currentMatrix = m_currentSequence->getMatrices().back();
		}
		else
		{
			m_currentMatrix = nullptr;
		}
	}
	else
	{
		m_currentMatrix = matrices[--index];
	}
}

void SequenceTree::MatrixIterator::withdraw()
{
	// Find index of current matrix in current sequence.
	auto& matrices = m_currentSequence->getMatrices();
	auto it = std::find(matrices.begin(), matrices.end(), m_currentMatrix);
	auto index = std::distance(matrices.begin(), it);

	if (m_currentMatrix == nullptr)
		index = -1;

	/// \todo MH handle case when matrix is not in a sequence.

	if (m_currentMatrix == matrices.back())
	{
		// Current matrix is last matrix in a sequence. Go to the previous sequence.
		auto prev = m_tree->m_beginSequence;
		auto prevsParent = m_tree->m_beginSequence;
		while ((prevsParent = toSequence(GraphManager::getParent(prev))) != m_currentSequence)
		{
			prev = prevsParent;
		}
		m_currentSequence = prev;
		m_currentMatrix = m_currentSequence->getMatrices().front();
	}
	else
	{
		m_currentMatrix = matrices[++index];
	}
}
