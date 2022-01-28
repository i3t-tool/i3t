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
    return diwne.getWorkAreaDiwne().Overlaps(getRectDiwne());
}

bool Link::initialize()
{
     diwne.mp_settingsDiwne->linkColor.Value.w = m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
     diwne.mp_settingsDiwne->linkColorSelected.Value.w = m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered : diwne.mp_settingsDiwne->linkAlphaSelected;

     return false;
}
bool Link::initializeDiwne()
{
    updateEndpoints();
    updateControlPoints();
    updateSquareDistanceMouseFromLink();

    return initialize();
}

bool Link::bypassFocusAction() {return m_squaredDistanceMouseFromLink < (diwne.mp_settingsDiwne->linkThicknessDiwne*diwne.mp_settingsDiwne->linkThicknessDiwne);}
bool Link::bypassFocusForInteractionAction() {return bypassFocusAction();}

bool Link::processObjectFocused()
{
    if (diwne.getDiwneAction() != DiwneAction::FocusOnObject && diwne.getDiwneAction() != DiwneAction::NewLink && bypassFocusAction())
    {
        m_focused = true;
        diwne.setDiwneAction(DiwneAction::FocusOnObject);
        return processFocused();
    }
    else{m_focused = false;}
    return false;
}

bool Link::processObjectFocusedForInteraction()
{
    if (diwne.getDiwneAction() != DiwneAction::FocusOnObject && diwne.getDiwneAction() != DiwneAction::NewLink && bypassFocusAction())
    {
        m_focused = true;
        diwne.setDiwneAction(DiwneAction::FocusOnObject);
        return processFocused();
    }
    else{m_focused = false;}
    return false;
}

bool Link::content()
{
    if (m_selected){diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, diwne.mp_settingsDiwne->linkColorSelected, diwne.mp_settingsDiwne->linkThicknessDiwne+diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne ); }
    diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne, diwne.mp_settingsDiwne->linkColor, diwne.mp_settingsDiwne->linkThicknessDiwne);
    return false;
}

} /* namespace DIWNE */
