/**
 * \file Core/Transform.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
 *
 * Specialization of node for transformations (4x4 matrices and quaternions)
 *
 */
#pragma once

#include <map>

#include "Node.h"
#include "Utils/Variant.h"

#if defined(WIN32)
#undef far
#undef near
#endif

namespace Core
{
class Sequence;

template <typename Node> FORCE_INLINE bool isRot(Node&& node)
{
	// static_assert(std::is_base_of_v<Node, Node>);

	auto& type = node->getOperation()->keyWord;
	return type == "EulerX" || type == "EulerY" || type == "EulerZ" || type == "AxisAngle";
	// \todo Q: and what about quaternions?
}

//===-- Value masks -------------------------------------------------------===//

/**
 * \brief Array of possible value codes for each matrix element: -1, 0, 1, ANY.
 * These arrays are defined for all Transform variants in TransformImpl.cpp
 * \note These definitions are in the ROW order - are flipped to COLUMN order in
 * validateValue()
 */
using ValueMask = std::array<int8_t, 16>;

constexpr int8_t VM_MINUS_ONE = -1; ///< Fixed matrix element (-1) in ValueMask
constexpr int8_t VM_ZERO = 0;       ///< Fixed matrix element (0) in ValueMask
constexpr int8_t VM_ONE = 1;        ///< Fixed matrix element (+1) in ValueMask
constexpr int8_t VM_ANY = 2;        ///< Editable matrix element in ValueMask - it's range of values may be
                                    ///< limited - the limit is checked in setValue() methods

inline bool canEditValue(EValueState valueState)
{
	return valueState == EValueState::Editable || valueState == EValueState::EditableSyn;
}
/**
 * \brief Check single element in the matrix, if it is in the allowed range.
 *
 * Possible range is defined by constants in the ValueMask array.
 * Editable values have the value VM_ANY, fixed values have VW_MINUS_ONE,
 * VM_ZERO, or WM_ONE
 * @param mask   code of possible values (in the ROW order!) -1, 0, 1, ANY
 * @param coords {x, y} x is column and y is row.
 * @param value  single value from the matrix to be validated against the mask
 * @return true for an allowed value on given position in the matrix
 */
bool validateValue(const ValueMask& mask, glm::ivec2 coords, float value);
/**
 * \brief Check if all matrix elements are in the allowed range.
 * Fixed elements are not changed. Editable elements (ANY) must be further
 * tested outside. \param mask code of possible values (in the ROW order!) -1,
 * 0, 1, ANY \param matrix matrix to be validated against the mask \return true
 * for a valid matrix
 */
bool validateValues(const ValueMask& mask, const glm::mat4& matrix);

//===-- Base Transform class ----------------------------------------------===//

class Transform : public Node
{
	friend class GraphManager;
	friend class Sequence;

	struct DefaultValuePair
	{
		std::string name;
		Data        data;
	};
	using DefaultValues = std::vector<DefaultValuePair>;

public:
	explicit Transform(const TransformOperation& transformType);

	//===-- Construct functions
	//-----------------------------------------------===//

	/**
	 * \brief Prepare storage for second level parameters (std::map with values)
	 */
	void createDefaults();

	/**
	 * \brief Init the (non-zero) second level parameters (from LOD::SetValues)
	 * via their initDefaults and update the internal transformation matrix).
	 * It is overriden in all transforms with their default values.
	 * This version is for nodes without the default value (now only teh Free node)
	 * - It resets the matrix to identity.
	 * - this is done in free->resetMatrixFromDefaults()
	 */
	virtual void initDefaults()
	{
		resetMatrixFromDefaults(); // no defaults => init the matrix to identity
	}

	//===----------------------------------------------------------------------===//

private:
	/// Pointer to the sequence the transform matrix is in (or nullptr if without).
	/// Cannot be smart pointer, because of cyclic dependency!
	Node* m_currentSequence = nullptr;

	/// index of the transform in the sequence
	int m_currentIndex = -1;

public:
	bool isInSequence() const { return m_currentSequence != nullptr; }
	Ptr<Node> getCurrentSequence();
	int getCurrentIndex() const { return m_currentIndex; }

	//===----------------------------------------------------------------------===//

	TransformOperation* properties() const;

public:
	/// Get the value which the transform can hold (current value stored in the
	/// transformation).
	///	It is not the initial value!
	///	\todo rename to getCurrentValue?
	/// \pre Default value with \p name exists.
	const Data& getDefaultValue(const std::string& name) const;

protected:
	/// \pre Default value with \p name exists.
	Data& getDefaultValueMut(const std::string& name);

public:
	/**
	 * \brief Setting of one second level parameter defining the transformation
	 * (in LOD::SetValues).
	 * You can find transform names, types, and their default values, in
	 * the file Core/Nodes/Operation.h. The initial values are hard-wired in
	 * initDefaults() of TransformImpl<ETransformType...> in TransformImpl.h
	 * \tparam T Type of the stored value \a val
	 * \param name Name of the parameter (such as Center)
	 * \param val New value
	 */
	template <typename T> ValueSetResult setDefaultValue(const std::string& name, T&& val)
	{
		const auto* props = properties();
		if (!props->hasDefaultValue(name))
		{
			return ValueSetResult(ValueSetResult::Status::Err_LogicError, "default value with this name does not exist");
		}

		getDefaultValueMut(name).setValue(val); // defaults
		resetMatrixFromDefaults();              // defaults to matrix

		return ValueSetResult();
	}

