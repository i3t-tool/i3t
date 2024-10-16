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

#include "NodeEditor.h"

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
DiwneObject::DiwneObject(DIWNE::NodeEditor& diwne, DIWNE::ID id, std::string const labelDiwne)
    : diwne(diwne), m_idDiwne(id), m_labelDiwne(fmt::format("{}:{}", labelDiwne, m_idDiwne)),
      m_popupIDDiwne(fmt::format("popup_{}", m_labelDiwne))
{}

void DiwneObject::draw(DrawMode drawMode)
{
	FrameContext context(drawMode);
	drawDiwne(context);
};

void DiwneObject::drawDiwne(FrameContext& context)
{
	bool other_object_focused = diwne.m_objectFocused; // TODO: Figure out what this is about
	initializeDiwne(context);

	if (allowDrawing())
	{
		beginDiwne(context);

		ImGui::PushStyleColor(ImGuiCol_Text,
		                      diwne.mp_settingsDiwne->fontColor); // TODO: Sort this out in a theme rework

		content(context);

		ImGui::PopStyleColor();

		endDiwne(context);
		//		updateSizes();
		//		DIWNE_DEBUG((diwne), {
		//			diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, DIWNE_YELLOW_50, 0,
		// ImDrawFlags_RoundCornersNone, 			                   1, true);
		//		});
		//		m_inner_interaction_happen |= afterEndDiwne();
		//		m_inner_interaction_happen |= processInteractionsDiwne();
	}
	finalizeDiwne(context);

	m_isActive = other_object_focused ? false : m_inner_interaction_happen;
}

