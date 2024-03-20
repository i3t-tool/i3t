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
#include "WorkspaceElementsWithCoreData.h"

#include "imgui_internal.h"        // for ImGui::ActivateItemByID()
#include "misc/cpp/imgui_stdlib.h" /* for changable text */

#include "State/StateManager.h"

#include "../Windows/WorkspaceWindow.h"
#include "GUI/Toolkit.h"
#include "Tools.h"

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase)
    : WorkspaceNode(diwne, nodebase->getId(), nodebase->getLabel(), nodebase->getOperation()->defaultLabel),
      m_nodebase(nodebase), m_numberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisiblePrecision)),
      m_dataItemsWidth(I3T::getTheme().get(ESize::Nodes_FloatWidth) *
                       diwne.getWorkAreaZoom()) /* just for safe if someone not call
                                                   setDataItemsWidth() in constructor of
                                                   child class... */
      ,
      m_levelOfDetail(WorkspaceLevelOfDetail::Full), m_floatPopupMode(Value)
{
	// Register connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.insert(std::make_pair(m_nodebase->getId(), this));
	// Register core node calbacks
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.registerNodeCallbacks(m_nodebase.get());
}

// TODO: (DR) Commented out for now, more info in the header file
// bool WorkspaceNodeWithCoreData::bypassDragAction()
// {
// 	return InputManager::isAxisActive("drag") != 0 &&
// 	       (InputManager::m_mouseXDragDelta > ImGui::GetIO().MouseDragThreshold
// || 	        InputManager::m_mouseYDragDelta >
// ImGui::GetIO().MouseDragThreshold || -InputManager::m_mouseXDragDelta >
// 	            ImGui::GetIO().MouseDragThreshold ||
// 	        -InputManager::m_mouseYDragDelta >
// ImGui::GetIO().MouseDragThreshold);
// }
// bool WorkspaceNodeWithCoreData::bypassHoldAction()
// {
// 	return InputManager::isActionTriggered("hold", EKeyState::Pressed);
// }
// bool WorkspaceNodeWithCoreData::bypassUnholdAction()
// {
// 	return InputManager::isActionTriggered("hold", EKeyState::Released);
// }
// bool WorkspaceNodeWithCoreData::bypassSelectAction()
// {
// 	return InputManager::isActionTriggered("select", EKeyState::Released);
// }
// bool WorkspaceNodeWithCoreData::bypassUnselectAction()
// {
// 	return InputManager::isActionTriggered("select", EKeyState::Released);
// }
// bool WorkspaceNodeWithCoreData::bypassTouchAction()
// {
// 	return InputManager::isActionTriggered("touch", EKeyState::Released);
// }

WorkspaceNodeWithCoreData::~WorkspaceNodeWithCoreData()
{
	m_nodebase->finalize();
	// Unregister connection between core node and gui node
	static_cast<WorkspaceDiwne&>(diwne).m_coreIdMap.erase(m_nodebase->getId());
}

bool WorkspaceNodeWithCoreData::topContent()
{
	bool interaction_happen = false;

	float zoom = diwne.getWorkAreaZoom();
	ImGuiStyle& style = ImGui::GetStyle();

	// TODO: (DR)(REFACTOR) This method doesn't draw the node header background, it expects subclass methods to do it.
	//   I'm not a huge fan of such design. Its confusing. Especially since the superclass WorkspaceNode draws it.

	// adding a border
	diwne.AddRectDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBorder),
	                   I3T::getTheme().get(ESize::Nodes_Border_Rounding), ImDrawFlags_RoundCornersAll,
	                   I3T::getTheme().get(ESize::Nodes_Border_Thickness));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
	                    I3T::getTheme().get(ESize::Nodes_LOD_Button_Rounding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getTheme().get(EColor::NodeLODButtonColorText));
	ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::NodeLODButtonColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::NodeLODButtonColorHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, I3T::getTheme().get(EColor::NodeLODButtonColorActive));

	WorkspaceLevelOfDetail detail = getLevelOfDetail();
	if (GUI::ButtonWithCorners(getButtonSymbolFromLOD(detail), ImDrawFlags_RoundCornersTopLeft,
	                           I3T::getTheme().get(ESizeVec2::Nodes_LODButtonSize) * diwne.getWorkAreaZoom()))
	{
		if (detail == WorkspaceLevelOfDetail::Full)
		{
			if (std::dynamic_pointer_cast<WorkspaceTransformation>(shared_from_this()) ||
			    std::dynamic_pointer_cast<WorkspaceCycle>(shared_from_this()))
			{
				setLevelOfDetail(WorkspaceLevelOfDetail::SetValues);
			}
			else if (!std::dynamic_pointer_cast<WorkspaceCamera>(shared_from_this())) // camera does not change state
			{
				setLevelOfDetail(WorkspaceLevelOfDetail::Label);
			}
		}
		else if (detail == WorkspaceLevelOfDetail::SetValues)
		{
			setLevelOfDetail(WorkspaceLevelOfDetail::Label);
		}
		else if (detail == WorkspaceLevelOfDetail::Label)
		{
			setLevelOfDetail(WorkspaceLevelOfDetail::Full);
		}
		// TODO: replace with a layer that blocks interaction
		this->setSelected(!this->getSelected());
	}
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

	return interaction_happen;
}

Ptr<Core::Node> const WorkspaceNodeWithCoreData::getNodebase() const
{
	return m_nodebase;
}

