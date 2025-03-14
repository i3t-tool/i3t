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
	m_style.addOverride<ImVec4>(DIWNE::DiwneStyle::nodeBg, I3T::getTheme().get(EColor::NodeBg));
	m_style.addOverride<ImVec4>(DIWNE::DiwneStyle::nodeHeaderBg, I3T::getTheme().get(EColor::NodeHeader));
	m_style.addOverride<float>(DIWNE::DiwneStyle::nodeRounding, I3T::getTheme().get(ESize::Nodes_Border_Rounding));
}

CoreNode::~CoreNode()
{
	// Unregister connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.erase(m_nodebase->getId());
}

void CoreNode::topContent(DIWNE::DrawInfo& context)
{
	// Note: This method does not call superclass topContent!
	// TODO: But it should :|

	bool interaction_happen = false;

	float zoom = diwne.getZoom();
	ImGuiStyle& style = ImGui::GetStyle();

	// TODO: (DR)(REFACTOR) This method doesn't draw the node header background, it expects subclass methods to do it.
	//   I'm not a huge fan of such design. Its confusing. Especially since the superclass WorkspaceNode draws it.

	drawHeader();
	// TODO: This should again be responsibility of the DIWNE library

	// adding a border
	diwne.canvas().AddRectDiwne(m_rect.Min, m_rect.Max, I3T::getTheme().get(EColor::NodeBorder),
	                            I3T::getTheme().get(ESize::Nodes_Border_Rounding), ImDrawFlags_RoundCornersAll,
	                            I3T::getTheme().get(ESize::Nodes_Border_Thickness));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
	                    I3T::getTheme().get(ESize::Nodes_LOD_Button_Rounding) * diwne.getZoom());
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getTheme().get(EColor::NodeLODButtonColorText));
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::NodeLODButtonColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::NodeLODButtonColorHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::NodeLODButtonColorActive));

	LevelOfDetail detail = getLevelOfDetail();
	if (GUI::ButtonWithCorners(getButtonSymbolFromLOD(detail), ImDrawFlags_RoundCornersTopLeft,
	                           I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize) * diwne.getZoom()))
	{
		if (detail == LevelOfDetail::Full)
		{
			if (std::dynamic_pointer_cast<TransformationBase>(shared_from_this()) ||
			    std::dynamic_pointer_cast<Cycle>(shared_from_this()))
			{
				setLevelOfDetail(LevelOfDetail::SetValues);
			}
			else if (!std::dynamic_pointer_cast<Camera>(shared_from_this())) // camera does not change state
			{
				setLevelOfDetail(LevelOfDetail::Label);
			}
		}
		else if (detail == LevelOfDetail::SetValues)
		{
			setLevelOfDetail(LevelOfDetail::Label);
		}
		else if (detail == LevelOfDetail::Label)
		{
			setLevelOfDetail(LevelOfDetail::Full);
		}
		this->setSelected(!this->getSelected());
	}
	if (ImGui::IsItemActive())
		context.consumeInput();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar();

	ImGui::SameLine(0, style.FramePadding.x);

	//	// Left frame padding, shouldn't be necessary as LabelText already includes frame padding
	//	float paddingWidth = ImGui::GetStyle().FramePadding.x; // Already scaled
	//	ImGui::Indent(paddingWidth);

	// Top label
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
		interaction_happen = ImGui::InputText(fmt::format("##{}topLabel", m_labelDiwne).c_str(), &(this->m_topLabel),
		                                      ImGuiInputTextFlags_NoHorizontalScroll);
		auto id = ImGui::GetItemID();
		if (m_isFirstDraw)
		{
			ImGui::ActivateItemByID(id);
			interaction_happen = true;
			m_isFirstDraw = false;
		}
		interaction_happen |= ImGui::IsItemActive();
		if (!interaction_happen)
		{
			m_isLabelBeingEdited = false;
			m_isFirstDraw = true;
		}
	}
	else
	{
		ImGui::LabelText(fmt::format("##{}topLabel", m_labelDiwne).c_str(), this->m_topLabel.c_str());
		interaction_happen = false;
	}
	ImGui::PopStyleColor();
	ImGui::PopItemWidth();

	const float widthSoFar =
	    I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize).x + ((2 * style.FramePadding.x + labelWidth) / zoom);

	// TODO: Handle extra header space using DiwnePanels and some rudimentary layouting

	// Header extra space
	float trailingDummyWidth = style.FramePadding.x / zoom;
	if (m_headerMinWidth > 0)
	{
		const float diff = m_headerMinWidth - widthSoFar;
		if (diff > 0)
		{
			trailingDummyWidth += diff;
		}
	}
	ImGui::SameLine(0, 0);
	ImGui::Dummy(ImVec2(trailingDummyWidth * zoom, 0));

	if (interaction_happen)
	{
		context.update(true, true, true);
	}
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

bool CoreNode::drawDataLabel()
{
	return false;
}

void CoreNode::drawMenuSetEditable()
{
	if (ImGui::MenuItem(_t("Rename"), nullptr, m_isLabelBeingEdited))
	{
		m_isLabelBeingEdited = !m_isLabelBeingEdited;
	}
}

void CoreNode::drawMenuDuplicate(DIWNE::DrawInfo& context)
{
	if (ImGui::MenuItem(_t("Duplicate"), "Ctrl+D"))
	{
		duplicate(context, false);
	}
}

void CoreNode::drawMenuLevelOfDetail_builder(Ptr<CoreNode> node, const std::vector<LevelOfDetail>& levels_of_detail)
{
	if (ImGui::BeginMenu(_t("Level of detail")))
	{
		// ImGui::TextUnformatted(fmt::format("Actual level: {}",
		// LevelOfDetailName[node->getLevelOfDetail()]).c_str());
		// ImGui::Separator();

		for (auto const& levelOfDetail : levels_of_detail)
		{
			if (ImGui::MenuItem(LevelOfDetailName[levelOfDetail].c_str(), NULL,
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
	if (ImGui::BeginMenu(_t("Decimal digits")))
	{
		// ImGui::TextUnformatted(fmt::format("Actual Decimal digits: {}",
		// getNumberOfVisibleDecimal()).c_str()); ImGui::Separator();
		for (int i = 0; i < 5; i++) /* \todo JH, \todo MH some better setter for
		                               precision - allowed values in settings? */
		{
			if (ImGui::MenuItem(fmt::format("{}", i).c_str(), NULL, getNumberOfVisibleDecimal() == i, true))
			{
				setNumberOfVisibleDecimal(i);
			}
		}
		ImGui::EndMenu();
	}
}

static void drawMenuStoreValues(Ptr<Core::Node> node)
{
	if (ImGui::BeginMenu(_t("Value")))
	{
		if (ImGui::MenuItem(_t("Store")))
		{
			node->dataMut(0).saveValue();
		}
		if (ImGui::MenuItem(_t("Restore"), nullptr, false, node->data(0).hasSavedValue()))
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


	DiwneObject::onReleased(justReleased, context);
}

const char* CoreNode::getButtonSymbolFromLOD(const LevelOfDetail detail)
{
	if (detail == LevelOfDetail::Full)
		return "v";
	if (detail == LevelOfDetail::SetValues)
		return "s";
	if (detail == LevelOfDetail::Label)
		return ">";
	if (detail == LevelOfDetail::LightCycle)
		return "."; // was a lightcycle error

	return "missingLOD";
}

void CoreNode::onDestroy(bool logEvent)
{
	m_nodebase->finalize();
	Super::onDestroy(logEvent);
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
