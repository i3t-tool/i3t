/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Pin.h"

#include "Logger/Logger.h"
#include "Node.h"

namespace Core
{
static IdGenerator generator;

Pin::Pin(EValueType valueType, bool isInput, Node& owner, int index)
    : ValueType(valueType), IsInput(isInput), Owner(owner), Index(index)
{
	Id = generator.next();
}

[[nodiscard]] Ptr<Node> Pin::getOwner() const
{
	return Owner.getPtr();
}

[[nodiscard]] const Pin* Pin::getParentPin() const
{
	if (!IsInput)
	{
		I3T_ABORT("Output pin cannot have a parent pin!");
	}

	if (!isPluggedIn())
	{
		return nullptr;
	}

	return m_input;
}

const Data& Pin::data() const
{
	if (IsInput)
	{
		I3T_ASSERT(isPluggedIn(), "This input pin is not plugged to any output pin!");
		return m_input->dataMut();
	}
	else
	{
		return Owner.getInternalData(Index);
	}
}

Data& Pin::dataMut()
{
	if (IsInput)
	{
		I3T_ASSERT(isPluggedIn(), "This input pin is not plugged to any output pin!");
		return m_input->dataMut();
	}
	else
	{
		return Owner.getInternalData(Index);
	}
}

const std::string& Pin::getLabel() const
{
	auto& op = getOwner()->getOperation();

	if (IsInput)
	{
		if (!op.defaultInputNames.empty())
		{
			return op.defaultInputNames[Index];
		}
	}
	else
	{
		if (!op.defaultOutputNames.empty())
		{
			return op.defaultOutputNames[Index];
		}
	}
	// (PF) no output label for pulse
	// if (!IsInput && ValueType == EValueType::Pulse)
	// {
	//	return defaultIoNames[static_cast<size_t>(ValueType)];
	// }

	/*
	// default names for pins, we don't show them in the UI
	if (isRendered())
	{
	    return defaultIoNames[static_cast<size_t>(ValueType)];
	}
	 */

	static std::string empty;
	return empty;
}

ENodePlugResult Pin::plug(Pin& other)
{
	if (IsInput)
		return Pin::plug(*this, other);
	else
		return Pin::plug(other, *this);
}

ENodePlugResult Pin::plug(Pin& input, Pin& output)
{
	auto result = Pin::isPlugCorrect(input, output);
	if (result != ENodePlugResult::Ok)
	{
		return result;
	}

	// Insert the input-pin to the vector of the output-pin outputs
	output.m_outputs.push_back(&input);

	// Attach given output-pin to the input-pin.
	input.m_input = &output;

	return ENodePlugResult::Ok;
}

void Pin::unplug()
{
	I3T_ASSERT(IsInput, "Must be an input pin!");

	if (!isPluggedIn())
	{
		return;
	}

	auto* output = m_input;

	auto it = std::find(output->m_outputs.begin(), output->m_outputs.end(), this);
	I3T_ASSERT(it != output->m_outputs.end(), "Can't find pointer to input pin in other node outputs!");
	if (it != output->m_outputs.end())
	{
		/// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
		LOG_DEBUG("Erasing input pin {} of node {} from output pin {} outputs of node {}.", Index, Owner.getSignature(),
		          output->Index, output->Owner.getSignature());
		output->m_outputs.erase(it);
	}

	m_input = nullptr;

	if (Owner.m_operation.isConstructor)
	{
		if (Owner.areAllInputsUnplugged())
		{
			for (auto& state : Owner.m_OperatorState)
			{
				state = EValueState::Editable;
			}
		}
	}

	Owner.updateValues(Index);
}

ENodePlugResult Pin::isPlugCorrect(const Pin& input, const Pin& output)
{
	if (input.isDisabled() || output.isDisabled())
	{
		return ENodePlugResult::Err_DisabledPin;
	}

	if (input.IsInput == output.IsInput)
	{
		// Do the input and output kind match?
		return ENodePlugResult::Err_MismatchedPinKind;
	}

	if (input.ValueType != output.ValueType)
	{
		// Do the input and output data types match?
		return ENodePlugResult::Err_MismatchedPinTypes;
	}

	if (input.ValueType == EValueType::Pulse)
	{
		// No restrictions on pulse pins.
		return ENodePlugResult::Ok;
	}

	if (input.Owner == output.Owner)
	{
		// Not a circular edge?
		return ENodePlugResult::Err_Loopback;
	}
	else
	{
		// Check node owners in the case of nested nodes.
		const auto lhsRoot = output.Owner.getRootOwner();
		const auto rhsRoot = input.Owner.getRootOwner();

		if (lhsRoot != nullptr && rhsRoot != nullptr && lhsRoot == rhsRoot)
		{
			return ENodePlugResult::Err_Loopback;
		}
	}

	// cycle detector
	auto toFind = input.Owner.getRootOwner(); // input

	std::vector<Ptr<Node>> stack;

	if (!output.getOwner()->getOperation().ignoreCycleDetection)
	{
		stack.push_back(output.getOwner());
	}

	while (!stack.empty())
	{
		auto act = stack.back();
		stack.pop_back();

		if (act->getRootOwner() == toFind)
		{
			return ENodePlugResult::Err_Loop;
		}

		const auto tryPushToStack = [&stack](const Pin& pin) {
			if (pin.isPluggedIn())
			{
				Pin* ct = pin.m_input;

				auto toPush = ct->Owner.getRootOwner();
				if (!toPush->getOperation().ignoreCycleDetection)
				{
					stack.push_back(ct->Owner.getRootOwner());
					// stack.push_back(ct->getOwner());
				}
			}
		};

		for (auto& pin : act->getInputPins())
		{
			tryPushToStack(pin);
		}

		// process nested nodes, for example camera subsequences
		for (auto& node : act->m_children)
		{
			for (auto& pin : node->getInputPins())
			{
				tryPushToStack(pin);
			}
		}
	}

	return ENodePlugResult::Ok;
}

std::string Pin::getSignature() const
{
	return fmt::format("pin {} of {}", Index, getOwner()->getSignature());
}

[[nodiscard]] bool Pin::isPluggedIn() const
{
	if (IsInput)
	{
		return m_input != nullptr;
	}
	else
	{
		return !m_outputs.empty();
	}
}
} // namespace Core