int WorkspaceNodeWithCoreData::getNumberOfVisibleDecimal()
{
	return m_numberOfVisibleDecimal;
}

void WorkspaceNodeWithCoreData::setNumberOfVisibleDecimal(int value)
{
	m_numberOfVisibleDecimal = (value >= 0 ? value : 0);
	updateDataItemsWidth();
}

float WorkspaceNodeWithCoreData::getDataItemsWidth()
{
	return m_dataItemsWidth;
}

float WorkspaceNodeWithCoreData::updateDataItemsWidth()
{
	const bool zoomScalingWasActive = diwne.ensureZoomScaling(true);
	const float fontSize = ImGui::GetFontSize();
	const float oneCharWidth = fontSize / 2;
	const float padding = I3T::getSize(ESize::Nodes_FloatInnerPadding) * diwne.getWorkAreaZoom();
	const float maxLength = static_cast<float>(maxLengthOfData());
	m_dataItemsWidth = maxLength * oneCharWidth + 2 * padding;
	// LOG_INFO("SetDataItemsWidth() in node: '{}'\nfS: {}, oCW: {}, mLOD: {}, dataWidth: {}",
	//         this->getNodebase()->getLabel(), fontSize, oneCharWidth, maxLengthOfData, m_dataItemsWidth);
	diwne.ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state
	return m_dataItemsWidth;
}

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail)
{
	m_levelOfDetail = levelOfDetail;
	return getLevelOfDetail();
}

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::getLevelOfDetail()
{
	return m_levelOfDetail;
}

bool WorkspaceNodeWithCoreData::drawDataLabel()
{
	return false;
}

void WorkspaceNodeWithCoreData::drawMenuSetEditable()
{
	if (ImGui::MenuItem("Rename", nullptr, m_isLabelBeingEdited))
	{
		m_isLabelBeingEdited = !m_isLabelBeingEdited;
	}
}

void WorkspaceNodeWithCoreData::drawMenuDuplicate()
{
	if (ImGui::MenuItem("Duplicate", "Ctrl+D"))
	{
		// duplicate
		static_cast<WorkspaceDiwne&>(diwne).deselectNodes();
		duplicateNode(std::static_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
		              I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset));
		// move original node behind the new one
		static_cast<WorkspaceDiwne&>(diwne).shiftNodesToBegin(static_cast<WorkspaceDiwne&>(diwne).getSelectedNodes());
	}
}

void WorkspaceNodeWithCoreData::drawMenuSetPrecision()
{
	if (ImGui::BeginMenu("Decimal digits"))
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
	if (ImGui::BeginMenu("Value"))
	{
		if (ImGui::MenuItem("Store"))
		{
			node->dataMut(0).saveValue();
		}
		if (ImGui::MenuItem("Restore", nullptr, false, node->data(0).hasSavedValue()))
		{
			node->dataMut(0).reloadValue();
		}

		ImGui::EndMenu();
	}
}

void WorkspaceNodeWithCoreData::popupContent()
{
	drawMenuSetEditable();

	if (m_nodebase->getOperation()->isConstructor)
	{
		drawMenuStoreValues(getNodebase());
	}

	ImGui::Separator();

	drawMenuSetPrecision();
	drawMenuLevelOfDetail();

	ImGui::Separator();

	drawMenuDuplicate();

	ImGui::Separator();

	WorkspaceNode::popupContent();
}

bool WorkspaceNodeWithCoreData::processObjectDrag()
{
	if (bypassDragAction() && allowProcessDrag())
	{
		m_isDragged = true;
		if (!getSelected() && diwne.getDiwneActionPreviousFrame() == getDragActionType())
		{
			static_cast<WorkspaceDiwne&>(diwne).deselectNodes();
		}
		diwne.setDiwneAction(getDragActionType());
		return processDrag();
	}
	return false;
}

bool WorkspaceNodeWithCoreData::processSelect()
{
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.resolveNeeded();
	return WorkspaceNode::processSelect();
}

bool WorkspaceNodeWithCoreData::processUnselect()
{
	static_cast<WorkspaceDiwne&>(diwne).m_viewportHighlightResolver.resolveNeeded();
	return WorkspaceNode::processUnselect();
}

const char* WorkspaceNodeWithCoreData::getButtonSymbolFromLOD(const WorkspaceLevelOfDetail detail)
{
	if (detail == WorkspaceLevelOfDetail::Full)
		return "v";
	if (detail == WorkspaceLevelOfDetail::SetValues)
		return "s";
	if (detail == WorkspaceLevelOfDetail::Label)
		return ">";
	if (detail == WorkspaceLevelOfDetail::LightCycle)
		return "."; // was a lightcycle error

	return "missingLOD";
}

WorkspaceCorePin::WorkspaceCorePin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
                                   WorkspaceNodeWithCoreData& node)
    : DIWNE::Pin(diwne, id), m_pin(pin), m_node(node), m_iconRectDiwne(ImRect(0, 0, 0, 0))
{}

// bool WorkspaceCorePin::allowInteraction()
//{
//     return m_pin->isEnabled();
// }
//
// bool WorkspaceCorePin::allowProcessFocused()
//{
//     return m_pin->isEnabled();
// }

/* DIWNE function */


/**
 * \brief Draw the pin icon
 * \return false - no interaction allowed
 */