/*
DrawResult DiwneObject::drawDiwne(const FrameContext& context)
{
    // TODO: REWRITE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Summary of side effects:
    // Member variables:
    m_inner_interaction_happen;
    m_isActive;
    // + Gotta update rect probably at some point
    // We're also reading:
    diwne.m_objectFocused;

    // Removal of "m_inner_interaction_happen"
    // The variable is only really used in processInteractionDiwne
    // There is decides 1. whether to processInteractions at all
    // And then its a condition whether

#if DIWNE_DEBUG_ENABLED
    bool interactionInitialize = false;
    bool interactionBegin = false;
    bool interactionContent = false;
    bool interactionAfterEnd = false;

    bool interactionProcessInteraction = false;
    bool interactionFinalize = false;
#endif

    if (ImGui::IsKeyDown(ImGuiKey_E)) // TODO: Remove when not using it anymore <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        int x = 5;

    DIWNE_DEBUG_EXTRA_1(diwne, {
        if (this == (DiwneObject*) &diwne)
        {
            LOG_INFO("NODE EDITOR START");
        }
        LOG_INFO("drawDiwne() begin {}", m_labelDiwne);
    });

    bool other_object_focused = diwne.m_objectFocused;
    m_inner_interaction_happen_previous_draw = m_inner_interaction_happen;
    m_inner_interaction_happen = false;
    m_drawMode = context.drawMode;
    m_inner_interaction_happen |= initializeDiwne();
    DIWNE_DEBUG_EXTRA_1(diwne, { interactionInitialize = m_inner_interaction_happen; });
    if (allowDrawing())
    {
        // TODO: Move zoom scaling to the NodeEditor subclass
        if (this == (DiwneObject*) &diwne)
        {
            diwne.applyZoomScaling();
        }
        m_inner_interaction_happen |= beginDiwne();
        DIWNE_DEBUG_EXTRA_1(diwne, { interactionBegin = m_inner_interaction_happen; });
        begin(context);

        ImGui::PushStyleColor(ImGuiCol_Text, diwne.mp_settingsDiwne->fontColor);

        content(context);
        //		m_inner_interaction_happen |= beforeContentDiwne();
        //		m_inner_interaction_happen |= contentDiwne();
        //		m_inner_interaction_happen |= afterContentDiwne();
        DIWNE_DEBUG_EXTRA_1(diwne, { interactionContent = m_inner_interaction_happen; });

        ImGui::PopStyleColor();
        DIWNE_DEBUG_EXTRA_2((diwne), {
            switch (m_drawMode)
            {
            case DrawMode::Interacting:
                ImGui::Text("Interacting mode");
                break;
            case DrawMode::JustDraw:
                ImGui::Text("JustDraw mode");
                break;
            }
            ImGui::TextUnformatted(m_labelDiwne.c_str());
            if (m_isHeld)
                ImGui::TextUnformatted("Held");
            if (m_isDragged)
                ImGui::TextUnformatted("Dragged");
            if (m_selected)
                ImGui::TextUnformatted("Selected");
        });

        end();
        updateSizes();
        DIWNE_DEBUG((diwne), {
            diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, DIWNE_YELLOW_50, 0, ImDrawFlags_RoundCornersNone,
                               1, true);
        });
        m_inner_interaction_happen |= afterEndDiwne();
        DIWNE_DEBUG_EXTRA_1(diwne, { interactionAfterEnd = m_inner_interaction_happen; });
        m_inner_interaction_happen |= processInteractionsDiwne();
        DIWNE_DEBUG_EXTRA_1(diwne, { interactionProcessInteraction = m_inner_interaction_happen; });
        // TODO: Move zoom scaling to the NodeEditor subclass
        if (this == (DiwneObject*) &diwne)
        {
            diwne.restoreZoomScaling();
        }
    }
    m_inner_interaction_happen |= finalizeDiwne();
    DIWNE_DEBUG_EXTRA_1(diwne, { interactionFinalize = m_inner_interaction_happen; });

    m_isActive = other_object_focused ? false : m_inner_interaction_happen;

    DIWNE_DEBUG_EXTRA_1(
        diwne, { LOG_INFO("drawDiwne() end {}, i: {}, a: {}", m_labelDiwne, m_inner_interaction_happen, m_isActive); });
    DIWNE_DEBUG_EXTRA_1(diwne, {
        ImVec2 originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Max.y);
        if (dynamic_cast<DIWNE::NodeEditor*>(this))
        {
            originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Min.y);
        }
        else if (dynamic_cast<DIWNE::Pin*>(this))
        {
            originPos = ImVec2(getRectDiwne().Max.x, getRectDiwne().Min.y);
        }
        ImGui::GetForegroundDrawList()->AddText(diwne.diwne2screen(originPos), IM_COL32_WHITE,
                                                fmt::format("i:{},a:{},f:{}", m_inner_interaction_happen ? "Y" : "N",
                                                            m_isActive ? "Y" : "N", m_focused ? "Y" : "N")
                                                    .c_str());

        ImGui::GetForegroundDrawList()->AddText(
            diwne.diwne2screen(originPos) + ImVec2(0, ImGui::GetFontSize()), IM_COL32_WHITE,
            fmt::format("init: {}\nbegin: {}\ncontent: {}\nafterEnd: {}\nprocessI: {}\nfinalize: {}",
                        interactionInitialize, interactionBeforeBegin, interactionContent, interactionAfterEnd,
                        interactionProcessInteraction, interactionFinalize)
                .c_str());
    });

    return m_inner_interaction_happen;
}
*/

bool DiwneObject::allowDrawing()
{
	return true;
}

void DiwneObject::initialize(FrameContext& context) {}
void DiwneObject::initializeDiwne(FrameContext& context)
{
	return initialize(context);
}

void DiwneObject::content(FrameContext& context)
{
	ImGui::TextUnformatted(fmt::format("{} object content", m_labelDiwne).c_str());
}

bool DiwneObject::allowInteraction()
{
	return m_focusedForInteraction;
}

