#include "diwne_include.h"

namespace DIWNE
{

/* ============================================ */
/* ========= D i w n e  O b j e c t =========== */
/* ============================================ */

/* Order of action is important -> usually are objects drawn in order Link-Pin-Node-Diwne, so Pin do not know about process that WILL happen in Node (have to ask for previous frame action) */

DiwneObject::DiwneObject(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne)
    :   diwne(diwne)
    ,   m_idDiwne(id)
    ,   m_labelDiwne(fmt::format("{}:{}", labelDiwne, m_idDiwne))
    ,   m_popupIDDiwne(fmt::format("popup_{}", m_labelDiwne))
    ,   m_inner_interaction_happen(false)
    ,   m_isHeld(false)
    ,   m_isDraged(false)
    ,   m_selected(false)
    ,   m_focusedForInteraction(false)
    ,   m_focused(false)
    ,   m_isActive(false)
    ,   m_drawMode(DrawMode::Interacting)
{}

bool DiwneObject::allowDrawing(){return true;}
bool DiwneObject::drawDiwne(DrawMode drawMode/*=DrawMode::Interacting*/)
{
    m_inner_interaction_happen_previous_draw = m_inner_interaction_happen;
    m_inner_interaction_happen = false;
    m_drawMode = drawMode;
    m_inner_interaction_happen |= initializeDiwne();
    if (allowDrawing())
    {
        m_inner_interaction_happen |= beforeBeginDiwne();
        begin();

        m_inner_interaction_happen |= beforeContentDiwne();
        m_inner_interaction_happen |= contentDiwne();
        m_inner_interaction_happen |= afterContentDiwne();

#ifdef DIWNE_DEBUG
DIWNE_DEBUG((diwne), {
        switch (m_drawMode) {case DrawMode::Interacting: ImGui::Text("Interacting mode"); break;
                             case DrawMode::JustDraw:    ImGui::Text("JustDraw mode");    break;}
        ImGui::TextUnformatted(m_labelDiwne.c_str());
        if (m_isHeld) ImGui::TextUnformatted("Held");
        if (m_isDraged) ImGui::TextUnformatted("Dragged");
        if (m_selected) ImGui::TextUnformatted("Selected");
}); /* close of macro */
#endif // DIWNE_DEBUG

        end();
        updateSizes();
#ifdef DIWNE_DEBUG
DIWNE_DEBUG((diwne), {
        diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255,0,0,50), 0, ImDrawCornerFlags_None, 3);
}); /* close of macro */
#endif // DIWNE_DEBUG

        m_inner_interaction_happen |= afterEndDiwne();
        m_inner_interaction_happen |= processInteractionsDiwne();
    }
    m_inner_interaction_happen |= finalizeDiwne();

    m_isActive = m_inner_interaction_happen;
    return m_inner_interaction_happen;
}

bool DiwneObject::initialize(){return false;}
bool DiwneObject::initializeDiwne()
{
    return initialize();
}

bool DiwneObject::beforeBegin(){return false;}
bool DiwneObject::beforeBeginDiwne(){return beforeBegin();}

bool DiwneObject::beforeContent(){return false;};
bool DiwneObject::beforeContentDiwne(){return beforeContent();}

void DiwneObject::updateSizes(){}

bool DiwneObject::content(){ImGui::TextUnformatted(fmt::format("{} object content", m_labelDiwne).c_str()); return false;}
bool DiwneObject::contentDiwne(){return content();}


bool DiwneObject::afterContent(){return false;}
bool DiwneObject::afterContentDiwne(){return afterContent();}

bool DiwneObject::afterEnd(){return false;}
bool DiwneObject::afterEndDiwne(){return afterEnd();}

bool DiwneObject::allowInteraction(){return m_focusedForInteraction;}

bool DiwneObject::processInteractionsAlways(){return processShowPopupDiwne();}
bool DiwneObject::processInteractions(){return false;}
bool DiwneObject::processInteractionsDiwne()
{
    bool interaction_happen = false;

    if (m_drawMode == Interacting && !m_inner_interaction_happen)
    {
        if (!ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
        {
            interaction_happen |= processObjectFocused();
            interaction_happen |= processObjectFocusedForInteraction();

            if (m_focused) /* diwne.m_objectFocused is checked in allowFocusedForInteraction() too */
            {
                diwne.m_objectFocused = true;
                if ( diwne.getDiwneActionActive() == None || diwne.getDiwneActionActive() == FocusOnObject) diwne.setDiwneAction(FocusOnObject);
            }

            if (allowInteraction())
            {
                interaction_happen |= processRaisePopupDiwne();
                interaction_happen |= m_selected ? processObjectUnselect() : processObjectSelect();
                interaction_happen |= m_isHeld ? processObjectUnhold() : processObjectHold();
                if (m_isHeld){interaction_happen |= m_isHeld; /* holding (not only change in hold state) is interaction */ diwne.setDiwneAction(getHoldActionType());}
                interaction_happen |= processObjectDrag();
                interaction_happen |= processInteractions();
            }

        }
        interaction_happen |= processInteractionsAlways();
    }
    if(m_inner_interaction_happen){diwne.m_objectFocused=true;} /* any inner interaction (imgui too) block other DiwneObject to focus */
#ifdef DIWNE_DEBUG
DIWNE_DEBUG((diwne), {
    if (m_isActive) {diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255,0,255,255), 0, ImDrawCornerFlags_None, 5);};
}); /* close of macro */
#endif // DIWNE_DEBUG
    return interaction_happen;
}

