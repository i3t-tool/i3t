#include "WorkspaceElementsWithCoreData.h"

#include "misc/cpp/imgui_stdlib.h" /* for changable text */

#include "State/StateManager.h"

#include "../Windows/WorkspaceWindow.h"
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
	if (m_topLabel.empty())
	{
		m_topLabel = m_nodebase->getLabel();
	}
	ImGui::Indent(ImGui::GetStyle().ItemSpacing.x);
	const char* topLabel = m_topLabel.c_str();
	bool interaction_happen;
	ImGui::PushItemWidth(ImGui::CalcTextSize(topLabel, topLabel + strlen(topLabel)).x + 5);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.00)); /* invisible bg */

	if (m_isLabelBeingEdited)
	{
		interaction_happen = ImGui::InputText(fmt::format("##{}topLabel", m_labelDiwne).c_str(), &(this->m_topLabel),
		                                      ImGuiInputTextFlags_NoHorizontalScroll);
		auto id = ImGui::GetItemID();
		if (m_isFirstDraw)
		{
			ImGui::ActivateItem(id);
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
	ImGui::SameLine();
	// adding dummy space for block interaction
	ImGui::Dummy(ImVec2(50, 10)); /* dummy for snap some item for same space on left right of label  */

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
	setDataItemsWidth();
}

float WorkspaceNodeWithCoreData::getDataItemsWidth()
{
	return m_dataItemsWidth;
}

float WorkspaceNodeWithCoreData::setDataItemsWidth()
{
	float size = ImGui::GetFontSize(); // /* \todo JH do not know why return incorrect
	                                   // value when zoom and than add node, but after
	                                   // changing value return correct one...  get width
	                                   // */ get current font size (= height in pixels) of
	                                   // current font with current scale applied
	float oneCharWidth = size / 2, padding = I3T::getSize(ESize::Nodes_FloatInnerPadding);
	m_dataItemsWidth = (float) (maxLenghtOfData()) * oneCharWidth + 2 * padding;
	return m_dataItemsWidth;
}

Core::DataMap const* WorkspaceNodeWithCoreData::setDataMap(Core::DataMap const* mapToSet)
{
	// m_nodebase->setDataMap(mapToSet);
	return getDataMap();
}

Core::DataMap const* WorkspaceNodeWithCoreData::getDataMap()
{
	return m_nodebase->getDataMap();
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
		              App::get().getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset));
		// move original node behind new one
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

