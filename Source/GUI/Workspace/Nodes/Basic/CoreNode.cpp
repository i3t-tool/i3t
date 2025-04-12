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
#include "CoreNode.h"

#include "imgui_internal.h"        // for ImGui::ActivateItemByID()
#include "misc/cpp/imgui_stdlib.h" // for changable text

#include "Core/Input/InputManager.h"
#include "GUI/Fonts/Bindings/IconsFontAwesome6.h"
#include "GUI/Fonts/Bindings/IconsFontAwesome6_I3T.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/Tools.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "Localization/Localization.h"
#include "State/StateManager.h"

using namespace Workspace;

CoreNode::CoreNode(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase)
    : Node(diwne, nodebase->getLabel()), m_nodebase(nodebase),
      m_numberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisiblePrecision)),
      m_dataItemsWidth(I3T::getTheme().get(ESize::Nodes_FloatWidth) *
                       diwne.getZoom()) /* just for safe if someone not call
                                                   setDataItemsWidth() in constructor of
                                                   child class... */
{
	// Register connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.insert(std::make_pair(m_nodebase->getId(), this));
	// Register core node calbacks
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.registerNodeCallbacks(m_nodebase.get());
	// Set a bit flag identifying this node as a core node
	setFlag(CORE_NODE_FLAG, true);
	// I3T DIWNE styling
	setStyleOverride(&I3T::getTheme().m_nodeStyle);
}

CoreNode::~CoreNode()
{
	// Unregister connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.erase(m_nodebase->getId());
}

void CoreNode::begin(DIWNE::DrawInfo& context)
{
	Node::begin(context);
	m_headerSpacing = m_bottomSpacing = getLevelOfDetail() != LevelOfDetail::Label;
}

void CoreNode::topContent(DIWNE::DrawInfo& context)
{
	// Note: This method does not call superclass topContent!

	ImGuiStyle& style = ImGui::GetStyle();

	LevelOfDetail detail = getLevelOfDetail();

	ImDrawFlags headerCornersFlag = ImDrawFlags_RoundCornersTop;
	ImDrawFlags lodButtonCornersFlag = ImDrawFlags_RoundCornersTopLeft;
	ImDrawFlags contextButtonCornerFlag = ImDrawFlags_RoundCornersTopRight;
	if (detail == LevelOfDetail::Label)
	{
		headerCornersFlag = ImDrawFlags_RoundCornersAll;
		lodButtonCornersFlag = ImDrawFlags_RoundCornersLeft;
		contextButtonCornerFlag = ImDrawFlags_RoundCornersRight;
	}

	drawHeader(headerCornersFlag);

	// adding a border
	diwne.canvas().AddRectDiwne(m_displayRect.Min, m_displayRect.Max, I3T::getTheme().get(EColor::NodeBorder),
	                            I3T::getTheme().get(ESize::Nodes_Border_Rounding), ImDrawFlags_RoundCornersAll,
	                            I3T::getTheme().get(ESize::Nodes_Border_Thickness));

	if (getLODCount() > 0)
	{
		drawLODButton(context, detail, lodButtonCornersFlag);
		ImGui::SameLine(0, style.FramePadding.x);
	}
	else
	{
		DIWNE::DGui::SameLineDummy({style.FramePadding.x, 0});
	}

	//	// Left frame padding, shouldn't be necessary as LabelText already includes frame padding
	//	float paddingWidth = ImGui::GetStyle().FramePadding.x; // Already scaled
	//	ImGui::Indent(paddingWidth);

	// Top label
	drawHeaderLabel(context);

	ImGui::SameLine(0, 0);
	ImGui::Dummy(ImVec2(style.FramePadding.x, 0));

	// const float widthSoFar =
	//     I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize).x + ((2 * style.FramePadding.x + labelWidth) / zoom);

	// TODO: Handle extra header space using DiwnePanels and some rudimentary layouting
	//   DiwnePanels should possibly have an optional forced minimum fixed size (minimum minimum size?)

	// Header extra space
	// float trailingDummyWidth = style.FramePadding.x / zoom;
	// if (m_headerMinWidth > 0)
	// {
	// 	const float diff = m_headerMinWidth - widthSoFar;
	// 	if (diff > 0)
	// 	{
	// 		trailingDummyWidth += diff;
	// 	}
	// }
	// ImGui::SameLine(0, 0);
	// ImGui::Dummy(ImVec2(style.FramePadding.x, 0));
	// ImGui::Dummy(ImVec2(trailingDummyWidth * zoom, 0));

	// Right corner context menu button
	// TODO: Rename Nodes_LODButtonSize to something like "HeaderButtonSize" as the buttons must have the same height

	if (m_drawContextMenuButton)
	{
		ImGui::SameLine(0, 0);
		m_top.spring(1.0f);

		drawContextMenuButton(context, contextButtonCornerFlag);
	}
}

