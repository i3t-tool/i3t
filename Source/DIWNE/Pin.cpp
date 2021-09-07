#include "Pin.h"

namespace DIWNE
{

Pin::Pin(DIWNE::ID id)
    : m_idDiwne(id)
    , m_linkDiwne(id)
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

    putInvisibleButtonUnder(fmt::format("Pin{}", m_idDiwne).c_str(), m_pinRectDiwne.GetSize() );
    if (ImGui::IsItemActive())
    {
        diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
        ImVec2 origin = diwne.screen2workArea( ImGui::GetIO().MouseClickedPos[0]);
        ImVec2 actual = diwne.screen2workArea( ImGui::GetIO().MousePos);

        diwne.getHelperLinkPointer()->setLinkEndpointsDiwne(origin, actual);
        diwne.getHelperLinkPointer()->drawLinkDiwne(diwne);
        //diwne.AddBezierCurveScreen(origin, ImVec2(origin.x-200, origin.y), ImVec2(actual.x+200, actual.y), actual, ImGui::GetColorU32(ImGuiCol_Button), 10.0f); // Draw a line between the button and the mouse cursor
    }




    /* debug - whole pin */
    diwne.AddRectDiwne(m_pinRectDiwne.Min, m_pinRectDiwne.Max, ImColor(255,150,150), 0, ImDrawCornerFlags_None, 5);

    ImGui::BeginHorizontal(fmt::format("pin{}", m_idDiwne).c_str());
        inner_interaction_happen |= pinContent(diwne);
    ImGui::EndHorizontal();
    m_pinRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
    m_pinRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );

    interaction_happen = ImGui::IsItemHovered();

    /* debug */
    if (!inner_interaction_happen && interaction_happen)
    {
            ImGui::Text("Pin is hoovered");
    }

    return inner_interaction_happen || interaction_happen;
}

} /* namespace DIWNE */
