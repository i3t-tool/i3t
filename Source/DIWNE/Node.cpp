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
    , m_drawAnywhere(true)
    , m_nodePosMode(OnItsPosition)
{}

Node::~Node()
{
    if(m_selected)
    {
        diwne.setNodesSelectionChanged(true);
    }
}

Node& Node::operator=(const Node& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Node::allowDrawing() {return m_drawAnywhere || getRectDiwne().Overlaps( diwne.getWorkAreaDiwne() ); }

bool Node::beforeBeginDiwne()
{
    switch(m_nodePosMode)
    {
    case DrawModeNodePosition::OnItsPosition:
        ImGui::SetCursorScreenPos(diwne.diwne2screen(m_nodePositionDiwne));
        break;
    case DrawModeNodePosition::OnCoursorPosition:
        setNodePositionDiwne(diwne.screen2diwne(ImGui::GetCursorScreenPos()));
        break;
    }
    if(m_drawAnywhere) m_drawAnywhere=false;

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
DIWNE_DEBUG((diwne), {
    ImRect nodeRectDiwne = getNodeRectDiwne();
    ImGui::Text(fmt::format( "D:{}-{}-{}-{}\nWA:{}-{}-{}-{}\nS:{}-{}-{}-{}",
                             nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                             diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                             diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());

}); /* close of macro */
#endif // DIWNE_DEBUG
    ImGui::EndGroup(); /* End of node */
    ImGui::PopID();
}

bool Node::content()
{
    bool interaction_happen = false;
    interaction_happen |= topContentDiwne();

    ImGui::PushID("Center");
    ImGui::BeginGroup();

        interaction_happen |= leftContentDiwne();
        ImGui::SameLine();

        if(m_centerDummySpace > 0){ ImGui::Indent((m_leftRectDiwne.GetWidth() + m_centerDummySpace*diwne.mp_settingsDiwne->middleAlign)*diwne.getWorkAreaZoom() + ImGui::GetStyle().ItemSpacing.x ); } /* spacing is already zoomed in Diwne */
        interaction_happen |= middleContentDiwne();
        ImGui::SameLine();

        if(m_centerDummySpace > 0){ ImGui::Indent((m_middleRectDiwne.GetWidth() + m_centerDummySpace*(1-diwne.mp_settingsDiwne->middleAlign))*diwne.getWorkAreaZoom() + ImGui::GetStyle().ItemSpacing.x ); }
        interaction_happen |= rightContentDiwne();

    ImGui::EndGroup(); /* Center */
    ImGui::PopID();

    interaction_happen |= bottomContentDiwne();
    return interaction_happen;
}


bool Node::afterEndDiwne()
{
    if (m_selectable)
    {
        bool prev_selected = m_selected;
        if ( diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::SelectionRectFull || diwne.getDiwneAction() == DIWNE::DiwneAction::SelectionRectFull)
        {
            setSelected(diwne.getSelectionRectangleDiwne().Contains(getNodeRectDiwne()) ? true : diwne.m_allowUnselectingNodes ? false : m_selected);
        }else if (diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::SelectionRectTouch || diwne.getDiwneAction() == DIWNE::DiwneAction::SelectionRectTouch )
        {
            setSelected(diwne.getSelectionRectangleDiwne().Overlaps(getNodeRectDiwne()) ? true : diwne.m_allowUnselectingNodes ? false : m_selected);
        }

        if (m_selected)
        {
            diwne.AddRectDiwne(getRectDiwne().Min, getRectDiwne().Max, diwne.mp_settingsDiwne->itemSelectedBorderColor, 0, ImDrawCornerFlags_None, diwne.mp_settingsDiwne->itemSelectedBorderThicknessDiwne);
        }

        if (prev_selected != m_selected)
        {
            diwne.m_takeSnap = true;
            diwne.setNodesSelectionChanged(true);
        }
    }

    /* always block interactions with other nodes */
    ImGui::SetCursorScreenPos(diwne.diwne2screen(getNodePositionDiwne()));
    ImGui::InvisibleButton("IBBlockingOtherImGuiInteractions", getNodeRectSizeDiwne()*diwne.getWorkAreaZoom());

    return DiwneObject::afterEndDiwne();
}

bool Node::processSelect()
{
    diwne.setNodesSelectionChanged(true);
    return true;
}

bool Node::processUnselect()
{
    diwne.setNodesSelectionChanged(true);
    return true;
}

bool Node::processDrag()
{
    translateNodePositionDiwne(diwne.bypassGetMouseDelta()/diwne.getWorkAreaZoom());
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
DIWNE_DEBUG((diwne), {
    diwne.AddRectDiwne( m_topRectDiwne.Min, m_topRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
}); /* close of macro */
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
DIWNE_DEBUG((diwne), {
    diwne.AddRectDiwne( m_leftRectDiwne.Min, m_leftRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
}); /* close of macro */
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
DIWNE_DEBUG((diwne), {
    diwne.AddRectDiwne(m_middleRectDiwne.Min, m_middleRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
}); /* close of macro */
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
DIWNE_DEBUG((diwne), {
    diwne.AddRectDiwne(m_rightRectDiwne.Min, m_rightRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
}); /* close of macro */
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
DIWNE_DEBUG((diwne), {
    diwne.AddRectDiwne(m_bottomRectDiwne.Min, m_bottomRectDiwne.Max, ImColor(0,0,255,100), 0, ImDrawCornerFlags_None, 2);
}); /* close of macro */
#endif // DIWNE_DEBUG

    return inner_interaction_happen;
}

void Node::updateSizes()
{
    /* \todo can use ImGui::ItemMax/Min */
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
