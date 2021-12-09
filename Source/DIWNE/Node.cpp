#include "Node.h"

namespace DIWNE
{


Node::Node(DIWNE::ID id) /* FLT_MAX for first frame - draw node anywhere it is and thus got its real size*/
    : m_topRectDiwne(ImRect(0,0,0,0))
    , m_leftRectDiwne(ImRect(0,0,0,0))
    , m_middleRectDiwne(ImRect(0,0,0,0))
    , m_rightRectDiwne(ImRect(0,0,0,0))
    , m_bottomRectDiwne(ImRect(0,0,0,0))
    , m_centerDummySpace(0)
    , m_middleAlign(0.5)
    , m_idDiwne(id)
    , m_selected(false)
    , m_translated(false)
    , m_isHeld(false)
    , m_firstDraw(true)
    , m_nodeInteractionAllowed(false)
    , m_popupID(fmt::format("nodePopup{}", id))
{}

Node::~Node()
{}


Node& Node::operator=(const Node& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Node::drawNodeDiwne(DIWNE::Diwne &diwne, bool drawHere/*= false*/)
{
    bool inner_interaction_happen = false, interaction_happen;

    if (drawHere)
    {
        setNodePositionDiwne(diwne.screen2diwne(ImGui::GetCursorScreenPos()));
    }

    if ( m_firstDraw || getNodeRectDiwne().Overlaps( diwne.getWorkAreaDiwne() ) )
    {
        if(m_firstDraw) m_firstDraw=false;
        /* Set cursor to position of node */
        if (!drawHere) ImGui::SetCursorScreenPos(diwne.diwne2screen(m_nodePositionDiwne));

        ImGui::PushID(fmt::format("Node{}", m_idDiwne).c_str());
        ImGui::BeginGroup(); /* Begin of node */

            inner_interaction_happen |= processInNodeBeforeContent(diwne);

            inner_interaction_happen |= drawTopDiwne(diwne);

						ImGui::PushID("LeftMiddleRight");
            ImGui::BeginGroup();
                inner_interaction_happen |= drawLeftDiwne(diwne);
                   ImGui::SameLine();
                   if(m_centerDummySpace > 0){ ImGui::Indent((m_leftRectDiwne.GetWidth() + m_centerDummySpace*m_middleAlign)*diwne.getWorkAreaZoomDiwne() + ImGui::GetStyle().ItemSpacing.x ); } /* spacing is already zoomed in BeginDiwne */
                inner_interaction_happen |= drawMiddleDiwne(diwne);
                    ImGui::SameLine();
                    if(m_centerDummySpace > 0){ ImGui::Indent((m_middleRectDiwne.GetWidth() + m_centerDummySpace*(1-m_middleAlign))*diwne.getWorkAreaZoomDiwne() + ImGui::GetStyle().ItemSpacing.x ); }
                inner_interaction_happen |= drawRightDiwne(diwne);
            ImGui::EndGroup(); /* Left-Middle-Right */
						ImGui::PopID();

            inner_interaction_happen |= drawBottomDiwne(diwne);

            updateSizeRectangles(diwne);

            inner_interaction_happen |= processInNodeAfterContent(diwne);
        ImGui::EndGroup(); /* End of node */
        ImGui::PopID();

        ImGui::SetItemAllowOverlap();

        interaction_happen = processNodeOnWorkspace(diwne, inner_interaction_happen);
    }
    else
    {
#ifdef DIWNE_DEBUG
        ImRect nodeRectDiwne = getNodeRectDiwne();
        ImGui::Text(fmt::format("Node is outside of WorkAreaDiwne {} {}", nodeRectDiwne.Min.x, nodeRectDiwne.Min.y ).c_str());
#endif // DIWNE_DEBUG

        inner_interaction_happen |= processNodeOutsideOfWorkspace(diwne);
    }

    return inner_interaction_happen || interaction_happen;
}

bool Node::processInNodeAfterContent(DIWNE::Diwne &diwne)
{
    if (m_selected)
    {
        ImRect nodeRectDiwne = getNodeRectDiwne();
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 5); /* \todo JH color and width from settings */
    }
    return false;
}

bool Node::bypassNodeHoveredAction() {return ImGui::IsItemHovered();}
bool Node::bypassNodeSelectAction() {return ImGui::IsMouseReleased(0) && bypassNodeHoveredAction();}
bool Node::bypassNodeUnselectAction() {return ImGui::IsMouseReleased(0) && bypassNodeHoveredAction();}
bool Node::bypassNodeHoldAction() {return ImGui::IsMouseClicked(0) && bypassNodeHoveredAction();}
bool Node::bypassNodeUnholdAction() {return !ImGui::IsMouseDown(0);}
bool Node::bypassNodeRaisePopupAction() {return ImGui::IsMouseReleased(1) && bypassNodeHoveredAction();}

bool Node::processNodeHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (bypassNodeHoveredAction())
    {
        ImRect nodeRectDiwne = getNodeRectDiwne();
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,255), 0, ImDrawCornerFlags_None, 3*diwne.getWorkAreaZoomDiwne());
    }
    return false;
}

