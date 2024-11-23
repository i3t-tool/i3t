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
#include "CorePins.h"

#include "Core/Input/InputManager.h"
#include "DataRenderer.h"
#include "GUI/Workspace/WorkspaceDiwne.h"

using namespace Workspace;

/// \todo Remove these.
/* DIWNE - \todo JH to remove, but I need something what use instead -> from
 * Type get Shape and Color */
/* \todo MH is it possible to store std::map in Theme? */
std::map<Core::EValueType, EColor> Workspace::PinColorBackground = {
    {Core::EValueType::Float, EColor::FloatPin},         {Core::EValueType::Matrix, EColor::MatrixPin},
    {Core::EValueType::MatrixMul, EColor::MatrixMulPin}, {Core::EValueType::Pulse, EColor::PulsePin},
    {Core::EValueType::Quat, EColor::QuatPin},           {Core::EValueType::Screen, EColor::ScreenPin},
    {Core::EValueType::Vec3, EColor::Vec3Pin},           {Core::EValueType::Vec4, EColor::Vec4Pin}};

std::map<Core::EValueType, DIWNE::IconType> Workspace::PinShapeBackground = {
    {Core::EValueType::Float, DIWNE::IconType::Rectangle},     {Core::EValueType::Matrix, DIWNE::IconType::Rectangle},
    {Core::EValueType::MatrixMul, DIWNE::IconType::Rectangle}, {Core::EValueType::Pulse, DIWNE::IconType::Rectangle},
    {Core::EValueType::Quat, DIWNE::IconType::Rectangle},      {Core::EValueType::Screen, DIWNE::IconType::Rectangle},
    {Core::EValueType::Vec3, DIWNE::IconType::Rectangle},      {Core::EValueType::Vec4, DIWNE::IconType::Rectangle}};

std::map<Core::EValueType, DIWNE::IconType> Workspace::PinShapeForeground = {
    {Core::EValueType::Float, DIWNE::IconType::TriangleRight},
    {Core::EValueType::Matrix, DIWNE::IconType::TriangleRight},
    {Core::EValueType::MatrixMul, DIWNE::IconType::Cross},
    {Core::EValueType::Pulse, DIWNE::IconType::TriangleRight},
    {Core::EValueType::Quat, DIWNE::IconType::TriangleRight},
    {Core::EValueType::Screen, DIWNE::IconType::TriangleRight},
    {Core::EValueType::Vec3, DIWNE::IconType::TriangleRight},
    {Core::EValueType::Vec4, DIWNE::IconType::TriangleRight}};

std::map<Core::EValueType, EColor> Workspace::PinColorForeground = {
    {Core::EValueType::Float, EColor::InnerFloatPin},         {Core::EValueType::Matrix, EColor::InnerMatrixPin},
    {Core::EValueType::MatrixMul, EColor::InnerMatrixMulPin}, {Core::EValueType::Pulse, EColor::InnerPulsePin},
    {Core::EValueType::Quat, EColor::InnerQuatPin},           {Core::EValueType::Screen, EColor::InnerScreenPin},
    {Core::EValueType::Vec3, EColor::InnerVec3Pin},           {Core::EValueType::Vec4, EColor::InnerVec4Pin}};

CorePin::CorePin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode& node)
    : DIWNE::Pin(diwne), m_pin(pin), m_node(node), m_iconRectDiwne(ImRect(0, 0, 0, 0))
{}

bool CorePin::allowInteraction() const
{
	return m_pin.isDisabled();
}

/**
 * \brief Draw the pin icon
 * \return false - no interaction allowed
 */
