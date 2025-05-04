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
	Pin(EValueType valueType, bool isInput, Node& owner, int index);

	Ptr<Node> getOwner() const;

	/// \returns nullptr if input is not plugged in.
	[[nodiscard]] const Pin* getParentPin() const;

	/**
	 * \return Input pins of connected nodes.
	 */
	[[nodiscard]] const std::vector<Pin*>& getOutComponents() const
	{
		return m_outputs;
	}

	/**
	 * Get stored data based on pin type.
	 *
	 * \return data storage owner by node connected to this input pin. If pin is
	 * output pin, it returns data storage of pin owner.
	 *
	 * \pre If pin is input, it must be plugged to some output pin.
	 */
	const Data& data() const;

	Data& dataMut();

	const std::string& getLabel() const;

	/// "pin {index} of {node signature}"
	std::string getSignature() const;

private:
	ENodePlugResult plug(Pin& other);

	static ENodePlugResult plug(Pin& input, Pin& output);

	/**
	 * Called only on input pin.
	 */
	void unplug();

public:
	static ENodePlugResult isPlugCorrect(const Pin& input, const Pin& output);

	/**
	 * Query if input of this object is plugged to any parent output.
	 *
	 * \return True if plugged to parent, false if not.
	 */
	[[nodiscard]] bool isPluggedIn() const;

	[[nodiscard]] bool isDisabled() const
	{
		return m_isDisabled;
	}
	void setDisabled(bool disabled)
	{
		m_isDisabled = disabled;
	}

	/**
	 * \brief False in Camera P and V matrices ONLY. Used to skip hidden values stored as pins.
	 */
	[[nodiscard]] bool isRendered() const
	{
		return m_isRendered;
	}
	void setRendered(bool value)
	{
		m_isRendered = value;
	}

public:
	ID Id;

	/// Index within a node.
	const int Index = -1;

	/// Pin type.
	const bool IsInput;

	const EValueType ValueType = EValueType::Pulse;

	Node& Owner;

private:
	bool m_isDisabled = false; //< Pin is visible, but cannot be interactively connected to
	                           //< (used for Camera mult output)

	bool m_isRendered = true; //< Render this pin. False in Camera P and V matrices ONLY.

	/**
	 * Corresponding output pin this pin is connected to.
	 * An input pin can only be connected to one output pin.
	 * \note Valid only for input pins.
	 */
	Pin* m_input = nullptr;

	/**
	 * Child nodes in the scene graph (coming out to the right).
	 * A pointer to input pins of nodes connected to this node's output pin.
	 * \note Valid only for output pins.
	 */
	std::vector<Pin*> m_outputs;

	friend class Node;
};
} // namespace Core
