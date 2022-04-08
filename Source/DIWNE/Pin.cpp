#include "diwne_include.h"

namespace DIWNE
{

Pin::Pin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne/*="DiwnePin"*/)
    : DiwneObject(diwne, id, labelDiwne)
    , m_pinRectDiwne(ImRect(0,0,0,0))
    , m_connectionPointDiwne(ImVec2(0,0))
{
    m_selectable = false;
}

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

void Pin::updateSizes()
{
    m_pinRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    updateConnectionPointDiwne();
}

bool Pin::processInteractionsAlways()
{
    bool interaction_happen = false;

    interaction_happen |= processPin_Pre_ConnectLinkDiwne();
    interaction_happen |= DiwneObject::processInteractionsAlways(); /* Selection rectangle block showing popup etc. */


    return interaction_happen;
}

bool Pin::bypassPinLinkConnectionPreparedAction()
{
    return bypassFocusForInteractionAction();
}

bool Pin::allowProcessPin_Pre_ConnectLink()
{
    return m_focusedForInteraction && !m_isHeld && (diwne.getDiwneAction() == DiwneAction::NewLink || diwne.getDiwneActionPreviousFrame() == DiwneAction::NewLink) /*&& diwne.getLastActivePin<DIWNE::Pin>().get() != this*/;
}

bool Pin::processPin_Pre_ConnectLinkDiwne()
{
    if (bypassPinLinkConnectionPreparedAction() && allowProcessPin_Pre_ConnectLink() )
    {
        return processConnectionPrepared();
    }
    return false;
}

bool Pin::processConnectionPrepared() {ImGui::TextUnformatted("Prepared for connecting link"); return true;}

bool Pin::processDrag()
{
    diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
    diwne.setLastActivePin(std::static_pointer_cast<DIWNE::Pin>(shared_from_this()));
    return true;
}

} /* namespace DIWNE */
