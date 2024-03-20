/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

/**
 * This code demonstrates how to use default values of transforms.
 */
TEST(DefaultValueTest, Example)
{
	auto axisAngle = Core::Builder::createTransform<ETransformType::AxisAngle>();

	// iterate through all its default values
	for (auto& [key, valueStore] : axisAngle->getDefaultValues())
	{
		// There are only four types of default values - float, vec3, vec4, and
		// quaternion.
		switch (valueStore.valueType)
		// or switch (valueStore.valueType)
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
