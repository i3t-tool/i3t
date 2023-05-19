#include "Pin.h"

#include "Logger/Logger.h"
#include "Node.h"

namespace Core
{
static IdGenerator generator;

Pin::Pin(EValueType valueType, bool isInput, Ptr<Node> owner, int index)
    : ValueType(valueType), IsInput(isInput), Owner(*owner.get()), Index(index)
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

const DataStore& Pin::data() const
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

DataStore& Pin::dataMut()
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

const char* Pin::getLabel() const
{
	auto* op = getOwner()->getOperation();
	const char* label = nullptr;

	if (IsInput)
	{
		if (!op->defaultInputNames.empty())
		{
			label = op->defaultInputNames[Index].c_str();
		}
	}
	else
	{
		if (!op->defaultOutputNames.empty())
		{
			label = op->defaultOutputNames[Index].c_str();
		}
	}
	if (label == nullptr && shouldRenderPins())
	{
		label = defaultIoNames[static_cast<size_t>(ValueType)];
	}

	return label;
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

	// Insert to toPlug output pin outputs this operator input pin.
	output.m_outputs.push_back(&input);

	// Attach given operator output pin to this operator input pin.
	input.m_input = &output;

	return ENodePlugResult::Ok;
}

void Pin::unplug()
{
	if (!isPluggedIn())
	{
		return;
	}

	if (IsInput)
	{
		auto* output = m_input;

		auto it = std::find(output->m_outputs.begin(), output->m_outputs.end(), this);
		if (it != output->m_outputs.end())
		{
			/// \todo LOG_EVENT_DISCONNECT(this, m_inComponent);
			output->m_outputs.erase(it);
		}
		else
		{
			I3T_ABORT("Can't find pointer to input pin in other node outputs!");
		}

		m_input = nullptr;

		if (Owner.m_operation->isConstructor)
		{
			if (Owner.areAllInputsUnplugged())
			{
				for (auto& state : Owner.m_OperatorState)
				{
					state = EValueState::Editable;
				}
			}
		}
	}
	else
	{
		// pin is output
		// Set all connected nodes input as nullptr.
		for (const auto& otherPin : m_outputs)
		{
			otherPin->m_input = nullptr;
			Owner.triggerUnplugCallback(&Owner, otherPin->getOwner().get(), Index, otherPin->Index);
		}

		m_outputs.clear();
	}
}

ENodePlugResult Pin::isPlugCorrect(const Pin& input, const Pin& output)
{
	if (input.isDisabled() || output.isDisabled())
	{
		return ENodePlugResult::Err_DisabledPin;
	}

	if (input.ValueType != output.ValueType)
	{
		// Do the input and output data types match?
		return ENodePlugResult::Err_MismatchedPinTypes;
	}

	if (input.IsInput == output.IsInput)
	{
		// Do the input and output kind match?
		return ENodePlugResult::Err_MismatchedPinKind;
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
	auto toFind = input.getOwner(); // INPUT

	// stack in vector - TOS is at the vector back.
	std::vector<Ptr<Node>> stack;

	// PUSH(output) insert element at end.
	stack.push_back(output.getOwner());

	while (!stack.empty())
	{
		// Return last element of mutable sequence.
		auto act = stack.back();
		stack.pop_back();

		if (act == toFind)
		{
			return ENodePlugResult::Err_Loop;
		}

		for (auto& pin : act->getInputPins())
		{
			if (pin.isPluggedIn())
			{
				Pin* ct = pin.m_input;
				stack.push_back(ct->getOwner());
			}
		}
	}

	/*
	  if (isOperatorPlugCorrectMod != NULL)
	    return isOperatorPlugCorrectMod(inp, out);
	*/

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
