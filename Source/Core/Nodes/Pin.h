#pragma once

#include "Core/Nodes/Id.h"
#include "Core/Nodes/NodeData.h"

namespace Core
{
class Node;

enum class ENodePlugResult
{
	Ok = 0,
	Err_MismatchedPinTypes,
	Err_MismatchedPinKind,
	Err_Loopback, /// Same nodes.
	Err_NonexistentPin,
	Err_Loop,
	Err_DisabledPin
};

/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 */
class Pin
{
public:
	Pin(EValueType valueType, bool isInput, Ptr<Node> owner, int index);

	Ptr<Node> getOwner() const;

	/// \returns Mapped pin owner or this->Owner if this pin is not mapped to any other node.
	Ptr<Node> getMappedOwner() const;

	/// \returns nullptr if input is not plugged in.
	[[nodiscard]] const Pin* getParentPin() const;

	/**
	 * \return Input pins of connected nodes.
	 */
	[[nodiscard]] const std::vector<Pin*>& getOutComponents() const
	{
		return get()->m_outputs;
	}

	/**
	 * Get stored data based on pin type.
	 *
	 * \return data storage owner by node connected to this input pin. If pin is
	 * output pin, it returns data storage of pin owner.
	 *
	 * \pre If pin is input, it must be plugged to some output pin.
	 */
	const DataStore& data() const;

	DataStore& dataMut();

	const char* getLabel() const;

	/// "pin {index} of {node signature}"
	std::string getSignature() const;

	ENodePlugResult plug(Pin& other);
	static ENodePlugResult plug(Pin& input, Pin& output);

	void unplug();

public:
	static ENodePlugResult isPlugCorrect(const Pin& input, const Pin& output);

	/**
	 * Query if input of this object is plugged to any parent output.
	 *
	 * \return True if plugged to parent, false if not.
	 */
	[[nodiscard]] bool isPluggedIn() const;

	[[nodiscard]] bool isDisabled() const { return m_isDisabled; }
	void setDisabled(bool disabled) { m_isDisabled = disabled; }

	[[nodiscard]] bool shouldRenderPins() const { return m_renderPins; }
	void setRenderPins(bool value) { m_renderPins = value; }

	/// Get this or mapped pin recursively.
	Pin* get() const;

	bool isMapped() const { return m_mappedTo != nullptr; }

	/// \pre pin must have same type (input/output, value) as this pin.
	void mapTo(Pin* pin);

public:
	ID Id;

	/// Index within a node.
	int Index = -1;

	/// Pin type.
	const bool IsInput;

	const EValueType ValueType = EValueType::Pulse;

	Node& Owner;

private:
	bool m_isDisabled = false;

	/// Inner node pin can be mapped to owner node pin.
	Pin* m_mappedTo = nullptr;

	bool m_renderPins = true;

	/**
	 * The box can have a single parent. Therefore, just a single input component
	 * (a single connected wire) to output tab of the parent node).
	 *
	 * \note Valid only for input pins.
	 */
	Pin* m_input = nullptr;

	/**
	 * Child boxes in the scene graph (coming out to the right).
	 * A pointer to input pins of boxes connected to this box output.
	 *
	 * \note Valid only for output pins.
	 */
	std::vector<Pin*> m_outputs;

	/// \todo MH Remove after refactoring.
	friend class GraphManager;
};
} // namespace Core