void CorePin::content(DIWNE::DrawInfo& context)
{
	// const bool interaction_happen = false; // no interaction in this function allowed
	if (getCorePin().isRendered())
	{
		const float alpha = ImGui::GetStyle().Alpha;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		const DIWNE::IconType iconTypeBg = PinShapeBackground[getType()];
		const ImColor iconColorBg = I3T::getColor(PinColorBackground[getType()]);
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
			iconTypeFg = PinShapeForeground[getType()];
		}
		const ImColor iconColorFg = I3T::getColor(PinColorForeground[getType()]);

		const ImVec2 iconSize = I3T::getSize(ESizeVec2::Nodes_IconSize) * diwne.getWorkAreaZoom();

		ImGuiContext& g = *GImGui;

		// space between icon symbol and icon boundary
		const float padding = I3T::getSize(ESize::Pins_IconPadding) * diwne.getWorkAreaZoom();

		// TODO: (DR) Don't really see why the "filled" parameters depends on isConnected(), currently the outlines are
		//   not visible anyway so we're just drawing stuff twice for no reason
		// todo (PF) - I have temporally added the pin border drawing of not-connected pins
		// connected pins have no border now
		diwne.m_renderer->DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg,
		                           createColor(232, 232, 232, 255) /*iconColorFg*/, iconSize,
		                           ImVec4(padding, padding, padding, padding), isConnected());
		m_iconRectDiwne =
		    ImRect(diwne.screen2diwne(ImGui::GetItemRectMin()), diwne.screen2diwne(ImGui::GetItemRectMax()));

		ImGui::PopStyleVar();
	}
}

void CorePin::popupContent(DIWNE::DrawInfo& context) {}

bool CorePin::bypassFocusForInteractionAction()
{
	return m_iconRectDiwne.Contains(diwne.screen2diwne(diwne.m_input->bypassGetMousePos()));
}

const Core::Pin& CorePin::getCorePin() const
{
	return m_pin;
}

int CorePin::getIndex() const
{
	return m_pin.Index;
}

PinKind CorePin::getKind() const
{
	return m_pin.IsInput ? PinKind::Input : PinKind::Output;
}

Core::EValueType CorePin::getType() const
{
	return m_pin.ValueType;
}

/* \todo JH \todo MH implement this function in Core? */
bool CorePin::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

// TODO: Uncomment
// bool CorePin::processDrag()
//{
//	const ImVec2 origin = getLinkConnectionPointDiwne();
//	const ImVec2 actual = diwne.screen2diwne(ImGui::GetIO().MousePos);
//
//	if (getKind() == PinKind::Output)
//		diwne.getHelperLink().setLinkEndpointsDiwne(origin, actual);
//	else
//		diwne.getHelperLink().setLinkEndpointsDiwne(actual, origin);
//
//	diwne.mp_settingsDiwne->linkColor = I3T::getTheme().get(PinColorBackground[getType()]);
//	diwne.mp_settingsDiwne->linkThicknessDiwne = I3T::getTheme().get(ESize::Links_Thickness);
//
//	return Pin::processDrag();
//}
//
// bool CorePin::processConnectionPrepared()
//{
//	/* here it is when goal pin is hoovered */
//	CorePin *input, *output;
//	if (getKind() == PinKind::Input)
//	{
//		input = this;
//		output = diwne.getLastActivePin<CorePin>().get();
//	}
//	else
//	{
//		input = diwne.getLastActivePin<CorePin>().get();
//		output = this;
//	}
//
//	const auto& coreInput = input->getCorePin();
//	const auto& coreOutput = output->getCorePin();
//
//	switch (Core::GraphManager::isPlugCorrect(coreInput, coreOutput))
//	{
//	case Core::ENodePlugResult::Ok:
//		diwne.showTooltipLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
//		if (bypassReleaseAction())
//		{
//			CoreInPin* in = dynamic_cast<CoreInPin*>(input);
//			in->plug(dynamic_cast<CoreOutPin*>(output));
//		}
//		break;
//
//	case Core::ENodePlugResult::Err_MismatchedPinTypes:
//		diwne.showTooltipLabel("Mismatched pin Types (matrix/float/vec/...)",
//		                       I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	case Core::ENodePlugResult::Err_MismatchedPinKind:
//		diwne.showTooltipLabel("Mismatched pin Kinds (in/out)", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	case Core::ENodePlugResult::Err_Loopback: /// Same nodes.
//		diwne.showTooltipLabel("Loop to the same node", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	case Core::ENodePlugResult::Err_NonexistentPin:
//		diwne.showTooltipLabel("Pin does not exist :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	case Core::ENodePlugResult::Err_Loop:
//		diwne.showTooltipLabel("Loop not allowed", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	case Core::ENodePlugResult::Err_DisabledPin:
//		diwne.showTooltipLabel("Pin is disabled :-D", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//		break;
//	default: // unreachable - all enum values are covered
//		diwne.showTooltipLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//	}
//	return true;
//}
//
// bool CorePin::processFocused()
//{
//	return DiwneObject::processFocused();
//}