void CoreNode::drawLODButton(DIWNE::DrawInfo& context, LevelOfDetail detail, ImDrawFlags cornerFlags)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
	                    I3T::getTheme().get(ESize::Nodes_LOD_Button_Rounding) * diwne.getZoom());
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getTheme().get(EColor::NodeLODButtonColorText));
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::NodeLODButtonColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::NodeLODButtonColorHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::NodeLODButtonColorActive));

	if (CoreNode_useDotsForMultiLOD)
		ImGui::AlignTextToFramePadding();
	if (GUI::ButtonWithCorners(getButtonSymbolFromLOD(detail), cornerFlags,
	                           I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize) * diwne.getZoom()))
	{
		switchLevelOfDetail(m_levelOfDetail);
		context.consumeInput();
	}
	if (ImGui::IsItemActive())
		context.consumeInput();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar();
}

const char* CoreNode::getButtonSymbolFromLOD(const LevelOfDetail detail)
{
	if (detail == LevelOfDetail::LightCycle)
		return "."; // was a lightcycle error
	if (!CoreNode_useLODIcons)
	{
		if (detail == LevelOfDetail::Full)
			return "v";
		if (detail == LevelOfDetail::SetValues)
			return "s";
		if (detail == LevelOfDetail::Label)
			return ">";
	}
	if (getLODCount() <= 2 || !CoreNode_useDotsForMultiLOD)
	{
		if (detail == LevelOfDetail::Full)
		{
			return CoreNode_useAngleLODIcon   ? ICON_FA_I3T_NSPACE_1 ICON_FA_ANGLE_DOWN
			       : CoreNode_useCaretLODIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_CARET_DOWN
			                                  : ICON_FA_I3T_NSPACE_1 ICON_FA_CHEVRON_DOWN;
		}
		if (detail == LevelOfDetail::SetValues)
		{
			return CoreNode_usePenInBoxIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_PEN_TO_SQUARE
			                                : ICON_FA_I3T_NSPACE_1 ICON_FA_PEN;
		}
		if (detail == LevelOfDetail::Label)
		{
			return CoreNode_useAngleLODIcon   ? ICON_FA_I3T_NSPACE_1 ICON_FA_ANGLE_RIGHT
			       : CoreNode_useCaretLODIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_CARET_RIGHT
			                                  : ICON_FA_I3T_NSPACE_1 ICON_FA_CHEVRON_RIGHT;
		}
	}
	else
	{
		if (detail == LevelOfDetail::Full)
		{
			return ICON_FA_I3T_DOTS_3_1;
		}
		if (detail == LevelOfDetail::SetValues)
		{
			return ICON_FA_I3T_DOTS_3_2;
		}
		if (detail == LevelOfDetail::Label)
		{
			return ICON_FA_I3T_DOTS_3_3;
		}
	}

	return "err";
}

void CoreNode::drawContextMenuButton(DIWNE::DrawInfo& context, ImDrawFlags cornerFlags)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
	                    I3T::getTheme().get(ESize::Nodes_LOD_Button_Rounding) * diwne.getZoom());
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::NodeLODButtonColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::NodeLODButtonColorHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::NodeLODButtonColorActive));
	if (GUI::ButtonWithCorners("###contextBtn", cornerFlags,
	                           I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize) * diwne.getZoom()))
	{
		openPopup();
		context.consumeInput();
	}
	diwne.canvas().DrawBurgerMenu(ImGui::GetWindowDrawList(), I3T::getTheme().get(EColor::NodeContextButtonColorText),
	                              ImGui::GetCurrentContext()->LastItemData.Rect, ImVec2(7.f, 7.5f), 1.2f);
	if (ImGui::IsItemActive())
		context.consumeInput();
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
}

