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
#include "DiwneObject.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/Style/StyleOverride.h"
#include "DIWNE/Core/diwne_actions.h"

#if DIWNE_DEBUG_ENABLED
#include "Link.h"
#include "Pin.h"
#include <bitset>
#endif

namespace DIWNE
{
unsigned long long DiwneObject::g_diwneIDCounter = 1; // 0 is reserved for no/invalid ID

DiwneObject::DiwneObject(DIWNE::NodeEditor& diwne, std::string labelDiwne)
    : diwne(diwne), m_idDiwne(g_diwneIDCounter++), m_labelDiwne(fmt::format("{}:{}", labelDiwne, m_idDiwne)),
      m_popupLabelDiwne(fmt::format("popup_{}", m_labelDiwne))
{}

DiwneObject::~DiwneObject()
{
	if (!m_destroy)
	{
		DIWNE_WARN("Improper DiwneObject destruction! Destroy method wasn't called on '" + m_labelDiwne + "'.");
	}
}

void DiwneObject::draw(DrawMode drawMode)
{
	// DiwneObjects should be drawn by an overarching NodeEditor or similar class
	// They are then drawn using the drawDiwne() method that passes along the drawing context created by the NodeEditor.
	// The code below draws them with a single use default constructed state and context.
	InteractionState state;
	DrawInfo context(state);
	drawDiwne(context, drawMode);
};

DrawInfo DiwneObject::drawDiwneEx(DrawInfo& context, DrawMode drawMode)
{
	ContextTracker tracker(context);
	drawDiwne(context, drawMode);
	return tracker.end(context);
}

void DiwneObject::drawDiwne(DrawInfo& context, DrawMode mode)
{
#if DIWNE_DEBUG_ENABLED
	auto debug_logicalUpdate = context.logicalUpdates;
#endif
	m_drawMode = mode;
	bool wasDrawnLastFrame = m_drawnThisFrame;
	m_drawnThisFrame = false;

	if (m_forceDraw)
		mode |= DrawMode_ForceDraw;

	initializeDiwne(context);
	if (allowDrawing() || mode & DrawMode_ForceDraw)
	{
		m_drawnThisFrame = true;
		beginDiwne(context);
		content(context);
		endDiwne(context);
		afterDrawDiwne(context);
#if DIWNE_DEBUG_ENABLED
		debugDrawing(context, debug_logicalUpdate);
#endif
	}
	m_justHidden = wasDrawnLastFrame && !m_drawnThisFrame;
	finalizeDiwne(context);

	m_forceDraw = false;
}

void DiwneObject::initialize(DrawInfo& context) {}
void DiwneObject::initializeDiwne(DrawInfo& context)
{
	return initialize(context);
}

void DiwneObject::beginDiwne(DrawInfo& context)
{
	setInitialPositionDiwne();

	// Update display rect
	ImVec2 truncPos = DIWNE_TRUNC_VEC(ImGui::GetCursorScreenPos());
	ImVec2 displayOffset = diwne.canvas().screen2diwne(truncPos) - m_rect.Min;
	m_displayRect = m_rect;
	m_displayRect.Translate(displayOffset);

	begin(context);
}

void DiwneObject::endDiwne(DrawInfo& context)
{
	end(context);
	updateLayout(context);
}

/**
 * Updates layout, processes interactions and then calls the user afterDraw() method.
 */
void DiwneObject::afterDrawDiwne(DrawInfo& context)
{
	processInteractionsDiwne(context);
	afterDraw(context);
}
void DiwneObject::afterDraw(DrawInfo& context){};

void DiwneObject::finalize(DrawInfo& context) {}
void DiwneObject::finalizeDiwne(DrawInfo& context)
{
	if (m_justHidden)
	{
		// TODO: [Medium priority] It might be better if dragging was processed outside the drawing code so that
		//  visibility of the object doesn't affect it.
		// When the object isn't drawn anymore its drag operation needs to stop
		stopDrag(context);
	}
	finalize(context);
}

bool DiwneObject::allowDrawing()
{
	return m_rendered;
}

bool DiwneObject::allowInteraction() const
{
	return true;
}

void DiwneObject::processInteractionsDiwne(DrawInfo& context)
{
	if ((m_drawMode & DrawMode_JustDraw) && !m_isDragged)
		return;

	if (!allowInteraction())
		return;

	processHoverDiwne(context);
	processPressAndReleaseDiwne(context);
	processDragDiwne(context);
	processSelectDiwne(context);
	processPopupAndTooltipDiwne(context);

	processInteractions(context); // Process other user interactions

	m_forceHoverDiwne = false;
}

void DiwneObject::processHoverDiwne(DrawInfo& context)
{
	bool hoveredTmp = (isHoveredDiwne() || m_forceHoverDiwne) && !context.hoverConsumed;
	bool hovered = hoveredTmp && (context.state.hoverTarget.empty() || context.state.hoverTarget == m_labelDiwne);
	m_hovered = hovered && allowHover();
	if (m_hovered)
		onHover(context);

	if (hovered || context.state.hoverTarget == m_labelDiwne)
	{
		// When we are being hovered but hover isn't allowed / is disabled, we still propagate the hover event,
		// propagation can still end if this is a hover root

		// Other special case when we still want to propagate hover is when one of our children is hovered despite
		// ourselves not being hovered, that can happen when a children rect reaches outside the parent rect.
		// In that case we are the hover target and will propagate hover too.

		// Handle hover propagation, hover is propagated from a child to its parent
		if (m_hoverRoot)
			context.hoverConsumed++;
		else
			context.state.hoverTarget = m_parentObject ? m_parentObject->m_labelDiwne : std::string();
	}
}

void DiwneObject::processPressAndReleaseDiwne(DrawInfo& context)
{
	bool wasPressed = m_isPressed;
	bool pressed = isPressedDiwne() && allowPress(context);
	m_justPressed = isJustPressedDiwne() && pressed;
	m_justReleased = wasPressed && !pressed;

	if (wasPressed)
	{
		m_isPressed = pressed;
	}
	else
	{
		m_isPressed = pressed && m_justPressed; // Require immediate key press to begin press cycle
	}

	if (m_isPressed)
	{
		onPressed(m_justPressed, context);
	}
	else
	{
		onReleased(m_justReleased, context);
	}
	// TODO: (DR) When drag ends we should take a snap, this is drags responsibility tho probably
	//  DR: But why though? Individual drag actions like drag/selrect/connectpin should take a snap, not dragging
}

void DiwneObject::processDragDiwne(DrawInfo& context)
{
	// TODO: [Low priority] Since we separated stopDrag into a method
	//  the logic below seems a little too nested and complicated, simplify
	bool isDragged = isDraggedDiwne();
	bool weAreDragSource = context.state.isDragSource(this);
	if (context.state.dragging) // Check if something is being dragged
	{
		if (weAreDragSource) // This object is being dragged
		{
			context.state.dragStart = false; // End drag start flag
			if (isDragged)
			{
				// Continue drag
				m_isDragged = true;
			}
			else
			{
				// Drag key no longer pressed, stop drag
				stopDrag(context); // Stop drag calls onDrag()
				return;
			}
		}
		else // Something else is being dragged
		{
			m_isDragged = false;
		}
	}
	else // Nothing is being dragged, try start drag
	{
		m_isDragged = isDragged && allowDragStart() && !context.state.dragEndedLastFrame;
		if (m_isDragged)
		{
			context.state.dragStart = true;
			context.state.dragging = true;
			context.state.dragSource = shared_from_this();
			context.state.dragSourceID = this->getId();
		}
	}
	if (m_isDragged && context.state.dragging) // Dispatch user method
	{
		onDrag(context, context.state.dragStart, false);
	}
}

bool DiwneObject::allowHover() const
{
	return m_hoverable;
}

bool DiwneObject::allowPress(const DrawInfo& context) const
{
	if (!m_hovered) // Can't be pressed when not being hovered
		return false;
	if (context.inputConsumed) // Can't be pressed when input is consumed
		return false;
	// Can't be pressed when some other object is being dragged
	if (context.state.dragging && !context.state.isDragSource(this))
		return false;
	return true;
}

bool DiwneObject::allowDragStart() const
{
	return m_isPressed && m_draggable;
}

void DiwneObject::destroy(bool logEvent)
{
	if (m_destroy)
		return;
	m_destroy = true;

	// Ensure the object stops its drag before being destroyed
	DrawInfo tmp = DrawInfo(diwne.interactionState); // Create an empty context for the stop drag call
	stopDrag(tmp);

	onDestroy(logEvent);
}

void DiwneObject::onDestroy(bool logEvent) {}

void DiwneObject::setPosition(const ImVec2& position)
{
	translate(position - m_rect.Min);
}

void DiwneObject::translate(const ImVec2& vec)
{
	m_rect.Translate(vec);
}

bool DiwneObject::isRendered() const
{
	return m_rendered;
}

void DiwneObject::setRendered(bool val)
{
	m_rendered = val;
}

bool DiwneObject::setSelected(bool selected)
{
	bool prevSelected = m_selected;
	m_selected = m_selectable ? selected : false;
	if (prevSelected != m_selected)
	{
		onSelection(m_selected);
		diwne.m_takeSnap = true;
	}
	return m_selected;
}

bool DiwneObject::getSelected() const
{
	return m_selected;
};
void DiwneObject::setSelectable(bool selectable)
{
	m_selectable = selectable;
};
bool DiwneObject::getSelectable()
{
	return m_selectable;
}

Actions::DiwneAction* InteractionState::startAction(std::unique_ptr<Actions::DiwneAction> a)
{
	if (!this->action)
	{
		this->action = std::move(a);
		return this->action.get();
	}
	DIWNE_WARN("Trying to start a new action '" + a->name + "' while an action '" + action->name +
	           "' is already active!");
	return nullptr;
}

void InteractionState::endAction(bool immediately)
{
	if (immediately)
	{
		action->onEnd();
		action = nullptr;
	}
	else
	{
		action->end();
	}
}

bool InteractionState::isActionActive(const std::string& name)
{
	return action != nullptr && action->name == name;
}

bool InteractionState::isActionActive(const std::string& name, const DiwneObject* source)
{
	return isActionActive(name) && action->isSource(source);
}

bool InteractionState::anyActionActive()
{
	return action != nullptr;
}

void InteractionState::nextFrame()
{
	if (action != nullptr)
	{
		// End the action prematurely if it's source was destroyed
		if (action->source.expired() || action->source.lock()->isDestroyed())
		{
			endAction(false);
			action->endActionNextFrame = 2; // End it immediately
		}

		if (action->endActionNextFrame == 0)
			action->onFrameEnd();

		// When action ends we want to keep it active for the remainder of the frame as well as the entirety of the
		// next frame. So any objects, even those rendered before the action was ended, can react to it ending.
		// Hence the action reset is deferred to the end of the next frame here.
		if (action->endActionNextFrame == 1)
		{
			action->endActionNextFrame++;
		}
		else if (action->endActionNextFrame >= 2)
		{
			endAction(true);
		}
	}

	// End dragging prematurely if it's source was destroyed
	if (dragging && (dragSource.expired() || dragSource.lock()->isDestroyed()))
		dragEnd = true;

	// Set dragging to false on dragEnd and enable dragEndedLastFrame for a frame.
	// In the frame when dragging ends, dragging and dragEnd are both true.
	// dragEnd resets next frame (we're not infering it over)
	// But dragging needs to be set to false here at the end of frame.
	// Drag source is retained for one more frame.
	if (dragEndedLastFrame)
	{
		dragEndedLastFrame = false;
		dragSource.reset();
		dragSourceID = 0;
	}
	if (dragEnd)
	{
		dragging = false;
		dragEndedLastFrame = true;
	}

	// Reset non-persistent variables
	dragEnd = false;
	dragStart = false;
	hoverTarget.clear();
}

DrawInfo::DrawInfo(InteractionState& state) : state(state) {}

DrawInfo DrawInfo::findChange(const DrawInfo& other) const
{
	DrawInfo change(other.state);
	change.visualUpdates = this->visualUpdates - other.visualUpdates;
	change.logicalUpdates = this->logicalUpdates - other.logicalUpdates;
	change.inputConsumed = this->inputConsumed - other.inputConsumed;
	change.hoverConsumed = this->hoverConsumed - other.hoverConsumed;
	return change;
}

bool DrawInfo::inputAvailable()
{
	return !inputConsumed;
}

bool DrawInfo::inputFullyAvailable()
{
	return inputAvailable() && !state.dragging && !state.anyActionActive();
}

void DrawInfo::visualUpdate()
{
	visualUpdates++;
}

void DrawInfo::logicalUpdate(bool isVisualUpdateAsWell)
{
	if (isVisualUpdateAsWell)
		visualUpdate();
	logicalUpdates++;
}

void DrawInfo::update(bool visual, bool logical, bool blockInput)
{
	if (visual)
		visualUpdate();
	if (logical)
		logicalUpdate(false);
	if (blockInput)
		consumeInput();
}

ContextTracker::ContextTracker(const DrawInfo& context)
{
	begin(context);
}

void ContextTracker::begin(const DrawInfo& context)
{
	m_contextCopy = std::make_shared<DrawInfo>(context);
}

DrawInfo ContextTracker::end(const DrawInfo& context)
{
	return context.findChange(*m_contextCopy);
}

//

/**
 * This implementation assumes that the last ImGui item represents the entirety of the object.
 * This method is indirectly called from processInteractionsDiwne() which is called right after end() method.
 *
 * If the DiwneObject isn't represented by an ImGui item, this method has to be overridden and modified accordingly.
 * This default implementation uses ImGui::IsItemHovered() call as it handles all various cases of other UI elements or
 * windows overlapping this object.
 */
bool DiwneObject::isHoveredDiwne()
{
	return m_internalHover;
}
bool DiwneObject::isPressedDiwne()
{
	return diwne.input().bypassIsMouseDown0();
}
bool DiwneObject::isJustPressedDiwne()
{
	// Note: ImGui "click" is the same as just a press. See https://github.com/ocornut/imgui/issues/2385.
	return diwne.input().bypassIsMouseClicked0();
}
bool DiwneObject::isDraggedDiwne()
{
	return diwne.input().bypassIsMouseDragging0();
}
void DiwneObject::popupContent(DrawInfo& context)
{
	ImGui::MenuItem("popupContent() is empty");
}

void DiwneObject::tooltipContent(DrawInfo& context)
{
	ImGui::TextUnformatted(!m_tooltipText.empty() ? m_tooltipText.c_str() : "No tooltip set");
}

void DiwneObject::onHover(DrawInfo& context)
{
	context.update(true);

	DIWNE_DEBUG_GENERAL(diwne, {
		diwne.canvas().AddRectDiwne(getRect().Min + ImVec2(1, 1), getRect().Max - ImVec2(1, 1), DIWNE_RED_50, 0,
		                            ImDrawFlags_RoundCornersNone, 1);
	});
}

void DiwneObject::onPressed(bool justPressed, DrawInfo& context)
{
	if (justPressed)
		context.logicalUpdate(false);
	DIWNE_DEBUG_INTERACTIONS(diwne, {
		diwne.canvas().AddRectDiwne(getRect().Min + ImVec2(2, 2), getRect().Max - ImVec2(2, 2), DIWNE_YELLOW_50, 0,
		                            ImDrawFlags_RoundCornersNone, 1);
		if (justPressed)
		{
			diwne.canvas().AddRectFilledDiwne(getRect().Min + ImVec2(2, 2), getRect().Max - ImVec2(2, 2),
			                                  DIWNE_YELLOW_50, 0, ImDrawFlags_RoundCornersNone);
		}
	});
}
void DiwneObject::onReleased(bool justReleased, DrawInfo& context)
{
	// Clicking on a node is considered a logical update
	if (justReleased)
		context.logicalUpdate(false);
	DIWNE_DEBUG_INTERACTIONS(diwne, {
		if (justReleased)
		{
			diwne.canvas().AddRectFilledDiwne(getRect().Min + ImVec2(2, 2), getRect().Max - ImVec2(2, 2),
			                                  DIWNE_ORANGE_50, 0, ImDrawFlags_RoundCornersNone);
		}
	});
}
void DiwneObject::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	context.update(true, true, false);

