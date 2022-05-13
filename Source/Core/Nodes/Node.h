/**
 * \file Core/Node.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
 *
 * Code taken from (former) Source/operator.h, Source/operatorForm.h and Source/operatorNode.h
 * file which used to define Operator, base class for all boxes.
 */
#pragma once

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
constexpr inline size_t MAX_NODES_COUNT = 1024;
}

enum class ENodePlugResult
{
	Ok = 0,
	Err_MismatchedPinTypes,
	Err_MismatchedPinKind, /* \todo JH snad to tu tím Martinovi nijak nerozbiju :-) ... */
	Err_Loopback,					 /// Same nodes.
	Err_NonexistentPin,
	Err_Loop,
	Err_DisabledPin
};

struct ValueSetResult
{
	enum class Status
	{
		Ok = 0,
		Err_ConstraintViolation,
		Err_LogicError
	};

	Status			status;
	std::string message;

	ValueSetResult() : status(Status::Ok), message("") {}

	explicit ValueSetResult(Status aStatus, std::string aMessage = "") : status(aStatus), message(std::move(aMessage)) {}
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

namespace Core
{
class Pin;

/**
 * Base class interface for all boxes.
 */
class Node : public ICloneable<Node>, public std::enable_shared_from_this<Node>
{
	friend class GraphManager;

public:
	struct PinView
	{
		enum class EStrategy
		{
			Output,
			Input
		};

		PinView() = default;
		PinView(EStrategy strategy, Ptr<Node> node)
		{
			this->strategy = strategy;
			this->node     = node;
		}

		PinView(EStrategy strategy, Ptr<Node> node, int index)
		{
			this->strategy = strategy;
			this->node     = node;
			this->index    = index;
		}

		PinView begin() const;
		PinView end() const;

		size_t size() const;
		bool   empty() const;

		PinView& operator++();
		const Pin& operator*();
		bool operator==(const PinView& view) const;
		bool operator!=(const PinView& view) const;

		Pin& operator[](size_t i);
		const Pin& operator[](size_t i) const;

	private:
		EStrategy strategy;
		Ptr<Node> node;
		int index;
	};

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
	 * \pre All inputs and outputs must be unplugged (calls Node::finalize function)!
	 */
	virtual ~Node();

public:
	/**
	 * Initialize node inputs and outputs according to preset node type.
	 *
	 * Called in create node function.
	 *
	 * \todo MH Override in derived classes (Sequence).
	 */
	void init();

	/**
	 * Prepares node for its destruction, after that destructor can be called.
	 */
	void finalize();

	//===-- Helper functions --------------------------------------------------===//

	/**
	 * \return Application unique ID.
	 */
	ID getId() const;

	/**
	 * @param newId Make sure the new ID is not used by another node.
	 */
	void changeId(ID newId);

	const Operation* getOperation() const { return m_operation; }

	/**
	 * Get reference to this node instance.
	 *
	 * Note that this operation may be slower. A new shared pointer must be created,
	 * it obtains atomic counter increment.
	 */
	Ptr<Node> getPtr() { return shared_from_this(); }

	template <typename T>
	Ptr<T> as()
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from NodeBase class.");
		I3T_ASSERT(std::dynamic_pointer_cast<T>(shared_from_this()) && "Cannot cast to Ptr<T>.");

		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	//===----------------------------------------------------------------------===//
	/// \todo Break chain Node -> Transform.
	std::vector<EValueState> m_OperatorState;

	EValueState getState(size_t pinIndex = 0);
	//===----------------------------------------------------------------------===//

	virtual Pin& getIn(size_t i) { return m_inputs[i]; }
	virtual Pin& getOut(size_t i) { return m_outputs[i]; }

	/// \deprecated Will be removed
	const Pin& getInPin(int index) { return getInputPins()[index]; }

	/// \deprecated Will be removed
	const Pin& getOutPin(int index) { return getOutputPins()[index]; }

	[[nodiscard]] PinView getInputPins()  { return PinView(PinView::EStrategy::Input, shared_from_this()); }
	[[nodiscard]] PinView getOutputPins() { return PinView(PinView::EStrategy::Output, shared_from_this()); }

protected:
	[[nodiscard]] PinView getInputPinsRef()  { return PinView(PinView::EStrategy::Input, shared_from_this()); }
	[[nodiscard]] PinView getOutputPinsRef() { return PinView(PinView::EStrategy::Output, shared_from_this()); }

	//===-- Obtaining value functions. ----------------------------------------===//
	/**
	 * Get data storage for read and write purposes. No written value validation
	 * is performed.
	 *
	 * Overridden in Sequence class.
	 */
public:
	/// \todo Make this function non public.
	virtual DataStore& getInternalData(size_t index = 0)
	{
		assert(index < m_internalData.size() && "Desired data storage does not exist!");

		return m_internalData[index];
	}

	/**
	 * Get Node contents, read only.
	 * \param index Index of the internal modifiable data field (e.g, 0 or 1 for two vectors).
	 *              Value of field[0] is returned if this parameter omitted)
	 * \return Struct which holds data
	 */
	const DataStore& getData(size_t index = 0) { return getInternalData(index); }

public:
	void setOwner(Ptr<Node> owner) { m_owner = owner; }

	void notifyOwner();

public:
	[[nodiscard]] virtual ValueSetResult setValue(void* ptr)
	{
		m_internalData[0].setValue(ptr);
		spreadSignal();

		return ValueSetResult{};
	}

