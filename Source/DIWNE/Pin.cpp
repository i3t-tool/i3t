#include "diwne_include.h"

namespace DIWNE
{

Pin::Pin(DIWNE::ID id)
    : m_idDiwne(id)
    , m_pinRectDiwne(ImRect(0,0,1,1)) /* can not have zero size because of InvisibleButton */
    , m_isHeld(false)
{ }

Pin::~Pin()
{
    //dtor
}

bool Pin::drawPinDiwne(DIWNE::Diwne &diwne)
{
    bool interaction_happen = false;
    bool inner_interaction_happen = false;
#ifdef DIWNE_DEBUG
    /* debug - whole pin */
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(255,150,150), 0, ImDrawCornerFlags_None, 5);
#endif // DIWNE_DEBUG

    ImGui::PushID(fmt::format("Pin{}", m_idDiwne).c_str());
    ImGui::BeginGroup();
        inner_interaction_happen |= pinContent(diwne);
    ImGui::EndGroup();
    ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug */
    if (!inner_interaction_happen && ImGui::IsItemHovered())
    {
        diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
    }
#endif // DIWNE_DEBUG

    if (!inner_interaction_happen && diwne.bypassItemClicked0())
    {
        m_isHeld = true;
    }

    if (m_isHeld)
    {
        interaction_happen = true;
        if (diwne.bypassIsMouseDragging0())
        {
            pinActiveProcess(diwne);
        }else if (diwne.bypassIsMouseReleased0())
        {
            if(diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink && diwne.getLastActivePin<DIWNE::Pin>() == this) /* link was drawn from this pin and now is not */
            {
                diwne.setDiwneAction(DIWNE::DiwneAction::None);
            }
            m_isHeld = false;
        }
    }
    else /* pin is not held */
    {
        if (pinConnectLinkCheck(diwne))
        {
            pinConnectLinkProcess(diwne);
        }
    }
    m_pinRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen || interaction_happen;
}

bool Pin::pinActiveCheck(DIWNE::Diwne &diwne)
{
    return diwne.bypassIsItemActive();
}

void Pin::pinActiveProcess(DIWNE::Diwne &diwne)
{
    diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
}

bool Pin::pinConnectLinkCheck(DIWNE::Diwne &diwne)
{
    return (diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink || diwne.getPreviousFrameDiwneAction() == DIWNE::DiwneAction::NewLink) && diwne.bypassIsItemHoovered() && diwne.getLastActivePin<DIWNE::Pin>() != this;
}

} /* namespace DIWNE */
