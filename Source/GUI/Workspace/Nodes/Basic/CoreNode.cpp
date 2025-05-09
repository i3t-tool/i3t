/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
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
      m_numberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisiblePrecision))
{
	// Register connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.insert(std::make_pair(m_nodebase->getId(), this));
	// Register core node calbacks
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.registerNodeCallbacks(m_nodebase.get());

	m_nodebase->addUpdateCallback([this](Core::Node* node) {
		this->queueUpdateDataItemsWidth();
	});

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
	if (m_updateDataItemsWidth)
		updateDataItemsWidth();
	m_updateDataItemsWidth = false;

	Node::begin(context);

	if (getLevelOfDetail() == LevelOfDetail::Label)
		m_headerSpacing = m_bottomSpacing = false;
	else
		m_headerSpacing = m_bottomSpacing = m_topBottomSpacingDefault;
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

		auto coreNode = this->getNodebase();
		if (auto t = coreNode->getTrackingData())
		{
			if (t->tracker->getSequenceID() == coreNode->getId())
			{
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(ICON_FA_CROSSHAIRS);
				ImGui::SameLine();
			}
		}

		drawContextMenuButton(context, contextButtonCornerFlag);
	}
}
void CoreNode::endDiwne(DIWNE::DrawInfo& context)
{
	Node::endDiwne(context);
	DIWNE_DEBUG_CUSTOM((diwne), {
		ImVec2 origin = diwne.canvas().diwne2screen(ImVec2(getRect().Min.x, getRect().Max.y) +
		                                            ImVec2(0, ImGui::GetFontSize() * 0.5f));
		std::string trackingInfo;
		if (m_nodebase)
		{
			if (auto t = m_nodebase->getTrackingData())
				trackingInfo =
				    fmt::format("Tracking ({:.2f} {})\nidx: {}, tIdx: {}, sIdx: {}, flags: c{:d}a{:d}i{:d}m:{:d}",
				                t->progress, t->interpolating ? "X" : "", t->mIndex, t->tIndex, t->seqIndex, t->chain,
				                t->active, t->interpolating, t->modelSubtree);
		}
		ImGui::GetForegroundDrawList()->AddText(
		    origin, IM_COL32_WHITE,
		    fmt::format("Core ID: {}\n{}", !m_nodebase ? "null" : std::to_string(m_nodebase->getId()), trackingInfo)
		        .c_str());
	});
}
bool CoreNode::allowDrawing()
{
	auto* t = getNodebase()->getTrackingData();
	return (t != nullptr && t->interpolating) || Super::allowDrawing();
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
	if (getLODCount() >= 3 && CoreNode_useDotsForMultiLOD)
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
	if (getLODCount() >= 3)
	{
		if ((detail == LevelOfDetail::Full && !CoreNode_shiftLODIcons) ||
		    (CoreNode_shiftLODIcons && detail == LevelOfDetail::SetValues))
		{
			return CoreNode_useAngleLODIcon   ? ICON_FA_I3T_NSPACE_1 ICON_FA_ANGLE_DOWN
			       : CoreNode_useCaretLODIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_CARET_DOWN
			                                  : ICON_FA_I3T_NSPACE_1 ICON_FA_CHEVRON_DOWN;
		}
		if ((detail == LevelOfDetail::SetValues && !CoreNode_shiftLODIcons) ||
		    (CoreNode_shiftLODIcons && detail == LevelOfDetail::Full))
		{
			return CoreNode_usePenInBoxIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_PEN_TO_SQUARE
			                                : ICON_FA_I3T_NSPACE_1 ICON_FA_PEN;
		}
		if ((detail == LevelOfDetail::Label && !CoreNode_shiftLODIcons) ||
		    (CoreNode_shiftLODIcons && detail == LevelOfDetail::Label))
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
			return CoreNode_useAngleLODIcon   ? ICON_FA_I3T_NSPACE_1 ICON_FA_ANGLE_DOWN
			       : CoreNode_useCaretLODIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_CARET_DOWN
			                                  : ICON_FA_I3T_NSPACE_1 ICON_FA_CHEVRON_DOWN;
		}
		if (detail == LevelOfDetail::Label)
		{
			return CoreNode_useAngleLODIcon   ? ICON_FA_I3T_NSPACE_1 ICON_FA_ANGLE_RIGHT
			       : CoreNode_useCaretLODIcon ? ICON_FA_I3T_NSPACE_1 ICON_FA_CARET_RIGHT
			                                  : ICON_FA_I3T_NSPACE_1 ICON_FA_CHEVRON_RIGHT;
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
	queueUpdateDataItemsWidth();
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
void CoreNode::queueUpdateDataItemsWidth()
{
	m_updateDataItemsWidth = true;
}

float CoreNode::updateDataItemsWidth()
{
	// NOTE: This method might be called OUTSIDE of ImGui / DIWNE context
	float fontSize;
	if (diwne.canvas().m_zoomScalingApplied)
		fontSize = diwne.canvas().m_unscaledFontSize * diwne.getZoom();
	else
		fontSize = ImGui::GetFontSize();

	int maxLength = maxLengthOfData();
	m_dataItemsWidth = GUI::calculateDataItemsWidth(fontSize, maxLength, diwne.getZoom());
	// LOG_DEBUG("updateDataItemsWidth() in node '{}': font: {} maxLen: {}, dataWidth: {}", m_labelDiwne, fontSize,
	//           maxLength, m_dataItemsWidth);
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
		// GetLevelOfDetailName.at(node->getLevelOfDetail())).c_str());
		// ImGui::Separator();

		for (auto const& levelOfDetail : levels_of_detail)
		{
			if (I3TGui::MenuItemWithLog(GetLevelOfDetailName().at(levelOfDetail).c_str(), NULL,
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

	drawMenuLevelOfDetail();

	ImGui::Separator();

	drawMenuSetPrecision();

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

void CoreNode::drawTrackingCursor(ImRect rect, const Core::TrackedNodeData* t, bool inactiveOverlay, bool round) const
{
	assert(t != nullptr);

	ImVec2 lineOfst =
	    ImVec2(0.f, diwne.canvas().screen2diwneSize(ImGui::GetStyle().ItemSpacing.y) * (round ? 1.f : 2.f));

	ImVec2 min = rect.Min;
	ImVec2 max = rect.Max;
	ImVec2 c = min;

	ImVec2 size = max - min;
	float progress = t->progress;
	bool trackingFromLeft = t->tracker->isTrackingFromLeft();

	ImVec2 fMin;
	ImVec2 fMax;
	ImVec2 iMin;
	ImVec2 iMax;
	float startX;
	if (!trackingFromLeft)
	{
		c.x = min.x + ((1.f - progress) * size.x);
		fMin = c;
		fMax = max;
		iMin = min;
		iMax = {c.x, max.y};
		startX = max.x;
	}
	else
	{
		c.x = min.x + (progress * size.x);
		fMin = min;
		fMax = {c.x, max.y};
		iMin = c;
		iMax = max;
		startX = min.x;
	}

	const ImVec4& inactiveCol = I3T::getColor(EColor::Nodes_Tracking_OverlayInactive);
	if (progress < 1.f && inactiveCol.w > 0.f && inactiveOverlay)
		diwne.canvas().AddRectFilledDiwne(iMin, iMax, inactiveCol, round ? I3T::getSize(ESize::Nodes_Rounding) : 0,
		                                  ImDrawFlags_RoundCornersBottom);

	const ImVec4& activeCol = I3T::getColor(EColor::Nodes_Tracking_OverlayActive);
	if (progress > 0.f && activeCol.w > 0.f)
		diwne.canvas().AddRectFilledDiwne(fMin, fMax, activeCol, round ? I3T::getSize(ESize::Nodes_Rounding) : 0,
		                                  ImDrawFlags_RoundCornersBottom);

	const ImVec4& cursorCol = I3T::getColor(EColor::Nodes_Tracking_Cursor);
	const float lW = 1;
	diwne.canvas().AddLine(ImVec2(fMin.x, fMin.y + lW / 2) + lineOfst, ImVec2(fMax.x, fMin.y + lW / 2) + lineOfst,
	                       cursorCol, lW);
	diwne.canvas().AddLine(ImVec2(fMin.x, fMax.y - lW / 2) - lineOfst, ImVec2(fMax.x, fMax.y - lW / 2) - lineOfst,
	                       cursorCol, lW);
	if (t->mIndex == 0)
	{
		diwne.canvas().AddLine(ImVec2(startX, fMin.y + lineOfst.y), {startX, fMax.y - lineOfst.y}, cursorCol, lW);
	}

	if (t->interpolating)
	{
		ImVec2 markCenter = ImVec2(c.x, rect.GetCenter().y);
		ImVec2 markSize = ImVec2(I3T::getSize(ESize::Nodes_Tracking_CursorSize), size.y);
		diwne.canvas().AddRectFilledDiwne(markCenter - markSize / 2, markCenter + markSize / 2, cursorCol);
		static_cast<WorkspaceDiwne&>(diwne).m_trackingCursorPos = diwne.canvas().diwne2screen(ImVec2(c.x, rect.Min.y));
	}
}

void CoreNode::drawTrackingBorder(bool active, bool interpolating, float progress) const
{
	DIWNE::Style& style = diwne.style();
	float borderWidthMul = interpolating ? 2.2f : 1.5f;

	const ImVec4& inactiveCol = I3T::getColor(EColor::Nodes_Tracking_ColorInactive);
	const ImVec4& activeCol = I3T::getColor(EColor::Nodes_Tracking_ColorActive);
	const ImRect& displayRect = getDisplayRect();
	diwne.canvas().AddRectDiwne(displayRect.Min, displayRect.Max, active ? activeCol : inactiveCol,
	                            style.decimal(DIWNE::Style::NODE_ROUNDING), ImDrawFlags_RoundCornersAll,
	                            style.decimal(DIWNE::Style::SELECTED_BORDER_WIDTH) * borderWidthMul);
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
