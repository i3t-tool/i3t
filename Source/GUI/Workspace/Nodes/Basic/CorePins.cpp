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

CorePin::CorePin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode* node, bool isInput)
    : DIWNE::Pin(diwne, node, isInput), m_pin(pin)
{}

bool CorePin::allowInteraction() const
{
	return !m_pin.isDisabled();
}

/**
 * \brief Draw the pin icon
 * \return false - no interaction allowed
 */
void CorePin::content(DIWNE::DrawInfo& context)
{
	// TODO: The entire pin shouldn't be rendered when isRendered is false.
	//  Either handle this here or in the Node with pins
	// const bool interaction_happen = false; // no interaction in this function allowed
	if (getCorePin().isRendered())
	{
		if (isInput())
		{
			drawPin(context);
			ImGui::SameLine();
			drawLabel(context);
			drawDataEx(context);
		}
		else
		{
			drawDataEx(context);
			drawLabel(context);
			drawPin(context);
		}
	}
}

void CorePin::drawPin(DIWNE::DrawInfo& context)
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

	const ImVec2 iconSize = I3T::getSize(ESizeVec2::Nodes_IconSize) * diwne.getZoom();

	ImGuiContext& g = *GImGui;

	// space between icon symbol and icon boundary
	const float padding = I3T::getSize(ESize::Pins_IconPadding) * diwne.getZoom();

	// TODO: (DR) Don't really see why the "filled" parameters depends on isConnected(), currently the outlines are
	//   not visible anyway so we're just drawing stuff twice for no reason
	// todo (PF) - I have temporally added the pi n border drawing of not-connected pins
	// connected pins have no border now

	// We're using a disabled IconButton, so that when its pressed / dragged it does not set an ActiveID in ImGui.
	// Setting ActiveID is the same thing what a DragFloat does when it drags, it disables interaction with other items
	// until the drag/press operation stops. This is not desirable for a pin as we want other things to hover still.
	diwne.canvas().IconButton("PinIcon", true, iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg,
	                          createColor(232, 232, 232, 255) /*iconColorFg*/, iconSize,
	                          ImVec4(padding, padding, padding, padding), isConnected());
	m_pinRect = ImRect(diwne.canvas().screen2diwne(ImGui::GetItemRectMin()),
	                   diwne.canvas().screen2diwne(ImGui::GetItemRectMax()));

	ImGui::PopStyleVar();
}

void CorePin::drawLabel(DIWNE::DrawInfo& context)
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
}

void CorePin::drawDataEx(DIWNE::DrawInfo& context)
{
	bool interaction_happen = false;
	// if (getNode().getLevelOfDetail() == LevelOfDetail::Full ||
	//     getNode().getLevelOfDetail() == LevelOfDetail::SetValues || // for cycle box
	//     getNode().getLevelOfDetail() == LevelOfDetail::LightCycle)  // for cycle box
	if (static_cast<CoreNode*>(getNode())->getLevelOfDetail() != LevelOfDetail::Label)
	{
		if (!m_isInput && m_showData)
			drawData(context);
	}
}

void CorePin::popupContent(DIWNE::DrawInfo& context) {}

