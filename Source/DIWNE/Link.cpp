#include "diwne_include.h"

namespace DIWNE
{
/* \todo JH constants from settings */
Link::Link(DIWNE::ID id)
    : m_idDiwne(id)
    , m_thickness(10)
    , m_color(ImColor(150,75,100,150))
    , m_selectedColor(ImColor(150,75,100,255))
    , m_startDiwne(ImVec2(0,0))
    , m_endDiwne(ImVec2(0,0))
    , m_startControlOffsetDiwne(ImVec2(10,0)) /* \todo JH minimal control points offset to settings */
    , m_endControlOffsetDiwne(ImVec2(-10,0))
    , m_popupID(fmt::format("linkPopup{}",id))
    , m_translated(false)
    , m_just_pluged(false)
{}

Link::~Link()
{
//dtor
}

void Link::updateSquareDistanceMouseFromLink(DIWNE::Diwne &diwne)
{
    ImVec2 mousePosDiwne = diwne.screen2diwne( diwne.bypassGetMousePos() );
    ImVec2 closestPointOnLink = ImBezierCubicClosestPointCasteljau(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne
                                             , mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
    ImVec2 diff = (closestPointOnLink - mousePosDiwne)/diwne.getWorkAreaZoomDiwne();
    m_squaredDistanceMouseFromLink = diff.x*diff.x + diff.y*diff.y;
}

bool Link::isLinkOnWorkArea(DIWNE::Diwne &diwne)
{
    return diwne.getWorkAreaDiwne().Overlaps(ImRect(
        std::min(std::min(m_controlPointStartDiwne.x, m_startDiwne.x), std::min(m_controlPointEndDiwne.x, m_endDiwne.x)),
        std::min(std::min(m_controlPointStartDiwne.y, m_startDiwne.y), std::min(m_controlPointEndDiwne.y, m_endDiwne.y)),
        std::max(std::max(m_controlPointStartDiwne.x, m_startDiwne.x), std::max(m_controlPointEndDiwne.x, m_endDiwne.x)),
        std::max(std::max(m_controlPointStartDiwne.y, m_startDiwne.y), std::max(m_controlPointEndDiwne.y, m_endDiwne.y)))
                                             );
}

bool Link::drawLinkDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false, interaction_happen = false;

    updateEndpoints();
    updateControlPointsOffsets();
    updateControlPoints();
    updateSquareDistanceMouseFromLink(diwne);

    inner_interaction_happen = linkContent(diwne);

    interaction_happen = processLink(diwne, inner_interaction_happen);

    m_just_pluged = false;
    return interaction_happen || inner_interaction_happen;
}

bool Link::bypassLinkHoveredAction(DIWNE::Diwne &diwne) {return m_squaredDistanceMouseFromLink < (m_thickness*m_thickness);}
bool Link::bypassLinkSelectAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(0) && bypassLinkHoveredAction(diwne);}
bool Link::bypassLinkUnselectAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(0) && bypassLinkHoveredAction(diwne);}
bool Link::bypassLinkHoldAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseClicked(0) && bypassLinkHoveredAction(diwne);}
bool Link::bypassLinkUnholdAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(0);}
bool Link::bypassLinkRaisePopupAction(DIWNE::Diwne &diwne) {return ImGui::IsMouseReleased(1) && bypassLinkHoveredAction(diwne);}

bool Link::processLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    bool interaction_happen = false;
    interaction_happen |= processLinkHovered(diwne, inner_interaction_happen);

    interaction_happen |= m_selected ? processLinkUnselected(diwne, inner_interaction_happen) : processLinkSelected(diwne, inner_interaction_happen);

    interaction_happen |= processLinkPopupDiwne(diwne, inner_interaction_happen);

    return interaction_happen;
}

bool Link::processLinkHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (bypassLinkHoveredAction(diwne))
    {
        m_color.Value.w = 1; /* \todo JH alpha to settings? */
        return true;
    }else
    {
        m_color.Value.w = 0.5; /* \todo JH alpha to settings? */
    }
    return false;
}

bool Link::processLinkSelected(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!m_selected && !m_translated && !m_just_pluged && !inner_interaction_happen && bypassLinkSelectAction(diwne))
    {
        m_selected = true;
        return true;
    }
    return false;
}

bool Link::processLinkUnselected(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (m_selected && !m_translated && !m_just_pluged && !inner_interaction_happen && bypassLinkUnselectAction(diwne))
    {
        m_selected = false;
        return true;
    }
    return false;
}

bool Link::linkContent(DIWNE::Diwne &diwne)
{
    if (m_selected){diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, m_selectedColor, m_thickness+4 ); }/* \todo color of selected and thicknes of selected mark to settings */
    diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, m_color, m_thickness);
    return false;
}

bool Link::processLinkPopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if(!inner_interaction_happen && bypassLinkRaisePopupAction(diwne))
    {
        ImGui::OpenPopup(m_popupID.c_str());
    }
    return diwne.popupDiwneItem(m_popupID, &expandPopupBackgroundContent, *this );
}

void Link::linkPopupContent()
{
    if (ImGui::MenuItem("Override this method with content of your link popup menu")) {}
}

} /* namespace DIWNE */
