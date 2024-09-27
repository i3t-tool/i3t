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
#include "Core/Input/InputManager.h"
#include "diwne_include.h"

namespace DIWNE
{

/* ============================================ */
/* ========= D i w n e  O b j e c t =========== */
/* ============================================ */

/* Order of actions is important -> usually,  objects are drawn in the order
 * Link-Pin-Node-Diwne, so Pin does not know about process that WILL happen in
 * Node (has to ask for previous frame action) */

DiwneObject::DiwneObject(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne)
    : diwne(diwne), m_idDiwne(id), m_labelDiwne(fmt::format("{}:{}", labelDiwne, m_idDiwne)),
      m_popupIDDiwne(fmt::format("popup_{}", m_labelDiwne)), m_inner_interaction_happen(false), m_isHeld(false),
      m_isDragged(false), m_selected(false), m_focusedForInteraction(false), m_focused(false), m_isActive(false),
      m_drawMode(DrawMode::Interacting), m_selectable(true)
{
	// \TODO m_inner_interaction_happen_previous_draw is not initialized
}

bool DiwneObject::allowDrawing()
{
	return true;
}
bool DiwneObject::drawDiwne(DrawMode drawMode /* = DrawMode::Interacting */)
{
	bool other_object_focused = diwne.m_objectFocused;
	m_inner_interaction_happen_previous_draw = m_inner_interaction_happen;
	m_inner_interaction_happen = false;
	m_drawMode = drawMode;
	m_inner_interaction_happen |= initializeDiwne();
	if (allowDrawing())
	{
		if (this == &diwne)
		{
			diwne.applyZoomScaling();
		}
		m_inner_interaction_happen |= beforeBeginDiwne();
		begin();

		ImGui::PushStyleColor(ImGuiCol_Text, diwne.mp_settingsDiwne->fontColor);

		m_inner_interaction_happen |= beforeContentDiwne();
		m_inner_interaction_happen |= contentDiwne();
		m_inner_interaction_happen |= afterContentDiwne();

		ImGui::PopStyleColor();
#ifdef DIWNE_DEBUG
		DIWNE_DEBUG((diwne), {
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
		}); /* close of macro */
#endif      // DIWNE_DEBUG

		end();
		updateSizes();
#ifdef DIWNE_DEBUG
		DIWNE_DEBUG((diwne), {
			diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255, 0, 0, 50), 0,
			                   ImDrawFlags_RoundCornersNone, 3);
		}); /* close of macro */
#endif      // DIWNE_DEBUG
		m_inner_interaction_happen |= afterEndDiwne();
		m_inner_interaction_happen |= processInteractionsDiwne();
		if (this == &diwne)
		{
			diwne.restoreZoomScaling();
		}
	}
	m_inner_interaction_happen |= finalizeDiwne();

	m_isActive = other_object_focused ? false : m_inner_interaction_happen;

	return m_inner_interaction_happen;
}

bool DiwneObject::initialize()
{
	return false;
}
bool DiwneObject::initializeDiwne()
{
	return initialize();
}

bool DiwneObject::beforeBegin()
{
	return false;
}
bool DiwneObject::beforeBeginDiwne()
{
	return beforeBegin();
}

bool DiwneObject::beforeContent()
{
	return false;
};
bool DiwneObject::beforeContentDiwne()
{
	return beforeContent();
}

void DiwneObject::updateSizes() {}

bool DiwneObject::content()
{
	ImGui::TextUnformatted(fmt::format("{} object content", m_labelDiwne).c_str());
	return false;
}
bool DiwneObject::contentDiwne()
{
	return content();
}

bool DiwneObject::afterContent()
{
	return false;
}
bool DiwneObject::afterContentDiwne()
{
	return afterContent();
}

bool DiwneObject::afterEnd()
{
	return false;
}
bool DiwneObject::afterEndDiwne()
{
	return afterEnd();
}

bool DiwneObject::allowInteraction()
{
	return m_focusedForInteraction;
}

