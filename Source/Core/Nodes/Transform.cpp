/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Petr Felkel <felkel@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Transform.h"

#include "Logger/Logger.h"
#include "Utils/Format.h"

namespace Core
{
bool validateValue(const ValueMask& mask, glm::ivec2 coords, float value)
{
	const int8_t maskValue = mask[coords.y * 4 + coords.x]; // mask is in the ROW order (see Value
	                                                        // masks in TransformImpl.cpp)

	if (maskValue == VM_ANY)
		return true;

	return Math::eq((float) maskValue, value);
}

bool validateValues(const ValueMask& mask, const glm::mat4& matrix)
{
	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			const float value = matrix[c][r]; /// column order

			if (!validateValue(mask, {c, r}, value))
			{
				return false;
			}
		}
	}
	return true;
}

//===----------------------------------------------------------------------===//

Transform::Transform(const TransformOperation& transformType) : Node(transformType.operation)
{
	m_internalData.push_back(Data(EValueType::Matrix));
}

void Transform::createDefaults()
{
	const auto& opName = getOperation().keyWord;
	const auto& transformType = getTransformOperation(magic_enum::enum_cast<ETransformType>(opName).value());

	for (const auto& [key, valueType] : transformType.defaultValuesTypes)
	{
		m_defaultValues.push_back({key, Data(valueType)});
	}
}

Ptr<Node> Transform::getCurrentSequence()
{
	if (m_currentSequence == nullptr)
	{
		return nullptr;
	}
	return m_currentSequence->getPtr();
}

TransformOperation* Transform::properties() const
{
	return *getTransformOperation(getOperation().keyWord);
}

const Data& Transform::getDefaultValue(const std::string& name) const
{
	const auto it = std::find_if(m_defaultValues.begin(), m_defaultValues.end(), [&name](const auto value) {
		return value.name == name;
	});

	I3T_ASSERT(it != m_defaultValues.end(), "Invalid value name!");

	return it->data;
}

Data& Transform::getDefaultValueMut(const std::string& name)
{
	const auto it = std::find_if(m_defaultValues.begin(), m_defaultValues.end(), [&name](const auto value) {
		return value.name == name;
	});

	I3T_ASSERT(it != m_defaultValues.end(), "Invalid value name!");

	return it->data;
}

void Transform::setDefaultValueWithSynergies(const std::string& name, Core::Data&& val)
{
	if (getDefaultValueMut(name).index() != val.index())
	{
		LOG_ERROR("Error in setDefaultValueWithSynergies - undefined value type.");
		return;
	}

	getDefaultValueMut(name) = val;
}

TransformOperation::ValueMap Transform::getDefaultTypes() const
{
	return getTransformDefaults(getOperation().keyWord);
}

Transform::DefaultValues& Transform::getDefaultValues()
{
	return m_defaultValues;
}

EValueState Transform::getValueState(glm::ivec2 coords) const
{
	const int idx = coords.y * 4 + coords.x;
	auto& map = getTransformMap(getOperation().keyWord);

	std::bitset<2> bitResult;
	bitResult[0] = map[15 - idx] && m_hasSynergies; // synergies bit
	bitResult[1] = map[15 - idx] || !m_isLocked;    // editable bit

	auto result = bitResult.to_ulong();

	return static_cast<EValueState>(result);
}

//===----------------------------------------------------------------------===//

bool Transform::isLocked() const
{
	return m_isLocked;
}

void Transform::lock()
{
	m_isLocked = true;
}

void Transform::unlock()
{
	m_isLocked = false;
}

bool Transform::hasSavedValue() const
{
	auto result = data(0).hasSavedValue();
	for (auto& [_, data] : m_defaultValues)
	{
		result = result || data.hasSavedValue();
	}

	return result;
}

void Transform::saveValue()
{
	dataMut(0).saveValue();
	for (auto& [_, data] : m_defaultValues)
	{
		data.saveValue();
	}
}

void Transform::reloadValue()
{
	dataMut(0).reloadValue();

	if (!m_defaultValues.empty())
	{
		for (auto& [_, data] : m_defaultValues)
		{
			data.reloadValue();
		}
		resetMatrixFromDefaults();
	}

	notifySequence();
}

SetValueResult Transform::setValue(const glm::mat4& mat)
{
	SetValueResult result;

	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			auto coords = glm::ivec2(c, r);
			const auto valueState = getValueState(coords);

			if (canEditValue(valueState))
			{
				const float val = mat[c][r];

				// MSVC was unable to compile this expression without using Node::
				result = setValue(val, coords);

				if (result.status != SetValueResult::Status::Ok)
				{
					notifySequence();

					return result;
				}
			}
		}
	}
	notifySequence();

	return SetValueResult{};
}

SetValueResult Transform::setValue(float val, glm::ivec2 coords) // PF
{
	// Default implementation, potentially overriden by subclasses
	if (isLocked())
	{
		return SetValueResult{SetValueResult::Status::Err_ConstraintViolation, "Invalid position!"};
	}

	setInternalValue(val, coords);
	notifySequence();

	return SetValueResult{};
}

void Transform::notifySequence()
{
	if (m_currentSequence)
	{
		m_currentSequence->receiveSignal(-1);
	}
}

bool Transform::canSetValue(const ValueMask& mask, glm::ivec2 coords, float value)
{
	if (m_isLocked)
	{
		const auto valueState = getValueState(coords);
		const auto isValid = m_isLocked ? validateValue(mask, coords, value) : true;

		return canEditValue(valueState) && isValid;
	}
	else
		return true;
}
} // namespace Core
