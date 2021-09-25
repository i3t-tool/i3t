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
        updateEndpoints();
        updateControlPointsOffsets();

        ImVec2 controlPointStart = m_startDiwne + m_startControlOffsetDiwne;
        ImVec2 controlPointEnd = m_endDiwne + m_endControlOffsetDiwne;

        ImVec2 mousePosDiwne = diwne.screen2diwne_noZoom( ImGui::GetIO().MousePos );
        ImVec2 closestPointOnLink = ImBezierClosestPointCasteljau(m_startDiwne, controlPointStart, controlPointEnd, m_endDiwne
                                                 , mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
        ImVec2 diff = closestPointOnLink - mousePosDiwne;
        float diffLenghtSquare = diff.x*diff.x + diff.y*diff.y;
        if (diffLenghtSquare < interactionWidth*interactionWidth + m_thickness)
        {
            m_color = ImColor(100,50,255,255); /* \todo JH color and hoovered/selected color to setting*/
            interaction_happen = true;
        }
        else
        {
            m_color = ImColor(100,50,255,100);
        }

        diwne.AddBezierCurveDiwne(m_startDiwne, controlPointStart, controlPointEnd, m_endDiwne, m_color, m_thickness);
        inner_interaction_happen = linkContent(diwne);
        return interaction_happen || inner_interaction_happen;
    }

} /* namespace DIWNE */
