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
#include "GUI/Workspace/WorkspaceModule.h"

using namespace Workspace;
using DIWNE::Style;

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

CorePin::CorePin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode* node, bool isInput, const std::string& label)
    : DIWNE::Pin(diwne, node, isInput, label), m_pin(pin)
{}

/**
 * \brief Draw the pin icon
 * \return false - no interaction allowed
 */
void CorePin::content(DIWNE::DrawInfo& context)
{
	ImGuiStyle& imStyle = ImGui::GetStyle();

	using namespace DIWNE;
	ImVec2 pinSpacing = style().size(Style::PIN_SPACING) * diwne.getZoom();
	float labelSpacing = style().decimal(Style::PIN_LABEL_SPACING) * diwne.getZoom();

	// Handle pin dimming
	const bool dimmingEnabled = style().boolean(Style::PIN_DIM_ENABLED);
	float alphaBackup = 1.0f;
	float pinAlpha = 1.0f;
	if (dimmingEnabled)
	{
		alphaBackup = imStyle.Alpha;
		auto action = context.state.getActiveAction<Actions::ConnectPinAction>();
		if (action != nullptr && !m_isDragged && !canPlug(action->draggedLink->getAnyPin()))
		{
			pinAlpha = style().decimal(Style::PIN_DIM_ALPHA);
			imStyle.Alpha = pinAlpha;
		}
	}

	// Draw content
	ImRect dragRect;
	if (isLeft())
	{
		DGui::RectData rectData = DGui::BeginRect();
		m_pinIconData = drawPin(true, pinAlpha);
		ImGui::SameLine(0, labelSpacing);

		if (drawLabel(context))
			ImGui::SameLine(0, pinSpacing.x);

		dragRect = DGui::EndRect(rectData);
		drawDataEx(context);
	}
	else
	{
		if (drawDataEx(context))
			ImGui::SameLine(0, pinSpacing.x);

		DGui::RectData rectData = DGui::BeginRect();
		if (drawLabel(context))
			ImGui::SameLine(0, labelSpacing);

		m_pinIconData = drawPin(false, pinAlpha);
		dragRect = DGui::EndRect(rectData);
	}
	if (style().boolean(Style::PIN_ENABLE_DRAG_LABEL))
	{
		if (m_pinIconData.protrusion > 0) // Adjust drag rect if the pin is stickin out of the node
			dragRect.Max.x += m_pinIconData.protrusion;
		else if (m_pinIconData.protrusion < 0)
			dragRect.Min.x += m_pinIconData.protrusion;
		m_dragRect = diwne.canvas().screen2diwne(dragRect);
	}

	// End dimming
	if (dimmingEnabled)
		imStyle.Alpha = alphaBackup;

	DIWNE_DEBUG_LAYOUT(diwne, {
		diwne.canvas().AddRectDiwne(m_dragRect.Min, m_dragRect.Max, ImColor(225, 255, 0, 150), 0.0f, 0, 3.0f, true);
	});
}

bool CorePin::allowDrawing()
{
	return Pin::allowDrawing() && getCorePin().isRendered();
}

DIWNE::PinIconDrawData CorePin::drawPin(bool left, float alpha)
{
	// Pin shall be at least ImGui::GetFrameHeight() high, see drawLabel()
	ImVec2 iconSize = getPinSize();

	ImVec2 nodeFramePadding = I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.canvas().getZoom();
	float frameHeight = DIWNE::DGui::GetFrameHeight(nodeFramePadding);
	bool alignPin = false;
	float verticalMargin = 0.0f;
	if (iconSize.y < frameHeight)
	{
		alignPin = true;
		verticalMargin = (frameHeight - iconSize.y) / 2.0f;
		DIWNE::DGui::BeginVerticalAlign(verticalMargin);
	}

	float pinOffset = 0.f;
	if (m_pinStyle == PinStyle::Socket || m_pinStyle == PinStyle::SocketSquare)
		pinOffset = style().decimal(Style::PIN_SOCKET_OFFSET) * diwne.getZoom();
	else if (m_pinStyle == PinStyle::Square)
		pinOffset = style().decimal(Style::PIN_SQUARE_OFFSET) * diwne.getZoom();
	else
		pinOffset = style().decimal(Style::PIN_CIRCLE_OFFSET) * diwne.getZoom();

	// Add the pin item
	DIWNE::PinIconDrawData data = addPinItem(iconSize, left, pinOffset);
	data.alpha = alpha;

	// Render the pin icon if it doesn't stick out of the node
	if (pinOffset >= 0.f)
		renderPinDiwne(data);

	if (alignPin)
		DIWNE::DGui::EndVerticalAlign(verticalMargin);

	if (!style().boolean(Style::PIN_ENABLE_DRAG_LABEL))
		m_dragRect = m_pinRect;

	DIWNE_DEBUG_LAYOUT(diwne, {
		ImGui::GetForegroundDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
		                                        IM_COL32(0, 255, 0, 255));
	});
	return data;
}

