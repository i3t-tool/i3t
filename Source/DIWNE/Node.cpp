#include "Node.h"

namespace DIWNE
{


Node::Node(DIWNE::ID id) /* FLT_MAX for first frame - draw node anywhere it is and thus got its real size*/
    : m_topRectDiwne(ImRect(100,100,0,0))
    , m_leftRectDiwne(ImRect(0,0,0,0))
    , m_middleRectDiwne(ImRect(0,0,0,0))
    , m_rightRectDiwne(ImRect(0,0,0,0))
    , m_bottomRectDiwne(ImRect(0,0,FLT_MAX, FLT_MAX))
    , m_idDiwne(id)
{}

Node::~Node()
{}


Node& Node::operator=(const Node& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Node::drawNodeDiwne(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
    bool interaction_happen = false;

    setNodeRectsPositionDiwne(getNodePositionDiwne()*diwne.getWorkAreaZoomDiwne());

    ImRect nodeRectDiwne = getNodeRectDiwne();

    if ( nodeRectDiwne.Overlaps( diwne.getWorkAreaDiwne() ) )
    {
        ImRect nodeOnWorkArea = ImRect( diwne.diwne2workArea_noZoom( m_topRectDiwne.Min )
                                    , diwne.diwne2workArea_noZoom( m_bottomRectDiwne.Max ) );


        /* debug - whole node */
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(0,255,0), 0, ImDrawCornerFlags_None, 2);


        /* Set cursor to position of node */
        ImGui::SetCursorScreenPos(diwne.workArea2screen(nodeOnWorkArea.Min));

        putInvisibleButtonUnder(fmt::format("NodeIB{}", m_idDiwne), getNodeRectSizeDiwne());
        if (ImGui::IsItemActive())
        {
            translateNodePositionDiwne(ImGui::GetIO().MouseDelta/diwne.getWorkAreaZoomDiwne());
        }

        ImGui::PushID(fmt::format("Node{}", m_idDiwne).c_str());
        ImGui::BeginGroup(); /* Begin of node */

            inner_interaction_happen |= drawTopDiwne(diwne);

            ImGui::BeginHorizontal("LeftMiddleRight");
                inner_interaction_happen |= drawLeftDiwne(diwne);
                inner_interaction_happen |= drawMiddleDiwne(diwne);
                inner_interaction_happen |= drawRightDiwne(diwne);
            ImGui::EndHorizontal(); /* Left-Middle-Right */

            inner_interaction_happen |= drawBottomDiwne(diwne);

        /* debug */
        ImGui::Text(fmt::format( "NodeDiwne : {}-{}  -  {}-{}\nNodeWA    : {}-{}  -  {}-{}\nNodeScreen: {}-{}  -  {}-{}\n",
                                 nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                                 diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                                 diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());

        ImGui::EndGroup(); /* End of node */
        ImGui::PopID();
        synchronizeSizeRectangles();


        interaction_happen = ImGui::IsItemHovered(); /* for debug */
        if (!inner_interaction_happen)
        {
            inner_interaction_happen |= nodePopupDiwne(diwne, fmt::format("nodeContextPopup{}", m_idDiwne));
        }


        if (!inner_interaction_happen && interaction_happen)
        {
            /* debug - whole node */
            diwne.AddRectDiwne( m_topRectDiwne.Min
                        , m_bottomRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
        }

    }
    else
    {
        ImGui::Text(fmt::format("Node is outside of WorkAreaDiwne {} {}", nodeRectDiwne.Min.x, nodeRectDiwne.Min.y ).c_str()); /* debug */
    }
    return inner_interaction_happen || interaction_happen;
}

bool Node::drawTopDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne( m_topRectDiwne.Min
                , m_topRectDiwne.Max, ImColor(0,0,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

    ImGui::BeginHorizontal("Top");
        inner_interaction_happen |= topContent(diwne);
    ImGui::EndHorizontal();

    m_topRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}

bool Node::drawLeftDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne( m_leftRectDiwne.Min
                , m_leftRectDiwne.Max, ImColor(0,255,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("Left");
            inner_interaction_happen |= leftContent(diwne);
        ImGui::EndVertical();

        m_leftRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_leftRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawMiddleDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne(m_middleRectDiwne.Min
                , m_middleRectDiwne.Max, ImColor(255,0,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("Middle");
            inner_interaction_happen |= middleContent(diwne);
        ImGui::EndVertical();

        m_middleRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_middleRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawRightDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne(m_rightRectDiwne.Min
                ,m_rightRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("Right");
            inner_interaction_happen |= rightContent(diwne);
        ImGui::EndVertical();

        m_rightRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_rightRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );

    return inner_interaction_happen;

}
bool Node::drawBottomDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne(m_bottomRectDiwne.Min
                , m_bottomRectDiwne.Max, ImColor(255,255,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginHorizontal("Bottom");
            inner_interaction_happen |= bottomContent(diwne);
        ImGui::EndHorizontal();

        m_bottomRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_bottomRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

void Node::synchronizeSizeRectangles()
{
    m_bottomRectDiwne.Max.x = std::max(m_topRectDiwne.Max.x, std::max(m_rightRectDiwne.Max.x, m_bottomRectDiwne.Max.x));

    /* \todo JH maybe increase width of middle instead of right? */
    m_rightRectDiwne.Max.x = m_bottomRectDiwne.Max.x;
    m_rightRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_middleRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_leftRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_topRectDiwne.Max.x = m_bottomRectDiwne.Max.x;
}

void Node::setNodeRectsPositionDiwne(ImVec2 const& position)
{
    translateNodeRectsDiwne(position - m_topRectDiwne.Min);
}

//void Node::translateNodeRectsDiwneZoomed(DIWNE::Diwne const &diwne, ImVec2 const& distance)
//{
//    float zoomDiwne = diwne.getWorkAreaZoomDiwne();
//    translateNodeRectsDiwne(ImVec2(distance.x/zoomDiwne, distance.y/zoomDiwne));
//}

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

bool Node::nodePopupDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring)
{
    bool interaction_happen = false;

    ImGui::SetNextWindowPos(diwne.getPopupPosition());
    if (ImGui::BeginPopupContextItem(popupIDstring.c_str()))
	{
	    interaction_happen = true;
	    /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not (zoomed) diwne */
        diwne.transformMouseFromDiwneToImGui();


        /* debug */
        ImGui::Text("Diwne ID: %i", m_idDiwne);
        ImGui::Separator();

        nodePopupContent();

		ImGui::EndPopup();
	}
	return interaction_happen;

}

void Node::nodePopupContent()
{
 //
//		if (context_node->isTransformation()) { context_node->drawMenuSetDataMap(); }
//		context_node->drawMenuLevelOfDetail();
//		context_node->drawMenuSetPrecision();

		if (ImGui::MenuItem("Increment id (haha just for test)")) { m_idDiwne++;}
}

} /* namespace DIWNE */
