#pragma once

#include "Core/Nodes/Id.h"
#include "Core/Nodes/NodeData.h"

namespace Core
{
class Node;

/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 */
class Node; /* JH without this it is not compile: unknown ty*/

class Pin
{
	friend class GraphManager;
	friend class Node;

	/// \todo MH do not access pin directly.
	friend class Sequence;

public:
	Pin(EValueType valueType, bool isInput, Ptr<Node> owner, int index);

	[[nodiscard]] ID getId() const { return m_id; }

	[[nodiscard]] int getIndex() const { return m_index; }

	[[nodiscard]] Ptr<Node> getOwner() const;

	[[nodiscard]] const Pin* getParentPin() const
	{
		if (m_isInput)
		{
			I3T_ASSERT(isPluggedIn() && "This input pin is not plugged to any output pin!");
			return m_input;
		}
		else
		{
			I3T_ABORT("Output pin cannot have a parent pin!");
			return nullptr;
		}
	}

	/**
	 * \return Input pins of connected nodes.
	 */
	[[nodiscard]] const std::vector<Pin*>& getOutComponents() const { return m_outputs; }

	/**
	 * \return Storage which belongs to this pin.
	 */
	[[nodiscard]] const DataStore& data() const;

	/**
	 * Get stored data based on pin type.
	 *
	 * \returns data storage owner by node connected to this input pin. If pin is
	 * output pin, it returns data storage of pin owner.
	 */
	[[nodiscard]] const DataStore& getStorage(unsigned id = 0);

	const char* getLabel() const;

	/// "pin {index} of {node signature}"
	std::string getSignature() const;

	[[nodiscard]] EValueType getType() const { return m_valueType; }

	/**
	 * Query if input of this object is plugged to any parent output.
	 *
	 * \return True if plugged to parent, false if not.
	 */
	[[nodiscard]] bool isPluggedIn() const { return m_input != nullptr; }

	[[nodiscard]] bool isInput() const { return m_isInput; }

	[[nodiscard]] bool isDisabled() const { return m_isDisabled; }
	void setDisabled(bool disabled) { m_isDisabled = disabled; }

private:
	ID m_id;

	/// Index within a node.
	int m_index = -1;

	/// Pin type.
	const bool m_isInput;

	bool m_isDisabled = false;

	/// Owner of the pin.
	Node* m_master;

	/**
	 * The box can have a single parent. Therefore, just a single input component
	 * (a single connected wire) to output tab of the parent node).
	 */
	Pin* m_input = nullptr;

	/**
	 * Child boxes in the scene graph (coming out to the right).
	 * A pointer to input pins of boxes connected to this box output.
	 */
	std::vector<Pin*> m_outputs;

	const EValueType m_valueType = EValueType::Pulse;
};
} // namespace Core