	/**
	 * Set a value of node.
	 *
	 * Sets value of the first float of DataStore. Derived types may override
	 * default behaviour.
	 *
	 * \param val
	 */
	[[nodiscard]] virtual ValueSetResult setValue(float val)            { return setValueEx(val); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec3& vec) { return setValueEx(vec); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec4& vec) { return setValueEx(vec); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::quat& q)   { return setValueEx(q); }
	[[nodiscard]] virtual ValueSetResult setValue(const glm::mat4& mat) { return setValueEx(mat); }

	/**
	 *
	 * \param val
	 * \param coords in column major order, coords.x is column index and coords.y is row index.
	 * \return
	 */
	[[nodiscard]] virtual ValueSetResult setValue(float val, glm::ivec2 coords)
	{
		setInternalValue(val, coords);
		return ValueSetResult{};
		// return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation on non transform object."};
	}

	/**
	 * Smart set function, used with constrained transformation for value checking.
	 *
	 * \param mask array of 16 chars.
	 * \param mat
	 */
	[[nodiscard]] virtual ValueSetResult setValue(const glm::mat4& mat, const Transform::DataMap& map)
	{
		return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Unsupported operation on non transform object."};
	}

	template <typename T>
	[[nodiscard]] ValueSetResult setValue(const T& value, unsigned index)
	{
		return setValueEx(value, index);
	}

private:
	/// Sets value of pin at \p index position.
	template <typename T>
	ValueSetResult setValueEx(T&& val, unsigned index = 0)
	{
		setInternalValue(val, index);
		return ValueSetResult{};
	}

protected:
	/**
	 * Sets node value without validation.
	 * \tparam T Value type, no need to specify it in angle brackets, it will be deduced
	 *    by compiler (C++17).
	 * \param value Value to set.
	 * \param index Index of DataStore (if the node stores more than one value)
	 */
	template <typename T>
	void setInternalValue(const T& value, size_t index = 0)
	{
		getInternalData(index).setValue(value);
		spreadSignal(index);

		/// \todo MH
		// if (m_owner)
			// m_owner->updateValues(-1);
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
	virtual void reset() {}

	/// \todo MH will be removed.
	static const Transform::DataMap* getDataMap();
	static const Transform::DataMap& getDataMapRef();

public:
	//===-- Values updating functions. ----------------------------------------===//
	/**
	 * Computes new values of outputs based on inputs.
	 *
	 * Encodes the box function by updating the output values for a
	 * given \a inputIndex - fired by receiveSignal().
	 *
	 * \todo Is it correct also for multiple edges? If the edges from one module are connected to more than one input,
	 * and the implementation of updateValues uses the \a inputIndex, only subset of outputs may be updated.
	 * PF: This method is intended for complex modules to allow for optimization.
	 * May be replaced by updateValues() or implemented in such a way. Do such optimizable modules exist?
	 *
	 * \param	inputIndex Index of the modified input (output pin).
	 */
	virtual void updateValues(int inputIndex = 0) {}

	/// Spread signal to all outputs.
	/// \todo Does not use operators for calling each follower just once
	void spreadSignal();

	/// Spread signal to the selected output \a outIndex only.
	void spreadSignal(size_t outIndex);

	/**
	 * Implements the operator reaction to the change of its \a inputIndex input.
	 *
	 * 1. Forces the operator to recompute its outputs based on the changed input \a
	 *    inputIndex by calling updateValues() and <BR>
	 * 2. Spread signal to the connected children by spreadSignal().
	 *
	 * This method is called by the spreadSignal() of the parent box in the
	 * scene graph. The computation may be restricted by internal bool value
	 * \a restrictedOutput to a single previously defined output index \a restrictedOutputIndex.
	 *
	 * \param	inputIndex	Index of the input that was changed and that forces the operator to recompute its outputs.
	 */
	virtual void receiveSignal(int inputIndex);
	//===----------------------------------------------------------------------===//

	bool areInputsPlugged(int numInputs);
	bool areAllInputsPlugged();

	const char* getLabel() const { return m_operation->defaultLabel.c_str(); }

	/**
	 * \warning Only for test purposes. May be removed anytime.
	 */
	std::string getSig()
	{
		std::string masterSig;

		if (m_owner)
		{
			masterSig = " of (" + m_owner->getSig() + ")";
		}

		return fmt::format("{}#{}{}", m_operation->keyWord, m_id, masterSig);
	};

protected:
	virtual ENodePlugResult isPlugCorrect(Pin const* input, Pin const* output);

private:
	void unplugAll();
	void unplugInput(size_t index);

private:
	void onUnplugInput(size_t index);

	void unplugOutput(size_t index);

protected:
	ID m_id{};

	/// Operator node properties.
	const Operation* m_operation = nullptr;

	/// Inputs of the box: Input tabs with glyphs.
	std::vector<Pin> m_inputs;

	/// Outputs of the box: output tabs with glyphs.
	std::vector<Pin> m_outputs;

	/// Results of operations.
	std::vector<DataStore> m_internalData;

	/**
	 * Owner of the node, used in complex type of nodes, such as sequence or camera.
	 */
	Ptr<Node> m_owner = nullptr;
};

using NodePtr  = Ptr<Node>;

/// \warning Will be removed, use Node type instead.
using NodeBase = Node;
} // namespace Core