CoreOutPin::CoreOutPin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode& node) : CorePin(diwne, pin, node) {}

/**
 * \brief Draw the output Pin: label and icon [float >]
 * \return true if value changed
 */
void CoreOutPin::content(DIWNE::DrawInfo& context)
{
	const std::string& label = m_pin.getLabel();
	// todo (PF) Label and value order should be switched (used by cycle, mat->TR, x->floats, pulse)
	// probably not - would be good for scalars, but wrong for mat4
	// if (!label.empty())
	if (m_pin.ValueType != Core::EValueType::Pulse && !label.empty()) // no labels for pulse and cycle
	{
		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();
	}
	CorePin::content(context); // icon
}

DataOutPin::DataOutPin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode& node) : CoreOutPin(diwne, pin, node) {}

void DataOutPin::content(DIWNE::DrawInfo& context)
{
	bool interaction_happen = false;
	// if (getNode().getLevelOfDetail() == LevelOfDetail::Full ||
	//     getNode().getLevelOfDetail() == LevelOfDetail::SetValues || // for cycle box
	//     getNode().getLevelOfDetail() == LevelOfDetail::LightCycle)  // for cycle box
	if (getNode().getLevelOfDetail() != LevelOfDetail::Label)
	{
		interaction_happen |= drawData();
		ImGui::SameLine();
	}
	CoreOutPin::content(context); // label and icon
}

/* >>>> Pin types <<<< */

bool DataOutPinMatrix::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	int rowOfChange, columnOfChange;
	float valueOfChange;
	CoreNode& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen =
	    DataRenderer::drawData4x4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
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

int DataOutPinMatrix::maxLengthOfData()
{
	return DataRenderer::maxLengthOfData4x4(getCorePin().data().getMat4(), getNode().getNumberOfVisibleDecimal());
}

