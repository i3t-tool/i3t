#include "diwne_include.h"

namespace DIWNE
{
/* \todo JH constants from settings */
Link::Link(DIWNE::ID id) : m_idDiwne(id), m_thickness(10)
{
    m_startDiwne = ImVec2(0,0);
    m_endDiwne = ImVec2(0,0);
    m_startControlOffsetDiwne = ImVec2(10,0); /* \todo JH minimal control points offset to settings */
    m_endControlOffsetDiwne = ImVec2(-10,0);
}

Link::~Link()
{
//dtor
}


bool Link::drawLinkDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false, interaction_happen = false;
    int alpha = 150; /* \todo JH to settings? */
    std::string const popupID = fmt::format("linkPopup{}",m_idDiwne);
    updateEndpoints();
    updateControlPointsOffsets();

    ImVec2 controlPointStart = m_startDiwne + m_startControlOffsetDiwne;
    ImVec2 controlPointEnd = m_endDiwne + m_endControlOffsetDiwne;

    ImVec2 mousePosDiwne = diwne.screen2diwne( ImGui::GetIO().MousePos );
    ImVec2 closestPointOnLink = ImBezierCubicClosestPointCasteljau(m_startDiwne, controlPointStart, controlPointEnd, m_endDiwne
                                             , mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
    ImVec2 diff = closestPointOnLink - mousePosDiwne;
    float diffLenghtSquare = diff.x*diff.x + diff.y*diff.y;
    if (diffLenghtSquare < interactionWidth*interactionWidth + m_thickness) /* hoovered */ /* \todo JH WorkAreaZoom */
    {
        alpha = 255; /* \todo JH to settings? */
        interaction_happen = true;
        if (ImGui::IsMouseReleased(0)) {getSelected() ? setSelected(false) : setSelected(true);}
        if (ImGui::IsMouseReleased(1)) {
                ImGui::OpenPopup(popupID.c_str()); /* link is not ImGui item - so we can not use ImGui::BeginPopupContextItem */
        }
    }

    linkPopupDiwne(diwne, popupID);
    m_color = ImColor(100,50,255,alpha); /* \todo JH color from setting */

    if (getSelected()) diwne.AddBezierCurveDiwne(m_startDiwne, controlPointStart, controlPointEnd, m_endDiwne, ImColor(150,150,0), m_thickness+4); /* \todo color of selected and thicknes of selected mark to settings */
    diwne.AddBezierCurveDiwne(m_startDiwne, controlPointStart, controlPointEnd, m_endDiwne, m_color, m_thickness);
    inner_interaction_happen = linkContent(diwne);
    return interaction_happen || inner_interaction_happen;
}

/* \todo JH make some diwne super element as ancestor of all elements and override just "elementPopupContent" */
/* ImGui::OpenPopup(popupIDstring.c_str()) has to be called before this function when popup should be shown */
bool Link::linkPopupDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring)
{
    bool interaction_happen = false;

    if (ImGui::IsPopupOpen(popupIDstring.c_str()))
	{
	    ImGui::SetNextWindowPos(diwne.getPopupPosition());
	    if (ImGui::BeginPopup(popupIDstring.c_str()) )
        {
            interaction_happen = true;
            /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not (zoomed) diwne */
            diwne.transformMouseFromDiwneToImGui();

#ifdef DIWNE_DEBUG
            /* debug */
            ImGui::Text("Diwne ID: %i", m_idDiwne);
            ImGui::Separator();
#endif // DIWNE_DEBUG

            linkPopupContent();

            ImGui::EndPopup();
        }
	}
	return interaction_happen;
}

void Link::linkPopupContent()
{
    if (ImGui::MenuItem("Override this method with content of your link popup menu")) {}
}

} /* namespace DIWNE */