bool WorkspaceCorePin::content()
{
	const bool interaction_happen = false; // no interaction in this function allowed
	if (getCorePin().isRendered())
	{
		const float alpha = ImGui::GetStyle().Alpha;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		const DIWNE::IconType iconTypeBg = WorkspacePinShapeBackground[getType()]; // from WorkspaceElements.cpp
		const ImColor iconColorBg = I3T::getColor(WorkspacePinColorBackground[getType()]);
		DIWNE::IconType iconTypeFg;
		if (getType() == Core::EValueType::Pulse)
		{
			iconTypeFg = m_iconType; // (PF) icon for the cycle box, Triangle elsewhere
			                         // iconTypeFg = DIWNE::IconType::SkipBack;
			                         // iconTypeFg = DIWNE::IconType::SkipBack2;
			                         // iconTypeFg = DIWNE::IconType::SkipForward;
			                         // iconTypeFg = DIWNE::IconType::SkipForward2;
			                         // iconTypeFg = DIWNE::IconType::Rewind;
			                         // iconTypeFg = DIWNE::IconType::FastForward;
		}
		else
		{
			iconTypeFg = WorkspacePinShapeForeground[getType()];
		}
		const ImColor iconColorFg = I3T::getColor(WorkspacePinColorForeground[getType()]);

		const ImVec2 iconSize = I3T::getSize(ESizeVec2::Nodes_IconSize) * diwne.getWorkAreaZoom();

		ImGuiContext& g = *GImGui;

		// space between icon symbol and icon boundary
		const float padding = I3T::getSize(ESize::Pins_IconPadding) * diwne.getWorkAreaZoom();

		// TODO: (DR) Don't really see why the "filled" parameters depends on isConnected(), currently the outlines are
		//   not visible anyway so we're just drawing stuff twice for no reason
		// todo (PF) - I have temporally added the pin border drawing of not-connected pins
		// connected pins have no border now
		diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg,
		               createColor(232, 232, 232, 255) /*iconColorFg*/, iconSize,
		               ImVec4(padding, padding, padding, padding), isConnected());
		m_iconRectDiwne =
		    ImRect(diwne.screen2diwne(ImGui::GetItemRectMin()), diwne.screen2diwne(ImGui::GetItemRectMax()));

		ImGui::PopStyleVar();
	}
	return interaction_happen;
}

void WorkspaceCorePin::popupContent() {}

bool WorkspaceCorePin::bypassFocusForInteractionAction()
{
	return m_iconRectDiwne.Contains(diwne.screen2diwne(diwne.bypassGetMousePos()));
}

Core::Pin const& WorkspaceCorePin::getCorePin() const
{
	return m_pin;
}

int WorkspaceCorePin::getIndex() const
{
	return m_pin.Index;
}

PinKind WorkspaceCorePin::getKind() const
{
	return m_pin.IsInput ? PinKind::Input : PinKind::Output;
}

Core::EValueType WorkspaceCorePin::getType() const
{
	return m_pin.ValueType;
}

/* \todo JH \todo MH implement this function in Core? */
bool WorkspaceCorePin::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

bool WorkspaceCorePin::processDrag()
{
	const ImVec2 origin = getLinkConnectionPointDiwne();
	const ImVec2 actual = diwne.screen2diwne(ImGui::GetIO().MousePos);

	if (getKind() == PinKind::Output)
		diwne.getHelperLink().setLinkEndpointsDiwne(origin, actual);
	else
		diwne.getHelperLink().setLinkEndpointsDiwne(actual, origin);

	diwne.mp_settingsDiwne->linkColor = I3T::getTheme().get(WorkspacePinColorBackground[getType()]);
	diwne.mp_settingsDiwne->linkThicknessDiwne = I3T::getTheme().get(ESize::Links_Thickness);

	return Pin::processDrag();
}

