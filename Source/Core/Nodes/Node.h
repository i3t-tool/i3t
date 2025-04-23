/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 18.11.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * Code taken from (former) Source/operator.h, Source/operatorForm.h and
 * Source/operatorNode.h file which used to define Operator, base class for all
 * boxes.
 */
#pragma once

#include <list>
#include <set>
#include <utility>
#include <vector>

#include "Core/Defs.h"
#include "Core/Nodes/Id.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Nodes/Operations.h"
#include "Core/Nodes/Pin.h"

namespace Core
{
class Node;
class Pin;
struct TrackedNodeData;

struct SetValueResult
{
	enum class Status
	{
		Ok = 0,
		Err_ConstraintViolation,
		Err_LogicError
	};

	Status status = Status::Ok;
	std::string message;

	SetValueResult() = default;
	SetValueResult(Status aStatus, std::string aMessage = "") : status(aStatus), message(std::move(aMessage)) {}
};

inline constexpr size_t I3T_INPUT0 = 0;
inline constexpr size_t I3T_INPUT1 = 1;
inline constexpr size_t I3T_INPUT2 = 2;

inline constexpr size_t I3T_OUTPUT0 = 0;
inline constexpr size_t I3T_OUTPUT1 = 1;
inline constexpr size_t I3T_OUTPUT2 = 2;

inline constexpr size_t I3T_DATA0 = 0;
inline constexpr size_t I3T_DATA1 = 1;
inline constexpr size_t I3T_DATA2 = 2;

/**
 * Base class interface for all boxes.
 */
class Node : public std::enable_shared_from_this<Node>
{
	friend class GraphManager;
	friend class MatrixTracker;
	friend class Pin;

	//===-- Lifecycle functions -----------------------------------------------===//
protected:
	/**
	 * Node is never constructed directory.
	 *
	 * \see init() function for pin and data storage initialization
	 *
	 * \param operation Node properties.
	 */
	explicit Node(const Operation& operation) : m_operation(operation) {}
	virtual ~Node();

public:
	bool operator==(const Node& other) const
	{
		return getId() == other.getId();
	}

	bool operator!=(const Node& other) const
	{
		return !(*this == other);
	}

	/**
	 * Initialize node id, inputs, and outputs according to the preset node type.
	 *
	 * Called in create node function.
	 */
	void init();

	virtual void onInit() {}

	void appendChildNode(Ptr<Node> node);

	// \todo MH: Remove, use destructor directly.
	//   DR: Finalize cannot be called in the destructor
	//   Unplugging pins triggers callbacks which expect valid pin references (with valid node parents)
	//   When unplugging a node during destruction, it is no longer a "valid" object as its underlying weak ptr
	//   is expired by then -> bad_weak_ptr in unplugInput otherPin->getOwner()
	/**
	 * Prepares node for its destruction, after that the destructor can be called.
	 */
	void finalize();

	/**
	 * \brief Connect this (parent) node output to the \p childNode input.
	 * \param childNode this node's \a toIndex input will be connected
	 * \param fromIndex index of the parent (this) node output
	 * \param toIndex index of the \a childNode input
	 * \return Result enum is returned from the function. \see ENodePlugResult.
	 */
	ENodePlugResult plug(const Ptr<Node>& childNode, unsigned fromIndex, unsigned toIndex);

	//===-- Helper functions --------------------------------------------------===//

	/**
	 * \return Application unique ID.
	 */
	ID getId() const;

	const Operation& getOperation() const
	{
		return m_operation;
	}

	/**
	 * Get reference to this node instance.
	 *
	 * Note that this operation may be slower. A new shared pointer must be
	 * created, it obtains atomic counter increment.
	 */
	Ptr<Node> getPtr()
	{
		return shared_from_this();
	}

	template <typename T>
	Ptr<T> as()
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node class.");
		I3T_ASSERT(std::dynamic_pointer_cast<T>(shared_from_this()), "Cannot cast to Ptr<T>.");

		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	// NOTE: The "as" method above is used quite a lot but has overhead (dynamic cast + shared ptr construction)
	//  This might be preferable in cases when nullptr isn't expected and shared ptr is not needed.
	template <typename T>
	T* asRaw()
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node class.");
		I3T_ASSERT(dynamic_cast<T*>(this), "Cannot cast to T*.");
		return static_cast<T*>(this);
	}

	//===----------------------------------------------------------------------===//
	/// \todo Break chain Node -> Transform.
	/// \todo This is pin related, move to Pin class.
	std::vector<EValueState> m_OperatorState;

	EValueState getState(size_t pinIndex = 0);
	//===----------------------------------------------------------------------===//

	Pin& getInput(size_t i)
	{
		return m_inputs[i];
	}
	Pin& getOutput(size_t i)
	{
		return m_outputs[i];
	}

	std::vector<Pin>& getInputPins()
	{
		return m_inputs;
	}
	std::vector<Pin>& getOutputPins()
	{
		return m_outputs;
	}

