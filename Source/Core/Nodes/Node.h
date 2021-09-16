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
#include "Id.h"
#include "NodeData.h"
#include "Operations.h"

enum class ENodePlugResult
{
	Ok = 0,
	Err_MismatchedPinTypes,
	Err_MismatchedPinKind, /* \todo JH snad to tu tím Martinovi nijak nerozbiju :-) ... */
	Err_Loopback,					 /// Same nodes.
	Err_NonexistentPin,
	Err_Loop,
};

struct ValueSetResult
{
	enum class Status
	{
		Ok = 0,
		Err_ConstraintViolation,
		Err_LogicError
	};

	const Status			status;
	const std::string message;

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
class NodeVisitor;

/**
 * Base class interface for all boxes.
 *
 * \todo Decompose class to smaller interfaces.
 */
class NodeBase : public std::enable_shared_from_this<NodeBase>
{
	friend class GraphManager;

public:
	virtual Pin& getIn(size_t i) { return m_inputs[i]; }
	virtual Pin& getOut(size_t i) { return m_outputs[i]; }

protected:
	ID m_id{};

	/// Outputs of the box: output tabs with glyphs.
	std::vector<Pin> m_outputs;

	/// Results of operations.
	std::vector<DataStore> m_internalData;

	const Transform::DataMap* m_initialMap{};
	const Transform::DataMap* m_currentMap = &Transform::g_AllLocked;

	/**
	 * Owner of the node, sequence or camera, otherwise null.
	 */
	Ptr<NodeBase> m_owner = nullptr;

	/**
	 * Operator node properties.
	 */
	const Operation* m_operation = nullptr;

	/// Inputs of the box: Input tabs with glyphs.
	std::vector<Pin> m_inputs;

protected:
	NodeBase(const Operation* operation) : m_operation(operation) {}

public:
	/** Delete node and unplug its all inputs and outputs. */
	virtual ~NodeBase();

	const Pin& getInPin(int index) { return getInputPins()[index]; }
	const Pin& getOutPin(int index) { return getOutputPins()[index]; }

protected:
	Pin& getInPinRef(int index) { return getInputPinsRef()[index]; }
	Pin& getOutPinRef(int index) { return getOutputPinsRef()[index]; }

public:
	Ptr<NodeBase> getPtr() { return shared_from_this(); }

	template <typename T>
	Ptr<T> as()
	{
		static_assert(std::is_base_of_v<NodeBase, T>, "T must be derived from NodeBase class.");
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	/**
	 * Initialize node inputs and outputs according to preset node type.
	 *
	 * Called in create node function.
	 *
	 * \todo MH Override in derived classes (Sequence).
	 */
	void init();

	[[nodiscard]] ID getId() const;

	const Operation* const getOperation() { return m_operation; }

	//===-- Obtaining value functions. ----------------------------------------===//
protected:
	/**
	 * Get data storage for read and write purposes. No written value validation
	 * is performed.
	 *
	 * Overridden in Sequence class.
	 */
	virtual DataStore& getInternalData(size_t index = 0)
	{
		assert(index < m_internalData.size() && "Desired data storage does not exist!");

		return m_internalData[index];
	}

public:
	/**
	 * Get Node contents, read only.
	 * \param index Index of the internal modifiable data field (e.g, 0 or 1 for two vectors).
	 *              Value of field[0] is returned if this parameter omitted)
	 * \return Struct which holds data
	 */
	const DataStore& getData(size_t index = 0) { return getInternalData(index); }

private:
	template <typename T>
	ValueSetResult setValueEx(T&& val)
	{
		if (m_currentMap == &Transform::g_AllLocked)
			return ValueSetResult{ValueSetResult::Status::Err_LogicError, "Values are locked."};

		setInternalValue(val);

		return ValueSetResult{};
	}

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
	[[nodiscard]] virtual ValueSetResult setValue(float val) { return setValueEx(val); }

	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec3& vec) { return setValueEx(vec); }

	[[nodiscard]] virtual ValueSetResult setValue(const glm::vec4& vec) { return setValueEx(vec); }

	[[nodiscard]] virtual ValueSetResult setValue(const glm::quat& q) { return setValueEx(q); }

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

	void pulse(size_t index);

protected:
	void setPinOwner(Pin& pin, Ptr<NodeBase> node);

	bool shouldPulse(size_t inputIndex, size_t outputIndex);

	/**
	 * Sets node value without validation.
	 * \tparam T Value type, no need to specify it in angle brackets, it will be deduced
	 *    by compiler.
	 * \param value Value to set.
	 * \param index Index of DataStore (if the node stores more than one value)
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
	virtual void reset() {}

	void setDataMap(const Transform::DataMap* map);

	const Transform::DataMap* getDataMap() { return m_currentMap; }

	/// \todo MH will be removed.
	const Transform::DataMap&																	 getDataMapRef() { return *m_currentMap; }
	[[nodiscard]] const std::vector<const Transform::DataMap*> getValidDataMaps() { return m_operation->validDatamaps; };

	class Strategy;

	//===----------------------------------------------------------------------===//
	/**
	 * Class which composes
	 */
	class PinView
	{
		using value			= Pin;
		using pointer		= Pin*;
		using reference = Pin&;