DIWNE::PinIconDrawData CorePin::addPinItem(const ImVec2& size, bool left, float offset)
{
	DIWNE::PinIconDrawData data;
	if (left)
	{
		if (offset > 0)
		{
			ImGui::Dummy(ImVec2(offset, 0.f));
			ImGui::SameLine(0, 0);
		}
		else if (offset < 0)
		{
			ImVec2 cp = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(cp.x + offset, cp.y));
		}
	}
	// We're using a disabled invisible, so that when its pressed / dragged it does not set an ActiveID in ImGui.
	// Setting ActiveID is the same thing what a DragFloat does when it drags, it disables interaction with other items
	// until the drag/press operation stops. This is not desirable for a pin as we want other things to hover still.
	bool hovered, active;
	DIWNE::DGui::InvisibleButton("PinSocket", size, true, data.hovered, data.active);
	data.rect = ImGui::GetCurrentContext()->LastItemData.Rect;
	m_pinRect = diwne.canvas().screen2diwne(data.rect);

	if (!left)
	{
		if (offset > 0)
		{
			ImGui::SameLine(0, 0);
			ImGui::Dummy(ImVec2(offset, 0.f));
		}
		else if (offset < 0)
		{
			DIWNE::DGui::LimitLine(ImGui::GetItemRectMax().x + offset);
		}
	}
	data.protrusion = offset >= 0.f ? 0.f : left ? offset : -offset;
	return data;
}

void CorePin::renderPinDiwne(DIWNE::PinIconDrawData& data) const
{
	if (m_pinStyle == PinStyle::Socket || m_pinStyle == PinStyle::SocketSquare)
	{
		renderSocketPin(data.rect.Min, data.rect.GetSize(), data.alpha,
		                m_pinStyle == PinStyle::SocketSquare
		                    ? style().decimal(Style::PIN_SOCKET_ROUNDING) * diwne.getZoom()
		                    : -1.0f);
		data.rendered = true;
	}
	else if (m_pinStyle == PinStyle::Square)
	{
		renderSquarePin(data.rect.Min, data.rect.GetSize(), data.alpha);
		data.rendered = true;
	}
	else
	{
		renderCirclePin(data.rect.Min, data.rect.GetSize(), data.alpha);
		data.rendered = true;
	}
}

