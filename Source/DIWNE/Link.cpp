#include "diwne_include.h"

namespace DIWNE
{
/* \todo JH constants from settings */
Link::Link(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne/*="DiwneLink"*/)
    : DiwneObject(diwne, id, labelDiwne)
    , m_thickness(10)
    , m_thickness_selected_border(4)
    , m_color(ImColor(150,75,100,150))
    , m_selectedColor(ImColor(150,75,100,255))
    , m_startDiwne(ImVec2(0,0))
    , m_endDiwne(ImVec2(0,0))
    , m_startControlOffsetDiwne(ImVec2(10,0)) /* \todo JH minimal control points offset to settings */
    , m_endControlOffsetDiwne(ImVec2(-10,0))
    , m_just_pluged(false)
{}

void Link::updateSquareDistanceMouseFromLink()
{
    ImVec2 mousePosDiwne = diwne.screen2diwne( diwne.bypassGetMousePos() );
    ImVec2 closestPointOnLink = ImBezierCubicClosestPointCasteljau(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne
                                             , mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
    ImVec2 diff = (closestPointOnLink - mousePosDiwne)/diwne.getWorkAreaZoom();
    m_squaredDistanceMouseFromLink = diff.x*diff.x + diff.y*diff.y;
}

bool Link::isLinkOnWorkArea()
{
    return diwne.getWorkAreaDiwne().Overlaps(ImRect(
        std::min(std::min(m_controlPointStartDiwne.x, m_startDiwne.x), std::min(m_controlPointEndDiwne.x, m_endDiwne.x)),
        std::min(std::min(m_controlPointStartDiwne.y, m_startDiwne.y), std::min(m_controlPointEndDiwne.y, m_endDiwne.y)),
        std::max(std::max(m_controlPointStartDiwne.x, m_startDiwne.x), std::max(m_controlPointEndDiwne.x, m_endDiwne.x)),
        std::max(std::max(m_controlPointStartDiwne.y, m_startDiwne.y), std::max(m_controlPointEndDiwne.y, m_endDiwne.y)))
                                             );
}

bool Link::initialize()
{
     m_color.Value.w = m_hovered ? 1 : 0.5; /* \todo JH alpha to settings? */
     return false;
}
bool Link::initializeDiwne()
{
    updateEndpoints();
    updateControlPointsOffsets();
    updateControlPoints();
    updateSquareDistanceMouseFromLink();

    return initialize();
}

bool Link::bypassHoveredAction() {return m_squaredDistanceMouseFromLink < (m_thickness*m_thickness);}

bool Link::content()
{
    if (m_selected){diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, m_selectedColor, m_thickness+m_thickness_selected_border ); }
    diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, m_color, m_thickness);
    return false;
}

} /* namespace DIWNE */
