#include "diwne_include.h"

namespace DIWNE
{


Node::Node(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne/*="DiwneNode"*/)
    : DiwneObject(diwne, id, labelDiwne)
    , m_topRectDiwne(ImRect(0,0,0,0))
    , m_leftRectDiwne(ImRect(0,0,0,0))
    , m_middleRectDiwne(ImRect(0,0,0,0))
    , m_rightRectDiwne(ImRect(0,0,0,0))
    , m_bottomRectDiwne(ImRect(0,0,0,0))
    , m_centerDummySpace(0)
    , m_middleAlign(0.5)
    , m_drawAnywhere(true)
    , m_nodeInteractionAllowed(false)
    , m_drawOnCursorPos(false)
{}

Node& Node::operator=(const Node& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Node::drawNodeDiwne(bool drawHere/*= false*/)
{
    m_drawOnCursorPos = drawHere;
    return drawDiwne();
}

bool Node::initializeDiwne()
{
    if ( m_drawAnywhere || getNodeRectDiwne().Overlaps( diwne.getWorkAreaDiwne() ) )
    {
        m_drawing = true;
        if(m_drawAnywhere) m_drawAnywhere=false;
    }else
    {
        m_drawing = false;
    }
    return initialize();
}

bool Node::beforeBeginDiwne()
{
    m_drawOnCursorPos ? setNodePositionDiwne(diwne.screen2diwne(ImGui::GetCursorScreenPos())) : ImGui::SetCursorScreenPos(diwne.diwne2screen(m_nodePositionDiwne));
    return beforeBegin();
}

void Node::begin()
{
    ImGui::PushID(m_labelDiwne.c_str());
    ImGui::BeginGroup(); /* Begin of node */
}

void Node::end()
{
#ifdef DIWNE_DEBUG
    ImRect nodeRectDiwne = getNodeRectDiwne();
    ImGui::Text(fmt::format( "D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}",
                             nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                             diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                             diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());

    diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,0,0,100), 0, ImDrawCornerFlags_None, 2);

    if (m_interactionAllowed && bypassHoveredAction())
    {
        diwne.AddRectDiwne( m_topRectDiwne.Min, m_bottomRectDiwne.Max, ImColor(0,0,0,100), 0, ImDrawCornerFlags_None, 2);
    }
#endif // DIWNE_DEBUG
    ImGui::EndGroup(); /* End of node */
    ImGui::PopID();
    ImGui::SetItemAllowOverlap();
}

bool Node::content()
{
    bool interaction_happen = false;
    interaction_happen |= topContentDiwne();

    ImGui::PushID("Center");
    ImGui::BeginGroup();

        interaction_happen |= leftContentDiwne();
        ImGui::SameLine();

        if(m_centerDummySpace > 0){ ImGui::Indent((m_leftRectDiwne.GetWidth() + m_centerDummySpace*m_middleAlign)*diwne.getWorkAreaZoom() + ImGui::GetStyle().ItemSpacing.x ); } /* spacing is already zoomed in Diwne */
        interaction_happen |= middleContentDiwne();
        ImGui::SameLine();

        if(m_centerDummySpace > 0){ ImGui::Indent((m_middleRectDiwne.GetWidth() + m_centerDummySpace*(1-m_middleAlign))*diwne.getWorkAreaZoom() + ImGui::GetStyle().ItemSpacing.x ); }
        interaction_happen |= rightContentDiwne();

    ImGui::EndGroup(); /* Left-Middle-Right */
    ImGui::PopID();

    interaction_happen |= bottomContentDiwne();
    return interaction_happen;
}

bool Node::afterContentDiwne()
{
    if (m_selected)
    {
        ImRect nodeRectDiwne = getNodeRectDiwne();
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 5); /* \todo JH color and width from settings */
    }
    return DiwneObject::afterContentDiwne();
}

bool Node::processHovered()
{
    ImRect nodeRectDiwne = getNodeRectDiwne();
    diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2*diwne.getWorkAreaZoom());
    return false;
}

bool Node::processSelected()
{
    diwne.setNodesSelectionChanged(true);
    return true;
}

bool Node::processUnselected()
{
    diwne.setNodesSelectionChanged(true);
    return true;
}

bool Node::processHold()
{
    diwne.setDiwneAction(DiwneAction::HoldNode);
    diwne.m_draged_hold_node = shared_from_this();
    return true;
}

bool Node::processUnhold()
{
    diwne.m_draged_hold_node = nullptr;
    diwne.setDiwneAction(DiwneAction::None);
    return true;
}

bool Node::processDrag()
{
    translateNodePositionDiwne(diwne.bypassGetMouseDelta()/diwne.getWorkAreaZoom());
    diwne.setDiwneAction(DiwneAction::DragNode);
    return true;
}

bool Node::topContentDiwne()
{
    bool inner_interaction_happen = false;

		ImGui::PushID("Top");
    ImGui::BeginGroup();
        inner_interaction_happen |= topContent();
    ImGui::EndGroup();
		ImGui::PopID();

    /* top.Min is position of node */
    m_topRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_topRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne( m_topRectDiwne.Min, m_topRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    return inner_interaction_happen;
}

bool Node::leftContentDiwne()
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Left");
        ImGui::BeginGroup();
            inner_interaction_happen |= leftContent();
        ImGui::EndGroup();
				ImGui::PopID();

    m_leftRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
    m_leftRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne( m_leftRectDiwne.Min, m_leftRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    return inner_interaction_happen;
}
bool Node::middleContentDiwne()
{
    bool inner_interaction_happen = false;
				ImGui::PushID("Middle");
        ImGui::BeginGroup();
            inner_interaction_happen |= middleContent();
        ImGui::EndGroup();
				ImGui::PopID();

        m_middleRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_middleRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(m_middleRectDiwne.Min, m_middleRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    return inner_interaction_happen;
}
bool Node::rightContentDiwne()
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Right");
        ImGui::BeginGroup();
            inner_interaction_happen |= rightContent();
        ImGui::EndGroup();
				ImGui::PopID();

        m_rightRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_rightRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(m_rightRectDiwne.Min, m_rightRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    return inner_interaction_happen;

}
bool Node::bottomContentDiwne()
{
    bool inner_interaction_happen = false;

				ImGui::PushID("Bottom");
        ImGui::BeginGroup();
            inner_interaction_happen |= bottomContent();
        ImGui::EndGroup();
				ImGui::PopID();

        m_bottomRectDiwne.Min = diwne.screen2diwne( ImGui::GetItemRectMin() );
        m_bottomRectDiwne.Max = diwne.screen2diwne( ImGui::GetItemRectMax() );

#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(m_bottomRectDiwne.Min, m_bottomRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG

    return inner_interaction_happen;
}

void Node::updateSizes()
{
    setNodeRectsPositionDiwne(m_nodePositionDiwne);
    ImVec2 spacing = ImGui::GetStyle().ItemSpacing / diwne.getWorkAreaZoom(); /* in BeginDiwne() is ItemSpacing scaled */

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

bool Node::topContent(){ImGui::TextUnformatted("Top"); return false;}
bool Node::leftContent(){ImGui::TextUnformatted("Left"); return false;}
bool Node::rightContent(){ImGui::TextUnformatted("Right"); return false;}
bool Node::middleContent(){ImGui::TextUnformatted("Middle"); return false;}
bool Node::bottomContent(){ImGui::TextUnformatted("Bottom"); return false;}

} /* namespace DIWNE */
