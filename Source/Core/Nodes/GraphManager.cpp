/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Core/Nodes/GraphManager.h"

#include <algorithm>

#include "Logger/Logger.h"

namespace Core
{
GraphManager* GraphManager::s_self = nullptr;

void GraphManager::init()
{
	s_self = new GraphManager;
}

void GraphManager::destroy()
{
	if (s_self != nullptr)
	{
		delete s_self;
	}
}

Ptr<Sequence> GraphManager::createSequence()
{
	/// \todo MH Temporary workaround:
	if (!s_self)
	{
		GraphManager::init();
	}

	return Builder::createSequence();
}

Ptr<Model> GraphManager::createModel()
{
	return Builder::createModelNode();
}

ENodePlugResult GraphManager::isPlugCorrect(const Pin& input, const Pin& output)
{
	auto& lhs = output.Owner;
	return lhs.isPlugCorrect(input, output);
}

ENodePlugResult GraphManager::plug(const Ptr<Core::Node>& lhs, const Ptr<Core::Node>& rhs)
{
	return GraphManager::plug(lhs, rhs, 0, 0);
}

ENodePlugResult GraphManager::plug(const Ptr<Core::Node>& leftNode, const Ptr<Core::Node>& rightNode,
                                   unsigned fromIndex, unsigned toIndex)
{
	return leftNode->plug(rightNode, fromIndex, toIndex);
}

ENodePlugResult GraphManager::plugSequenceValueInput(const Ptr<Core::Node>& seq, const Ptr<Core::Node>& node,
                                                     unsigned nodeIndex)
{
	return plug(node, seq, nodeIndex, 1);
}

ENodePlugResult GraphManager::plugSequenceValueOutput(const Ptr<Core::Node>& seq, const Ptr<Core::Node>& node,
                                                      unsigned nodeIndex)
{
	return plug(seq, node, 1, nodeIndex);
}

void GraphManager::unplugAll(const Ptr<Core::Node>& node)
{
	node.get()->unplugAll();
}

void GraphManager::unplugInput(const Ptr<Core::Node>& node, int index)
{
	node.get()->unplugInput(index);
}

std::vector<Ptr<Node>> GraphManager::getAllInputNodes(const Ptr<Node>& node)
{
	std::vector<Ptr<Node>> result;
	size_t inputsCount = node->getInputPins().size();
	for (size_t i = 0; i < inputsCount; ++i)
	{
		auto parent = getParent(node);
		if (parent != nullptr)
			result.push_back(parent);
	}

	return result;
}

Ptr<Node> GraphManager::getParent(const Ptr<Node>& node, size_t index)
{
	auto pins = node->getInputPins();

	if (index >= pins.size())
	{
		return nullptr;
	}

	auto& input = pins[index];

	if (!input.isPluggedIn())
	{
		return nullptr;
	}

	// Can result in nested node, check is node is owned by another node.
	auto expected = input.getParentPin()->getOwner();

	if (expected->getRootOwner())
	{
		return expected->getRootOwner();
	}

	return expected;
}

std::vector<Ptr<Node>> GraphManager::getAllOutputNodes(Ptr<Core::Node>& node)
{
	std::vector<Ptr<Node>> result;
	auto pins = node->getOutputPins();
	for (size_t i = 0; i < pins.size(); ++i)
	{
		auto pinOutputs = getOutputNodes(node, i);
		result.insert(result.end(), pinOutputs.begin(), pinOutputs.end());
	}

	return result;
}

std::vector<Ptr<Node>> GraphManager::getOutputNodes(const Ptr<Core::Node>& node, size_t index)
{
	I3T_ASSERT(index < node->getOutputPins().size(), "Out of range.");

	std::vector<Ptr<Node>> result;
	auto pin = node->getOutputPins()[index];
	auto othersInputs = pin.getOutComponents();
	for (const auto& other : othersInputs)
	{
		result.push_back(other->getOwner());
	}

	return result;
}

void GraphManager::update(double tick)
{
	for (auto& cycle : s_self->m_cycles)
	{
		cycle->update(tick);
	}

	getTracker()->update();
}

const Operation& GraphManager::getOperation(const Pin* pin)
{
	return pin->Owner.getOperation();
}

bool GraphManager::isTracking()
{
	return getTracker()->isTracking();
}

bool GraphManager::isTrackingFromLeft()
{
	return getTracker()->isTrackingFromLeft();
}

void GraphManager::startTracking(Ptr<Sequence> beginSequence, TrackingDirection direction)
{
	stopTracking();
	s_self->m_tracker = std::make_unique<MatrixTracker>(beginSequence, direction);
}

void GraphManager::stopTracking()
{
	if (isTracking())
		s_self->m_tracker = std::make_unique<MatrixTracker>();
}
} // namespace Core
