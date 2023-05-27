/**
 * \file Core/Node.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
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

struct ValueSetResult
{
	enum class Status
	{
		Ok = 0,
		Err_ConstraintViolation,
		Err_LogicError
	};

	Status status = Status::Ok;
	std::string message;

	ValueSetResult() = default;
	ValueSetResult(Status aStatus, std::string aMessage = "") : status(aStatus), message(std::move(aMessage)) {}
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
class Node : public ICloneable<Node>, public std::enable_shared_from_this<Node>
{
	friend class GraphManager;
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
	Node(const Operation* operation) : m_operation(operation) {}

	/**
	 * Delete node.
	 *
	 * \pre All inputs and outputs must be unplugged (calls Node::finalize
	 * function)!
	 */
	virtual ~Node() = default;

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
	 *
	 * \todo MH Override in derived classes (Sequence).
	 */
	void init();

	virtual void onInit() {}

	/**
	 * Prepares node for its destruction, after that the destructor can be called.
	 */
	void finalize();

	/// Connect this node's output pin to another node's input pin.
	///
	/// \return Result enum is returned from the function. \see ENodePlugResult.
	ENodePlugResult plug(const Ptr<Node>& rightNode, unsigned fromIndex, unsigned toIndex);

	//===-- Helper functions --------------------------------------------------===//

	/**
	 * \return Application unique ID.
	 */
	ID getId() const;

	const Operation* getOperation() const { return m_operation; }

	/**
	 * Get reference to this node instance.
	 *
	 * Note that this operation may be slower. A new shared pointer must be
	 * created, it obtains atomic counter increment.
	 */
	Ptr<Node> getPtr() { return shared_from_this(); }

	template <typename T> Ptr<T> as()
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node class.");
		I3T_ASSERT(std::dynamic_pointer_cast<T>(shared_from_this()), "Cannot cast to Ptr<T>.");

		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	//===----------------------------------------------------------------------===//
	/// \todo Break chain Node -> Transform.
	/// \todo This is pin related, move to Pin class.
	std::vector<EValueState> m_OperatorState;

	EValueState getState(size_t pinIndex = 0);
	//===----------------------------------------------------------------------===//

	Pin& getInput(size_t i) { return m_inputs[i]; }
	Pin& getOutput(size_t i) { return m_outputs[i]; }

	std::vector<Pin>& getInputPins() { return m_inputs; }
	std::vector<Pin>& getOutputPins() { return m_outputs; }

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
	const Data& getData(size_t index = 0) { return getInternalData(index); }

	/// \todo MH Replace getData with this function.
	const Data& data(size_t index = 0) { return getData(index); }

public:
	/// Get direct owner of this node.
	///
	/// \returns nullptr if owner is not set.
	Ptr<Node> getOwner() const;

	/// \todo Does it needs to be public?
	void setOwner(Node* owner) { m_owner = owner; }

	/// Get the topmost owner of this node.
	Ptr<Node> getRootOwner();

	void notifyOwner();

public:
	[[nodiscard]] virtual ValueSetResult setValue(void* ptr)
	{
		m_internalData[0].setValue(ptr);
		spreadSignal();

		return ValueSetResult{};
	}

	/**
	 * Set the value of the node.
	 * Sets the value of the first float of DataStore. Derived types may override
	 * the default behavior.
	 *
	 * \param val
	 */
	[[nodiscard]] virtual ValueSetResult setValue(float val) { return setValueEx(val); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec3& vec) { return setValueEx(vec); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec4& vec) { return setValueEx(vec); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::quat& q) { return setValueEx(q); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::mat4& mat) { return setValueEx(mat); }

	/**
	 * \param val new value
	 * \param coords in the column major order: coords.x is the column index and
	 * coords.y is the row index. \return
	 */
	[[nodiscard]] virtual ValueSetResult setValue(float val, glm::ivec2 coords)
	{
		setInternalValue(val, coords);
		return ValueSetResult{};
		// return ValueSetResult{ValueSetResult::Status::Err_LogicError,
		// "Unsupported operation on non transform object."};
	}

	/**
	 * Smart set function, used with constrained transformation for value
	 * checking. \param mat \param map array of 16 chars.
	 */
	[[nodiscard]] virtual ValueSetResult setValue(const glm::mat4& mat, const DataMap& map)
	{
		return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation on non transform object."};
	}

	template <typename T> [[nodiscard]] ValueSetResult setValue(const T& value, unsigned index)
	{
		return setValueEx(value, index);
	}