bool DiwneObject::processInteractionsAlways()
{
	return processShowPopupDiwne();
}
bool DiwneObject::processInteractions()
{
	return false;
}
bool DiwneObject::processInteractionsDiwne()
{
	bool interaction_happen = false;

	// Logger::getInstance().getAppLogger()->info("MouseDown: "
	// +std::to_string(ImGui::IsMouseDown(0)));
	// Logger::getInstance().getAppLogger()->info("MouseReleased: " +
	// std::to_string(ImGui::IsMouseReleased(0)));
	// Logger::getInstance().getAppLogger()->info("MouseDragging: " +
	// std::to_string(ImGui::IsMouseDragging(0)));
	// Logger::getInstance().getAppLogger()->info("MouseClicked: " +
	// std::to_string(ImGui::IsMouseClicked(0, false)));
	// Logger::getInstance().getAppLogger()->info("MouseClickedRepeat: " +
	// std::to_string(ImGui::IsMouseClicked(0, true)));

	if (m_drawMode == Interacting && !m_inner_interaction_happen)
	{
		if (!ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
		{
			interaction_happen |= processFocusedDiwne();
			interaction_happen |= processFocusedForInteractionDiwne();

			if (m_focused) /* diwne.m_objectFocused is checked in
			                  allowFocusedForInteraction() too */
			{
				diwne.m_objectFocused = true;
				if (diwne.getDiwneActionActive() == None || diwne.getDiwneActionActive() == FocusOnObject)
					diwne.setDiwneAction(FocusOnObject);
			}

			if (allowInteraction())
			{
				interaction_happen |= processRaisePopupDiwne();
				if (m_selectable)
				{
					interaction_happen |= m_selected ? processUnselectDiwne() : processSelectDiwne();
				}
				interaction_happen |= m_isHeld ? processUnholdDiwne() : processHoldDiwne();
				if (m_isHeld)
				{
					interaction_happen |= m_isHeld; /* holding (not only change in hold
					                                   state) is interaction */
					diwne.setDiwneAction(getHoldActionType());
				}
				interaction_happen |= processDragDiwne();
				interaction_happen |= processInteractions();
			}
		}
		interaction_happen |= processInteractionsAlways();
	}
	if (m_inner_interaction_happen && diwne.getDiwneActionActive() != DIWNE::DiwneAction::NewLink)
	{
		diwne.m_objectFocused = true;
	} /* any inner interaction (imgui too) block other DiwneObject to focus */
#ifdef DIWNE_DEBUG
	DIWNE_DEBUG((diwne), {
		if (m_isActive)
		{
			diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255, 0, 255, 255), 0,
			                   ImDrawFlags_RoundCornersNone, 5);
		};
	}); /* close of macro */
#endif  // DIWNE_DEBUG
	return interaction_happen;
}

bool DiwneObject::finalize()
{
	return false;
}
bool DiwneObject::finalizeDiwne()
{
	return finalize();
}

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

bool DiwneObject::processFocused()
{
	if (bypassTouchAction())
	{
		diwne.setDiwneAction(getTouchActionType());
	}
	diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, diwne.mp_settingsDiwne->objectFocusBorderColor,
	                   diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
	                   diwne.mp_settingsDiwne->objectFocusBorderThicknessDiwne);
	return true;
}
bool DiwneObject::allowProcessFocused()
{
	return m_isActive                 /* object is active from previous frame */
	       || (!diwne.m_objectFocused /* only one object can be focused */
	           && !(diwne.getDiwneActionActive() == SelectionRectFull ||
	                diwne.getDiwneActionActive() == SelectionRectTouch) /* not stole focus from selecting action */
	           && (diwne.getDiwneAction() == None ||
	               diwne.getDiwneActionActive() == NewLink /* we want focus of other object while new link */));
}

bool DiwneObject::processFocusedDiwne()
{
	if (bypassFocusAction() && allowProcessFocused())
	{
		m_focused = true;
		return processFocused();
	}
	else
	{
		m_focused = false;
		return false;
	}
}