	DIWNE_DEBUG_INTERACTIONS(diwne, {
		if (dragStart)
		{
			diwne.canvas().AddRectFilledDiwne(getRect().Min + ImVec2(1, 1), getRect().Max - ImVec2(1, 1),
			                                  DIWNE_GREEN_50, 0, ImDrawFlags_RoundCornersNone);
		}
		if (dragEnd)
		{
			diwne.canvas().AddRectFilledDiwne(getRect().Min + ImVec2(1, 1), getRect().Max - ImVec2(1, 1), DIWNE_RED_50,
			                                  0, ImDrawFlags_RoundCornersNone);
		}
		diwne.canvas().AddRectDiwne(getRect().Min + ImVec2(1, 1), getRect().Max - ImVec2(1, 1), DIWNE_CYAN_50, 0,
		                            ImDrawFlags_RoundCornersNone, 2);
	});
}

void DiwneObject::onSelection(bool selected) {}

bool DiwneObject::allowSelectOnClick(const DrawInfo& context) const
{
	return !context.state.anyActionActive();
}

bool DiwneObject::processSelectDiwne(DrawInfo& context)
{
	if (!m_selectable)
		return true;

	if (m_justReleased && !context.inputConsumed && allowSelectOnClick(context))
	{
		bool wasSelected = m_selected;
		bool multiSelect = diwne.input().multiSelectionActive();
		if (!multiSelect)
			diwne.deselectAllNodes();
		setSelected(!wasSelected);
		context.consumeInput(); // Only one item is selected per click
	}
	return false;
}