bool CorePin::preparePlug(Pin* otherPin, DIWNE::Link* link, bool hovering)
{
	CorePin *input, *output;

	// In I3T we always call the plug() method on the input pin, not the output pin.
	if (isInput())
	{
		input = this;
		output = static_cast<CorePin*>(otherPin);
	}
	else
	{
		input = static_cast<CorePin*>(otherPin);
		output = this;
	}

	const auto& coreInput = input->getCorePin();
	const auto& coreOutput = output->getCorePin();

	switch (Core::GraphManager::isPlugCorrect(coreInput, coreOutput))
	{
	case Core::ENodePlugResult::Ok:
		diwne.showTooltipLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
		if (!hovering)
		{
			input->plugLink(output, link, true);
			return true;
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
	return false;
}

bool CorePin::plugLink(Pin* otherPin, DIWNE::Link* link, bool logEvent)
{
	I3T_ASSERT(!isInput() || !otherPin->isInput(), "Both pins cannot be input pins!");

	Pin* startPin = otherPin;
	Pin* endPin = this;
	if (otherPin->isInput())
		std::swap(startPin, endPin); // Other pin is input, this pin is output

	const Core::Pin* coreOutput = &(static_cast<CorePin*>(startPin)->getCorePin());
	const Core::Pin* coreInput = &(static_cast<CorePin*>(endPin)->getCorePin());

	// Check if Core pins can be plugged
	Core::ENodePlugResult plugResult = Core::GraphManager::isPlugCorrect(*coreInput, *coreOutput);
	if (plugResult != Core::ENodePlugResult::Ok)
		return false;

	// Then plug the UI
	if (!Pin::plugLink(otherPin, link, logEvent))
	{
		LOG_ERROR("[WORKSPACE] Connected Core pins but failed to connect UI pins!");
		return false;
	}

	// TODO: (DR) This can actually be moved to onPlug, it should never fail as we check isPlugCorrect() above
	// And finally plug the Core pins, after UI unplug callbacks were already called
	plugResult =
	    Core::GraphManager::plug(coreOutput->getOwner(), coreInput->getOwner(), coreOutput->Index, coreInput->Index);
	if (plugResult != Core::ENodePlugResult::Ok)
	{
		LOG_ERROR("[WORKSPACE] Connected UI pins but failed to connect Core pins!");
		return false;
	}
	return true;
}

void CorePin::onPlug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool isStartPin, bool logEvent)
{
	Pin::onPlug(otherPin, link, isStartPin, logEvent);
	if (isInput())
	{
		if (logEvent)
		{
			Core::Pin const* coreInput = &(getCorePin());
			Core::Pin const* coreOutput = &(static_cast<CorePin*>(otherPin)->getCorePin());
			LOG_EVENT_CONNECT(coreOutput, coreInput);
		}
	}
}

void CorePin::onUnplug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool wasStartPin, bool logEvent)
{
	Pin::onUnplug(otherPin, link, wasStartPin, logEvent);
	if (isInput())
	{
		const Core::Pin* corePin = &getCorePin();
		assert(getNode<CoreNode>()->getNodebase() == corePin->getOwner());
		const auto* inputPin = corePin;
		const auto* outputPin = inputPin->getParentPin();
		Core::GraphManager::unplugInput(corePin->getOwner(), getCoreIndex());
		if (logEvent)
		{
			LOG_EVENT_DISCONNECT(outputPin, inputPin);
			diwne.m_takeSnap = true;
		}
	}
}

bool CorePin::allowConnection() const
{
	return m_pinRect.Contains(diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos()));
}

const Core::Pin& CorePin::getCorePin() const
{
	return m_pin;
}

int CorePin::getCoreIndex() const
{
	return m_pin.Index;
}

Core::EValueType CorePin::getType() const
{
	return m_pin.ValueType;
}

void CorePin::drawData(DIWNE::DrawInfo& context)
{
	switch (m_pin.ValueType)
	{
	case Core::EValueType::Matrix:
	case Core::EValueType::Vec4:
	case Core::EValueType::Vec3:
	case Core::EValueType::Float:
	case Core::EValueType::Quat:
		drawBasicPinData(context);
		break;
	case Core::EValueType::Pulse:
		drawPulsePinData(context);
		break;
	case Core::EValueType::Screen:
		drawPulsePinData(context);
		break;
	case Core::EValueType::MatrixMul: // We never show data for matrix mul type
	case Core::EValueType::Ptr:       // Pin with type Ptr have no graphical representation
		break;
	default:
		I3T_ABORT("Unknown Pin type while loading output pins from "
		          "Core to Workspace");
	}
}