bool WorkspaceCorePin::processConnectionPrepared()
{
	/* here it is when goal pin is hoovered */
	WorkspaceCorePin *input, *output;
	if (getKind() == PinKind::Input)
	{
		input = this;
		output = diwne.getLastActivePin<WorkspaceCorePin>().get();
	}
	else
	{
		input = diwne.getLastActivePin<WorkspaceCorePin>().get();
		output = this;
	}

	const auto& coreInput = input->getCorePin();
	const auto& coreOutput = output->getCorePin();

	switch (Core::GraphManager::isPlugCorrect(coreInput, coreOutput))
	{
	case Core::ENodePlugResult::Ok:
		diwne.showTooltipLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
		if (bypassUnholdAction())
		{
			WorkspaceCoreInputPin* in = dynamic_cast<WorkspaceCoreInputPin*>(input);
			in->plug(dynamic_cast<WorkspaceCoreOutputPin*>(output));
		}
		break;

	case Core::ENodePlugResult::Err_MismatchedPinTypes:
		diwne.showTooltipLabel("Mismatched pin Types (matrix/float/vec/...)",
		                       I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_MismatchedPinKind:
		diwne.showTooltipLabel("Mismatched pin Kinds (in/out)", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_Loopback: /// Same nodes.
		diwne.showTooltipLabel("Loop to the same node", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_NonexistentPin:
		diwne.showTooltipLabel("Pin does not exist :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_Loop:
		diwne.showTooltipLabel("Loop not allowed", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_DisabledPin:
		diwne.showTooltipLabel("Pin is disabled :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	default: // unreachable - all enum values are covered
		diwne.showTooltipLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
	}
	return true;
}

WorkspaceCoreOutputPin::WorkspaceCoreOutputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
                                               WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(diwne, id, pin, node)
{}

/**
 * \brief Draw the output Pin: label and icon [float >]
 * \return true if value changed
 */
bool WorkspaceCoreOutputPin::content()
{
	const std::string& label = m_pin.getLabel();
	// todo (PF) Label and value order should be switched (used by cycle, mat->TR, x->floats, pulse)
	// probably not - would be good for scalars, but wrong for mat4
	// if (!label.empty())
	if (!(m_pin.ValueType == Core::EValueType::Pulse) && !label.empty()) // no labels for pulse and cycle
	{
		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();
	}
	const auto inner_interaction_happen = WorkspaceCorePin::content(); // icon
	return inner_interaction_happen;
}

WorkspaceCoreOutputPinWithData::WorkspaceCoreOutputPinWithData(DIWNE::Diwne& diwne, DIWNE::ID const id,
                                                               Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCoreOutputPin(diwne, id, pin, node)
{}

bool WorkspaceCoreOutputPinWithData::content()
{
	bool interaction_happen = false;
	// if (getNode().getLevelOfDetail() == WorkspaceLevelOfDetail::Full ||
	//     getNode().getLevelOfDetail() == WorkspaceLevelOfDetail::SetValues || // for cycle box
	//     getNode().getLevelOfDetail() == WorkspaceLevelOfDetail::LightCycle)  // for cycle box
	if (getNode().getLevelOfDetail() != WorkspaceLevelOfDetail::Label)
	{
		interaction_happen |= drawData();
		ImGui::SameLine();
	}
	interaction_happen |= WorkspaceCoreOutputPin::content(); // label and icon
	return interaction_happen;
}

/* >>>> Pin types <<<< */

bool WorkspaceCoreOutputPinMatrix4x4::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	int rowOfChange, columnOfChange;
	float valueOfChange;
	WorkspaceNodeWithCoreData& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = drawData4x4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
	                                 node.getFloatPopupMode(), getCorePin().data().getMat4(),
	                                 {valState, valState, valState, valState, valState, valState, valState, valState,
	                                  valState, valState, valState, valState, valState, valState, valState, valState},
	                                 valueChanged, rowOfChange, columnOfChange, valueOfChange);
	if (valueChanged)
	{
		/* \todo JM MH set values to given (this) pin*/
		node.getNodebase()->setValue(valueOfChange, {columnOfChange, rowOfChange});
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinMatrix4x4::maxLengthOfData()
{
	return maxLengthOfData4x4(getCorePin().data().getMat4(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinVector4::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::vec4 valueOfChange;
	WorkspaceNodeWithCoreData& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = drawDataVec4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
	                                  node.getFloatPopupMode(), getCorePin().data().getVec4(),
	                                  {valState, valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinVector4::maxLengthOfData()
{
	return maxLengthOfDataVec4(getCorePin().data().getVec4(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinVector3::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::vec3 valueOfChange;
	WorkspaceNodeWithCoreData& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = drawDataVec3(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
	                                  node.getFloatPopupMode(), getCorePin().data().getVec3(),
	                                  {valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinVector3::maxLengthOfData()
{
	return maxLengthOfDataVec3(getCorePin().data().getVec3(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinFloat::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	float valueOfChange;
	WorkspaceNodeWithCoreData& node = getNode();

	interaction_happen = drawDataFloat(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
	                                   node.getFloatPopupMode(), getCorePin().data().getFloat(),
	                                   node.getNodebase()->getState(getIndex()), valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinFloat::maxLengthOfData()
{
	return maxLengthOfDataFloat(getCorePin().data().getFloat(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinQuaternion::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::quat valueOfChange;
	WorkspaceNodeWithCoreData& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = drawDataQuaternion(
	    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
	    getCorePin().data().getQuat(), {valState, valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinQuaternion::maxLengthOfData()
{
	return maxLengthOfDataQuaternion(getCorePin().data().getQuat(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinPulse::drawData()
{
	// (PF) Pulse box: The Pulse button appears only if no input is connected.
	const Core::EValueState& state = getNode().getNodebase()->getState(getIndex());
	if (state == Core::EValueState::Editable)
	{
		if (ImGui::SmallButton(fmt::format("{}##n{}:p{}", "Pulse", getNode().getId(), m_idDiwne).c_str()))
		{
			getNode().getNodebase()->pulse(getIndex());
			return true;
		}
	}
	else // Pulse with a connected Input. Cycle Pulse outputs.
	{
		// const std::string& label = m_pin.getLabel();
		// if (!label.empty())
		// {
		//	ImGui::TextUnformatted(label.c_str());
		//	ImGui::SameLine();
		// }

		// ImGui::TextUnformatted(m_buttonText.c_str());

		ImGui::Dummy(ImVec2(0.0, 0.0)); // to avoid unlimited cycle width
	}
	return false;
}
int WorkspaceCoreOutputPinPulse::maxLengthOfData()
{
	return 0;
} /* no data with length here*/

WorkspaceCoreOutputPinScreen::WorkspaceCoreOutputPinScreen(DIWNE::Diwne& diwne, DIWNE::ID const id,
                                                           Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node)
{}

bool WorkspaceCoreOutputPinScreen::drawData()
{
	// TODO: (DR) This seems unused?
	// TODO: (DR) Remove this or whole class, maybe?
	if (getCorePin().isPluggedIn())
	{
		glm::mat4 camera =
		    Core::GraphManager::getParent(getNode().getNodebase())->data(2).getMat4(); /* JH why magic 2? */

		// ImGui::Image((void*)(intptr_t)renderTexture,I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize),ImVec2(0.0f,1.0f),
		// ImVec2(1,0));
		ImGui::Image(reinterpret_cast<ImTextureID>(renderTexture),
		             I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize) * diwne.getWorkAreaZoom(), ImVec2(0.0f, 1.0f),
		             ImVec2(1, 0)); // vertical flip
	}
	return false;
}
int WorkspaceCoreOutputPinScreen::maxLengthOfData()
{
	return 0;
} /* no data with length here*/

/* >>>> WorkspaceCoreInputPin <<<< */

WorkspaceCoreInputPin::WorkspaceCoreInputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
                                             WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(diwne, id, pin, node), m_link(diwne, id, this)
{}

void mujDebug()
{
	static bool flag = false;

	ImGui::Begin("Overlap");

	if (ImGui::Checkbox("Use AlignTextToFramePadding() ", &flag))
		ImGui::AlignTextToFramePadding();

	auto style = ImGui::GetStyle();
	ImGui::Text(fmt::format("Item   x: {}, y: {}", style.ItemSpacing.x, style.ItemSpacing.y).c_str());
	// ImGui::Text(fmt::format("Indent x: {}, y: {}", std::to_string(style.IndentSpacing)).c_str());
	ImGui::DebugDrawItemRect(ImColor(255, 127, 40, 127));
	// ImGui::Checkbox(fmt::format("##{}smooth", getId()).c_str(), &smooth))

	ImGui::End();
}


bool WorkspaceCoreInputPin::drawDiwne(DIWNE::DrawMode drawMode /*=DIWNE::DrawMode::Interacting*/)
{
	m_connectionChanged = false;

	const bool inner_interaction_happen = WorkspaceCorePin::drawDiwne(m_drawMode);

	// ImGui::DebugDrawItemRect(ImColor(255, 127, 100, 127));

	if (isConnected())
	{
		// inner_interaction_happen |= getLink().drawDiwne(m_drawMode);
		static_cast<WorkspaceDiwne&>(diwne).m_linksToDraw.push_back(&getLink());
	}
	return inner_interaction_happen;
}

void WorkspaceCoreInputPin::setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou)
{
	m_link.setStartPin(ou);
}

void WorkspaceCoreInputPin::unplug(bool logEvent)
{
	const auto rightNode = getNode().getNodebase();
	const auto* inputPin = &rightNode->getInputPins()[getIndex()];

	if (logEvent)
	{
		LOG_EVENT_DISCONNECT(inputPin->getParentPin(), inputPin);
		diwne.m_takeSnap = true;
	}

	Core::GraphManager::unplugInput(getNode().getNodebase(), getIndex());
	m_link.setStartPin(nullptr);
	m_connectionChanged = true;
}

bool WorkspaceCoreInputPin::plug(WorkspaceCoreOutputPin* ou, bool logEvent)
{
	Core::Pin const* coreInput = &(getCorePin());
	Core::Pin const* coreOutput = &(ou->getCorePin());

	if (Core::ENodePlugResult::Ok ==
	    Core::GraphManager::plug(coreOutput->getOwner(), coreInput->getOwner(), coreOutput->Index, coreInput->Index))
	{
		setConnectedWorkspaceOutput(ou);
		m_link.m_just_pluged = true;
		m_connectionChanged = true;

		if (logEvent)
		{
			LOG_EVENT_CONNECT(coreOutput, coreInput);
			diwne.m_takeSnap = true;
		}
		return true;
	}
	return false;
}

bool WorkspaceCoreInputPin::connectionChanged() const
{
	return m_connectionChanged;
}

bool WorkspaceCoreInputPin::content()
{
	float inner_interaction_happen = WorkspaceCorePin::content(); // icon

	const std::string& label = m_pin.getLabel(); // label
	if (!(m_pin.ValueType == Core::EValueType::Pulse) &&
	    !label.empty()) // no labels for pulse type in PulseToPulse and Cycle
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(label.c_str());
	}
	// std::cout << "Input Pin connected = " << isConnected() << std::endl;
	return inner_interaction_happen;
}

bool WorkspaceCoreInputPin::bypassCreateAndPlugConstructorNodeAction()
{
	return InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Pressed);
}
bool WorkspaceCoreInputPin::allowCreateAndPlugConstructorNodeAction()
{
	return diwne.getDiwneActionActive() != DIWNE::DiwneAction::NewLink && m_focusedForInteraction;
}
bool WorkspaceCoreInputPin::processCreateAndPlugConstrutorNode()
{
	if (allowCreateAndPlugConstructorNodeAction() && bypassCreateAndPlugConstructorNodeAction())
	{
		dynamic_cast<WorkspaceDiwne&>(diwne).m_workspaceDiwneAction =
		    WorkspaceDiwneAction::CreateAndPlugTypeConstructor;
		diwne.setLastActivePin<WorkspaceCoreInputPin>(
		    std::static_pointer_cast<WorkspaceCoreInputPin>(shared_from_this()));
		diwne.m_takeSnap = true;
		return true;
	}
	return false;
}

bool WorkspaceCoreInputPin::bypassUnplugAction()
{
	return InputManager::isActionTriggered("unplugInput", EKeyState::Pressed);
}
bool WorkspaceCoreInputPin::processUnplug()
{
	if (isConnected() && bypassUnplugAction())
	{
		unplug();
		return true;
	}
	return false;
}

bool WorkspaceCoreInputPin::processInteractions()
{
	bool interaction_happen = WorkspaceCorePin::processInteractions();
	interaction_happen |= processUnplug();
	interaction_happen |= processCreateAndPlugConstrutorNode();
	return interaction_happen;
}
/* >>>> WorkspaceCoreLink <<<< */

WorkspaceCoreLink::WorkspaceCoreLink(DIWNE::Diwne& diwne, DIWNE::ID id, WorkspaceCoreInputPin* endPin)
    : DIWNE::Link(diwne, id), m_endPin(endPin), m_startPin(nullptr)
{}

void WorkspaceCoreLink::unplug()
{
	m_endPin->unplug();
	m_startPin = nullptr;
}

void WorkspaceCoreLink::popupContent()
{
	if (ImGui::MenuItem("Delete"))
	{
		unplug();
	}
}

void WorkspaceCoreLink::updateEndpoints()
{
	ImVec2 start, end;
	WorkspaceCoreOutputPin* startPin = getStartPin();
	WorkspaceCoreInputPin* endPin = getEndPin();
	if (startPin)
		start = startPin->getLinkConnectionPointDiwne();
	if (endPin)
		end = endPin->getLinkConnectionPointDiwne();
	setLinkEndpointsDiwne(start, end);
}

void WorkspaceCoreLink::updateControlPointsOffsets()
{
	float offset =
	    (getEndpoint().x - getStartpoint().x) * I3T::getTheme().get(ESize::Links_ControlpointsPositionFraction);
	if (offset < I3T::getTheme().get(ESize::Links_ControlpointsPositionMin))
		offset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMin);
	diwne.mp_settingsDiwne->linkStartControlOffsetDiwne = ImVec2(offset, 0);
	diwne.mp_settingsDiwne->linkEndControlOffsetDiwne = ImVec2(-offset, 0);
}

bool WorkspaceCoreLink::initialize()
{
	updateControlPointsOffsets();

	diwne.mp_settingsDiwne->linkColor = I3T::getTheme().get(WorkspacePinColorBackground[m_endPin->getType()]);
	diwne.mp_settingsDiwne->linkThicknessDiwne = I3T::getTheme().get(ESize::Links_Thickness);

	if (m_selected)
	{
		diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne =
		    I3T::getTheme().get(ESize::Links_ThicknessSelectedBorder);
		diwne.mp_settingsDiwne->linkColorSelected = diwne.mp_settingsDiwne->linkColor;

		diwne.mp_settingsDiwne->linkColorSelected.x += I3T::getColor(EColor::Links_selected_colorShift).x;
		diwne.mp_settingsDiwne->linkColorSelected.y += I3T::getColor(EColor::Links_selected_colorShift).y;
		diwne.mp_settingsDiwne->linkColorSelected.z += I3T::getColor(EColor::Links_selected_colorShift).z;

		diwne.mp_settingsDiwne->linkColorSelected.w = I3T::getSize(ESize::Links_selected_alpha);
	}
	return false;
}

WorkspaceNodeWithCoreDataWithPins::WorkspaceNodeWithCoreDataWithPins(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase,
                                                                     bool showDataOnPins /*=true*/)
    : WorkspaceNodeWithCoreData(diwne, nodebase), m_showDataOnPins(showDataOnPins), m_minRightAlignOfRightPins(0)
{
	const auto& inputPins = m_nodebase->getInputPins();
	const auto& outputPins = m_nodebase->getOutputPins();

	m_workspaceInputs.reserve(inputPins.size());
	m_workspaceOutputs.reserve(outputPins.size());

	for (Core::Pin const& pin : inputPins)
	{
		switch (pin.ValueType)
		{
		case Core::EValueType::MatrixMul:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>(
			//                pin.Id
			//                                                                ,   pin
			//                                                                ,
			//                                                                *this));
			m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPin>(diwne, pin.Id, pin, *this));
			break;
			//            case Core::EValueType::Vec4:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector4>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Vec3:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector3>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Float:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinFloat>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Quat:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinQuaternion>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Pulse:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinPulse>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::MatrixMul:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Screen:
			//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinScreen>(
			//                pin.Id
			//                                                                                            ,   pin
			//                                                                                            ,   *this));
			//                break;
			//            case Core::EValueType::Ptr:
			//                /* Pin with type Ptr have no graphic representation */
			//                break;
		default:
			m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPin>(diwne, pin.Id, pin, *this));
			// Debug::Assert(false , "Unknown Pin type while loading input pins from
			// Core to Workspace");
		}
	}
	if (!m_showDataOnPins) /* default true, false for Camera and Sequence - they don't show data on their output pins*/
	{
		for (Core::Pin const& pin : outputPins)
		{
			m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPin>(diwne, pin.Id, pin, *this));
		}
	}
	else
	{

		auto outs = nodebase->getOutputPins();
		auto outsBegin = outs.begin();
		auto outsEnd = outs.end();

		for (Core::Pin const& pin : outputPins)
		{
			switch (pin.ValueType)
			{
			case Core::EValueType::Matrix:
				m_workspaceOutputs.push_back(
				    std::make_unique<WorkspaceCoreOutputPinMatrix4x4>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Vec4:
				m_workspaceOutputs.push_back(
				    std::make_unique<WorkspaceCoreOutputPinVector4>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Vec3:
				m_workspaceOutputs.push_back(
				    std::make_unique<WorkspaceCoreOutputPinVector3>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Float:
				m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinFloat>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Quat:
				m_workspaceOutputs.push_back(
				    std::make_unique<WorkspaceCoreOutputPinQuaternion>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Pulse:
				m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinPulse>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::MatrixMul:
				m_workspaceOutputs.push_back(
				    std::make_unique<WorkspaceCoreOutputPinMatrixMul>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Screen:
				m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinScreen>(diwne, pin.Id, pin, *this));
				break;
			case Core::EValueType::Ptr:
				/* Pin with type Ptr have no graphic representation */
				break;
			default:
				I3T_ABORT("Unknown Pin type while loading output pins from "
				          "Core to Workspace");
			}
		}
	}
}

bool WorkspaceNodeWithCoreDataWithPins::finalize()
{
	bool inner_interaction_happen = false;
	WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);

	if (!allowDrawing())
	{
		for (auto const& pin : m_workspaceInputs)
		{
			if (pin->isConnected())
			{
				Ptr<WorkspaceCoreInputPin> in = std::dynamic_pointer_cast<WorkspaceCoreInputPin>(pin);
				if (in->getLink().isLinkOnWorkArea())
				{
					// inner_interaction_happen |= in->getLink().drawDiwne();
					wd.m_linksToDraw.push_back(&pin->getLink());
				}
			}
		}
	}
	inner_interaction_happen |= WorkspaceNodeWithCoreData::finalize();
	return inner_interaction_happen;
}

bool WorkspaceNodeWithCoreDataWithPins::leftContent()
{
	bool inner_interaction_happen = false;
	bool pinsVisible = false;

	// todo (PF) effectivity???
	for (auto pin : this->getNodebase()->getInputPins())
	{
		if (pin.isRendered())
		{
			pinsVisible = true;
			break;
		}
	}

	if (pinsVisible)
	{
		WorkspaceDiwne& wd = static_cast<WorkspaceDiwne&>(diwne);

		if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
		{
			// register the connected wires only.
			// Connect them to the middle of the box left side (showing just the label)
			ImRect nodeRect = getNodeRectDiwne();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_workspaceInputs)
			{
				// if (!pin->getCorePin().isRendered()) // todo (PF) Label did not draw the wires!
				{
					pin->setConnectionPointDiwne(pinConnectionPoint);
					if (pin->isConnected())
					{
						wd.m_linksToDraw.push_back(&pin->getLink()); // register the wire
					}
				}
			}
		}
		else
		{
			for (auto const& pin : m_workspaceInputs)
			{
				if (pin->getCorePin().isRendered())
				{
					inner_interaction_happen |= pin->drawDiwne(); // pin + register the wire
				}
			}
		}
	}
	return inner_interaction_happen;
}

bool WorkspaceNodeWithCoreDataWithPins::rightContent()
{
	bool inner_interaction_happen = false;
	bool pinsVisible = false;

	for (auto pin : this->getNodebase()->getOutputPins())
	{
		if (pin.isRendered())
		{
			pinsVisible = true;
			break;
		}
	}

	if (pinsVisible)
	{
		if (m_levelOfDetail == WorkspaceLevelOfDetail::Label ||   // Label draws the wires only
		    m_levelOfDetail == WorkspaceLevelOfDetail::SetValues) // SetValues must add the invisible Pulse outputs
		{
			// register the connected wires only.
			// Connect them to the middle of the box right side (showing just the label)

			const ImRect nodeRect = getNodeRectDiwne();
			// todo (PF) pinConnectionPoint is wrong when output pulse pins are not drawn
			const ImVec2 pinConnectionPoint = ImVec2(nodeRect.Max.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : getOutputs())
			{
				if (pin->getCorePin().isRendered())
					pin->setConnectionPointDiwne(pinConnectionPoint);
				else
					int i = 7; // NOOP
			}
		}
		// else - include SetValues
		// uses getOutputsToShow()) = subset of outputs, based of the level. Override function in the WorkspaceCycle
		if (m_levelOfDetail != WorkspaceLevelOfDetail::Label) // SetValues must draw the value pin
		{
			const float prev_minRightAlign = m_minRightAlignOfRightPins; /* prev is used when node gets
			                                                                smaller (for example when switch from
			                                                                precision 2 to precision 0) */
			m_minRightAlignOfRightPins = FLT_MAX;
			for (auto const& pin : getOutputsToShow()) // subset of outputs, based of the level
			{
				if (pin->getCorePin().isRendered())
				{
					float act_align = std::max(0.0f, (m_rightRectDiwne.GetWidth() - pin->getRectDiwne().GetWidth()) *
					                                     diwne.getWorkAreaZoom()); /* no shift to the left */
					m_minRightAlignOfRightPins =
					    std::min(m_minRightAlignOfRightPins, act_align); /* over all min align is 0 when no switching
					                                                        between two node sizes */
					const float cursor_pos = ImGui::GetCursorPosX();
					// LOG_INFO(cursor_pos);
					ImGui::SetCursorPosX(cursor_pos + act_align - prev_minRightAlign); /* right align if not all output
					                                                                      pins have the same width */
					inner_interaction_happen |= pin->drawDiwne();
				}
			}
		}
	}
	else
	{
		// TODO: (DR) (zoom-aware) Uncomment perhaps
		// ImGui::Dummy(I3T::getUI()->getTheme().get(ESizeVec2::Nodes_noPinsSpacing));
	}
	return inner_interaction_happen;
}

/* >>>>> STATIC FUNCTIONS <<<<< */
bool bypassFloatFocusAction()
{
	return ImGui::IsItemHovered();
}
bool bypassFloatRaisePopupAction()
{
	return InputManager::isActionTriggered("raisePopup", EKeyState::Released);
}

bool drawDragFloatWithMap_Inline(DIWNE::Diwne& diwne, int const numberOfVisibleDecimals, FloatPopupMode& floatPopupMode,
                                 std::string const label, float& value, Core::EValueState const& valueState,
                                 bool& valueChanged)
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

	if (diwne.bypassIsItemActive())
		inner_interaction_happen = true;

	if (!inactive && !diwne.m_popupDrawn)
	{
		if (bypassFloatFocusAction() && bypassFloatRaisePopupAction())
		{
			ImGui::OpenPopup(label.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
			diwne.setPopupPosition(diwne.bypassDiwneGetPopupNewPositionAction());
		}

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

void popupFloatContent(FloatPopupMode& popupMode, float& selectedValue, bool& valueSelected)
{
	ImGui::Text("Choose value...                ");
	ImGui::Separator();

	if (ImGui::RadioButton("Angle", popupMode == FloatPopupMode::Angle))
	{
		popupMode = FloatPopupMode::Angle;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Value", popupMode == FloatPopupMode::Value))
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
bool drawData4x4(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth,
                 FloatPopupMode& floatPopupMode, const glm::mat4& data,
                 std::array<std::array<Core::EValueState, 4> const, 4> const& dataState, bool& valueChanged,
                 int& rowOfChange, int& columnOfChange, float& valueOfChange)
{
	bool inner_interaction_happen = false;
	bool actualValueChanged = false;
	float localData; /* user can change just one value at the moment */

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());
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

int maxLengthOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;
	for (int column = 0; column < 4; column++)
	{
		for (int row = 0; row < 4; row++)
		{
			act = numberOfCharWithDecimalPoint(data[column][row], numberOfVisibleDecimal);
			if (act > maximal)
			{
				maximal = act;
			}
		}
	}
	return maximal;
}

bool drawDataVec4(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth,
                  FloatPopupMode& floatPopupMode, const glm::vec4& data,
                  std::array<Core::EValueState, 4> const& dataState, bool& valueChanged, glm::vec4& valueOfChange)
{
	//    const glm::vec4& coreData = m_nodebase->data(index).getVec4();
	//    const Core::DataMap& coreMap = m_nodebase->getDataMapRef();

	bool actualValueChanged = false;
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

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

int maxLengthOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 4; column++)
	{
		act = numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

bool drawDataVec3(DIWNE::Diwne& diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, float dataWidth,
                  FloatPopupMode& floatPopupMode, const glm::vec3& data,
                  std::array<Core::EValueState, 3> const& dataState, bool& valueChanged, glm::vec3& valueOfChange)
{
	bool actualValueChanged = false;
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

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
int maxLengthOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 3; column++)
	{
		act = numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

bool drawDataFloat(DIWNE::Diwne& diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, float dataWidth,
                   FloatPopupMode& floatPopupMode, const float& data, Core::EValueState const& dataState,
                   bool& valueChanged, float& valueOfChange)
{
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

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
int maxLengthOfDataFloat(const float& data, int numberOfVisibleDecimal)
{
	return numberOfCharWithDecimalPoint(data, numberOfVisibleDecimal);
}

bool drawDataQuaternion(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
                        float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data,
                        std::array<Core::EValueState, 4> const& dataState, bool& valueChanged, glm::quat& valueOfChange)
{
	bool inner_interaction_happen = false;
	bool actualValueChanged = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                    I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.getWorkAreaZoom());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    I3T::getSize(ESizeVec2::Nodes_ItemsSpacing) * diwne.getWorkAreaZoom());

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

int maxLengthOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal)
{
	int act, maximal = 0;

	for (int column = 0; column < 4; column++)
	{
		act = numberOfCharWithDecimalPoint(data[column], numberOfVisibleDecimal);
		if (act > maximal)
		{
			maximal = act;
		}
	}

	return maximal;
}

void drawMenuLevelOfDetail_builder(Ptr<WorkspaceNodeWithCoreData> node,
                                   std::vector<WorkspaceLevelOfDetail> const& levels_of_detail)
{
	if (ImGui::BeginMenu("Level of detail"))
	{
		// ImGui::TextUnformatted(fmt::format("Actual level: {}",
		// WorkspaceLevelOfDetailName[node->getLevelOfDetail()]).c_str());
		// ImGui::Separator();

		for (auto const& levelOfDetail : levels_of_detail)
		{
			if (ImGui::MenuItem(WorkspaceLevelOfDetailName[levelOfDetail].c_str(), NULL,
			                    node->getLevelOfDetail() == levelOfDetail, true))
			{
				node->setLevelOfDetail(levelOfDetail);
			}
		}
		ImGui::EndMenu();
	}
}
