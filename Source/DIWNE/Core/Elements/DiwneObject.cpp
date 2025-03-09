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

// TODO: REMOVE THIS IMPORT, move the functionality to the concrete Workspace implementation
#include "Core/Input/InputManager.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_actions.h"

#if DIWNE_DEBUG_ENABLED
#include "Pin.h"
#endif

/*
 * Order of actions is important -> usually,  objects are drawn in the order
 * Link-Pin-Node-Diwne, so Pin does not know about process that WILL happen in
 * Node (has to ask for previous frame action)
 */

namespace DIWNE
{
unsigned long long DiwneObject::g_diwneIDCounter = 1; // 0 is reserved for no/invalid ID

DiwneObject::DiwneObject(DIWNE::NodeEditor& diwne, std::string const labelDiwne)
    : diwne(diwne), m_idDiwne(g_diwneIDCounter++), m_labelDiwne(fmt::format("{}:{}", labelDiwne, m_idDiwne)),
      m_popupIDDiwne(fmt::format("popup_{}", m_labelDiwne))
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
	throw std::exception("Not implemented");
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

	bool other_object_focused = diwne.m_objectFocused; // TODO: Figure out what this is about

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

	// TODO: Remove
	m_isActive = other_object_focused ? false : m_inner_interaction_happen;
}

void DiwneObject::initialize(DrawInfo& context) {}
void DiwneObject::initializeDiwne(DrawInfo& context)
{
	return initialize(context);
}

void DiwneObject::beginDiwne(DrawInfo& context)
{
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
	if (m_drawMode != DrawMode_Interactive)
		return;

	if (!allowInteraction())
		return;

	processHoverDiwne(context);
	processPressAndReleaseDiwne(context);
	processDragDiwne(context);
	processSelectDiwne(context);
	processPopupDiwne(context);

	processInteractions(context); // Process other user interactions
}