	//===-- Obtaining value functions. ----------------------------------------===//
	/**
	 * Get data storage for read and write purposes. No written value validation
	 * is performed.
	 *
	 * Overridden in Sequence class.
	 */
public:
	/// This function won't work for mapped pins.
	/// \todo Make this function non public.
	Data& getInternalData(size_t index = 0);

	/**
	 * Get Node contents, read only.
	 * \param index Index of the internal modifiable data field (e.g, 0 or 1 for
	 * two vectors). Value of field[0] is returned if this parameter omitted)
	 * \return Struct which holds data
	 */
	const Data& data(size_t index = 0) const
	{
		I3T_ASSERT(index < m_internalData.size(), "Desired data storage does not exist!");

		return m_internalData[index];
	}

	Data& dataMut(size_t index)
	{
		return getInternalData(index);
	}

public:
	/// Get direct owner of this node.
	///
	/// \returns nullptr if owner is not set.
	Ptr<Node> getOwner() const;

private:
	void setOwner(Node* owner)
	{
		m_owner = owner;
	}

public:
	/// Get the topmost owner of this node.
	Ptr<Node> getRootOwner();

	void notifyOwner();

public:
	[[nodiscard]] virtual SetValueResult setValue(void* ptr)
	{
		m_internalData[0].setValue(ptr);
		spreadSignal();

		return SetValueResult{};
	}

	/**
	 * Set the value of the node.
	 * Sets the value of the first float of DataStore. Derived types may override
	 * the default behavior.
	 *
	 * \param val
	 */
	[[nodiscard]] virtual SetValueResult setValue(float val)
	{
		return setValueEx(val);
	}
	[[nodiscard]] virtual SetValueResult setValue(const glm::vec3& vec)
	{
		return setValueEx(vec);
	}
	[[nodiscard]] virtual SetValueResult setValue(const glm::vec4& vec)
	{
		return setValueEx(vec);
	}
	[[nodiscard]] virtual SetValueResult setValue(const glm::quat& q)
	{
		return setValueEx(q);
	}
	[[nodiscard]] virtual SetValueResult setValue(const glm::mat4& mat)
	{
		return setValueEx(mat);
	}

	/**
	 * \param val new value
	 * \param coords in the column major order: coords.x is the column index and
	 * coords.y is the row index. \return
	 */
	[[nodiscard]] virtual SetValueResult setValue(float val, glm::ivec2 coords)
	{
		setInternalValue(val, coords);
		return SetValueResult{};
		// return SetValueResult{SetValueResult::Status::Err_LogicError,
		// "Unsupported operation on non transform object."};
	}

	/**
	 * Smart set function, used with constrained transformation for value
	 * checking. \param mat \param map array of 16 chars.
	 */
	[[nodiscard]] virtual SetValueResult setValue(const glm::mat4& mat, const DataMap& map)
	{
		return SetValueResult{SetValueResult::Status::Err_LogicError, "Unsupported operation on non transform object."};
	}

	template <typename T>
	[[nodiscard]] SetValueResult setValue(const T& value, unsigned index)
	{
		return setValueEx(value, index);
	}

private:
	/// Sets value of pin at \p index position.
	template <typename T>
	SetValueResult setValueEx(T&& val, unsigned index = 0)
	{
		setInternalValue(val, index);
		return SetValueResult{};
	}

protected:
	/**
	 * Sets the node value without validation.
	 * \tparam T Value type, no need to specify it in angle brackets, it will be
	 * deduced by the compiler (C++17).
	 * \param value Value to set.
	 * \param index Index of the DataStore (if the node stores more than one value)
	 */
	template <typename T>
	void setInternalValue(const T& value, size_t index = 0)
	{
		getInternalData(index).setValue(value);
		spreadSignal(index);
	}

	void setInternalValue(float value, glm::ivec2 coordinates, size_t index = 0)
	{
		getInternalData(index).getMat4Ref()[coordinates.x][coordinates.y] = value;
		spreadSignal(index);
	}

public:
	void pulse(size_t index);

protected:
	/**
	 * Tests if the \p updatedInputIndex should be pulsed.
	 * @param inputIndex Index of the tested input.
	 * @param updatedInputIndex Index of the input which was updated.
	 * @return
	 */
	bool shouldPulse(size_t inputIndex, size_t updatedInputIndex);

public:
	//===-- Values updating functions. ----------------------------------------===//
	/**
	 * Computes new values of outputs based on inputs.
	 *
	 * Encodes the box function by updating the output values for a
	 * given \a inputIndex - fired by receiveSignal().
	 *
	 * The base class method calls update callbacks registered with
	 * addUpdateCallback(). So derived methods should also call their base method
	 * if they want to inform any outside observers.
	 *
	 * \param	inputIndex Index of the modified input.
	 */
	virtual void updateValues(int inputIndex = 0);

	/// Spread signal to all outputs.
	/// \todo Does not use operators for calling each follower just once
	void spreadSignal();