bool DiwneObject::finalize(){return false;}
bool DiwneObject::finalizeDiwne(){return finalize();}

bool DiwneObject::bypassRaisePopupAction(){return diwne.bypassIsMouseReleased1();}
bool DiwneObject::bypassFocusAction(){return ImGui::IsItemHovered();} /* block interaction with other elements; you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassFocusForInteractionAction(){return ImGui::IsItemHovered(); } /* you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassHoldAction(){return diwne.bypassIsMouseClicked0();}
bool DiwneObject::bypassUnholdAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassSelectAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassUnselectAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassDragAction(){return diwne.bypassIsMouseDragging0();}
bool DiwneObject::bypassTouchAction(){return diwne.bypassIsMouseClicked0();}

bool DiwneObject::processFocused()
{
    if (bypassTouchAction()) {diwne.setDiwneAction(getTouchActionType());}
    diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, diwne.mp_settingsDiwne->objectFocusBorderColor, 0, ImDrawCornerFlags_None, diwne.mp_settingsDiwne->objectFocusBorderThicknessDiwne);
    return true;
}
bool DiwneObject::allowProcessFocused(){return m_isActive /* object is active from previous frame */
                                                || (!diwne.m_objectFocused /* only one object can be focused */
                                                    && !(diwne.getDiwneActionActive() == SelectionRectFull || diwne.getDiwneActionActive() == SelectionRectTouch) /* not stole focus from selecting action */
                                                    && (diwne.getDiwneAction() == None || diwne.getDiwneActionActive() == NewLink /* we want focus of other object while new link */ ))
                                        ;}

bool DiwneObject::processObjectFocused()
{
    if ( bypassFocusAction() && allowProcessFocused() )
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
    diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor, 0, ImDrawCornerFlags_None, diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
    return true;
}
bool DiwneObject::allowProcessFocusedForInteraction(){return allowProcessFocused();}
bool DiwneObject::processObjectFocusedForInteraction()
{
    if ( (bypassFocusForInteractionAction() || m_isHeld /* between frames mouse can go out of focus scope */ ) && allowProcessFocusedForInteraction() )
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

bool DiwneObject::processHold() {return true;}
bool DiwneObject::allowProcessHold(){return m_focusedForInteraction;}
bool DiwneObject::processObjectHold()
{
    if (bypassHoldAction() && allowProcessHold())
    {
        m_isHeld = true;
        return processHold();
    }
    return false;
}

bool DiwneObject::processUnhold() {return true;}
bool DiwneObject::allowProcessUnhold(){return true;}
bool DiwneObject::processObjectUnhold()
{
    if (bypassUnholdAction() && allowProcessUnhold())
    {
        m_isHeld = false;
        if (m_isDraged){diwne.m_takeSnap = true;}
        m_isDraged = false;
        return processUnhold();
    }
    return false;
}

bool DiwneObject::processDrag() {return true;}
bool DiwneObject::allowProcessDrag(){return m_isHeld;}
bool DiwneObject::processObjectDrag()
{
    if (bypassDragAction() && allowProcessDrag())
    {
        m_isDraged = true;
        diwne.setDiwneAction(getDragActionType());
        return processDrag();
    }
    return false;
}

bool DiwneObject::processSelect() {return true;}
bool DiwneObject::allowProcessSelect(){return m_isHeld && !m_isDraged;}
bool DiwneObject::processObjectSelect()
{
    if (bypassSelectAction() && allowProcessSelect())
    {
        m_selected = true;
        diwne.m_takeSnap = true;
        return processSelect();
    }
    return false;
}

bool DiwneObject::processUnselect() {return true;}
bool DiwneObject::allowProcessUnselect(){return m_isHeld && !m_isDraged;}
bool DiwneObject::processObjectUnselect()
{
    if (bypassUnselectAction() && allowProcessUnselect())
    {
        m_selected = false;
        diwne.m_takeSnap = true;
        return processUnselect();
    }
    return false;
}


bool DiwneObject::allowProcessRaisePopup(){return !diwne.blockRaisePopup();}
bool DiwneObject::processRaisePopupDiwne()
{
    if(bypassRaisePopupAction() && allowProcessRaisePopup())
    {
        diwne.setPopupPosition(diwne.bypassDiwneGetPopupNewPositionAction());
        ImGui::OpenPopup(m_popupIDDiwne.c_str());
        return true;
    }
    return false;
}

bool DiwneObject::processShowPopupDiwne()
{
    if(diwne.m_popupDrawn)
    {
        return false;
    }
    else
    {
        diwne.m_popupDrawn = popupDiwne(m_popupIDDiwne, diwne.getPopupPosition(), &expandPopupContent, *this);;
        return diwne.m_popupDrawn;
    }
}

void DiwneObject::popupContent(){ if(ImGui::MenuItem("Override this method with content of popup menu of your object")) {} }

void DiwneObject::showTooltipLabel(std::string const& label, ImColor const&& color)
{
    if(!diwne.m_tooltipDrawn)
    {
        diwne.m_tooltipDrawn = true;
        ImGui::BeginTooltip();
            ImGui::TextColored(color, label.c_str());
        ImGui::EndTooltip();
    }
}

} /* namespace DIWNE */