void DiwneObject::processInteractions(FrameContext& context) {}
void DiwneObject::processInteractionsDiwne(FrameContext& context)
{
	//	bool interaction_happen = false;
	//
	//	// Logger::getInstance().getAppLogger()->info("MouseDown: "
	//	// +std::to_string(ImGui::IsMouseDown(0)));
	//	// Logger::getInstance().getAppLogger()->info("MouseReleased: " +
	//	// std::to_string(ImGui::IsMouseReleased(0)));
	//	// Logger::getInstance().getAppLogger()->info("MouseDragging: " +
	//	// std::to_string(ImGui::IsMouseDragging(0)));
	//	// Logger::getInstance().getAppLogger()->info("MouseClicked: " +
	//	// std::to_string(ImGui::IsMouseClicked(0, false)));
	//	// Logger::getInstance().getAppLogger()->info("MouseClickedRepeat: " +
	//	// std::to_string(ImGui::IsMouseClicked(0, true)));
	//
	//	if (context.drawMode == Interacting && !m_inner_interaction_happen)
	//	{
	//		if (!ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
	//		{
	//			processFocusedDiwne(context);
	//			processFocusedForInteractionDiwne(context);
	//
	//			if (m_focused) /* diwne.m_objectFocused is checked in
	//			                  allowFocusedForInteraction() too */
	//			{
	//				diwne.m_objectFocused = true;
	//				if (diwne.getDiwneActionActive() == None || diwne.getDiwneActionActive() == FocusOnObject)
	//					diwne.setDiwneAction(FocusOnObject);
	//			}
	//
	//			if (allowInteraction())
	//			{
	//				processRaisePopupDiwne(context);
	//				if (m_selectable)
	//				{
	//					m_selected ? processUnselectDiwne(context) : processSelectDiwne(context);
	//				}
	//				m_isHeld ? processUnholdDiwne(context) : processHoldDiwne(context);
	//				if (m_isHeld)
	//				{
	//					interaction_happen |= m_isHeld; /* holding (not only change in hold
	//					                                   state) is interaction */
	//					diwne.setDiwneAction(getHoldActionType());
	//				}
	//				processDragDiwne(context);
	//				processInteractions(context);
	//			}
	//		}
	//		processShowPopupDiwne(context);
	//	}
	//	if (m_inner_interaction_happen && diwne.getDiwneActionActive() != DIWNE::DiwneAction::NewLink)
	//	{
	//		diwne.m_objectFocused = true;
	//	} /* any inner interaction (imgui too) block other DiwneObject to focus */
	//	DIWNE_DEBUG_EXTRA_2((diwne), {
	//		if (m_isActive)
	//		{
	//			diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255, 0, 255, 255), 0,
	//			                   ImDrawFlags_RoundCornersNone, 1, true);
	//		};
	//	});
}

void DiwneObject::finalize(FrameContext& context) {}
void DiwneObject::finalizeDiwne(FrameContext& context)
{
	finalize(context);
}

//

bool DiwneObject::setSelected(const bool selected)
{
	m_selected = m_selectable ? selected : false;
	return m_selected;
}
bool DiwneObject::getSelected() const
{
	return m_selected;
};
void DiwneObject::setSelectable(bool const selectable)
{
	m_selectable = selectable;
};
bool DiwneObject::getSelectable()
{
	return m_selectable;
}

void DiwneObject::beginDiwne(FrameContext& context)
{
	begin(context);
}
void DiwneObject::endDiwne(FrameContext& context)
{
	end(context);
	processInteractions(context);
}

void FrameContext::merge(const FrameContext& other)
{
	this->inputConsumed |= other.inputConsumed;
	this->interacted |= other.interacted;
}

void FrameContext::operator|=(const FrameContext& other)
{
	merge(other);
}

FrameContext& FrameContext::operator|(const FrameContext& other)
{
	merge(other);
	return *this;
}

//