	public:
		class Iterator
		{
		public:
			Iterator(Ptr<Strategy> strategy, Ptr<NodeBase> node, int index);

			reference		operator*() const;
			pointer			operator->();
			Iterator&		operator++();
			Iterator		operator++(int);
			Iterator&		operator=(const Iterator&) = default;
			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_index == b.m_index; };
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_index != b.m_index; };

		protected:
			int						m_index;
			Ptr<NodeBase> m_node;
			Ptr<Strategy> m_strategy;
		};

	public:
		PinView(Ptr<Strategy> strategy, Ptr<NodeBase> node) : m_strategy(strategy), m_node(node){};
		Pin&		 operator[](size_t i) const;
		Iterator begin() const;
		Iterator end() const;
		bool		 empty() const;
		size_t	 size() const;

	private:
		Ptr<NodeBase> m_node;
		Ptr<Strategy> m_strategy;
	};


	class Strategy : public std::enable_shared_from_this<Strategy>
	{
	public:
		Strategy(Ptr<NodeBase> node) : m_node(node) {};
		Ptr<Strategy> getThis() { return shared_from_this(); }

		virtual PinView::Iterator begin()						= 0;
		virtual PinView::Iterator end()							= 0;
		virtual bool							empty()						= 0;
		virtual size_t						size()						= 0;
		virtual Pin&							get(size_t index) = 0;

	protected:
		Ptr<NodeBase> m_node;
	};

	class InputStrategy : public Strategy
	{
	public:
		InputStrategy(Ptr<NodeBase> node) :
				Strategy(node) {}

		virtual PinView::Iterator begin();
		virtual PinView::Iterator end();
		virtual bool							empty();
		virtual size_t						size();
		virtual Pin&							get(size_t index);
	};

	class OutputStrategy : public Strategy
	{
	public:
		OutputStrategy(Ptr<NodeBase> node) :
				Strategy(node) {}

		virtual PinView::Iterator begin();
		virtual PinView::Iterator end();
		virtual bool							empty();
		virtual size_t						size();
		virtual Pin&							get(size_t index);
	};

	[[nodiscard]] const PinView getInputPins();
	[[nodiscard]] const PinView getOutputPins();

protected:
	[[nodiscard]] PinView getInputPinsRef();
	[[nodiscard]] PinView getOutputPinsRef();

	public:
	//===----------------------------------------------------------------------===//

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
	std::string getSig()
	{
		std::string masterSig;

		if (m_owner)
		{
			masterSig = " of (" + m_owner->getSig() + ")";
		}

		return fmt::format("{}#{}{}", m_operation->keyWord, m_id, masterSig);
	};

	virtual void accept(NodeVisitor& visitor);

protected:
	virtual ENodePlugResult isPlugCorrect(Pin const* input, Pin const* output);

private:
	void unplugAll();
	void unplugInput(size_t index);
	void unplugOutput(size_t index);
};

using Node		= NodeBase;
using NodePtr = Ptr<Node>;


/**
 * Pin used for connecting nodes.
 *
 * OperatorCurveTab from I3T v1.
 */
class Pin
{
	friend class GraphManager;
	friend class NodeBase;

	/// \todo MH do not access pin directly.
	friend class Sequence;

	ID m_id;

	/// Index within a node.
	int m_index = -1;

	/// Pin type.
	const bool m_isInput;

	/// Owner of the pin.
	Ptr<NodeBase> m_master;

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

public:
	Pin(EValueType valueType, bool isInput, Ptr<NodeBase> owner, int index) :
			m_valueType(valueType), m_isInput(isInput), m_master(owner), m_index(index)
	{
		m_id = IdGenerator::next();
	}

	[[nodiscard]] ID getId() const { return m_id; }

	[[nodiscard]] int getIndex() const { return m_index; }

	[[nodiscard]] NodePtr getOwner() const { return m_master; };

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
	 * Get stored data based on pin type.
	 *
	 * \returns data storage owner by node connected to this input pin. If pin is output pin,
	 *          it returns data storage of pin owner.
	 */
	[[nodiscard]] const DataStore& getStorage(unsigned id = 0);

	const char* getLabel() const
	{
		auto*				op		= m_master->getOperation();
		const char* label = nullptr;

		if (m_isInput)
		{
			if (!op->defaultInputNames.empty()) { label = op->defaultInputNames[m_index].c_str(); }
		}
		else
		{
			if (!op->defaultOutputNames.empty()) { label = op->defaultOutputNames[m_index].c_str(); }
		}

		if (label == nullptr) { label = defaultIoNames[static_cast<size_t>(m_valueType)]; }

		return label;
	}

	std::string getSig()
	{
		return fmt::format("{} [{}, index: {}]", valueTypeToString(m_valueType), m_master->getSig(), m_index);
	};

	[[nodiscard]] EValueType getType() const { return m_valueType; }

	/**
	 * Query if input of this object is plugged to any parent output.
	 *
	 * \return True if plugged to parent, false if not.
	 */
	[[nodiscard]] bool isPluggedIn() const { return m_input != nullptr; }

	[[nodiscard]] bool isInput() const { return m_isInput; }
};
} // namespace Core