bool Node::processNodeSelected(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!m_selected && !m_translated && !inner_interaction_happen && bypassNodeSelectAction())
    {
        m_selected = true;
        diwne.setNodesSelectionChanged(true);
        return true;
    }
    return false;
}

bool Node::processNodeUnselected(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (m_selected && !m_translated && !inner_interaction_happen && bypassNodeUnselectAction())
    {
        m_selected = false;
        diwne.setNodesSelectionChanged(true);
        return true;
    }
    return false;
}

bool Node::processNodeHold(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!inner_interaction_happen && bypassNodeHoldAction())
    {
        m_isHeld = true;
        diwne.setDiwneAction(DiwneAction::HoldNode);
        diwne.m_draged_hold_node = shared_from_this();
        return true;
    }
    return false;
}

bool Node::processNodeUnhold(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!inner_interaction_happen && bypassNodeUnholdAction())
    {
        m_isHeld = false;
        m_translated = false;
        diwne.m_draged_hold_node = nullptr;
        diwne.setDiwneAction(DiwneAction::None);
        return true;
    }
    return false;
}

bool Node::processNodeDrag(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!inner_interaction_happen && m_isHeld && diwne.bypassIsMouseDragging0())
    {
        translateNodePositionDiwne(diwne.bypassGetMouseDelta()/diwne.getWorkAreaZoomDiwne());
        m_translated = true;
        diwne.setDiwneAction(DiwneAction::DragNode);
        return true;
    }
    return false;
}

bool Node::processNodeOnWorkspace(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    bool interaction_happen = false;

    interaction_happen |= processNodeHovered(diwne, inner_interaction_happen);

    interaction_happen |= m_selected ? processNodeUnselected(diwne, inner_interaction_happen) : processNodeSelected(diwne, inner_interaction_happen);

    interaction_happen |= m_isHeld ? processNodeUnhold(diwne, inner_interaction_happen) : processNodeHold(diwne, inner_interaction_happen);
    interaction_happen |= m_isHeld; /* holding (not only change in hold state) is interaction */

    interaction_happen |= processNodeDrag(diwne, inner_interaction_happen);

    inner_interaction_happen |= processNodePopupDiwne(diwne, inner_interaction_happen);

#ifdef DIWNE_DEBUG
    ImRect nodeRectDiwne = getNodeRectDiwne();
    ImGui::Text(fmt::format( "D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}",
                             nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                             diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                             diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());

    /* debug - whole node */
    diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,0,0,100), 0, ImDrawCornerFlags_None, 2);

    if (!inner_interaction_happen && ImGui::IsItemHovered())
    {
        /* debug - whole node */
        diwne.AddRectDiwne( m_topRectDiwne.Min
                    , m_bottomRectDiwne.Max, ImColor(0,0,0,100), 0, ImDrawCornerFlags_None, 2);
    }
#endif // DIWNE_DEBUG

    return interaction_happen;

}

