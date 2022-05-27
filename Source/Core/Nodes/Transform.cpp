#include "Transform.h"

#include "Logger/Logger.h"
#include "Utils/Format.h"

namespace Core
{
/**
 * @param mask   code of possible values (in the ROW order!) -1, 0, 1, ANY
 * @param coords {x, y} x is column and y is row.
 * @param value  value from the matrix to be validated against the mask 
 * @return true for an allowed value on given position in the matrix
 */
bool validateValue(const ValueMask& mask, glm::ivec2 coords, float value)
{
	const int8_t maskValue = mask[coords.y * 4 + coords.x];  //mask is in the ROW order (see Value masks in TransformImpl.cpp)

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
			const float value = matrix[c][r];   /// column order

			if (!validateValue(mask, { c, r }, value))
			{
				return false;
			}
		}
	}
	return true;
}

//===----------------------------------------------------------------------===//

Transformation::Transformation(const TransformOperation& transformType)
		: NodeBase(&(transformType.operation))
{
	m_internalData.push_back(DataStore(EValueType::Matrix));
	m_savedData = DataStore(EValueType::Matrix);
}

void Transformation::createDefaults()
{
	const auto& opName        = getOperation()->keyWord;
	const auto& transformType = getTransformOperation(magic_enum::enum_cast<ETransformType>(opName).value());

	for (const auto& [key, valueType] : transformType.defaultValuesTypes)
	{
		m_defaultValues[key] = Data(valueType);
	}
}

const Data& Transformation::getDefaultValue(const std::string& name) const
{
	return m_defaultValues.at(name);
}

TransformOperation::ValueMap Transformation::getDefaultTypes()
{
	return getTransformDefaults(getOperation()->keyWord);
}

Transformation::DefaultValues& Transformation::getDefaultValues()
{
	return m_defaultValues;
}

EValueState Transformation::getValueState(glm::ivec2 coords)
{
	const int idx = coords.y * 4 + coords.x;
	auto& map = getTransformMap(getOperation()->keyWord);

	std::bitset<2> bitResult;
	bitResult[0] = map[15 - idx] && m_hasEnabledSynergies;  // synergies bit
	bitResult[1] = map[15 - idx] || !m_isLocked;            // editable bit

	auto result = bitResult.to_ulong();

	return static_cast<EValueState>(result);
}

//===----------------------------------------------------------------------===//

bool Transformation::isLocked() const
{
	return m_isLocked;
}

void Transformation::lock()
{
	m_isLocked = true;
}

void Transformation::unlock()
{
	m_isLocked = false;
}

void Transformation::saveValue()
{
	m_savedData = getData(0);

	m_hasSavedData = true;
}

void Transformation::reloadValue()
{
	if (!m_hasSavedData)
		return;

	setInternalValue(m_savedData.getMat4(), 0);
	notifySequence();
}

const glm::mat4& Transformation::getSavedValue() const
{
	return m_savedData.getMat4();
}

void Transformation::setSavedValue(const glm::mat4& values)
{
	m_savedData.setValue(values);
}

ValueSetResult Transformation::setValue(const glm::mat4& mat)
{
	ValueSetResult result;

	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			auto coords           = glm::ivec2(c, r);
			const auto valueState = getValueState(coords);

			if (canEditValue(valueState))
			{
				const float val = mat[c][r];

				// MSVC was unable to compile this expresion without using Node::
				result = setValue(val, coords);

				if (result.status != ValueSetResult::Status::Ok)
				{
					notifySequence();

					return result;
				}
			}
		}
	}
	notifySequence();

	return ValueSetResult{};
}

void Transformation::notifySequence()
{
	if (m_currentSequence)
	{
		m_currentSequence->updateValues(-1);
	}
}

bool Transformation::canSetValue(const ValueMask& mask, glm::ivec2 coords, float value)
{
	const auto valueState = getValueState(coords);
	const auto isValid    = m_isLocked ? validateValue(mask, coords, value) : true;

	return canEditValue(valueState) && isValid;
}

//===----------------------------------------------------------------------===//
}