private:
	/// Sets value of pin at \p index position.
	template <typename T> ValueSetResult setValueEx(T&& val, unsigned index = 0)
	{
		setInternalValue(val, index);
		return ValueSetResult{};
	}

protected:
	/**
	 * Sets the node value without validation.
	 * \tparam T Value type, no need to specify it in angle brackets, it will be
	 * deduced by the compiler (C++17).
	 * \param value Value to set.
	 * \param index Index of the DataStore (if the node stores more than one value)
	 */
	template <typename T> void setInternalValue(const T& value, size_t index = 0)
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
	bool shouldPulse(size_t inputIndex, size_t outputIndex);

public:
	/// \todo MH will be removed.
	static const DataMap* getDataMap();

	/// \todo MH will be removed.
	static const DataMap& getDataMapRef();

public:
	//===-- Values updating functions. ----------------------------------------===//
	/**
	 * Computes new values of outputs based on inputs.
	 *
	 * Encodes the box function by updating the output values for a
	 * given \a inputIndex - fired by receiveSignal().
	 *
	 * \todo Is it correct also for multiple edges? If the edges from one module
	 * are connected to more than one input, and the implementation of
	 * updateValues uses the \a inputIndex, only subset of outputs may be updated.
	 * PF: This method is intended for complex modules to allow for optimization.
	 * May be replaced by updateValues() or implemented in such a way. Do such
	 * optimize-able modules exist?
	 *
	 * The base class method calls update callbacks registered with
	 * addUpdateCallback(). So derived methods should also call their base method
	 * if they want to inform any outside observers.
	 *
	 * \param	inputIndex Index of the modified input (output pin).
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

	const char* getLabel() const { return m_operation->defaultLabel.c_str(); }

	/// "{node type} #{node ID}"
	std::string getSignature()
	{
		if (m_owner)
		{
			return fmt::format("{} #{}", m_owner->m_operation->keyWord, m_owner->m_id);
		}

		return fmt::format("{} #{}", m_operation->keyWord, m_id);
	};

protected:
	// TODO: (DR) Callbacks cannot be unregistered! That could cause issues when lifetime of listener ends before the
	//   dispatcher.
	//   Callbacks are very primitively implemented using std::function, std::function instances cannot be compared and
	//   thus can't be unregistered. Possible fix is to pack std::function into a listener object that also contains some
	//   id or pointer to the callback owner and compare those

	std::list<std::function<void(Node*)>> m_updateCallbacks;
	std::list<std::function<void(Node*)>> m_deleteCallbacks;
	std::list<std::function<void(Node*, Node*, size_t, size_t)>> m_plugCallbacks;
	std::list<std::function<void(Node*, Node*, size_t, size_t)>> m_unplugCallbacks;

	void triggerUpdateCallback(Node* node);
	void triggerDeleteCallback(Node* node);
	void triggerPlugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);
	void triggerUnplugCallback(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex);

public:
	/**
	 * Registers a callback that gets called on any updateValues() call.
	 * Note that some derived nodes might not always call this callback.
	 * <br><br>
	 * The callback parameters:<br>
	 * Node* = node that has been updated<br>
	 */
	virtual void addUpdateCallback(std::function<void(Node*)> callback);

	/**
	 * <br><br>
	 * The callback parameters:<br>
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
	 */
	virtual void addUnplugCallback(std::function<void(Node*, Node*, size_t, size_t)> callback);

protected:
	virtual ENodePlugResult isPlugCorrect(const Pin& input, const Pin& output);

private:
	void unplugAll();
	void unplugInput(size_t index);

private:
	void unplugOutput(size_t index);

protected:
	virtual void onUnplugInput(size_t index) {}

	ID m_id{};

	/// Operator node properties.
	const Operation* m_operation = nullptr;

	/// Inputs of the box: Input tabs with glyphs.
	std::vector<Pin> m_inputs;

	/// Outputs of the box: output tabs with glyphs.
	std::vector<Pin> m_outputs;

	/// Results of operations.
	std::vector<Data> m_internalData;

	/// Owner of the node, used in complex type of nodes, such as sequence or
	/// camera.
	Node* m_owner = nullptr;
};
} // namespace Core
