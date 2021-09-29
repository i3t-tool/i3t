#include "diwne_include.h"

namespace DIWNE
{

Pin::Pin(DIWNE::ID id)
    : m_idDiwne(id)
    , m_pinRectDiwne(ImRect(0,0,1,1)) /* can not have zero size because of InvisibleButton */
{ }

Pin::~Pin()
{
    //dtor
}

bool Pin::drawPinDiwne(DIWNE::Diwne &diwne)
{
    bool interaction_happen = false;
    bool inner_interaction_happen = false;

    putInvisibleButtonUnder(fmt::format("PinIB{}", m_idDiwne).c_str(), m_pinRectDiwne.GetSize() );

    if (pinActiveCheck(diwne))
    {
        pinActiveProcess(diwne);
    }
    else /* pin is not active */
    {
        if(diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink && diwne.getLastActivePin<DIWNE::Pin>() == this) /* link was drawn from this pin and now is not */
        {
            diwne.setDiwneAction(DIWNE::DiwneAction::None);
        }
    }

    if (pinConnectLinkCheck(diwne))
    {
        pinConnectLinkProcess(diwne);
    }


    interaction_happen = ImGui::IsItemHovered(); /* debug interaction move to function */

    /* debug - whole pin */
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(255,150,150), 0, ImDrawCornerFlags_None, 5);

		ImGui::PushID(fmt::format("Pin{}", m_idDiwne).c_str());
    ImGui::BeginGroup();
        inner_interaction_happen |= pinContent(diwne);

            /* debug */
        if (!inner_interaction_happen && interaction_happen)
        {
                /* debug - whole pin */
                diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
        }
    ImGui::EndGroup();
		ImGui::PopID();
    m_pinRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );





    return inner_interaction_happen || interaction_happen;
}

bool Pin::pinActiveCheck(DIWNE::Diwne &diwne)
{
    return ImGui::IsItemActive();
}

void Pin::pinActiveProcess(DIWNE::Diwne &diwne)
{
    diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
}

bool Pin::pinConnectLinkCheck(DIWNE::Diwne &diwne)
{
    return (diwne.getDiwneAction() == DIWNE::DiwneAction::NewLink || diwne.getPreviousFrameDiwneAction() == DIWNE::DiwneAction::NewLink) && ImGui::IsItemHovered() && diwne.getLastActivePin<DIWNE::Pin>() != this;
}





} /* namespace DIWNE */
