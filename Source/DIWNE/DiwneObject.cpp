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
    ,   m_interactionAllowed(true)
    ,   m_inner_interaction_happen(false)
    ,   m_isHeld(false)
    ,   m_isDraged(false)
    ,   m_selected(false)
    ,   m_drawing(true)
    ,   m_drawWithInteraction(true)
{}

bool DiwneObject::drawDiwne(bool with_interaction/*=true*/)
{
    m_inner_interaction_happen = false;
    m_drawWithInteraction = with_interaction;
    m_inner_interaction_happen |= initializeDiwne();
    if (m_drawing)
    {
        m_inner_interaction_happen |= beforeBeginDiwne();
        begin();

        m_inner_interaction_happen |= beforeContentDiwne();
        m_inner_interaction_happen |= contentDiwne();
        m_inner_interaction_happen |= afterContentDiwne();

#ifdef DIWNE_DEBUG
    m_drawWithInteraction ? ImGui::Text("With    interaction") : ImGui::Text("Without interation");
    m_interactionAllowed ? ImGui::Text("Interaction allowed") : ImGui::Text("Interaction not allowed");
    ImGui::TextUnformatted(m_labelDiwne.c_str());
    if (m_isHeld) ImGui::TextUnformatted("Held");
    if (m_isDraged) ImGui::TextUnformatted("Dragged");
    if (m_selected) ImGui::TextUnformatted("Selected");
#endif // DIWNE_DEBUG
        end();
        if(m_drawWithInteraction) m_inner_interaction_happen |= afterEndDiwne();
    }
    m_inner_interaction_happen |= finalizeDiwne();

    return m_inner_interaction_happen;
}

/* no Diwne initialization */
bool DiwneObject::initialize(){return false;}
bool DiwneObject::initializeDiwne(){return initialize();}

bool DiwneObject::beforeBegin(){return false;}
bool DiwneObject::beforeBeginDiwne(){return beforeBegin();}

bool DiwneObject::beforeContent(){return false;};
bool DiwneObject::beforeContentDiwne(){return beforeContent();}

void DiwneObject::updateSizes(){}

bool DiwneObject::content(){ImGui::TextUnformatted(fmt::format("{} object content", m_labelDiwne).c_str()); return false;}
bool DiwneObject::contentDiwne(){bool interaction_happen = content(); updateSizes(); return interaction_happen;}


bool DiwneObject::afterContent(){return false;}
bool DiwneObject::afterContentDiwne(){return afterContent();}

void DiwneObject::allowInteraction(){m_interactionAllowed = !m_inner_interaction_happen; }
bool DiwneObject::afterEnd(){return false;}
bool DiwneObject::afterEndDiwne()
{
    bool interaction_happen = false;
    allowInteraction();
    if (m_interactionAllowed)
    {
        /* order is important */
        interaction_happen |= processObjectHovered();
        interaction_happen |= processRaisePopupDiwne();
        interaction_happen |= m_selected ? processObjectUnselect() : processObjectSelect();
        interaction_happen |= m_isHeld ? processObjectUnhold() : processObjectHold();
        interaction_happen |= m_isHeld; /* holding (not only change in hold state) is interaction */
        interaction_happen |= processObjectDrag();
        interaction_happen |= afterEnd();
    }
    interaction_happen |= processShowPopupDiwne();
    return interaction_happen;
}

bool DiwneObject::finalize(){return false;}
bool DiwneObject::finalizeDiwne(){return finalize();}

bool DiwneObject::bypassRaisePopupAction(){return diwne.bypassIsMouseReleased1() && bypassHoveredAction();}
bool DiwneObject::bypassHoveredAction(){return ImGui::IsItemHovered();} /* you have to override this if your object is not ImGui item (like Link) */
bool DiwneObject::bypassHoldAction(){return diwne.bypassIsMouseClicked0() && bypassHoveredAction();}
bool DiwneObject::bypassUnholdAction(){return diwne.bypassIsMouseReleased0();}
bool DiwneObject::bypassSelectAction(){return diwne.bypassIsMouseReleased0() && bypassHoveredAction();}
bool DiwneObject::bypassUnselectAction(){return diwne.bypassIsMouseReleased0() && bypassHoveredAction();}
bool DiwneObject::bypassDragAction(){return diwne.bypassIsMouseDragging0();};

bool DiwneObject::processHovered() {return false; /* Hovering usually do not block other interaction */}
bool DiwneObject::processObjectHovered()
{
    if (bypassHoveredAction())
    {
        m_hovered = true;
        return processHovered();
    }
    else{m_hovered = false;}
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
    if(bypassRaisePopupAction())
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
    ImGui::BeginTooltip();
        ImGui::TextColored(color, label.c_str());
    ImGui::EndTooltip();
}

} /* namespace DIWNE */