void CoreNode::drawHeaderLabel(DIWNE::DrawInfo& context)
{
	ImGuiStyle& style = ImGui::GetStyle();
	if (m_topLabel.empty())
	{
		m_topLabel = m_nodebase->getLabel();
	}
	const char* topLabel = m_topLabel.c_str();
	const ImVec2 textSize = ImGui::CalcTextSize(topLabel, topLabel + strlen(topLabel));

	//	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	//	ImGui::GetWindowDrawList()->AddRectFilled(cursorPos + style.FramePadding, cursorPos + textSize +
	//  style.FramePadding, IM_COL32(0, 255, 255, 120));

	const float labelWidth = textSize.x + style.FramePadding.x * 2;
	ImGui::PushItemWidth(labelWidth);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.00)); /* invisible bg */

	if (m_isLabelBeingEdited)
	{
		float prevTopLabelWidth = m_topLabelWidth;
		bool interacted = ImGui::InputText(fmt::format("##{}topLabel", m_labelDiwne).c_str(), &(this->m_topLabel),
		                                   ImGuiInputTextFlags_NoHorizontalScroll);

		// Ensure that when the label width changes, the layout does not flicker
		// InputText doesn't change width on edit until the NEXT frame
		if (ImGui::IsItemEdited())
		{
			m_topLabelWidth = ImGui::CalcTextSize(m_topLabel.c_str()).x;
			float diff = m_topLabelWidth - prevTopLabelWidth;
			m_topLabelWidthChange = diff;
		}
		else
		{
			if (m_topLabelWidthChange != 0)
				m_top.expectWidthChangeThisFrame(diwne.canvas().screen2diwneSize(m_topLabelWidthChange));
			m_topLabelWidthChange = 0;
		}

		auto id = ImGui::GetItemID();
		if (m_isFirstDraw)
		{
			ImGui::ActivateItemByID(id);
			interacted = true;
			m_isFirstDraw = false;
		}
		interacted |= ImGui::IsItemActive();
		if (!interacted)
		{
			m_isLabelBeingEdited = false;
			m_isFirstDraw = true;
		}
		else
		{
			context.consumeInput();
		}
	}
	else
	{
		ImGui::LabelText(fmt::format("##{}topLabel", m_labelDiwne).c_str(), this->m_topLabel.c_str());
		if (m_topLabelWidth == 0 || diwne.canvas().m_prevZoom != diwne.canvas().m_zoom)
			m_topLabelWidth = ImGui::CalcTextSize(m_topLabel.c_str()).x;
	}
	ImGui::PopStyleColor();
	ImGui::PopItemWidth();
}


Ptr<Core::Node> CoreNode::getNodebase() const
{
	return m_nodebase;
}

const std::string& CoreNode::getKeyword() const
{
	return m_nodebase->getOperation().keyWord;
}

int CoreNode::getNumberOfVisibleDecimal()
{
	return m_numberOfVisibleDecimal;
}

void CoreNode::setNumberOfVisibleDecimal(int value)
{
	m_numberOfVisibleDecimal = (value >= 0 ? value : 0);
	updateDataItemsWidth();
}

FloatPopupMode& CoreNode::getFloatPopupMode()
{
	return m_floatPopupMode;
};
void CoreNode::setFloatPopupMode(FloatPopupMode mode)
{
	m_floatPopupMode = mode;
};

float CoreNode::getDataItemsWidth()
{
	return m_dataItemsWidth;
}

float CoreNode::updateDataItemsWidth()
{
	// TODO: It should be possible to determine scaled font size without actually swapping fonts / applying scaling
	//  All we have to know is the original unscaled font size
	const bool zoomScalingWasActive = diwne.canvas().ensureZoomScaling(true);
	const float fontSize = ImGui::GetFontSize();
	const float oneCharWidth = fontSize / 2;
	const float padding = I3T::getSize(ESize::Nodes_FloatInnerPadding) * diwne.getZoom();
	const float maxLength = static_cast<float>(maxLengthOfData());
	m_dataItemsWidth = maxLength * oneCharWidth + 2 * padding;
	// LOG_INFO("SetDataItemsWidth() in node: '{}'\nfS: {}, oCW: {}, mLOD: {}, dataWidth: {}",
	//         this->getNodebase()->getLabel(), fontSize, oneCharWidth, maxLengthOfData, m_dataItemsWidth);
	diwne.canvas().ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state
	return m_dataItemsWidth;
}

LevelOfDetail CoreNode::setLevelOfDetail(LevelOfDetail levelOfDetail)
{
	m_levelOfDetail = levelOfDetail;
	return getLevelOfDetail();
}

LevelOfDetail CoreNode::getLevelOfDetail()
{
	return m_levelOfDetail;
}

LevelOfDetail CoreNode::switchLevelOfDetail(LevelOfDetail oldLevel)
{
	if (oldLevel == LevelOfDetail::Full)
		return setLevelOfDetail(LevelOfDetail::Label);
	if (oldLevel == LevelOfDetail::Label)
		return setLevelOfDetail(LevelOfDetail::Full);
	return LevelOfDetail::Full;
}

int CoreNode::getLODCount()
{
	return 2;
}

