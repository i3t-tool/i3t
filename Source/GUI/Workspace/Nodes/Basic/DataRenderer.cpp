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
#include "DataRenderer.h"

#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/Tools.h"
#include "I3T.h"

using namespace Workspace;

bool DataRenderer::drawDragFloatWithMap_Inline(DIWNE::NodeEditor& diwne, int const numberOfVisibleDecimals,
                                               FloatPopupMode& floatPopupMode, std::string const label, float& value,
                                               Core::EValueState const& valueState, bool& valueChanged)
{
	bool inactive = (valueState == Core::EValueState::Locked || valueState == Core::EValueState::LockedSyn);
	bool synergies = (valueState == Core::EValueState::EditableSyn || valueState == Core::EValueState::LockedSyn);
	bool inner_interaction_happen = false, valueChangedByPopup = false;

	if (synergies)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, I3T::getColor(EColor::Synergies_FloatBg));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, I3T::getColor(EColor::Synergies_FloatBgHovered));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, I3T::getColor(EColor::Synergies_FloatBgActive));
	}
	if (inactive)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
		                    ImGui::GetStyle().Alpha * I3T::getSize(ESize::Float_inactive_alphaMultiplicator));
	}

	float step = I3T::getSize(ESize::Nodes_dragSpeedDefaulrRatio);

	//	ImGuiStyle& style = ImGui::GetStyle();
	//	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Nodes_FloatText);
	//	style.Colors[ImGuiCol_FrameBg] = I3T::getColor(EColor::FloatBg);
	//	style.Colors[ImGuiCol_FrameBgHovered] =
	// I3T::getColor(EColor::FloatBgHovered);
	// style.Colors[ImGuiCol_FrameBgActive] =
	// I3T::getColor(EColor::FloatBgActive);

	// \todo JH is it done? make step a configurable constant - same or smaller
	// than dragStep - other way drag is fired when step is not fired...
	valueChanged =
	    ImGui::DragFloat(label.c_str(), &value, step, 0.0f, 0.0f, fmt::format("%.{}f", numberOfVisibleDecimals).c_str(),
	                     1.0f); /* if power >1.0f the number changes logarithmic */

	if (ImGui::IsItemActive()) // TODO: This seems odd, why in the world would we override IsItemActive??
		inner_interaction_happen = true;

	if (!inactive && !diwne.m_popupDrawn)
	{
		if (bypassFloatFocusAction() && bypassFloatRaisePopupAction())
		{
			ImGui::OpenPopup(label.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
			diwne.setPopupPosition(diwne.input().bypassGetMousePos());
		}

		// TODO: Investigate the popup behaviour now that it's changed! <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		diwne.m_popupDrawn = DIWNE::popupDiwne(label, diwne.getPopupPosition(), &popupFloatContent, floatPopupMode,
		                                       value, valueChangedByPopup);
		inner_interaction_happen |= diwne.m_popupDrawn;

		valueChanged |= valueChangedByPopup;
	}

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	if (synergies)
	{
		ImGui::PopStyleColor(3);
	}

	//	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Text);

	return inner_interaction_happen || valueChanged;
}