void CorePin::renderSquarePin(const ImVec2& pos, const ImVec2& size, float alpha) const
{
	// TODO: (DR) Don't really see why the "filled" parameters depends on isConnected(), currently the outlines are
	//   not visible anyway so we're just drawing stuff twice for no reason
	// todo (PF) - I have temporally added the pi n border drawing of not-connected pins
	// connected pins have no border now
	bool filled = isConnected();
	filled = true;

	Core::EValueType pinType = getType();
	const DIWNE::IconType iconTypeBg = PinShapeBackground[pinType];
	const ImColor iconColorBg = I3T::getColor(PinColorBackground[pinType]) * ImVec4(1.f, 1.f, 1.f, alpha);

	DIWNE::IconType iconTypeFg;
	if (m_iconType != DIWNE::IconType::NoIcon)
		iconTypeFg = m_iconType;
	else
		iconTypeFg = PinShapeForeground[pinType];

	// Adjust icon if the pin is on a different side than usually expected (inputs on the left)
	if (iconTypeFg == DIWNE::IconType::TriangleRight && this->isInput() != this->isLeft())
		iconTypeFg = DIWNE::IconType::TriangleLeft;

	const ImColor iconColorFg = I3T::getColor(PinColorForeground[pinType]) * ImVec4(1.f, 1.f, 1.f, alpha);

	// space between icon symbol and icon boundary
	const float padding = I3T::getSize(ESize::Pins_IconPadding) * diwne.getZoom();
	const float rounding = style().decimal(Style::PIN_SQUARE_ROUNDING) * diwne.getZoom();

	ImVec2 origPos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(pos);
	diwne.canvas().DrawIcon(iconTypeBg, iconColorBg, iconColorBg, iconTypeFg, iconColorFg, iconColorFg, size,
	                        ImVec4(padding, padding, padding, padding), filled, {1, 1}, rounding);

	ImDrawList* idl = ImGui::GetWindowDrawList();
	// Optionally draw pin icon border
	float borderWidth = style().decimal(Style::PIN_SQUARE_BORDER_WIDTH) * diwne.getZoom();
	if (borderWidth > 0.0f)
	{
		DIWNE::DDraw::AddInnerRoundedRectScreen(pos, pos + size, style().color(Style::PIN_SQUARE_BORDER_COLOR),
		                                        rounding, ImDrawFlags_RoundCornersAll, borderWidth, borderWidth * 0.8f);
	}
	// Disabled overlay
	if (isDisabled())
	{
		idl->AddRectFilled(pos, pos + size,
		                   ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::DisabledPinColor)), rounding);
	}
	ImGui::SetCursorScreenPos(origPos); // Restore cursor
}
void CorePin::renderSocketPin(const ImVec2& pos, const ImVec2& size, float alpha, float rounding) const
{
	Core::EValueType pinType = getType();
	const ImColor baseColor = I3T::getColor(PinColorBackground[pinType]);

	ImVec4 color = baseColor;
	ImVec4 hoverColor = baseColor;
	ImVec4 bgColor = style().color(Style::PIN_SOCKET_BG);
	if (m_previewPlugged)
		hoverColor = hoverColor + style().color(Style::PIN_HOVER_COLOR_SHIFT);

	color.w *= alpha;
	hoverColor.w *= alpha;
	bgColor.w *= alpha;

	ImDrawList* idl = ImGui::GetWindowDrawList();

	float socketThickness = style().decimal(Style::PIN_SOCKET_THICKNESS) * diwne.getZoom();
	float connectGap = style().decimal(Style::PIN_SOCKET_CONNECTED_GAP) * diwne.getZoom();

	if (rounding == -1.0f)
	{
		ImVec2 center = pos + size / 2.f;
		float outerRadius = (size.x - socketThickness) / 2.f;
		float innerRadius = size.x / 2.f - socketThickness;

		idl->AddCircleFilled(center, outerRadius, ImGui::ColorConvertFloat4ToU32(bgColor), 0);
		idl->AddCircle(center, outerRadius, ImGui::ColorConvertFloat4ToU32(hoverColor), 0, socketThickness);

		// Optionally draw pin socket border
		float borderWidth = style().decimal(Style::PIN_SOCKET_BORDER_WIDTH) * diwne.getZoom();
		if (borderWidth > 0.0f)
		{
			idl->AddCircle(center, outerRadius + socketThickness / 2.f - borderWidth / 2.f,
			               ImGui::ColorConvertFloat4ToU32(style().color(Style::PIN_SOCKET_BORDER_COLOR)), 0,
			               borderWidth);
		}
		if (isConnected() || m_previewPlugged)
			idl->AddCircleFilled(center, innerRadius - connectGap, ImGui::ColorConvertFloat4ToU32(color), 0);

		if (isDisabled()) // Disabled overlay
		{
			idl->AddCircleFilled(center, outerRadius + socketThickness / 2.f,
			                     ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::DisabledPinColor)), 0);
		}
	}
	else
	{
		assert(rounding >= 0.0f);
		ImVec2 min = pos;
		ImVec2 max = pos + size;

		ImVec2 minO = pos + ImVec2(socketThickness + DIWNE_PIXEL_EPSILON, socketThickness + DIWNE_PIXEL_EPSILON) / 2.f;
		ImVec2 maxO =
		    pos + size - ImVec2(socketThickness - DIWNE_PIXEL_EPSILON, socketThickness - DIWNE_PIXEL_EPSILON) / 2.f;

		idl->AddRectFilled(minO, maxO, ImGui::ColorConvertFloat4ToU32(bgColor), rounding, ImDrawFlags_RoundCornersAll);
		idl->AddRect(minO, maxO, ImGui::ColorConvertFloat4ToU32(hoverColor), rounding, ImDrawFlags_RoundCornersAll,
		             socketThickness);

		// Optionally draw pin socket border
		float borderWidth = style().decimal(Style::PIN_SOCKET_BORDER_WIDTH) * diwne.getZoom();
		if (borderWidth > 0.0f)
		{
			idl->AddRect(minO, maxO, ImGui::ColorConvertFloat4ToU32(style().color(Style::PIN_SOCKET_BORDER_COLOR)),
			             rounding, ImDrawFlags_RoundCornersAll, borderWidth);
		}
		if (isConnected() || m_previewPlugged)
		{
			DIWNE::DDraw::AddRectFilledOffsetScreen(min, max, color, style().decimal(Style::PIN_SOCKET_INNER_ROUNDING),
			                                        ImDrawFlags_RoundCornersAll, socketThickness + connectGap, false);
		}
		if (isDisabled()) // Disabled overlay
		{
			idl->AddRectFilled(min, max, ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::DisabledPinColor)),
			                   rounding, ImDrawFlags_RoundCornersAll);
		}
	}
}
void CorePin::renderCirclePin(const ImVec2& pos, const ImVec2& size, float alpha) const
{
	Core::EValueType pinType = getType();
	const ImColor baseColor = I3T::getColor(PinColorBackground[pinType]);

	ImVec4 color = baseColor;
	ImVec4 hoverColor = baseColor;
	ImVec4 borderColor = style().color(Style::PIN_CIRCLE_BORDER_COLOR);
	if (m_previewPlugged)
		hoverColor = hoverColor + style().color(Style::PIN_HOVER_COLOR_SHIFT);

	color.w *= alpha;
	hoverColor.w *= alpha;
	borderColor.w *= alpha;

	ImDrawList* idl = ImGui::GetWindowDrawList();

	float borderWidth = style().decimal(Style::PIN_CIRCLE_BORDER_WIDTH) * diwne.getZoom();

	ImVec2 center = pos + size / 2.f;
	float outerRadius = size.x / 2.f;
	float innerRadius = outerRadius - borderWidth;

	idl->AddCircleFilled(center, outerRadius, ImGui::ColorConvertFloat4ToU32(borderColor), 0);
	idl->AddCircleFilled(center, innerRadius, ImGui::ColorConvertFloat4ToU32(hoverColor), 0);

	if (isDisabled()) // Disabled overlay
	{
		idl->AddCircleFilled(center, outerRadius,
		                     ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::DisabledPinColor)), 0);
	}
}

