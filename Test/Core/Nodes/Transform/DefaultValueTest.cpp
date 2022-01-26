#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

/**
 * This code demonstrates how to use default values of transforms.
 */
TEST(DefaultValueTest, Example)
{
	auto axisAngle = Core::Builder::createTransform<ETransformType::AxisAngle>();

	// get transform properties
	const auto& props = Core::getTransformProps(axisAngle->getOperation());

	// iterate through all its default values
	for (const auto& [key, valueType] : props.defaultValuesTypes)
	{
		// get a DataStore for a given key
		const auto& valueStore = axisAngle->getDefaultValue(key);

		// There are only four types of default values - float, vec3, vec4, and quaternion.
		switch (valueType)
		// or switch (valueStore.opValueType)
		{
		case EValueType::Float:
		{
			// Query the DataStore
			const float val = valueStore.getFloat();

			/*
			if (ImGui::DragFloat(key.c_str(), &val))
			{
			 	axisAngle->setDefaultValue(key, val);
			}
			 */
		}
		case EValueType::Vec3:
		{
			// get vec3
			// draw vec3
			// set vec3
		}
		case EValueType::Vec4:
		case EValueType::Quat:
		{
			// ...
		}
		default:
			break;
		}
	}
}