void DiwneObject::processHoverDiwne(DrawInfo& context)
{
	bool hovered = isHoveredDiwne() && !context.hoverConsumed &&
	               (context.state.hoverTarget.empty() || context.state.hoverTarget == m_labelDiwne);
	m_hovered = hovered && allowHover();
	if (m_hovered)
		onHover(context);

	if (hovered)
	{
		// We are being hovered but hover isn't allowed / is disabled.
		// In that case we still propagate the hover event, propagation can still end if this is a hover root

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
	bool weAreDragSource = context.state.dragSource == m_labelDiwne;
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
		m_isDragged = isDragged && allowDragStart(); // TODO: Rename allowDrag to isDragAllowed? isDragStartAllowed?
		                                             // allowStartDrag, yeah <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		if (m_isDragged)
		{
			context.state.dragStart = true;
			context.state.dragging = true;
			context.state.dragSource = m_labelDiwne;
		}
	}
	if (m_isDragged) // Dispatch user method
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
	if (context.state.dragging && context.state.dragSource != m_labelDiwne)
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

bool InteractionState::isActionActive(const std::string& name, const std::string& source)
{
	return isActionActive(name) && action->source == source;
}

bool InteractionState::anyActionActive()
{
	return action != nullptr;
}

void InteractionState::nextFrame()
{
	// TODO: Dragging and current action must be stopped when the source is destroyed
	//  Good solution to safeguard all posibilities would be to use a weak ptr as source
	//  When object is REMOVED, onDestroy is not called and cannot ensure drag end
	// CONTINUE HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if (action != nullptr)
	{
		action->onFrameEnd();

		// When action ends we want to keep it active for the remainder of the frame (so we can react to it ending)
		// Hence the action reset is defered to the end of the frame here.
		if (action->endActionThisFrame)
		{
			endAction(true);
		}
	}

	// Set dragging to false on dragEnd
	// In the frame when dragging ends, dragging and dragEnd are both true.
	// dragEnd resets next frame (we're not infering it over)
	// But dragging needs to be set to false here at the end of frame.
	if (dragEnd)
	{
		dragging = false;
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

bool DiwneObject::bypassSelectAction()
{
	return diwne.input().bypassIsMouseReleased0();
}
bool DiwneObject::bypassUnselectAction()
{
	return diwne.input().bypassIsMouseReleased0();
}
bool DiwneObject::isDraggedDiwne()
{
	return diwne.input().bypassIsMouseDragging0();
}
bool DiwneObject::bypassTouchAction()
{
	return diwne.input().bypassIsMouseClicked0();
}

void DiwneObject::popupContent(DrawInfo& context)
{
	ImGui::MenuItem("Override this method with content of popup menu of your object");
}

void DiwneObject::onHover(DrawInfo& context)
{
	// TODO: CONTINUE HERE add custom pin impl <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  Pins should probably have their own effect, come back to this later

	context.update(true);

	// Draw hover border
	// TODO: Not sure if this should remain in DiwneObject or be virtual.
	//  Maybe move this impl into node and make it purely virtual?
	//	diwne.canvas().AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max,
	//	                               diwne.mp_settingsDiwne->objectHoverBorderColor,
	//	                               diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
	//	                               diwne.mp_settingsDiwne->objectHoverBorderThicknessDiwne);
	// TODO: Remove later, temporarily use a bright color for hover
	diwne.canvas().AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255, 0, 0, 150),
	                            diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
	                            diwne.mp_settingsDiwne->objectHoverBorderThicknessDiwne);

	DIWNE_DEBUG(diwne, {
		diwne.canvas().AddRectDiwne(getRectDiwne().Min + ImVec2(1, 1), getRectDiwne().Max - ImVec2(1, 1),
		                            DIWNE_MAGENTA_50, 0, ImDrawFlags_RoundCornersNone, 1);
	});
}

void DiwneObject::onPressed(bool justPressed, DrawInfo& context)
{
	if (justPressed)
		context.logicalUpdate(false);
	DIWNE_DEBUG_INTERACTIONS(diwne, {
		diwne.canvas().AddRectDiwne(getRectDiwne().Min + ImVec2(2, 2), getRectDiwne().Max - ImVec2(2, 2),
		                            DIWNE_YELLOW_50, 0, ImDrawFlags_RoundCornersNone, 1);
		if (justPressed)
		{
			diwne.canvas().AddRectFilledDiwne(getRectDiwne().Min + ImVec2(2, 2), getRectDiwne().Max - ImVec2(2, 2),
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
			diwne.canvas().AddRectFilledDiwne(getRectDiwne().Min + ImVec2(2, 2), getRectDiwne().Max - ImVec2(2, 2),
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
			diwne.canvas().AddRectFilledDiwne(getRectDiwne().Min + ImVec2(1, 1), getRectDiwne().Max - ImVec2(1, 1),
			                                  DIWNE_GREEN_50, 0, ImDrawFlags_RoundCornersNone);
		}
		if (dragEnd)
		{
			diwne.canvas().AddRectFilledDiwne(getRectDiwne().Min + ImVec2(1, 1), getRectDiwne().Max - ImVec2(1, 1),
			                                  DIWNE_RED_50, 0, ImDrawFlags_RoundCornersNone);
		}
		diwne.canvas().AddRectDiwne(getRectDiwne().Min + ImVec2(1, 1), getRectDiwne().Max - ImVec2(1, 1), DIWNE_CYAN_50,
		                            0, ImDrawFlags_RoundCornersNone, 2);
	});
}

void DiwneObject::onSelection(bool selected) {}

bool DiwneObject::bypassPressAction()
{
	return false;
}
bool DiwneObject::bypassReleaseAction()
{
	return false;
}

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
			diwne.deselectNodes();
		setSelected(!wasSelected);
		context.consumeInput(); // Only one item is selected per click
	}
	return false;
}

bool DiwneObject::allowPopup() const
{
	return true;
}
void DiwneObject::processPopupDiwne(DrawInfo& context)
{
	if (context.state.anyActionActive())
		return;
	// TODO: Make bypassIsMouseReleased1 a triggerPopup() or something method
	// TODO: What about dragging? Will popup open at the end of a drag?
	if (m_hovered && !context.popupOpened && diwne.input().bypassIsMouseReleased1() && allowPopup())
	{
		ImGui::OpenPopup(m_popupIDDiwne.c_str());
		context.update(true, true, true);
		context.popup();
		// Store popup position for stuff like adding nodes at the popup location
		diwne.setPopupPosition(diwne.input().bypassGetMousePos());
	}
	if (ImGui::IsPopupOpen(m_popupIDDiwne.c_str()))
	{
		const bool zoomScalingWasActive = diwne.canvas().ensureZoomScaling(false);
		// I don't think we need to call ImGui::SetNextWindowPos as we want the popup to simply be where the mouse is
		// and ImGui handles that on its own
		if (ImGui::BeginPopup(m_popupIDDiwne.c_str()))
		{
			popupContent(context);
			ImGui::EndPopup();
		}
		diwne.canvas().ensureZoomScaling(zoomScalingWasActive);
	}
}

void DiwneObject::showTooltipLabel(const std::string& label, const ImColor&& color)
{
	if (!diwne.m_tooltipDrawn)
	{
		diwne.m_tooltipDrawn = true;
		ImGui::BeginTooltip();
		ImGui::TextColored(color, label.c_str());
		ImGui::EndTooltip();
	}
}

bool DiwneObject::isDragging(DrawInfo& context)
{
	return context.state.dragging && context.state.dragSource == m_labelDiwne;
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
	DIWNE_DEBUG((diwne), {
		diwne.canvas().AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, DIWNE_YELLOW_50, 0,
		                            ImDrawFlags_RoundCornersNone, 1, true);
	});
	DIWNE_DEBUG_INTERACTIONS(diwne, {
		ImVec2 originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Max.y);
		if (dynamic_cast<DIWNE::NodeEditor*>(this))
		{
			originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Min.y);
		}
		else if (dynamic_cast<DIWNE::Pin*>(this))
		{
			originPos = ImVec2(getRectDiwne().Max.x, getRectDiwne().Min.y);
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
			dbgMsg2 += (state.dragging ? "[Dragging (" + state.dragSource + ")]" : "");
			dbgMsg2 += (state.dragEnd ? "[DragEnd (" + state.dragSource + ")]" : "");
			dbgMsg2 += (context.inputConsumed ? " [Input Consumed]" : "");
			if (state.action)
			{
				dbgMsg2 += " [" + state.action->name + " (" + state.action->source;
				if (auto action = state.getActiveAction<Actions::DragNodeAction>())
					dbgMsg2 += ", count: " + std::to_string(action->nodes.size());
				dbgMsg2 += ")]";
			}
			ImGui::GetForegroundDrawList()->AddText(diwne.canvas().diwne2screen(originPos) +
			                                            ImVec2(diwne.canvas().getViewportRectScreen().GetWidth() * 0.25,
			                                                   ImGui::GetTextLineHeightWithSpacing()),
			                                        IM_COL32(255, 0, 255, 255), dbgMsg2.c_str());

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
} /* namespace DIWNE */
