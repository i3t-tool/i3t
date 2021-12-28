#include "diwne_include.h"

namespace DIWNE
{

Pin::Pin(DIWNE::ID id)
    : m_idDiwne(id)
    , m_pinRectDiwne(ImRect(0,0,1,1)) /* can not have zero size because of InvisibleButton */
    , m_connectionPointDiwne(ImVec2(0,0))
    , m_isHeld(false)
    , m_popupID(fmt::format("pinPopup{}", id))
{ }

Pin::~Pin()
{
    //dtor
}

bool Pin::drawPinDiwne(DIWNE::Diwne &diwne)
{
    bool interaction_happen = false;
    bool inner_interaction_happen = false;


    ImGui::PushID(fmt::format("Pin{}", m_idDiwne).c_str());
    ImGui::BeginGroup();
        inner_interaction_happen |= pinContent(diwne);
    ImGui::EndGroup();
    ImGui::PopID();

    m_pinRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    updateConnectionPointDiwne(diwne);

    interaction_happen |= processPin(diwne, inner_interaction_happen);

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(255,150,150), 0, ImDrawCornerFlags_None, 5);
    if (!inner_interaction_happen && ImGui::IsItemHovered()) {diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);}
#endif // DIWNE_DEBUG


    return inner_interaction_happen || interaction_happen;
}

bool Pin::bypassPinHoveredAction(DIWNE::Diwne &diwne) {return ImGui::IsItemHovered();}
bool Pin::bypassPinHoldAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseClicked(0) && bypassPinHoveredAction(diwne);}
bool Pin::bypassPinUnholdAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(0);}
bool Pin::bypassPinRaisePopupAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(1) && bypassPinHoveredAction(diwne);}
bool Pin::bypassPinPreconnectLinkAction(DIWNE::Diwne &diwne)
{
    return (diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink || diwne.getPreviousFrameDiwneAction() == DIWNE::DiwneAction::NewLink) && bypassPinHoveredAction(diwne) && diwne.getLastActivePin<DIWNE::Pin>() != this;
}

bool Pin::processPinNewLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (m_isHeld && diwne.bypassIsMouseDragging0())
    {
        diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
        return true;
    }
    return false;
}

bool Pin::processPin(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    bool interaction_happen = false;

    interaction_happen |= processPinHovered(diwne, inner_interaction_happen);

    interaction_happen |= m_isHeld ? processPinUnhold(diwne, inner_interaction_happen) : processPinHold(diwne, inner_interaction_happen);
    interaction_happen |= m_isHeld;

    interaction_happen |= m_isHeld ? processPinNewLink(diwne, inner_interaction_happen) : processPinConnectLink(diwne, inner_interaction_happen);

    interaction_happen |= processPinPopupDiwne(diwne, inner_interaction_happen);

    return interaction_happen;
}

bool Pin::processPinConnectLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!m_isHeld && bypassPinPreconnectLinkAction(diwne))
    {
        /* here it is when goal pin is hovered */
        return true;
    }
    return false;
}

bool Pin::processPinHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if(bypassPinHoveredAction(diwne))
    {
        diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
    }
    return false;
}

bool Pin::processPinHold(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!inner_interaction_happen && bypassPinHoldAction(diwne))
    {
        m_isHeld = true;
        return true;
    }
    return false;
}

bool Pin::processPinUnhold(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!inner_interaction_happen && bypassPinUnholdAction(diwne))
    {
        if(diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink && diwne.getLastActivePin<DIWNE::Pin>() == this) /* link was drawn from this pin and now is not */
        {
            diwne.setDiwneAction(DIWNE::DiwneAction::None);
        }
        m_isHeld = false;
        return true;
    }
    return false;
}

bool Pin::processPinPopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if(!inner_interaction_happen && bypassPinRaisePopupAction(diwne))
    {
        ImGui::OpenPopup(m_popupID.c_str());
    }
    return diwne.popupDiwneItem(m_popupID, &expandPopupBackgroundContent, *this );
}

void Pin::pinPopupContent()
{
    if (ImGui::MenuItem("Override this method with content of your pin popup menu")) {}
}

} /* namespace DIWNE */
