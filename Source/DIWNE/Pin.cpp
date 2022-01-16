#include "diwne_include.h"

namespace DIWNE
{

Pin::Pin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne/*="DiwnePin"*/)
    : DiwneObject(diwne, id, labelDiwne)
    , m_pinRectDiwne(ImRect(0,0,1,1)) /* can not have zero size because of InvisibleButton */
    , m_connectionPointDiwne(ImVec2(0,0))
{ }

void Pin::begin()
{
    ImGui::PushID(m_labelDiwne.c_str());
    ImGui::BeginGroup();
}

void Pin::end()
{
    ImGui::EndGroup();
    ImGui::PopID();
}

bool Pin::afterEndDiwne()
{
    bool interaction_happen = false;
    m_pinRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );
    updateConnectionPointDiwne();

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(255,150,150), 0, ImDrawCornerFlags_None, 5);
    if (!m_interactionAllowed && bypassHoveredAction()) {diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);}
#endif // DIWNE_DEBUG

    if (m_interactionAllowed) interaction_happen |= processPin_Pre_ConnectLink();
    interaction_happen |= DiwneObject::afterEndDiwne();
    return interaction_happen;
}

bool Pin::bypassPinLinkConnectionPreparedAction()
{
    return !m_isHeld && (diwne.getDiwneAction() == DiwneAction::NewLink || diwne.getDiwneActionPreviousFrame() == DiwneAction::NewLink) && bypassHoveredAction() && diwne.getLastActivePin<DIWNE::Pin>().get() != this;
}

bool Pin::processDrag()
{
    diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
    diwne.setLastActivePin(std::static_pointer_cast<DIWNE::Pin>(shared_from_this()));
    return true;
}

bool Pin::processConnectionPrepared() {ImGui::TextUnformatted("Prepared for connecting link"); return true;}

bool Pin::processPin_Pre_ConnectLink()
{
    if (bypassPinLinkConnectionPreparedAction())
    {
        return processConnectionPrepared();
    }
    return false;
}

bool Pin::processHovered()
{
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2*diwne.getWorkAreaZoom());
    return false;
}


} /* namespace DIWNE */