void CorePin::drawBasicPinData(DIWNE::DrawInfo& context)
{
	bool valueChanged = false;
	bool interaction_happen = false;
	CoreNode& node = *static_cast<CoreNode*>(getNode());

	Core::EValueState valState = node.getNodebase()->getState(getCoreIndex());

	switch (m_pin.ValueType)
	{
	case Core::EValueType::Matrix:
	{
		int rowOfChange, columnOfChange;
		float valueOfChange;

		interaction_happen =
		    DataRenderer::drawData4x4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
		                              node.getFloatPopupMode(), getCorePin().data().getMat4(),
		                              {valState, valState, valState, valState, valState, valState, valState, valState,
		                               valState, valState, valState, valState, valState, valState, valState, valState},
		                              valueChanged, rowOfChange, columnOfChange, valueOfChange);
		if (valueChanged) // TODO: JM MH set values to given (this) pin*/
			node.getNodebase()->setValue(valueOfChange, {columnOfChange, rowOfChange});
	}
	break;
	case Core::EValueType::Vec4:
	{
		glm::vec4 valueOfChange;
		interaction_happen = DataRenderer::drawDataVec4(
		    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
		    getCorePin().data().getVec4(), {valState, valState, valState, valState}, valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Vec3:
	{
		glm::vec3 valueOfChange;
		interaction_happen = DataRenderer::drawDataVec3(
		    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
		    getCorePin().data().getVec3(), {valState, valState, valState}, valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Float:
	{
		float valueOfChange;
		interaction_happen = DataRenderer::drawDataFloat(
		    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
		    getCorePin().data().getFloat(), node.getNodebase()->getState(getCoreIndex()), valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Quat:
	{
		glm::quat valueOfChange;
		interaction_happen = DataRenderer::drawDataQuaternion(
		    diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(), node.getFloatPopupMode(),
		    getCorePin().data().getQuat(), {valState, valState, valState, valState}, valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	default:
		I3T_ABORT("Unknown standard data pin!");
		break;
	}
	ImGui::SameLine();
	if (valueChanged)
	{
		node.updateDataItemsWidth();
	}
	if (interaction_happen)
		context.consumeInput();
}

void CorePin::drawPulsePinData(DIWNE::DrawInfo& context)
{
	// (PF) Pulse box: The Pulse button appears only if no input is connected.
	CoreNode* node = static_cast<CoreNode*>(getNode());
	const Core::EValueState& state = node->getNodebase()->getState(getCoreIndex());
	if (state == Core::EValueState::Editable)
	{
		if (ImGui::SmallButton(fmt::format("{}##n{}:p{}", "Pulse", node->getId(), m_idDiwne).c_str()))
		{
			node->getNodebase()->pulse(getCoreIndex());
			context.consumeInput();
		}
		if (ImGui::IsItemActive())
			context.consumeInput();
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
	ImGui::SameLine();
}

int CorePin::maxLengthOfData()
{
	switch (m_pin.ValueType)
	{
	case Core::EValueType::Matrix:
		return DataRenderer::maxLengthOfData4x4(getCorePin().data().getMat4(),
		                                        static_cast<CoreNode*>(getNode())->getNumberOfVisibleDecimal());
	case Core::EValueType::Vec4:
		return DataRenderer::maxLengthOfDataVec4(getCorePin().data().getVec4(),
		                                         static_cast<CoreNode*>(getNode())->getNumberOfVisibleDecimal());
	case Core::EValueType::Vec3:
		return DataRenderer::maxLengthOfDataVec3(getCorePin().data().getVec3(),
		                                         static_cast<CoreNode*>(getNode())->getNumberOfVisibleDecimal());
	case Core::EValueType::Float:
		return DataRenderer::maxLengthOfDataFloat(getCorePin().data().getFloat(),
		                                          static_cast<CoreNode*>(getNode())->getNumberOfVisibleDecimal());
	case Core::EValueType::Quat:
		return DataRenderer::maxLengthOfDataQuaternion(getCorePin().data().getQuat(),
		                                               static_cast<CoreNode*>(getNode())->getNumberOfVisibleDecimal());
	case Core::EValueType::Pulse:
	case Core::EValueType::MatrixMul:
	case Core::EValueType::Screen:
	case Core::EValueType::Ptr: /* Pin with type Ptr have no graphic representation */
		return 0;
	default:
		I3T_ABORT("Unknown Pin type while loading output pins from "
		          "Core to Workspace");
		return 0;
	}
}

/* \todo JH \todo MH implement this function in Core? */
bool CorePin::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

std::shared_ptr<DIWNE::Link> CorePin::createLink()
{
	return diwne.createLink<CoreLink>();
}

void CorePin::onReleased(bool justReleased, DIWNE::DrawInfo& context)
{
	// TODO: Hookup I3T input bindings
	// Create a new appropriate node for this pin when it is Ctrl clicked.
	if (justReleased && !context.inputConsumed && !context.state.dragging && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (isInput() && !isPlugged())
		{
			createNodeFromPin();
			context.consumeInput();
		}
	}
	DiwneObject::onReleased(justReleased, context);
}


void CorePin::createNodeFromPin()
{
	switch (getType())
	{
	case Core::EValueType::Pulse:
		createNodeFromPinImpl<Operator<Core::EOperatorType::PulseToPulse>>();
		break;
	case Core::EValueType::Float:
		createNodeFromPinImpl<Operator<Core::EOperatorType::FloatToFloat>>();
		break;
	case Core::EValueType::Vec3:
		createNodeFromPinImpl<Operator<Core::EOperatorType::Vector3ToVector3>>();
		break;
	case Core::EValueType::Vec4:
		createNodeFromPinImpl<Operator<Core::EOperatorType::Vector4ToVector4>>();
		break;
	case Core::EValueType::Matrix:
		createNodeFromPinImpl<Operator<Core::EOperatorType::MatrixToMatrix>>();
		break;
	case Core::EValueType::Quat:
		createNodeFromPinImpl<Operator<Core::EOperatorType::QuatToQuat>>();
		break;
	case Core::EValueType::MatrixMul:
		createNodeFromPinImpl<Sequence>();
		break;
	case Core::EValueType::Screen:
		createNodeFromPinImpl<Camera>();
		break;
	}
}

template <typename T>
void CorePin::createNodeFromPinImpl()
{
	std::shared_ptr<T> node;
	node = diwne.createNode<T>(this->getConnectionPoint(), true);
	this->plug(node->getOutputs().at(0).get());
}
