#include "Core/Nodes/GraphManager.h"

#include <algorithm>

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

Ptr<NodeBase> GraphManager::getParent(Ptr<Sequence> node, size_t index)
{
	auto seq = std::dynamic_pointer_cast<NodeBase>(node);
	return getParent(seq, index);
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