void WorkspaceNodeWithCoreData::popupContent()
{
	drawMenuSetEditable();

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
		m_isDraged = true;
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
bool WorkspaceCorePin::content()
{
	bool interaction_happen = false;
	if (getCorePin().shouldRenderPins())
	{
		float alpha = ImGui::GetStyle().Alpha;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		DIWNE::IconType iconTypeBg = WorkspacePinShapeBackground[getType()];
		ImColor iconColorBg = I3T::getColor(WorkspacePinColorBackground[getType()]);
		DIWNE::IconType iconTypeFg = WorkspacePinShapeForeground[getType()];
		ImColor iconColorFg = I3T::getColor(WorkspacePinColorForeground[getType()]);

		ImVec2 iconSize = I3T::getSize(ESizeVec2::Nodes_IconSize) * diwne.getWorkAreaZoom();

		float padding = I3T::getSize(ESize::Pins_IconPadding) * diwne.getWorkAreaZoom();

		// TODO: (DR) Don't really see why the "filled" parameters depends on isConnected(), currently the outlines are
		// not visible anyway so we're just drawing stuff twice for no reason
		diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, iconSize,
		               ImVec4(padding, padding, padding, padding), isConnected());
		m_iconRectDiwne =
		    ImRect(diwne.screen2diwne(ImGui::GetItemRectMin()), diwne.screen2diwne(ImGui::GetItemRectMax()));

		ImGui::PopStyleVar();
	}
	return interaction_happen;
}

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
	ImVec2 origin = getLinkConnectionPointDiwne();
	ImVec2 actual = diwne.screen2diwne(ImGui::GetIO().MousePos);

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
			diwne.m_takeSnap = true;
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
		diwne.showTooltipLabel("Loop to same node", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_NonexistentPin:
		diwne.showTooltipLabel("Pin not exists :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_Loop:
		diwne.showTooltipLabel("Unallowed loop", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	case Core::ENodePlugResult::Err_DisabledPin:
		diwne.showTooltipLabel("Pin is desabled :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	default:
		diwne.showTooltipLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
	}
	return true;
}

WorkspaceCoreOutputPin::WorkspaceCoreOutputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
                                               WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(diwne, id, pin, node)
{}

bool WorkspaceCoreOutputPin::content()
{
	const std::string& label = m_pin.getLabel();
	if (!label.empty())
	{
		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();
	}
	const auto inner_interaction_happen = WorkspaceCorePin::content();
	return inner_interaction_happen;
}

WorkspaceCoreOutputPinWithData::WorkspaceCoreOutputPinWithData(DIWNE::Diwne& diwne, DIWNE::ID const id,
                                                               Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCoreOutputPin(diwne, id, pin, node)
{}

bool WorkspaceCoreOutputPinWithData::content()
{
	bool interaction_happen = false;
	if (getNode().getLevelOfDetail() == WorkspaceLevelOfDetail::Full)
	{
		interaction_happen |= drawData();
		ImGui::SameLine();
	}
	interaction_happen |= WorkspaceCoreOutputPin::content();
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
		node.setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinMatrix4x4::maxLengthOfData()
{
	return maxLenghtOfData4x4(getCorePin().data().getMat4(), getNode().getNumberOfVisibleDecimal());
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
		node.setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinVector4::maxLengthOfData()
{
	return maxLenghtOfDataVec4(getCorePin().data().getVec4(), getNode().getNumberOfVisibleDecimal());
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
		node.setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinVector3::maxLengthOfData()
{
	return maxLenghtOfDataVec3(getCorePin().data().getVec3(), getNode().getNumberOfVisibleDecimal());
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
		node.setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinFloat::maxLengthOfData()
{
	return maxLenghtOfDataFloat(getCorePin().data().getFloat(), getNode().getNumberOfVisibleDecimal());
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
		node.setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceCoreOutputPinQuaternion::maxLengthOfData()
{
	return maxLenghtOfDataQuaternion(getCorePin().data().getQuat(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinPulse::drawData()
{
	if (ImGui::SmallButton(fmt::format("{}##n{}:p{}", m_buttonText, getNode().getId(), m_idDiwne).c_str()))
	{
		getNode().getNodebase()->pulse(getIndex());
		return true;
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
		    Core::GraphManager::getParent(getNode().getNodebase())->getData(2).getMat4(); /* JH why magic 2? */

		// ImGui::Image((void*)(intptr_t)renderTexture,I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize),ImVec2(0.0f,1.0f),
		// ImVec2(1,0));
		ImGui::Image(reinterpret_cast<ImTextureID>(renderTexture),
		             I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize) * diwne.getWorkAreaZoom(), ImVec2(0.0f, 1.0f),
		             ImVec2(1, 0)); // vertiocal flip
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

void WorkspaceCoreInputPin::setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou)
{
	m_link.setStartPin(ou);
}

void WorkspaceCoreInputPin::unplug()
{
	const auto rightNode = getNode().getNodebase();
	const auto* inputPin = &rightNode->getInputPins()[getIndex()];

	LOG_EVENT_DISCONNECT(inputPin->getParentPin(), inputPin);

	Core::GraphManager::unplugInput(getNode().getNodebase(), getIndex());
	m_link.setStartPin(nullptr);
	diwne.m_takeSnap = true;
	m_connection_changed = true;
}

bool WorkspaceCoreInputPin::drawDiwne(DIWNE::DrawMode drawMode /*=DIWNE::DrawMode::Interacting*/)
{
	m_connection_changed = false;
	bool inner_interaction_happen = WorkspaceCorePin::drawDiwne(m_drawMode);
	if (isConnected())
	{
		// inner_interaction_happen |= getLink().drawDiwne(m_drawMode);
		static_cast<WorkspaceDiwne&>(diwne).m_linksToDraw.push_back(&getLink());
	}
	return inner_interaction_happen;
}

void WorkspaceCoreInputPin::plug(WorkspaceCoreOutputPin* ou)
{
	Core::Pin const* coreInput = &(getCorePin());
	Core::Pin const* coreOutput = &(ou->getCorePin());

	if (Core::ENodePlugResult::Ok ==
	    Core::GraphManager::plug(coreOutput->getOwner(), coreInput->getOwner(), coreOutput->Index, coreInput->Index))
	{
		setConnectedWorkspaceOutput(ou);
		m_link.m_just_pluged = true;
		m_connection_changed = true;

		LOG_EVENT_CONNECT(coreOutput, coreInput);
	}
}

bool WorkspaceCoreInputPin::content()
{
	float inner_interaction_happen = WorkspaceCorePin::content();
	const std::string& label = m_pin.getLabel();
	if (!label.empty())
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(label.c_str());
	}
	return inner_interaction_happen;
}

bool WorkspaceCoreInputPin::bypassCreateAndPlugConstrutorNodeAction()
{
	return InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Pressed);
}
bool WorkspaceCoreInputPin::allowCreateAndPlugConstrutorNodeAction()
{
	return diwne.getDiwneActionActive() != DIWNE::DiwneAction::NewLink && m_focusedForInteraction;
}
bool WorkspaceCoreInputPin::processCreateAndPlugConstrutorNode()
{
	if (allowCreateAndPlugConstrutorNodeAction() && bypassCreateAndPlugConstrutorNodeAction())
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
	if (!m_showDataOnPins) /* for example sequence do not show data in output pins
	                        */
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

	for (auto pin : this->getNodebase()->getInputPins())
	{
		if (pin.shouldRenderPins())
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
			ImRect nodeRect = getNodeRectDiwne();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Min.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : m_workspaceInputs)
			{
				if (!pin->getCorePin().shouldRenderPins())
					continue;

				pin->setConnectionPointDiwne(pinConnectionPoint);
				if (pin->isConnected())
				{
					wd.m_linksToDraw.push_back(&pin->getLink());
				}
			}
		}
		else
		{
			for (auto const& pin : m_workspaceInputs)
			{
				if (!pin->getCorePin().shouldRenderPins())
				{
					continue;
				}

				inner_interaction_happen |= pin->drawDiwne();
				/* is in pin->drawDiwne()
				      if (pin->isconnected())
				      {
				        wd.m_linkstodraw.push_back(&pin->getlink());
				      }
				*/
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
		if (pin.shouldRenderPins())
		{
			pinsVisible = true;
			break;
		}
	}

	if (pinsVisible)
	{
		if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
		{
			ImRect nodeRect = getNodeRectDiwne();
			ImVec2 pinConnectionPoint = ImVec2(nodeRect.Max.x, (nodeRect.Min.y + nodeRect.Max.y) / 2);
			for (auto const& pin : getOutputs())
			{
				if (!pin->getCorePin().shouldRenderPins())
					continue;

				pin->setConnectionPointDiwne(pinConnectionPoint);
			}
		}
		else
		{
			float act_align, prev_minRightAlign = m_minRightAlignOfRightPins; /* prev is used when node gets smaller
			                                                                     (for example when switch from
			                                                                     precision 2 to precision 0) */
			m_minRightAlignOfRightPins = FLT_MAX;
			for (auto const& pin : getOutputsToShow())
			{
				if (!pin->getCorePin().shouldRenderPins())
					continue;

				act_align = std::max(0.0f, (m_rightRectDiwne.GetWidth() - pin->getRectDiwne().GetWidth()) *
				                               diwne.getWorkAreaZoom()); /* no shift to left */
				m_minRightAlignOfRightPins =
				    std::min(m_minRightAlignOfRightPins, act_align); /* over all min align is 0 when no switching
				                                                        between two node sizes */
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + act_align -
				                     prev_minRightAlign); /* right align if not all output
				                                             pins have same width */
				inner_interaction_happen |= pin->drawDiwne();
			}
		}
	}
	else
	{
		ImGui::Dummy(App::get().getUI()->getTheme().get(ESizeVec2::Nodes_noPinsSpacing));
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	valueChanged = false;

	/* Drawing is row-wise */
	ImGui::BeginGroup();
	for (int rows = 0; rows < 4; rows++)
	{
		if (rows == 1)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() -
			                     ImGui::GetStyle().ItemSpacing.y); /*\todo JH hard to say why...*/
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

int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
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
	//    const glm::vec4& coreData = m_nodebase->getData(index).getVec4();
	//    const Core::DataMap& coreMap = m_nodebase->getDataMapRef();

	bool actualValueChanged = false;
	bool inner_interaction_happen = false;

	ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

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

int maxLenghtOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal)
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

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
int maxLenghtOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal)
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

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
int maxLenghtOfDataFloat(const float& data, int numberOfVisibleDecimal)
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	valueChanged = false;
	ImGui::BeginGroup();
	for (int columns = 0; columns < 4; columns++)
	{
		valueOfChange[columns] = data[columns];
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
		    valueOfChange[columns], dataState[columns], actualValueChanged);

		if (actualValueChanged)
			valueChanged = true;
		if (columns < 3)
			ImGui::SameLine();
	}
	ImGui::EndGroup();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	return inner_interaction_happen;
}

int maxLenghtOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal)
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
