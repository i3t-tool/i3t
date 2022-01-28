#include "diwne_include.h"

namespace DIWNE
{

/* ============================================ */
/* ========= D i w n e  O b j e c t =========== */
/* ============================================ */

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
    ,   m_drawMode(DrawMode::Interacting)
{}

bool DiwneObject::allowDrawing(){return true;}
bool DiwneObject::drawDiwne(DrawMode drawMode/*=DrawMode::Interacting*/)
{
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
        switch (m_drawMode) {case DrawMode::Interacting: ImGui::Text("Interacting mode"); break;
                             case DrawMode::JustDraw:    ImGui::Text("JustDraw mode");    break;}
        ImGui::TextUnformatted(m_labelDiwne.c_str());
        if (m_isHeld) ImGui::TextUnformatted("Held");
        if (m_isDraged) ImGui::TextUnformatted("Dragged");
        if (m_selected) ImGui::TextUnformatted("Selected");
#endif // DIWNE_DEBUG
        end();
        updateSizes();
#ifdef DIWNE_DEBUG
        diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(255,0,0,50), 0, ImDrawCornerFlags_None, 3);
#endif // DIWNE_DEBUG
        m_inner_interaction_happen |= afterEndDiwne();
        m_inner_interaction_happen |= processInteractionsDiwne();
    }
    m_inner_interaction_happen |= finalizeDiwne();

    return m_inner_interaction_happen;
}

bool DiwneObject::initialize(){return false;}
bool DiwneObject::initializeDiwne(){m_focused=false; m_focusedForInteraction=false; return initialize();}

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

bool DiwneObject::allowInteraction(){return m_drawMode == DrawMode::Interacting && diwne.getDiwneAction() != DiwneAction::SelectionRectFull && diwne.getDiwneAction() != DiwneAction::SelectionRectTouch && (m_isHeld || (!m_inner_interaction_happen && (diwne.getDiwneAction() != DiwneAction::FocusOnObject || m_focusedForInteraction || m_focused) && bypassFocusForInteractionAction())); }

bool DiwneObject::processInteractions(){return false;}
bool DiwneObject::processInteractionsDiwne()
{
    bool interaction_happen = false;

    /* order is important */
    interaction_happen |= processObjectFocused();
    if (allowInteraction())
    {
        interaction_happen |= processObjectFocusedForInteraction();
        interaction_happen |= processRaisePopupDiwne();
        interaction_happen |= m_selected ? processObjectUnselect() : processObjectSelect();
        interaction_happen |= m_isHeld ? processObjectUnhold() : processObjectHold();
        if (m_isHeld){interaction_happen |= m_isHeld; /* holding (not only change in hold state) is interaction */ diwne.setDiwneAction(DiwneAction::HoldNode);}
        interaction_happen |= processObjectDrag();
        interaction_happen |= processInteractions();
#ifdef DIWNE_DEBUG
        if(m_focused) {diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(0,0,0,50), 0, ImDrawCornerFlags_None, 10);}
        if(m_focusedForInteraction) {diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, ImColor(0,0,0,255), 0, ImDrawCornerFlags_None, 10);}
#endif // DIWNE_DEBUG
    }
    interaction_happen |= processShowPopupDiwne();
    return interaction_happen;
}

bool DiwneObject::finalize(){return false;}
bool DiwneObject::finalizeDiwne(){return finalize();}

bool DiwneObject::bypassRaisePopupAction(){return diwne.bypassIsMouseReleased1();}
bool DiwneObject::bypassFocusAction(){return ImGui::IsItemHovered();} /* block interaction with other elements; you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassFocusForInteractionAction(){return ImGui::IsItemHovered();} /* you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassHoldAction(){return diwne.bypassIsMouseClicked0();}
bool DiwneObject::bypassUnholdAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassSelectAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassUnselectAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassDragAction(){return diwne.bypassIsMouseDragging0();};

bool DiwneObject::processFocused() {return true;}
bool DiwneObject::processObjectFocused()
{
    if (m_drawMode == DrawMode::Interacting && diwne.getDiwneAction() != DiwneAction::SelectionRectFull && diwne.getDiwneAction() != DiwneAction::SelectionRectTouch && !m_inner_interaction_happen && (diwne.getDiwneAction() != DiwneAction::FocusOnObject || m_focused) && bypassFocusAction())
    {
        m_focused = true;
        diwne.setDiwneAction(DiwneAction::FocusOnObject);
        return processFocused();
    }
    else{m_focused = false;}
    return false;
}

bool DiwneObject::processFocusedForInteraction() {return true;}
bool DiwneObject::processObjectFocusedForInteraction()
{
    if (bypassFocusForInteractionAction())
    {
        m_focusedForInteraction = true;
        diwne.setDiwneAction(DiwneAction::FocusOnObject);
        return processFocusedForInteraction();
    }
    else{m_focusedForInteraction = false;}
    return false;
}

bool DiwneObject::processHold() {return true;}
bool DiwneObject::processObjectHold()
{
    if (bypassHoldAction())
    {
        m_isHeld = true;
        return processHold();
    }
    return false;
}

bool DiwneObject::processUnhold() {return true;}
bool DiwneObject::processObjectUnhold()
{
    if (bypassUnholdAction())
    {
        m_isHeld = false;
        m_isDraged = false;
        return processUnhold();
    }
    return false;
}

bool DiwneObject::processDrag() {return true;}
bool DiwneObject::processObjectDrag()
{
    if (m_isHeld && bypassDragAction())
    {
        m_isDraged = true;
        return processDrag();
    }
    return false;
}

bool DiwneObject::processSelect() {return true;}
bool DiwneObject::processObjectSelect()
{
    if (!m_isDraged && bypassSelectAction())
    {
        m_selected = true;
        return processSelect();
    }
    return false;
}

bool DiwneObject::processUnselect() {return true;}
bool DiwneObject::processObjectUnselect()
{
    if (!m_isDraged && bypassUnselectAction())
    {
        m_selected = false;
        return processUnselect();
    }
    return false;
}

bool DiwneObject::processRaisePopupDiwne()
{
    if(bypassRaisePopupAction() && !diwne.blockPopup())
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