bool DataOutPinVector4::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::vec4 valueOfChange;
	CoreNode& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = DataRenderer::drawDataVec4(
	    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
	    getCorePin().data().getVec4(), {valState, valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int DataOutPinVector4::maxLengthOfData()
{
	return DataRenderer::maxLengthOfDataVec4(getCorePin().data().getVec4(), getNode().getNumberOfVisibleDecimal());
}

bool DataOutPinVector3::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::vec3 valueOfChange;
	CoreNode& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = DataRenderer::drawDataVec3(
	    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
	    getCorePin().data().getVec3(), {valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int DataOutPinVector3::maxLengthOfData()
{
	return DataRenderer::maxLengthOfDataVec3(getCorePin().data().getVec3(), getNode().getNumberOfVisibleDecimal());
}

bool DataOutPinFloat::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	float valueOfChange;
	CoreNode& node = getNode();

	interaction_happen = DataRenderer::drawDataFloat(
	    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
	    getCorePin().data().getFloat(), node.getNodebase()->getState(getIndex()), valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int DataOutPinFloat::maxLengthOfData()
{
	return DataRenderer::maxLengthOfDataFloat(getCorePin().data().getFloat(), getNode().getNumberOfVisibleDecimal());
}

bool DataOutPinQuat::drawData()
{
	bool valueChanged = false, interaction_happen = false;
	// int                        rowOfChange, columnOfChange;
	glm::quat valueOfChange;
	CoreNode& node = getNode();

	Core::EValueState valState = node.getNodebase()->getState(getIndex());
	interaction_happen = DataRenderer::drawDataQuaternion(
	    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
	    getCorePin().data().getQuat(), {valState, valState, valState, valState}, valueChanged, valueOfChange);

	if (valueChanged)
	{
		node.getNodebase()->setValue(valueOfChange);
		node.updateDataItemsWidth();
	}
	return interaction_happen;
}

int DataOutPinQuat::maxLengthOfData()
{
	return DataRenderer::maxLengthOfDataQuaternion(getCorePin().data().getQuat(),
	                                               getNode().getNumberOfVisibleDecimal());
}

bool DataOutPinPulse::drawData()
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
int DataOutPinPulse::maxLengthOfData()
{
	return 0;
} /* no data with length here*/

DataOutPinScreen::DataOutPinScreen(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode& node)
    : DataOutPin(diwne, pin, node)
{}

bool DataOutPinScreen::drawData()
{
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
int DataOutPinScreen::maxLengthOfData()
{
	return 0;
} /* no data with length here*/

/* >>>> WorkspaceCoreInputPin <<<< */

CoreInPin::CoreInPin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode& node)
    : CorePin(diwne, pin, node), m_link(diwne, this)
{}

void CoreInPin::drawDiwne(DIWNE::DrawInfo& context, DIWNE::DrawMode drawMode)
{
	// TODO: Investigate this method, why do we need to touch the DIWNE implementation?
	DIWNE::DrawMode drawModeTEST = m_interactive ? drawMode : DIWNE::DrawMode::JustDraw;

	m_connectionChanged = false;

	//	const bool inner_interaction_happen = CorePin::drawDiwne(m_drawMode);
	CorePin::drawDiwne(context);

	// ImGui::DebugDrawItemRect(ImColor(255, 127, 100, 127));

	if (isConnected())
	{
		// inner_interaction_happen |= getLink().drawDiwne(m_drawMode);
		static_cast<WorkspaceDiwne&>(diwne).m_linksToDraw.push_back(&getLink());
	}
}

void CoreInPin::setConnectedWorkspaceOutput(CoreOutPin* ou)
{
	m_link.setStartPin(ou);
}

void CoreInPin::unplug(bool logEvent)
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

bool CoreInPin::plug(CoreOutPin* ou, bool logEvent)
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

bool CoreInPin::connectionChanged() const
{
	return m_connectionChanged;
}

void CoreInPin::content(DIWNE::DrawInfo& context)
{
	CorePin::content(context); // icon

	const std::string& label = m_pin.getLabel(); // label
	if (m_pin.ValueType != Core::EValueType::Pulse &&
	    !label.empty()) // no labels for pulse type in PulseToPulse and Cycle
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(label.c_str());
	}
	// std::cout << "Input Pin connected = " << isConnected() << std::endl;
}

bool CoreInPin::bypassCreateAndPlugConstructorNodeAction()
{
	return InputManager::isActionTriggered("createAndPlugConstructor", EKeyState::Pressed);
}
bool CoreInPin::allowCreateAndPlugConstructorNodeAction()
{
	return diwne.getDiwneActionActive() != DIWNE::DiwneAction::NewLink && m_focusedForInteraction;
}
bool CoreInPin::processCreateAndPlugConstrutorNode()
{
	if (allowCreateAndPlugConstructorNodeAction() && bypassCreateAndPlugConstructorNodeAction())
	{
		dynamic_cast<WorkspaceDiwne&>(diwne).m_workspaceDiwneAction =
		    WorkspaceDiwneAction::CreateAndPlugTypeConstructor;
		diwne.setLastActivePin<CoreInPin>(std::static_pointer_cast<CoreInPin>(shared_from_this()));
		diwne.m_takeSnap = true;
		return true;
	}
	return false;
}

bool CoreInPin::bypassUnplugAction()
{
	return InputManager::isActionTriggered("unplugInput", EKeyState::Pressed);
}
bool CoreInPin::processUnplug()
{
	if (isConnected() && bypassUnplugAction())
	{
		unplug();
		return true;
	}
	return false;
}
// TODO: Uncomment
// bool CoreInPin::processInteractions()
//{
//	bool interaction_happen = CorePin::processInteractions();
//	interaction_happen |= processUnplug();
//	interaction_happen |= processCreateAndPlugConstrutorNode();
//	return interaction_happen;
//}