bool DiwneObject::allowPopup() const
{
	return true;
}
void DiwneObject::setPopupEnabled(bool val)
{
	m_popupEnabled = val;
}
void DiwneObject::processPopupAndTooltipDiwne(DrawInfo& context)
{
	// Ignore tooltips when input is consumed, or we're dragging or an action is active
	if (!context.inputFullyAvailable())
		return;

	// TODO: Make bypassIsMouseReleased1 a triggerPopup() or something method
	if (m_popupEnabled && (m_openPopup || (m_hovered && diwne.input().bypassIsMouseReleased1())))
	{
		if (!context.popupOpened && allowPopup())
		{
			ImGui::OpenPopup(m_popupLabelDiwne.c_str());
			context.update(true, true, true);
			context.popup();
			// Store popup position for stuff like adding nodes at the popup location
			diwne.setPopupPosition(diwne.input().bypassGetMousePos());
			onPopup();
		}
	}
	m_openPopup = false;

	if (ImGui::IsPopupOpen(m_popupLabelDiwne.c_str()))
	{
		// TODO: Stopping and starting zoom scaling DOES NOT create a backup of the current style
		//  Meaning any pushed style variables will get reset when zoom scaling is applied again
		const bool zoomScalingWasActive = diwne.canvas().ensureZoomScaling(false);
		if (ImGui::BeginPopup(m_popupLabelDiwne.c_str()))
		{
			popupContent(context);
			ImGui::EndPopup();
		}
		diwne.canvas().ensureZoomScaling(zoomScalingWasActive);
	}

	if (m_tooltipEnabled && m_openTooltip)
	{
		context.tooltip();
		if (ImGui::BeginTooltip())
		{
			tooltipContent(context);
			ImGui::EndTooltip();
		}
	}
	m_openTooltip = false;
}