bool CorePin::drawLabel(DIWNE::DrawInfo& context)
{
	const std::string& label = m_pin.getLabel();
	if (label.empty())
		return false;
	// if (m_pin.ValueType == Core::EValueType::Pulse) // no labels for pulse and cycle
	// todo (PF) Label and value order should be switched (used by cycle, mat->TR, x->floats, pulse)

	// Each label + pin combo should be at least Nodes_FloatPadding (eg. FramePadding for nodes) high,
	// that makes vertical centering easy.
	// Height of the label should be the same as the pin, eg. we want to center the label on the pin
	// When the pin is small then we do the opposite and we center the pin on the frame height
	bool beganGroup = false;
	ImVec2 pinSize = getPinSize();
	ImVec2 nodeFramePadding = I3T::getSize(ESizeVec2::Nodes_FloatPadding) * diwne.canvas().getZoom();
	if (pinSize.y <= DIWNE::DGui::GetFrameHeight(nodeFramePadding))
	{
		DIWNE::DGui::AlignTextToPadding(nodeFramePadding);
	}
	else
	{
		ImGui::BeginGroup();
		beganGroup = true;
		float verticalMargin = (pinSize.y - ImGui::GetTextLineHeight()) / 2.0f;
		DIWNE::DGui::DummyXY(ImVec2(0.0f, verticalMargin));
	}
	ImGui::TextUnformatted(label.c_str());
	if (beganGroup)
		ImGui::EndGroup();
	return true;
}

bool CorePin::drawDataEx(DIWNE::DrawInfo& context)
{
	if (m_isInput || !m_showData)
		return false;
	return drawData(context);
}

void CorePin::popupContent(DIWNE::DrawInfo& context) {}