	template <typename T> void setDefaultValueNoUpdate(const std::string& name, T&& val)
	{
		getDefaultValueMut(name).setValue(val);
	}

	/**
	 * \return A map of valueName and value pairs.
	 */
	TransformOperation::ValueMap getDefaultTypes() const;
	DefaultValues& getDefaultValues();

	void setDefaultValues(const DefaultValues& values) { m_defaultValues = values; }

	EValueState getValueState(glm::ivec2 coords) const;

	//===----------------------------------------------------------------------===//

	// ValueSetResult setValue(float val, glm::ivec2 coords) override;
	// virtual ValueSetResult onSetValue(float val, glm::ivec2 coords) {}

	/**
	 * \brief Checks the validity of the stored Transform matrix (used by GUI to
	 * show the corrupted flag). \todo isValid checks matrix AND parameters from
	 * the SetValues LOD (Default values) \return true if the stored matrix
	 * represents a correct transform
	 */
	virtual bool isValid() const = 0;
	bool isLocked() const;
	void lock();
	void unlock();
	bool hasMenuSynergies() const { return m_hasMenuSynergies; } // PF TODO should be const for the given Transformation
	bool hasSynergies() const { return m_hasSynergies; }
	void disableSynergies() { m_hasSynergies = false; }
	void enableSynergies() { m_hasSynergies = m_hasMenuSynergies ? true : false; }
	void free()
	{
		unlock();
		disableSynergies();
	}

	/**
	 * \brief Reset the transform matrix visible in LOD::Full (internalValue)
	 * to match the defaultValues (from LOD::SetValues).
	 * Specialized functions are created for each Transform type.
	 * For transforms with no default values (now only Free), resets the matrix
	 * directly.
	 *
	 * The opposite setup - from matrix to Defaults - is done in the setValue()
	 * functions. It should also lock the matrix.
	 * \todo For synergies, it has to be resolved. Most probably, it should
	 *       leave the synergies unchanged.
	 *       - for Scale When setting X value in non-uniform scale -> this switch
	 * to uniform scale (due to enable synergies)
	 */
	virtual void resetMatrixFromDefaults() = 0; // PF Pure virtual, defined in TransformImpl for each transformation

	//===----------------------------------------------------------------------===//

	bool hasSavedValue() const { return m_hasSavedData; }

	/** Save current values of the transformation for future reloading. */
	void saveValue();

	/** Restore saved values if they exist. */
	void reloadValue();

	const glm::mat4& getSavedValue() const;

	const DefaultValues& getSavedDefaults() const { return m_savedValues; }

	/**
	 * \brief Save the value, read from YAML
	 * \param values matrix from YAML
	 */
	void setSavedValue(const glm::mat4& values);

	//===----------------------------------------------------------------------===//

	/**
	 * \brief Lock non-editable values. And enable synergies, if exist, i.e.,
	 * hasSynergies() == true
	 */
	// void resetModifiers()
	//{
	//	m_isLocked						= true;
	//	//m_hasMenuSynergies = true;
	//	enableSynergies(); // if (m_hasMenuSynergies) only
	// }

	ValueSetResult setValue(const glm::mat4& mat) override;
	ValueSetResult setValue(float val, glm::ivec2 coords) override; // PF

	//===----------------------------------------------------------------------===//
	struct HalfspaceSign
	{
		float cos = 1.0f; // Initially in the first quadrant
		float sin = 1.0f;
	} halfspaceSign; // remember the quadrant for eulerRotations
	//===----------------------------------------------------------------------===//

	void notifySequence();

protected:
	bool canSetValue(const ValueMask& mask, glm::ivec2 coords, float value);

public:
	/// \todo MH these should not be public.
	void nullSequence()
	{
		m_currentSequence = nullptr;
		m_currentIndex = -1;
	}

	void setSequence(Node* s, int index)
	{
		m_currentSequence = s;
		m_currentIndex = index;
	}

	float getActivePart() const { return m_activePart; }

protected:
	DefaultValues m_defaultValues;

	/**
	 * @brief True for transformations, that support synergies, such as scale,
	 * eulerAngleXYZ, ortho, frustum, and quaternion ONLY
	 *
	 * Synergies variants
	 * | Transformation | hasMenuSynergies | show in Full LOD | show in SetValues
	 * | synergies force | |
	 * ---------------|------------------|------------------|-------------------|-----------------|
	 * | Free           | no               | no               | no | | | Translate
	 * | no               | no               | no                | | | AxisAngle
	 * rot  | no               | no               | no                | | | LookAt
	 * | no               | no               | no                | | | Perspective
	 * | no               | no               | no                | | | Ortho | yes
	 * | no               | yes               |                 | | Frustum | yes
	 * | no               | yes               |                 | | Quat | yes |
	 * no               | yes               | unitQuaternion  | | Scale          |
	 * yes              | yes              | yes               | uniform scale   |
	 * | EulerX,Y,Z     | yes              | yes              | no | cos and sins
	 * |
	 */
	bool m_hasMenuSynergies = false;
	bool m_hasSynergies = false; ///< applicable for: uniform scale, eulerAngleXYZ, ortho, frustum,
	                             ///< and quaternion. All other undefined (false)

	bool m_isLocked = true;

private:
	friend class MatrixTracker;

	friend void setActivePart(Ptr<Node> node, float value);

	float m_activePart = 0.0;

	bool m_hasSavedData = false;

	/// \todo Rename to m_savedMatrix
	Data m_savedData;
	DefaultValues m_savedValues;
};

using TransformPtr = Ptr<Transform>;
} // namespace Core
