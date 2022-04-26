#pragma once

#include <map>

#include "Node.h"

#if defined(WIN32)
#undef far
#undef near
#endif

namespace Core
{
FORCE_INLINE bool isTransform(const NodePtr& node)
{
	/*
	auto it = std::find_if(g_transforms.begin(), g_transforms.end(),
	                       [&node](const std::pair<Operation, std::map<std::string, EValueType>>& pair) { return pair.first.keyWord == node->getOperation()->keyWord; });
	return it != g_transforms.end();
	 */
	auto& keyWord = node->getOperation()->keyWord;
	auto	type		= magic_enum::enum_cast<ETransformType>(keyWord);

	return type.has_value();
}

template <typename Node>
FORCE_INLINE bool isRot(Node&& node)
{
	// static_assert(std::is_base_of_v<NodeBase, Node>);

	auto& type = node->getOperation()->keyWord;
	return type == "EulerX" || type == "EulerY" || type == "EulerZ" || type == "AxisAngle";
}

enum class ETransformState
{
	Invalid = 0,
	Valid,
	Unknown
};

//===-- Value masks -------------------------------------------------------===//

using ValueMask = std::array<uint8_t, 16>;

constexpr uint8_t VM_ZERO = 0;
constexpr uint8_t VM_ONE  = 1;
constexpr uint8_t VM_ANY  = 2;

inline bool canEditValue(EValueState valueState)
{
	return valueState == EValueState::Editable || valueState == EValueState::EditableSyn;
}

bool validateValue(const ValueMask& mask, glm::ivec2 coords, float value);
bool validateValues(const ValueMask& mask, const glm::mat4& matrix);

//===-- Base Transform class ----------------------------------------------===//

class Transformation : public Node
{
	friend class GraphManager;
	friend class Storage;

	using DefaultValues = std::map<std::string, Data>;

public:
	explicit Transformation(const TransformOperation& transformType);

	//===-- Construct functions -----------------------------------------------===//

	void createDefaults();

	virtual void initDefaults() {}

	//===----------------------------------------------------------------------===//

private:
	/// \todo MH Use Node::m_owner.
	Ptr<NodeBase> m_currentSequence = nullptr;
	int						m_currentIndex		= -1;

public:
	bool					isInSequence() { return m_currentSequence != nullptr; }
	Ptr<NodeBase> getCurrentSequence() { return m_currentSequence; }
	int						getCurrentIndex() const { return m_currentIndex; }

	//===----------------------------------------------------------------------===//

	/// Get default value which transform can hold.
	const Data& getDefaultValue(const std::string& name) const;

	/** You can find transform default values names and types at the file Core/Nodes/Operation.h. */
	template <typename T>
	void setDefaultValue(const std::string& name, T&& val)
	{
		I3T_ASSERT(m_defaultValues.find(name) != m_defaultValues.end() && "Default value with this name does not exist.");

		m_defaultValues.at(name).setValue(val);
		reset();
	}

	/**
	 * \return A map of valueName and value pairs.
	 */
	TransformOperation::ValueMap getDefaultTypes();
	DefaultValues&               getDefaultValues();

	void setDefaultValues(const DefaultValues& values) { m_defaultValues = values; }

	EValueState getValueState(glm::ivec2 coords);

	//===----------------------------------------------------------------------===//

	// ValueSetResult setValue(float val, glm::ivec2 coords) override;
	// virtual ValueSetResult onSetValue(float val, glm::ivec2 coords) {}

	virtual ETransformState isValid() const { return ETransformState::Unknown; }
	bool										isLocked() const;
	void										lock();
	void										unlock();
	bool										hasSynergies() const { return m_hasEnabledSynergies; }
	void										disableSynergies() { m_hasEnabledSynergies = false; }
	void										enableSynergies() { m_hasEnabledSynergies = true; }
	void										free()
	{
		unlock();
		disableSynergies();
	}

	void reset() override
	{
		onReset();
		notifySequence();
	}

	/// \todo JH When setting X value in non-uniform scale -> this switch to uniform scale (due to enable synergies)
	virtual void onReset() {}

	//===----------------------------------------------------------------------===//

	bool hasSavedValue() const { return m_hasSavedData; }

	/** Save current values of the transformation for future reloading. */
	void saveValue();

	/** Restore saved values if they exist. */
	void reloadValue();

	const glm::mat4& getSavedValue() const;

	void setSavedValue(const glm::mat4& values);

	//===----------------------------------------------------------------------===//

	void resetModifiers()
	{
		m_isLocked						= true;
		m_hasEnabledSynergies = true;
	}

	ValueSetResult setValue(const glm::mat4& mat) override;
	ValueSetResult setValue(float, glm::ivec2) override { return ValueSetResult{}; }

	//===----------------------------------------------------------------------===//
	struct
	{
		float cos;
		float sin;
	} halfspaceSign;   // remember the quadrant for eulerRotations
	//===----------------------------------------------------------------------===//

	void notifySequence();

protected:
	bool canSetValue(const ValueMask& mask, glm::ivec2 coords, float value);

public:
	/// \todo MH these should not be public.
	void nullSequence()
	{
		m_currentSequence = nullptr;
		m_currentIndex		= -1;
	}

	void setSequence(Ptr<NodeBase>&& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex		= index;
	}

	void setSequence(Ptr<NodeBase>& s, int index)
	{
		m_currentSequence = s;
		m_currentIndex		= index;
	}

protected:
	DefaultValues m_defaultValues;

	bool m_hasEnabledSynergies = true;
	bool m_isLocked						 = true;

private:
	bool      m_hasSavedData = false;
	DataStore m_savedData;
};

using TransformPtr = Ptr<Transformation>;
}