void DiwneObject::openPopup()
{
	m_openPopup = true;
}

void DiwneObject::showTooltip(const std::string& label, const ImColor&& color, DIWNE::DrawInfo& context)
{
	if (!context.tooltipOpened)
	{
		context.tooltip();
		if (ImGui::BeginTooltip())
		{
			ImGui::TextColored(color, label.c_str());
			ImGui::EndTooltip();
		}
	}
}
void DiwneObject::showTooltip(const std::string& label, const std::string& desc, const ImColor&& color,
                              DrawInfo& context, float wrapWidth)
{
	if (!context.tooltipOpened)
	{
		context.tooltip();
		DGui::Tooltip(label.c_str(), desc.c_str(), color, wrapWidth);
	}
}
void DiwneObject::setTooltipEnabled(bool val)
{
	m_tooltipEnabled = val;
}
void DiwneObject::setTooltip(const std::string& text)
{
	m_tooltipText = text;
	if (!m_tooltipText.empty())
		setTooltipEnabled(true);
}

void DiwneObject::setStyleOverride(StyleOverride* styleOverride)
{
	m_styleOverride = styleOverride;
}
StyleOverride* DiwneObject::getStyleOverride() const
{
	return m_styleOverride;
}

Style& DiwneObject::style() const
{
	if (m_styleOverride != nullptr)
		return *m_styleOverride;
	return diwne.style();
}