	/// Spread signal to the selected output \a outIndex only.
	void spreadSignal(size_t outIndex);

	/**
	 * Implements the operator reaction to the change of its \a inputIndex input.
	 *
	 * 1. Forces the operator to recompute its outputs based on the changed input
	 * \a inputIndex by calling updateValues() and <BR>
	 * 2. Spread signal to the connected children by spreadSignal().
	 *
	 * This method is called by the spreadSignal() of the parent box in the
	 * scene graph. The computation may be restricted by internal bool value
	 * \a restrictedOutput to a single previously defined output index \a
	 * restrictedOutputIndex.
	 *
	 * \param	inputIndex	Index of the input that was changed and that forces the
	 * operator to recompute its outputs.
	 */
	virtual void receiveSignal(int inputIndex);
	//===----------------------------------------------------------------------===//

	bool areInputsPlugged(int numInputs);
	bool areAllInputsPlugged();
	bool areAllInputsUnplugged() const;

	const char* getLabel() const
	{
		return m_operation.defaultLabel.c_str();
	}

	/// "{node type} #{node ID}"
	std::string getSignature()
	{
		if (m_owner)
		{
			return fmt::format("{} #{}", m_owner->m_operation.keyWord, m_owner->m_id);
		}

		return fmt::format("{} #{}", m_operation.keyWord, m_id);
	}

	TrackedNodeData* getTrackingData()
	{
		return m_trackingData;
	}

private:
	// TODO: (DR) Callbacks cannot be unregistered! That could cause issues when lifetime of listener ends before the
	//   dispatcher.
	//   Callbacks are very primitively implemented using std::function, std::function instances cannot be compared and
	//   thus can't be unregistered. Possible fix is to pack std::function into a listener object that also contains
	//   some id or pointer to the callback owner and compare those

	std::list<std::function<void(Node*)>> m_updateCallbacks;
	std::list<std::function<void(Node*)>> m_deleteCallbacks;
	std::list<std::function<void(Node*, Node*, size_t, size_t)>> m_plugCallbacks;
	std::list<std::function<void(Node*, Node*, size_t, size_t)>> m_unplugCallbacks;

protected:
	virtual void onPlug(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);
	virtual void onUnplug(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);
	virtual void onUpdate();

	void triggerUpdateCallback(Node* node);
	void triggerDeleteCallback(Node* node);
	void triggerPlugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);
	void triggerUnplugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);

public:
	/**
	 * Registers a callback that gets called on any updateValues() call.
	 * <br><br>
	 * The callback parameters:<br>
	 * Node* = node that has been updated<br>
	 * @note The callback cannot be removed, meaning they are meant to last for the entire node lifetime.
	 */
	virtual void addUpdateCallback(std::function<void(Node*)> callback);

	/**
	 * <br><br>
	 * The callback parameters:<br>
	 * @note The callback cannot be removed, meaning they are meant to last for the entire node lifetime.
	 * Node* = node that has been deleted<br>
	 */
	virtual void addDeleteCallback(std::function<void(Node*)> callback);

	/**
	 * Registers a callback that gets called when the node's output pins get plugged in.
	 * <br><br>
	 * The callback parameters:<br>
	 * Node* = Start node<br>
	 * Node* = End node<br>
	 * size_t = Start node's output pin index<br>
	 * size_t = End node's input pin index<br>
	 * @note The callback cannot be removed, meaning they are meant to last for the entire node lifetime.
	 */
	virtual void addPlugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback);

	/**
	 * Registers a callback that gets called when the node's input pins get unplugged.
	 * <br><br>
	 * The callback parameters:<br>
	 * Node* = Start node<br>
	 * Node* = End node<br>
	 * size_t = Start node's output pin index<br>
	 * size_t = End node's input pin index<br>
	 * @note The callback cannot be removed, meaning they are meant to last for the entire node lifetime.
	 */
	virtual void addUnplugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback);

protected:
	ENodePlugResult isPlugCorrect(const Pin& input, const Pin& output);

private:
	void unplugAll();
	void unplugInput(size_t index);
	void unplugOutput(size_t index);

protected:
	ID m_id{};

	/// Operator node properties.
	Operation m_operation;

	/// Inputs of the box: Input tabs with glyphs.
	std::vector<Pin> m_inputs;

	/// Outputs of the box: output tabs with glyphs.
	std::vector<Pin> m_outputs;

	/// Results of operations.
	std::vector<Data> m_internalData;

	/// Owner of the node, used in complex type of nodes, such as sequence or
	/// camera.
	Node* m_owner = nullptr;

	/// Nested nodes.
	std::vector<Node*> m_children;

	/// Reference to additional tracking data if applicable, nullptr indicates no active tracking operation.
	/// This pointer is managed by an active MatrixTracker.
	TrackedNodeData* m_trackingData{nullptr};

	bool finalized = false; /// Internal helper flag to keep track of Node::finalize() calls
};
} // namespace Core