void CoreNode::drawMenuSetEditable()
{
	if (I3TGui::MenuItemWithLog(_t("Rename"), nullptr, m_isLabelBeingEdited))
	{
		m_isLabelBeingEdited = !m_isLabelBeingEdited;
	}
}

void CoreNode::drawMenuDuplicate(DIWNE::DrawInfo& context)
{
	if (I3TGui::MenuItemWithLog(_t("Duplicate"), "Ctrl+D"))
	{
		duplicate(context, false);
	}
}

void CoreNode::drawMenuLevelOfDetail_builder(Ptr<CoreNode> node, const std::vector<LevelOfDetail>& levels_of_detail)
{
	if (levels_of_detail.size() == 2)
	{
		if (node->getLevelOfDetail() == LevelOfDetail::Full)
		{
			if (I3TGui::MenuItemWithLog(_t("Collapse node")))
			{
				node->setLevelOfDetail(LevelOfDetail::Label);
			}
		}
		else
		{
			if (I3TGui::MenuItemWithLog(_t("Expand node")))
			{
				node->setLevelOfDetail(LevelOfDetail::Full);
			}
		}
		return;
	}
	if (I3TGui::BeginMenuWithLog(_t("Set display mode")))
	{
		// ImGui::TextUnformatted(fmt::format("Actual level: {}",
		// LevelOfDetailName[node->getLevelOfDetail()]).c_str());
		// ImGui::Separator();

		for (auto const& levelOfDetail : levels_of_detail)
		{
			if (I3TGui::MenuItemWithLog(LevelOfDetailName[levelOfDetail].c_str(), NULL,
			                            node->getLevelOfDetail() == levelOfDetail, true))
			{
				node->setLevelOfDetail(levelOfDetail);
			}
		}
		ImGui::EndMenu();
	}
}

void CoreNode::drawMenuSetPrecision()
{
	if (I3TGui::BeginMenuWithLog(_t("Decimal digits")))
	{
		// ImGui::TextUnformatted(fmt::format("Actual Decimal digits: {}",
		// getNumberOfVisibleDecimal()).c_str()); ImGui::Separator();
		for (int i = 0; i < 5; i++) /* \todo JH, \todo MH some better setter for
		                               precision - allowed values in settings? */
		{
			if (I3TGui::MenuItemWithLog(fmt::format("{}", i).c_str(), NULL, getNumberOfVisibleDecimal() == i, true))
			{
				setNumberOfVisibleDecimal(i);
			}
		}
		ImGui::EndMenu();
	}
}

static void drawMenuStoreValues(Ptr<Core::Node> node)
{
	if (I3TGui::BeginMenuWithLog(_t("Value")))
	{
		if (I3TGui::MenuItemWithLog(_t("Store")))
		{
			node->dataMut(0).saveValue();
		}
		if (I3TGui::MenuItemWithLog(_t("Restore"), nullptr, false, node->data(0).hasSavedValue()))
		{
			node->dataMut(0).reloadValue();
		}

		ImGui::EndMenu();
	}
}

void CoreNode::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuSetEditable();

	if (m_nodebase->getOperation().isConstructor)
	{
		drawMenuStoreValues(getNodebase());
	}

	ImGui::Separator();

	drawMenuSetPrecision();
	drawMenuLevelOfDetail();

	ImGui::Separator();

	drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void CoreNode::onReleased(bool justReleased, DIWNE::DrawInfo& context)
{
	// TODO: Hookup I3T input bindings
	// Handle quick node duplication
	if (justReleased && !context.inputConsumed && !context.state.dragging && ImGui::IsKeyDown(ImGuiKey_LeftAlt))
	{
		duplicate(context, true);
		context.consumeInput();
	}

	Super::onReleased(justReleased, context);
}

void CoreNode::onDestroy(bool logEvent)
{
	m_nodebase->finalize();
	Super::onDestroy(logEvent);
}

void CoreNode::onPopup()
{
	Node::onPopup();
	LOG_EVENT_OPEN_POP_UP(m_labelDiwne);
}

void CoreNode::duplicate(DIWNE::DrawInfo& context, bool multiDuplication)
{
	if (m_selected && multiDuplication)
	{ // If selected and multi duplication is allowed, duplicate this and any other selected nodes
		for (auto& node : static_cast<WorkspaceDiwne&>(diwne).getAllSelectedCoreNodesWithoutNesting())
		{
			node.setDuplicateNode(true);
		}
	}
	else
	{ // Otherwise we duplicate this single node
		setDuplicateNode(true);
	}
	diwne.deselectAllNodes();
	context.consumeInput();
}
void CoreNode::onSelection(bool selected)
{
	Node::onSelection(selected);
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.resolveNeeded();
}