bool DiwneObject::processFocusedForInteraction()
{
	diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max,
	                   diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
	                   diwne.mp_settingsDiwne->selectionRounding, ImDrawFlags_RoundCornersAll,
	                   diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
	return true;
}
bool DiwneObject::allowProcessFocusedForInteraction()
{
	return allowProcessFocused();
}
bool DiwneObject::processFocusedForInteractionDiwne()
{
	/* between frames mouse can go out of focus scope */
	if ((bypassFocusForInteractionAction() || m_isHeld) && allowProcessFocusedForInteraction())
	{
		m_focusedForInteraction = true;
		return processFocusedForInteraction();
	}
	else
	{
		m_focusedForInteraction = false;
		return false;
	}
}

bool DiwneObject::processHold()
{
	return true;
}
bool DiwneObject::allowProcessHold()
{
	return m_focusedForInteraction;
}
bool DiwneObject::processHoldDiwne()
{
	if (bypassHoldAction() && allowProcessHold())
	{
		m_isHeld = true;
		return processHold();
	}
	return false;
}

bool DiwneObject::processUnhold()
{
	return true;
}
bool DiwneObject::allowProcessUnhold()
{
	return true;
}
bool DiwneObject::processUnholdDiwne()
{
	if (bypassUnholdAction() && allowProcessUnhold())
	{
		m_isHeld = false;
		if (m_isDragged)
		{
			diwne.m_takeSnap = true;
		}
		m_isDragged = false;
		return processUnhold();
	}
	return false;
}

bool DiwneObject::processDrag()
{
	return true;
}
bool DiwneObject::allowProcessDrag()
{
	return m_isHeld;
}
bool DiwneObject::processDragDiwne()
{
	if (bypassDragAction() && allowProcessDrag())
	{
		m_isDragged = true;
		diwne.setDiwneAction(getDragActionType());
		return processDrag();
	}
	return false;
}

bool DiwneObject::processSelect()
{
	return true;
}
bool DiwneObject::allowProcessSelect()
{
	return m_isHeld && !m_isDragged;
}
bool DiwneObject::processSelectDiwne()
{
	if (bypassSelectAction() && allowProcessSelect() && !InputManager::isAxisActive("DONTselect"))
	{
		setSelected(true);
	}
	return false;
}

bool DiwneObject::processUnselect()
{
	return true;
}
bool DiwneObject::allowProcessUnselect()
{
	return m_isHeld && !m_isDragged;
}
bool DiwneObject::processUnselectDiwne()
{
	if (bypassUnselectAction() && allowProcessUnselect())
	{
		setSelected(false);
	}
	return false;
}

bool DiwneObject::allowProcessRaisePopup()
{
	return !diwne.blockRaisePopup();
}
bool DiwneObject::processRaisePopupDiwne()
{
	if (bypassRaisePopupAction() && allowProcessRaisePopup())
	{
		diwne.setPopupPosition(diwne.bypassDiwneGetPopupNewPositionAction());
		ImGui::OpenPopup(m_popupIDDiwne.c_str());
		return true;
	}
	return false;
}

bool DiwneObject::processShowPopupDiwne()
{
	if (diwne.m_popupDrawn)
	{
		return false;
	}
	else
	{
		diwne.restoreZoomScaling();
		diwne.m_popupDrawn = popupDiwne(m_popupIDDiwne, diwne.getPopupPosition(), &expandPopupContent, *this);
		diwne.applyZoomScaling();
		return diwne.m_popupDrawn;
	}
}

void DiwneObject::popupContent()
{
	if (ImGui::MenuItem("Override this method with content of popup menu of your object"))
	{}
}

void DiwneObject::showTooltipLabel(std::string const& label, ImColor const&& color)
{
	if (!diwne.m_tooltipDrawn)
	{
		diwne.m_tooltipDrawn = true;
		ImGui::BeginTooltip();
		ImGui::TextColored(color, label.c_str());
		ImGui::EndTooltip();
	}
}

} /* namespace DIWNE */
