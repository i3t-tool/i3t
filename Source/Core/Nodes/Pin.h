#pragma once

#include "Id.h"
#include "NodeData.h"

namespace Core
{
class Node;

/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 *
 * \todo MH - Remove ID.
 */
class Pin
{
	friend class GraphManager;
	friend class Node;

	/// \todo MH do not access pin directly.
	friend class Sequence;

public:
	Pin(EValueType valueType, bool isInput, Ptr<Node> owner, int index);
	~Pin();

	[[nodiscard]] ID getId() const { return m_id; }

	[[nodiscard]] int getIndex() const { return m_index; }

	[[nodiscard]] Ptr<Node> getOwner() const;

	[[nodiscard]] const Pin* getParentPin() const
	{
		if (m_isInput)
		{
			Debug::Assert(isPluggedIn(), "This input pin is not plugged to any output pin!");
			return m_input;
		}
		else
		{
			Debug::Assert(false, "Output pin can not have a parent pin!");
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
	 * \returns data storage owner by node connected to this input pin. If pin is output pin,
	 *          it returns data storage of pin owner.
	 */
	[[nodiscard]] const DataStore& getStorage(unsigned id = 0);

	const char* getLabel() const;

	/// Only for test purposes, it can be removed anytime.
	std::string getSig();

	[[nodiscard]] EValueType getType() const { return m_valueType; }

	/**
	 * Query if input of this object is plugged to any parent output.
	 *
	 * \return True if plugged to parent, false if not.
	 */
	[[nodiscard]] bool isPluggedIn() const { return m_input != nullptr; }

	[[nodiscard]] bool isInput() const { return m_isInput; }

private:
	ID m_id;

	/// Index within a node.
	int m_index = -1;

	/// Pin type.
	const bool m_isInput;

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
}