bool CorePin::preparePlug(Pin* otherPin, DIWNE::Link* link, bool hovering)
{
	assert(this != otherPin && "Pin cannot be plugged into itself");

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
		// diwne.showTooltipLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
		if (!hovering)
		{
			input->plugLink(output, link, true);
		}
		return true;
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
		diwne.showTooltipLabel("Pin is disabled", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
		break;
	default: // unreachable - all enum values are covered
		diwne.showTooltipLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
	}
	return false;
}

bool CorePin::canPlug(Pin* other) const
{
	if (this == other)
		return false;
	if (!other)
		return false;
	if (isInput() == other->isInput())
		return false;
	CorePin* corePin = static_cast<CorePin*>(other);
	if (getCorePin().ValueType != corePin->getCorePin().ValueType)
		return false;
	return true;
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
		}
		diwne.m_takeSnap = true;
	}
}

bool CorePin::allowConnection() const
{
	if (!Pin::allowConnection())
		return false;
	return isDragAreaHovered();
}

bool CorePin::isDisabled() const
{
	return m_pin.isDisabled();
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

bool CorePin::drawData(DIWNE::DrawInfo& context)
{
	switch (m_pin.ValueType)
	{
	case Core::EValueType::Matrix:
	case Core::EValueType::Vec4:
	case Core::EValueType::Vec3:
	case Core::EValueType::Float:
	case Core::EValueType::Quat:
		return drawBasicPinData(context);
	case Core::EValueType::Pulse:
		return drawPulsePinData(context);
	case Core::EValueType::Screen:
		return drawPulsePinData(context);
	case Core::EValueType::MatrixMul: // We never show data for matrix mul type
	case Core::EValueType::Ptr:       // Pin with type Ptr have no graphical representation
		return false;
	default:
		I3T_ABORT("Unknown Pin type while loading output pins from "
		          "Core to Workspace");
		return false;
	}
}

bool CorePin::drawBasicPinData(DIWNE::DrawInfo& context)
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
		    DataRenderer::drawData4x4(diwne, context, node.getId(), node.m_labelDiwne, node.getNumberOfVisibleDecimal(),
		                              node.getDataItemsWidth(), node.getFloatPopupMode(), getCorePin().data().getMat4(),
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
		    diwne, context, node.getId(), node.m_labelDiwne, node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
		    node.getFloatPopupMode(), getCorePin().data().getVec4(), {valState, valState, valState, valState},
		    valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Vec3:
	{
		glm::vec3 valueOfChange;
		interaction_happen = DataRenderer::drawDataVec3(diwne, context, node.getId(), node.m_labelDiwne,
		                                                node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
		                                                node.getFloatPopupMode(), getCorePin().data().getVec3(),
		                                                {valState, valState, valState}, valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Float:
	{
		float valueOfChange;
		interaction_happen = DataRenderer::drawDataFloat(
		    diwne, context, node.getId(), node.m_labelDiwne, node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
		    node.getFloatPopupMode(), getCorePin().data().getFloat(), node.getNodebase()->getState(getCoreIndex()),
		    valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	case Core::EValueType::Quat:
	{
		glm::quat valueOfChange;
		interaction_happen = DataRenderer::drawDataQuaternion(
		    diwne, context, node.getId(), node.m_labelDiwne, node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(),
		    node.getFloatPopupMode(), getCorePin().data().getQuat(), {valState, valState, valState, valState},
		    valueChanged, valueOfChange);
		if (valueChanged)
			node.getNodebase()->setValue(valueOfChange);
	}
	break;
	default:
		I3T_ABORT("Unknown standard data pin!");
		break;
	}
	if (valueChanged)
	{
		node.updateDataItemsWidth();
	}
	if (interaction_happen)
		context.consumeInput();
	return true;
}

bool CorePin::drawPulsePinData(DIWNE::DrawInfo& context)
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
		return true;
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

		// ImGui::Dummy(ImVec2(0.0, 0.0)); // to avoid unlimited cycle width
		return false;
	}
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
	Super::onReleased(justReleased, context);
}

ImVec2 CorePin::getPinSize() const
{
	if (getType() == Core::EValueType::MatrixMul)
		return I3T::getSize(ESizeVec2::Nodes_PinSize_MatrixMul) * diwne.getZoom();
	return I3T::getSize(ESizeVec2::Nodes_PinSize) * diwne.getZoom();
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