void DataRenderer::popupFloatContent(FloatPopupMode& popupMode, float& selectedValue, bool& valueSelected)
{
	ImGui::Text(_t("Choose value..."));
	ImGui::Separator();

	if (ImGui::RadioButton(_t("Angle"), popupMode == FloatPopupMode::Angle))
	{
		popupMode = FloatPopupMode::Angle;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(_t("Value"), popupMode == FloatPopupMode::Value))
	{
		popupMode = FloatPopupMode::Value;
	}

	if (popupMode == FloatPopupMode::Angle)
	{
		if (ImGui::BeginTable("##Angle", 2))
		{

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("PI/6 (30°)"))
			{
				selectedValue = M_PI / 6;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-PI/6 (-30°)"))
			{
				selectedValue = -M_PI / 6;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("PI/4 (45°)"))
			{
				selectedValue = M_PI / 4;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-PI/4 (-45°)"))
			{
				selectedValue = -M_PI / 4;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("PI/3 (60°)"))
			{
				selectedValue = M_PI / 3;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-PI/3 (-60°)"))
			{
				selectedValue = -M_PI / 3;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("PI/2 (90°)"))
			{
				selectedValue = M_PI / 2;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-PI/2 (-90°)"))
			{
				selectedValue = -M_PI / 2;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("PI (180°)"))
			{
				selectedValue = M_PI;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-PI (-180°)"))
			{
				selectedValue = -M_PI;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("3PI/2 (270°)"))
			{
				selectedValue = 3 * M_PI / 2;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-3PI/2 (-270°)"))
			{
				selectedValue = -3 * M_PI / 2;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("2PI (360°)"))
			{
				selectedValue = 2 * M_PI;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-2PI (-360°)"))
			{
				selectedValue = -2 * M_PI;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("0"))
			{
				selectedValue = 0;
				valueSelected = true;
			}

			ImGui::EndTable();
		}
	}
	else if (popupMode == FloatPopupMode::Value)
	{
		if (ImGui::BeginTable("##Values", 2))
		{

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("1/2"))
			{
				selectedValue = 1.0f / 2.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-1/2"))
			{
				selectedValue = -1.0f / 2.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("sqrt(2)/2"))
			{
				selectedValue = sqrtf(2.0f) / 2.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-sqrt(2)/2"))
			{
				selectedValue = -sqrtf(2.0f) / 2.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("sqrt(3)/2"))
			{
				selectedValue = sqrtf(3.0f) / 2.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-sqrt(3)/2"))
			{
				selectedValue = -sqrtf(3.0f) / 2.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("1"))
			{
				selectedValue = 1.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-1"))
			{
				selectedValue = -1.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("2"))
			{
				selectedValue = 2.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-2"))
			{
				selectedValue = -2.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("3"))
			{
				selectedValue = 3.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-3"))
			{
				selectedValue = -3.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("4"))
			{
				selectedValue = 4.0f;
				valueSelected = true;
			}
			ImGui::TableNextColumn();
			if (ImGui::Selectable("-4"))
			{
				selectedValue = -4.0f;
				valueSelected = true;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable("0"))
			{
				selectedValue = 0.0f;
				valueSelected = true;
			}

			ImGui::EndTable();
		}
	}
}

/* nodebase->getValueState({colum, row}) /* EValueState */
bool DataRenderer::drawData4x4(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals,
                               float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data,
                               std::array<std::array<Core::EValueState, 4> const, 4> const& dataState,
                               bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange)
{
	bool inner_interaction_happen = false;
	bool actualValueChanged = false;
	float localData; /* user can change just one value at the moment */

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.canvas().getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.canvas().getZoom());
	valueChanged = false;

	/* Drawing is row-wise */
	ImGui::BeginGroup();
	for (int rows = 0; rows < 4; rows++)
	{
		// To be quite honest I don't know why this call is necessary but window text baseline offset is alwazs set here
		// to ~4 (FramePadding.y) when it really should be 0, causes any text to be too high and then it works fine
		// after the next ImGui::SameLine() call. This created a vertical gap between the first and second matrix rows.
		// JH solved this (haphazardly like me) by moving the Y cursor position by ItemSpacing which however broke down
		// at other zoom levels.
		ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0.0f;
		for (int columns = 0; columns < 4; columns++)
		{
			localData = data[columns][rows]; /* Data are column-wise */

			inner_interaction_happen |= drawDragFloatWithMap_Inline(
			    diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:r{}c{}", node_id, rows, columns),
			    localData, dataState[rows][columns], actualValueChanged);

			if (actualValueChanged)
			{
				valueChanged = true;
				columnOfChange = columns;
				rowOfChange = rows;
				valueOfChange = localData;
			}

			if (columns < 3)
			{
				ImGui::SameLine();
			}
		}
	}
	ImGui::EndGroup();
	//    if (ImGui::BeginTable(fmt::format("##{}_4x4",node_id).c_str(), 4,
	//    ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoHostExtendX |
	//    ImGuiTableFlags_SizingFixedFit))
	//    {
	//        for (int rows = 0; rows < 4; rows++)
	//        {
	//            ImGui::TableNextRow();
	//            for (int columns = 0; columns < 4; columns++)
	//            {
	//                ImGui::TableNextColumn();
	//                localData = data[columns][rows]; /* Data are column-wise */
	//
	//                ImGui::PushItemWidth(dataWidth); /* \todo JH maybe some
	//                better settings of width */ inner_interaction_happen |=
	//                drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals,
	//                floatPopupMode, fmt::format("##{}:r{}c{}", node_id, rows,
	//                columns),
	//                                                                        localData,
	//                                                                        dataState[rows][columns],
	//                                                                        actualValueChanged);
	//
	//                ImGui::PopItemWidth();
	//                if (actualValueChanged)
	//                {
	//                    valueChanged = true;
	//                    columnOfChange = columns; /* \todo JH row, columns and
	//                    value maybe unused -> changes possible directly in (not
	//                    const) passed local_data from calling function */
	//                    rowOfChange = rows; valueOfChange = localData;
	//                }
	//            }
	//        }
	//        ImGui::EndTable();
	//    }

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}

int DataRenderer::maxLengthOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;
	for (int column = 0; column < 4; column++)
	{
		for (int row = 0; row < 4; row++)
		{
			act = Tools::numberOfCharWithDecimalPoint(data[column][row], numberOfVisibleDecimal);
			if (act > maximal)
			{
				maximal = act;
			}
		}
	}
	return maximal;
}

bool DataRenderer::drawDataVec4(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals,
                                float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data,
                                std::array<Core::EValueState, 4> const& dataState, bool& valueChanged,
                                glm::vec4& valueOfChange)
{
	//    const glm::vec4& coreData = m_nodebase->data(index).getVec4();
	//    const Core::DataMap& coreMap = m_nodebase->getDataMapRef();

	bool actualValueChanged = false;
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	valueChanged = false;
	// valueOfChange = data; /* will this work? */
	ImGui::BeginGroup();
	for (int columns = 0; columns < 4; columns++)
	{
		valueOfChange[columns] = data[columns]; /* \todo JH \todo MH copy whole data directly - not in
		                                           for see lines above */
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
		    valueOfChange[columns], dataState[columns], actualValueChanged);
		if (actualValueChanged)
			valueChanged = true;
	}
	ImGui::EndGroup();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}

int DataRenderer::maxLengthOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 4; column++)
	{
		act = Tools::numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

bool DataRenderer::drawDataVec3(DIWNE::NodeEditor& diwne, DIWNE::ID node_id, int numberOfVisibleDecimals,
                                float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data,
                                std::array<Core::EValueState, 3> const& dataState, bool& valueChanged,
                                glm::vec3& valueOfChange)
{
	bool actualValueChanged = false;
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	valueChanged = false;
	ImGui::BeginGroup();
	for (int columns = 0; columns < 3; columns++)
	{
		valueOfChange[columns] = data[columns];
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
		    valueOfChange[columns], dataState[columns], actualValueChanged);
		;
		if (actualValueChanged)
			valueChanged = true;
	}
	ImGui::EndGroup();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}
int DataRenderer::maxLengthOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 3; column++)
	{
		act = Tools::numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

bool DataRenderer::drawDataFloat(DIWNE::NodeEditor& diwne, DIWNE::ID node_id, int numberOfVisibleDecimals,
                                 float dataWidth, FloatPopupMode& floatPopupMode, const float& data,
                                 Core::EValueState const& dataState, bool& valueChanged, float& valueOfChange)
{
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	valueChanged = false;
	valueOfChange = data;
	inner_interaction_happen |=
	    drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:_", node_id),
	                                valueOfChange, dataState, valueChanged);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}
int DataRenderer::maxLengthOfDataFloat(const float& data, int numberOfVisibleDecimal)
{
	return Tools::numberOfCharWithDecimalPoint(data, numberOfVisibleDecimal);
}

bool DataRenderer::drawDataQuaternion(DIWNE::NodeEditor& diwne, DIWNE::ID const node_id,
                                      int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode floatPopupMode,
                                      const glm::quat& data, std::array<Core::EValueState, 4> const& dataState,
                                      bool& valueChanged, glm::quat& valueOfChange)
{
	bool inner_interaction_happen = false;
	bool actualValueChanged = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getZoom());

	valueChanged = false;

	ImGui::BeginGroup();

	// Top labels
	constexpr char const* labels[4] = {"x", "y", "z", "w"};
	for (int column = 0; column < 4; column++)
	{
		float labelHeight = ImGui::GetFontSize();
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImRect rect(cursorPos, cursorPos + ImVec2(dataWidth, labelHeight));
		GUI::TextCentered(labels[column], rect, ImGui::GetColorU32(ImGuiCol_Text));
		ImGui::Dummy(ImVec2(rect.GetWidth(), rect.GetHeight()));
		if (column < 3)
			ImGui::SameLine();
	}

	// Quat values
	for (int column = 0; column < 4; column++)
	{
		valueOfChange[column] = data[column];
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, column),
		    valueOfChange[column], dataState[column], actualValueChanged);

		if (actualValueChanged)
			valueChanged = true;
		if (column < 3)
			ImGui::SameLine();
	}

	ImGui::EndGroup();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}

int DataRenderer::maxLengthOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 4; column++)
	{
		act = Tools::numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

/* >>>>> STATIC FUNCTIONS <<<<< */
bool DataRenderer::bypassFloatFocusAction()
{
	return ImGui::IsItemHovered();
}
bool DataRenderer::bypassFloatRaisePopupAction()
{
	return InputManager::isActionTriggered("raisePopup", EKeyState::Released);
}