bool DiwneObject::bypassRaisePopupAction()
{
	return diwne.bypassIsMouseReleased1();
}
bool DiwneObject::bypassFocusAction()
{
	return ImGui::IsItemHovered();
} /* block interaction with other elements; you have to override this if your
     object is not ImGui item (like Link) */
bool DiwneObject::bypassFocusForInteractionAction()
{
	return ImGui::IsItemHovered();
} /* you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassHoldAction()
{
	return diwne.bypassIsMouseClicked0();
}
bool DiwneObject::bypassUnholdAction()
{
	return diwne.bypassIsMouseReleased0();
}
bool DiwneObject::bypassSelectAction()
{
	return diwne.bypassIsMouseReleased0();
}
bool DiwneObject::bypassUnselectAction()
{
	return diwne.bypassIsMouseReleased0();
}
bool DiwneObject::bypassDragAction()
{
	return diwne.bypassIsMouseDragging0();
}
bool DiwneObject::bypassTouchAction()
{
	return diwne.bypassIsMouseClicked0();
}

// void DiwneObject::processFocused(FrameContext& context)
//{
//	// TODO: CONTINUE HERE add custom pin impl <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	if (bypassTouchAction())
//	{
//		diwne.setDiwneAction(getTouchActionType());
//	}
//	else
//	{
//		int x = 5; // Debug thing
//	}
//	diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, diwne.mp_settingsDiwne->objectFocusBorderColor,
//	                   diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
//	                   diwne.mp_settingsDiwne->objectFocusBorderThicknessDiwne);
//	DIWNE_DEBUG(diwne, {
//		diwne.AddRectDiwne(getRectDiwne().Min + ImVec2(1, 1), getRectDiwne().Max - ImVec2(1, 1), DIWNE_MAGENTA_50, 0,
//		                   ImDrawFlags_RoundCornersNone, 1);
//	});
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessFocused()
//{
//	// TODO: Rename, decode what we're doing here, note that his function is fully overriden in node editor which
//	//  duplicates this code but changes the SelectionRect behaviour (which should to be changed)
//	/* object is active from previous frame */
//	/* only one object can be focused */
//	/* not stole focus from selecting action */
//	/* we want focus of other object while new link */
//	bool ret =
//	    m_isActive ||
//	    (!diwne.m_objectFocused &&
//	     !(diwne.getDiwneActionActive() == SelectionRectFull || diwne.getDiwneActionActive() == SelectionRectTouch) &&
//	     (diwne.getDiwneAction() == None || diwne.getDiwneActionActive() == NewLink));
//	LOG_INFO("{} FOCUSED active: {}, focused: {}, actionActive: {}, action: {}, source: {}", (ret ? "YES" : "NO "),
//	         m_isActive, diwne.m_objectFocused, EnumUtils::name(diwne.getDiwneActionActive()),
//	         EnumUtils::name(diwne.getDiwneAction()), m_labelDiwne)
//	return ret;
// }
//
// void DiwneObject::processFocusedDiwne(FrameContext& context)
//{
//	if (bypassFocusAction() && allowProcessFocused())
//	{
//		m_focused = true;
//		return processFocused(context);
//	}
//	else
//	{
//		m_focused = false;
//		return;
//	}
// }
//
// void DiwneObject::processFocusedForInteraction(FrameContext& context)
//{
//	// TODO: Investiage exact usages and rename
//	diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max,
//	                   diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
//	                   diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
//	                   diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
//	DIWNE_DEBUG(diwne, {
//		diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, DIWNE_GREEN_50, 0, ImDrawFlags_RoundCornersNone, 1);
//	});
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessFocusedForInteraction()
//{
//	return allowProcessFocused();
// }
// void DiwneObject::processFocusedForInteractionDiwne(FrameContext& context)
//{
//	/* between frames mouse can go out of focus scope */
//	if ((bypassFocusForInteractionAction() || m_isHeld) && allowProcessFocusedForInteraction())
//	{
//		if (!dynamic_cast<DIWNE::NodeEditor*>(this))
//		{
//			int x = 5;
//		}
//		m_focusedForInteraction = true;
//		return processFocusedForInteraction(context);
//	}
//	else
//	{
//		m_focusedForInteraction = false;
//		return;
//	}
// }
//
// void DiwneObject::processHold(FrameContext& context)
//{
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessHold()
//{
//	return m_focusedForInteraction;
// }
// void DiwneObject::processHoldDiwne(FrameContext& context)
//{
//	if (bypassHoldAction() && allowProcessHold())
//	{
//		m_isHeld = true;
//		return processHold(context);
//	}
// }
//
// void DiwneObject::processUnhold(FrameContext& context)
//{
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessUnhold()
//{
//	return true;
// }
// void DiwneObject::processUnholdDiwne(FrameContext& context)
//{
//	if (bypassUnholdAction() && allowProcessUnhold())
//	{
//		m_isHeld = false;
//		if (m_isDragged)
//		{
//			diwne.m_takeSnap = true;
//		}
//		m_isDragged = false;
//		return processUnhold(context);
//	}
// }
//
// void DiwneObject::processDrag(FrameContext& context)
//{
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessDrag()
//{
//	return m_isHeld;
// }
// void DiwneObject::processDragDiwne(FrameContext& context)
//{
//	if (bypassDragAction() && allowProcessDrag())
//	{
//		m_isDragged = true;
//		diwne.setDiwneAction(getDragActionType());
//		return processDrag(context);
//	}
// }
//
// void DiwneObject::processSelect(FrameContext& context)
//{
//	context.interacted = true;
// }
// bool DiwneObject::allowProcessSelect()
//{
//	return m_isHeld && !m_isDragged;
// }
// void DiwneObject::processSelectDiwne(FrameContext& context)
//{
//	if (bypassSelectAction() && allowProcessSelect() && !InputManager::isAxisActive("DONTselect"))
//	{
//		setSelected(true);
//	}
//	return false;
// }
//
// void DiwneObject::processUnselect(FrameContext& context)
//{
//	return true;
// }
// bool DiwneObject::allowProcessUnselect()
//{
//	return m_isHeld && !m_isDragged;
// }
// void DiwneObject::processUnselectDiwne(FrameContext& context)
//{
//	if (bypassUnselectAction() && allowProcessUnselect())
//	{
//		setSelected(false);
//	}
// }
//
// bool DiwneObject::allowProcessRaisePopup()
//{
//	return !diwne.blockRaisePopup();
// }
// void DiwneObject::processRaisePopupDiwne(FrameContext& context)
//{
//	if (bypassRaisePopupAction() && allowProcessRaisePopup())
//	{
//		diwne.setPopupPosition(diwne.bypassDiwneGetPopupNewPositionAction());
//		ImGui::OpenPopup(m_popupIDDiwne.c_str());
//		return true;
//	}
//	return false;
// }
//
// void DiwneObject::processShowPopupDiwne(FrameContext& context)
//{
//	if (diwne.m_popupDrawn)
//	{
//		return false;
//	}
//	else
//	{
//		diwne.restoreZoomScaling();
//		diwne.m_popupDrawn = popupDiwne(m_popupIDDiwne, diwne.getPopupPosition(), &expandPopupContent, *this);
//		diwne.applyZoomScaling();
//		return diwne.m_popupDrawn;
//	}
// }
//
// void DiwneObject::popupContent(FrameContext& context)
//{
//	ImGui::MenuItem("Override this method with content of popup menu of your object");
// }
//
// void DiwneObject::showTooltipLabel(std::string const& label, ImColor const&& color)
//{
//	if (!diwne.m_tooltipDrawn)
//	{
//		diwne.m_tooltipDrawn = true;
//		ImGui::BeginTooltip();
//		ImGui::TextColored(color, label.c_str());
//		ImGui::EndTooltip();
//	}
// }
} /* namespace DIWNE */