bool Node::drawTopDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;

		ImGui::PushID("Top");
    ImGui::BeginGroup();
        inner_interaction_happen |= topContent(diwne);
    ImGui::EndGroup();
		ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne( m_topRectDiwne.Min
                , m_topRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    /* top.Min is position of node */
    m_topRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_topRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

bool Node::drawLeftDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Left");
        ImGui::BeginGroup();
            inner_interaction_happen |= leftContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug - left of node */
    diwne.AddRectDiwne( m_leftRectDiwne.Min
                , m_leftRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    m_leftRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_leftRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawMiddleDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
				ImGui::PushID("Middle");
        ImGui::BeginGroup();
            inner_interaction_happen |= middleContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug - middle of node */
    diwne.AddRectDiwne(m_middleRectDiwne.Min
                , m_middleRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

        m_middleRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_middleRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}
bool Node::drawRightDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Right");
        ImGui::BeginGroup();
            inner_interaction_happen |= rightContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug - right of node */
    diwne.AddRectDiwne(m_rightRectDiwne.Min
                ,m_rightRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

        m_rightRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_rightRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );
    return inner_interaction_happen;

}
bool Node::drawBottomDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Bottom");
        ImGui::BeginGroup();
            inner_interaction_happen |= bottomContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

#ifdef DIWNE_DEBUG
    /* debug - bottom of node */
    diwne.AddRectDiwne(m_bottomRectDiwne.Min
                , m_bottomRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
        m_bottomRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_bottomRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

void Node::updateSizeRectangles(DIWNE::Diwne &diwne)
{
    setNodeRectsPositionDiwne(m_nodePositionDiwne);
    ImVec2 spacing = ImGui::GetStyle().ItemSpacing / diwne.getWorkAreaZoomDiwne(); /* in BeginDiwne() is ItemSpacing scaled */

    float rightWidth = m_rightRectDiwne.GetWidth();
    float centerWidth = m_leftRectDiwne.GetWidth() + m_middleRectDiwne.GetWidth() + rightWidth + spacing.x*2; /* space is between left-middle and middle-right */
    float maxWidth = std::max(m_topRectDiwne.GetWidth(), std::max(centerWidth, m_bottomRectDiwne.GetWidth()));

    float bottomYOfCentre = m_bottomRectDiwne.Min.y - spacing.y;

    m_topRectDiwne.Max.x = m_topRectDiwne.Min.x + maxWidth; /* top.Max.x is most right point now */

    m_leftRectDiwne.Max.y = bottomYOfCentre;

    m_centerDummySpace = maxWidth - centerWidth; /* how much shift middle or right content for right-alignation */
    /* \todo JH span graphic of middle backgrount to fill middle of node or left it just around middle content?
    m_middleRectDiwne.Min.x = m_leftRectDiwne.Max.x + spacing.x;
    m_middleRectDiwne.Max.x = m_topRectDiwne.Max.x - rightWidth - spacing.x;*/ /* space between middle-right */
    m_middleRectDiwne.Max.y = bottomYOfCentre;

    m_rightRectDiwne.Max.y = bottomYOfCentre;

    m_bottomRectDiwne.Max.x = m_topRectDiwne.Max.x;

}

void Node::setNodeRectsPositionDiwne(ImVec2 const& position)
{
    translateNodeRectsDiwne(position - m_topRectDiwne.Min);
}

void Node::translateNodeRectsDiwne(ImVec2 const& distance)
{
    m_topRectDiwne.Translate(distance);
    m_leftRectDiwne.Translate(distance);
    m_middleRectDiwne.Translate(distance);
    m_rightRectDiwne.Translate(distance);
    m_bottomRectDiwne.Translate(distance);
}

bool Node::topContent(DIWNE::Diwne &diwne){return false;}
bool Node::leftContent(DIWNE::Diwne &diwne){return false;}

bool Node::rightContent(DIWNE::Diwne &diwne){return false;}
bool Node::middleContent(DIWNE::Diwne &diwne){return false;}
bool Node::bottomContent(DIWNE::Diwne &diwne){return false;}

bool Node::processNodePopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (inner_interaction_happen) return false;
    bool interaction_happen = false;

    if (bypassNodeRaisePopupAction()){ImGui::OpenPopup(m_popupID.c_str());}

    if(ImGui::IsPopupOpen(m_popupID.c_str()))
    {
        ImGui::SetNextWindowPos(diwne.getPopupPosition());
        if (ImGui::BeginPopup(m_popupID.c_str()))
        {
            interaction_happen = true;
            /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not (zoomed) diwne */
            //diwne.transformMouseFromDiwneToImGui();

    #ifdef DIWNE_DEBUG
            /* debug */
            ImGui::Text("Diwne ID: %i", m_idDiwne);
            ImGui::Separator();
    #endif // DIWNE_DEBUG

            nodePopupContent();

            ImGui::EndPopup();
        }
    }
	return interaction_happen;
}

void Node::nodePopupContent()
{
    if (ImGui::MenuItem("Override this method with content of your node popup menu")) {}
}

} /* namespace DIWNE */