void DiwneObject::setSize(const ImVec2& size)
{
	m_rect.Max = m_rect.Min + size;
	m_displayRect.Max = m_displayRect.Min + size;
}
void DiwneObject::updateRectFromImGuiItem()
{
	setSize(diwne.canvas().screen2diwneSize(ImGui::GetItemRectSize()));
}

void DiwneObject::setInitialPositionDiwne()
{
	// By default the rect's of DiwneObjects are fixed to the ImGui cursor position.
	// NOTE: This causes m_rects to shift a little based on the view/zoom, as the cursor position is sometimes rounded.
	// But nodes should always stay fixed at their position, their position is persistant and hence cannot be modified
	// by ImGui layout drift
	// Links have their position entirely determined by the connection points so this doesn't apply either.

	const ImVec2 offset = diwne.canvas().screen2diwne(ImGui::GetCursorScreenPos()) - m_rect.Min;
	translate(offset); // Move m_rect using the translate method so that derived classes can react accordingly
}

bool DiwneObject::isDragging(DrawInfo& context)
{
	return context.state.dragging && context.state.isDragSource(this);
}

void DiwneObject::stopDrag(DrawInfo& context)
{
	if (isDragging(context))
	{
		m_isDragged = false;
		// We only mark the drag end by setting context.dragEnd to true
		// But the dragging variables get reset at the end of the frame by the context.
		context.state.dragEnd = true;
		onDrag(context, false, true);
	}
}
bool DiwneObject::isFixed() const
{
	return m_fixed;
}
void DiwneObject::setFixed(bool val)
{
	m_fixed = val;
}
DiwneObject* DiwneObject::getParentObject() const
{
	return m_parentObject;
}
void DiwneObject::setParentObject(DiwneObject* parent)
{
	m_parentObject = parent;
}
bool DiwneObject::isChildObject() const
{
	return getParentObject() != nullptr && getParentObject() != &diwne;
}
void DiwneObject::debugDrawing(DrawInfo& context, int debug_logicalUpdate)
{
	DIWNE_DEBUG_GENERAL((diwne), {
		// TODO: Change to display rect?
		diwne.canvas().AddRectForegroundDiwne(getRect().Min, getRect().Max, DIWNE_YELLOW_50, 0,
		                                      ImDrawFlags_RoundCornersNone, 1, true);
	});
	DIWNE_DEBUG_INTERACTIONS(diwne, {
		ImVec2 originPos = ImVec2(getRect().Min.x, getRect().Max.y);
		if (dynamic_cast<DIWNE::NodeEditor*>(this))
		{
			originPos = ImVec2(getRect().Min.x, getRect().Min.y);
		}
		else if (dynamic_cast<DIWNE::Pin*>(this))
		{
			originPos = ImVec2(getRect().Max.x, getRect().Min.y);
		}
		else if (dynamic_cast<DIWNE::Link*>(this))
		{
			originPos = ImVec2(getRect().Min.x, getRect().Min.y);
		}
		auto interactionCount = context.logicalUpdates - debug_logicalUpdate;
		std::string topLeftString =
		    (std::string() + m_labelDiwne +
		     (!m_parentObject ? " (no parent)\n" : " (" + m_parentObject->m_labelDiwne + ")\n") +
		     (m_hovered ? "Hovered\n" : "") + (m_isPressed ? "Held\n" : "") + (m_isDragged ? "Dragged\n" : "") +
		     (m_selected ? "Selected\n" : "") +
		     (interactionCount > 0 ? "Logic update (" + std::to_string(interactionCount) + ")\n" : ""));

		if (auto editor = dynamic_cast<NodeEditor*>(this))
		{
			std::string dbgMsg1;
			dbgMsg1 += "v: " + std::to_string(context.visualUpdates);
			dbgMsg1 += " l: " + std::to_string(context.logicalUpdates);
			dbgMsg1 += " ic: " + std::to_string(context.inputConsumed);
			dbgMsg1 += " hc: " + std::to_string(context.hoverConsumed);
			dbgMsg1 += " po: " + std::to_string(context.popupOpened);
			ImGui::GetForegroundDrawList()->AddText(
			    diwne.canvas().diwne2screen(originPos) +
			        ImVec2(diwne.canvas().getViewportRectScreen().GetWidth() * 0.3, 0),
			    IM_COL32(255, 255, 0, 255), dbgMsg1.c_str());

			InteractionState& state = context.state;
			std::string dbgMsg2;
			std::string dragSource = (state.dragSource.expired() ? "no source" : state.dragSource.lock()->m_labelDiwne);
			dbgMsg2 += (state.dragging ? "[Dragging (" + dragSource + ")]" : "");
			dbgMsg2 += (state.dragEnd ? "[DragEnd (" + dragSource + ")]" : "");
			dbgMsg2 += (context.inputConsumed ? " [Input Consumed]" : "");
			if (state.action)
			{
				dbgMsg2 += " [" + state.action->name;
				dbgMsg2 +=
				    " (" + (state.action->source.expired() ? "no source" : state.action->source.lock()->m_labelDiwne);
				if (auto action = state.getActiveAction<Actions::DragNodeAction>())
					dbgMsg2 += ", count: " + std::to_string(action->nodes.size());
				dbgMsg2 += ")]";
			}
			ImGui::GetForegroundDrawList()->AddText(diwne.canvas().diwne2screen(originPos) +
			                                            ImVec2(diwne.canvas().getViewportRectScreen().GetWidth() * 0.25,
			                                                   ImGui::GetTextLineHeightWithSpacing()),
			                                        IM_COL32(255, 0, 255, 255), dbgMsg2.c_str());
			topLeftString += "\nMode: " + std::bitset<sizeof(int) * 2>(m_drawMode).to_string();
			auto lastActiveNode = editor->getLastActiveNode<>();
			if (lastActiveNode)
			{
				topLeftString +=
				    "\nLast active node: " + lastActiveNode->m_labelDiwne +
				    (lastActiveNode->getParentObject() ? "(" + lastActiveNode->getParentObject()->m_labelDiwne + ")"
				                                       : "(no parent)") +
				    " [" + std::to_string(lastActiveNode.use_count() - 1) + " refs]";
			}
		}
		ImGui::GetForegroundDrawList()->AddText(diwne.canvas().diwne2screen(originPos) + ImVec2(0, 0),
		                                        m_drawMode & DrawMode_JustDraw ? IM_COL32(224, 96, 116, 255)
		                                                                       : IM_COL32(97, 239, 255, 255),
		                                        topLeftString.c_str());
	});
}
void DiwneObject::setBringToFront(bool val)
{
	m_bringToFront = val;
}
bool DiwneObject::isToBeBroughtToFront()
{
	return m_bringToFront;
}
void DiwneObject::setForceDraw(bool val)
{
	m_forceDraw = val;
}
} /* namespace DIWNE */
