#include "diwne_include.h"

namespace DIWNE
{
/* \todo JH constants from settings */
Link::Link(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne/*="DiwneLink"*/)
    : DiwneObject(diwne, id, labelDiwne)
    , m_startDiwne(ImVec2(0,0)) /* only initialize value - see initializeDiwne() */
    , m_endDiwne(ImVec2(0,0))
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

void Link::updateControlPoints()
{
    m_controlPointStartDiwne=m_startDiwne+diwne.mp_settingsDiwne->linkStartControlOffsetDiwne;
    m_controlPointEndDiwne=m_endDiwne+diwne.mp_settingsDiwne->linkEndControlOffsetDiwne;
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
     diwne.mp_settingsDiwne->linkColor.Value.w = m_hovered ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
     diwne.mp_settingsDiwne->linkColorSelected.Value.w = m_hovered ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered : diwne.mp_settingsDiwne->linkAlphaSelected;

     return false;
}
bool Link::initializeDiwne()
{
    updateEndpoints();
    updateControlPoints();
    updateSquareDistanceMouseFromLink();

    return initialize();
}

bool Link::bypassHoveredAction() {return m_squaredDistanceMouseFromLink < (diwne.mp_settingsDiwne->linkThicknessDiwne*diwne.mp_settingsDiwne->linkThicknessDiwne);}

bool Link::content()
{
    if (m_selected){diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, diwne.mp_settingsDiwne->linkColorSelected, diwne.mp_settingsDiwne->linkThicknessDiwne+diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne ); }
    diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, diwne.mp_settingsDiwne->linkColor, diwne.mp_settingsDiwne->linkThicknessDiwne);
    return false;
}

} /* namespace DIWNE */
