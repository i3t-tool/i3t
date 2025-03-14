/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "DIWNE/diwne_include.h"

#include "Core/Nodes/Transform.h"
#include "Node.h" //TODO: Try to remove this include, move enums elsewhere maybe?

namespace Workspace
{
class DataRenderer
{
public:
	static bool drawDragFloatWithMap_Inline(DIWNE::NodeEditor& diwne, int const numberOfVisibleDecimals,
	                                        FloatPopupMode& floatPopupMode, std::string const label, float& value,
	                                        Core::EValueState const& valueState, bool& valueChanged);
	static void popupFloatContent(FloatPopupMode& popupMode, float& selectedValue, bool& valueSelected);

	/**
	 * \param valueOfChange is set to value setted by user if so - not touched if no
	 * user change heppen \return whether some interaction happen
	 */
	static bool drawData4x4(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals,
	                        float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data,
	                        std::array<std::array<Core::EValueState, 4> const, 4> const& dataState, bool& valueChanged,
	                        int& rowOfChange, int& columnOfChange, float& valueOfChange);
	// static bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int
	// const numberOfVisibleDecimals, float dataWidth, FloatPopupMode&
	// floatPopupMode, const glm::mat4& data, const Core::DataMap&
	// dataMap, bool& valueChanged, int& rowOfChange, int& columnOfChange, float&
	// valueOfChange );
	static int maxLengthOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal);

	static bool drawDataVec4(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
	                         float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data,
	                         std::array<Core::EValueState, 4> const& dataState, bool& valueChanged,
	                         glm::vec4& valueOfChange);
	static int maxLengthOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal);

	static bool drawDataVec3(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
	                         float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data,
	                         std::array<Core::EValueState, 3> const& dataState, bool& valueChanged,
	                         glm::vec3& valueOfChange);
	static int maxLengthOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal);

	static bool drawDataFloat(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
	                          float dataWidth, FloatPopupMode& floatPopupMode, const float& data,
	                          Core::EValueState const& dataState, bool& valueChanged, float& valueOfChange);
	static int maxLengthOfDataFloat(const float& data, int numberOfVisibleDecimal);

	static bool drawDataQuaternion(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
	                               float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data,
	                               std::array<Core::EValueState, 4> const& dataState, bool& valueChanged,
	                               glm::quat& valueOfChange);
	static int maxLengthOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal);

	static bool bypassFloatFocusAction();
	static bool bypassFloatRaisePopupAction();
};
} // namespace Workspace