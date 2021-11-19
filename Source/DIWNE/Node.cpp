#include "Node.h"

namespace DIWNE
{


Node::Node(DIWNE::ID id) /* FLT_MAX for first frame - draw node anywhere it is and thus got its real size*/
    : m_topRectDiwne(ImRect(100,100,0,0))
    , m_leftRectDiwne(ImRect(0,0,0,0))
    , m_middleRectDiwne(ImRect(0,0,0,0))
    , m_rightRectDiwne(ImRect(0,0,0,0))
    , m_bottomRectDiwne(ImRect(0,0,FLT_MAX, FLT_MAX))
//    , m_topRectDiwne_temp(ImRect(100,100,0,0))
//    , m_leftRectDiwne_temp(ImRect(0,0,0,0))
//    , m_middleRectDiwne_temp(ImRect(0,0,0,0))
//    , m_rightRectDiwne_temp(ImRect(0,0,0,0))
//    , m_bottomRectDiwne_temp(ImRect(0,0,FLT_MAX, FLT_MAX))
    , m_middleDummyWidthForAlign(0)
    , m_idDiwne(id)
    , m_selected(false)
    , m_translated(false)
    , m_isHeld(false)
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
    bool inner_interaction_happen = false;
    bool interaction_happen = false;

    if (drawHere) setNodePositionDiwne(diwne.screen2diwne(ImGui::GetCursorScreenPos()));
    setNodeRectsPositionDiwne(getNodePositionDiwne()*diwne.getWorkAreaZoomDiwne()); /*draw here change location*/

//    m_topRectDiwne_temp.Min = m_topRectDiwne.Max;

    ImRect nodeRectDiwne = getNodeRectDiwne();

    if ( nodeRectDiwne.Overlaps( diwne.getWorkAreaDiwne() ) )
    {
        ImRect nodeOnWorkArea = ImRect( diwne.diwne2workArea_noZoom( m_topRectDiwne.Min )
                                    , diwne.diwne2workArea_noZoom( m_bottomRectDiwne.Max ) );

#ifdef DIWNE_DEBUG
        /* debug - whole node */
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(0,255,0), 0, ImDrawCornerFlags_None, 8);
#endif // DIWNE_DEBUG

        /* Set cursor to position of node */
        if (!drawHere) ImGui::SetCursorScreenPos(diwne.workArea2screen(nodeOnWorkArea.Min));

        ImGui::PushID(fmt::format("Node{}", m_idDiwne).c_str());
        ImGui::BeginGroup(); /* Begin of node */

            inner_interaction_happen |= drawNodeBackground(diwne);

            inner_interaction_happen |= drawTopDiwne(diwne);

						ImGui::PushID("LeftMiddleRight");
            ImGui::BeginGroup();
                inner_interaction_happen |= drawLeftDiwne(diwne);
                   ImGui::SameLine();
                inner_interaction_happen |= drawMiddleDiwne(diwne);
                    ImGui::SameLine();
                inner_interaction_happen |= drawRightDiwne(diwne);
            ImGui::EndGroup(); /* Left-Middle-Right */
						ImGui::PopID();

            inner_interaction_happen |= drawBottomDiwne(diwne);

#ifdef DIWNE_DEBUG
        /* debug */
        ImGui::Text(fmt::format( "NodeDiwne : {}-{}  -  {}-{}\nNodeWA    : {}-{}  -  {}-{}\nNodeScreen: {}-{}  -  {}-{}\n",
                                 nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                                 diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                                 diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());
#endif // DIWNE_DEBUG

        ImGui::EndGroup(); /* End of node */
        ImGui::PopID();
        ImGui::SetItemAllowOverlap();

        if (ImGui::IsItemHovered()) {
            diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,255), 0, ImDrawCornerFlags_None, 3);
        }

        if (!inner_interaction_happen)
        {
            if (ImGui::IsItemClicked(0)) {
                m_isHeld = true;
                diwne.setDiwneAction(DiwneAction::HoldNode);
                diwne.m_draged_hold_node = shared_from_this();
            }

            if (m_isHeld) /* be aware of same button for clicked and dragging and released*/
            {
                interaction_happen = true;
                if (ImGui::IsMouseDragging(0))
                {
                    translateNodePositionDiwne(ImGui::GetIO().MouseDelta/diwne.getWorkAreaZoomDiwne());
                    m_translated = true;
                    diwne.setDiwneAction(DiwneAction::DragNode);
                }

                if (ImGui::IsMouseReleased(0))
                {
                    if (m_translated)
                    {
                            m_translated = false;
                            diwne.m_draged_hold_node = nullptr;
                            diwne.setDiwneAction(DiwneAction::None);
                    }
                    else {m_selected = !m_selected; diwne.setNodesSelectionChanged(true);}

                    m_isHeld = false;
                }
            }

        }

        updateSizeRectangles();

        if (m_selected)
        {
            diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 5); /* \todo JH color and width from settings */
        }

        if (!inner_interaction_happen)
        {
            inner_interaction_happen |= nodePopupDiwne(diwne, fmt::format("nodePopup{}", m_idDiwne));
        }


#ifdef DIWNE_DEBUG
        if (!inner_interaction_happen && ImGui::IsItemHovered())
        {
            /* debug - whole node */
            diwne.AddRectDiwne( m_topRectDiwne.Min
                        , m_bottomRectDiwne.Max, ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
        }
#endif // DIWNE_DEBUG

    }
    else
    {
#ifdef DIWNE_DEBUG
        ImGui::Text(fmt::format("Node is outside of WorkAreaDiwne {} {}", nodeRectDiwne.Min.x, nodeRectDiwne.Min.y ).c_str()); /* debug */
#endif // DIWNE_DEBUG
        inner_interaction_happen |= processNodeOutsideOfWorkspace(diwne);

    }
    return inner_interaction_happen || interaction_happen;
}

bool Node::drawTopDiwne(DIWNE::Diwne &diwne)
{
#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne( m_topRectDiwne.Min
                , m_topRectDiwne.Max, ImColor(0,0,255), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
    bool inner_interaction_happen = false;

		ImGui::PushID("Top");
    ImGui::BeginGroup();
        inner_interaction_happen |= topContent(diwne);
    ImGui::EndGroup();
		ImGui::PopID();

//    m_topRectDiwne_temp.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    m_topRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

bool Node::drawLeftDiwne(DIWNE::Diwne &diwne)
{
#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne( m_leftRectDiwne.Min
                , m_leftRectDiwne.Max, ImColor(0,255,255), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
    bool inner_interaction_happen = false;

				ImGui::PushID("Left");
        ImGui::BeginGroup();
            inner_interaction_happen |= leftContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

//        m_leftRectDiwne_temp.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
//        m_leftRectDiwne_temp.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    m_leftRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
    m_leftRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawMiddleDiwne(DIWNE::Diwne &diwne)
{
#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne(m_middleRectDiwne.Min
                , m_middleRectDiwne.Max, ImColor(255,0,255), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
    bool inner_interaction_happen = false;

				ImGui::PushID("Middle");
        ImGui::BeginGroup();
            inner_interaction_happen |= middleContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

//        m_middleRectDiwne_temp.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
//        m_middleRectDiwne_temp.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
        m_middleRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_middleRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawRightDiwne(DIWNE::Diwne &diwne)
{
#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne(m_rightRectDiwne.Min
                ,m_rightRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
    bool inner_interaction_happen = false;

				ImGui::PushID("Right");
        ImGui::BeginGroup();
            inner_interaction_happen |= rightContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

//        m_rightRectDiwne_temp.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
//        m_rightRectDiwne_temp.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
        m_rightRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_rightRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;

}
bool Node::drawBottomDiwne(DIWNE::Diwne &diwne)
{
#ifdef DIWNE_DEBUG
    /* debug - top of node */
    diwne.AddRectDiwne(m_bottomRectDiwne.Min
                , m_bottomRectDiwne.Max, ImColor(255,255,255), 0, ImDrawCornerFlags_None, 2);
#endif // DIWNE_DEBUG
    bool inner_interaction_happen = false;

				ImGui::PushID("Bottom");
        ImGui::BeginGroup();
            inner_interaction_happen |= bottomContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

//        m_bottomRectDiwne_temp.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
//        m_bottomRectDiwne_temp.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
        m_bottomRectDiwne.Min = diwne.screen2diwne_noZoom( ImGui::GetItemRectMin() );
        m_bottomRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

void Node::updateSizeRectangles()
{
    float topWidth = m_topRectDiwne.GetWidth();
    float rightWidth = m_rightRectDiwne.GetWidth();
    float centerWidth = m_leftRectDiwne.GetWidth()+m_middleRectDiwne.GetWidth()+rightWidth;
    float bottomWidth = m_bottomRectDiwne.GetWidth();
    float maxWidth = std::max(centerWidth, std::max(topWidth, bottomWidth));

    m_topRectDiwne.Max.x = m_topRectDiwne.Min.x + maxWidth;

    m_leftRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_middleDummyWidthForAlign = maxWidth - centerWidth; /* how much shift right content for right-align */
    m_middleRectDiwne.Max.x = m_topRectDiwne.Max.x - rightWidth; /* top.max.x is most right point here */
    m_middleRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_rightRectDiwne.Min.x = m_middleRectDiwne.Max.x; /* shift right content to the most right place */
    m_rightRectDiwne.Max.x = m_topRectDiwne.Max.x;
    m_rightRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_bottomRectDiwne.Max.x = m_topRectDiwne.Max.x;


//    m_topRectDiwne.Min = m_topRectDiwne_temp.Min;
//    m_topRectDiwne.Max.y = m_topRectDiwne_temp.Max.y;
//    m_topRectDiwne.Max.x = std::max(m_topRectDiwne_temp.Max.x, std::max(m_rightRectDiwne_temp.Max.x, m_bottomRectDiwne_temp.Max.x)); /* compute most right point */
//
//    m_leftRectDiwne.Min = m_leftRectDiwne_temp.Min;
//    m_leftRectDiwne.Max.x = m_leftRectDiwne_temp.Max.x;
//    m_leftRectDiwne.Max.y = m_bottomRectDiwne_temp.Min.y;
//
//    m_rightRectDiwne.Min.x += (m_topRectDiwne.Max.x - m_rightRectDiwne_temp.Max.x); /* shift right content to the most right place */
//    m_rightRectDiwne.Min.y = m_rightRectDiwne_temp.Min.y;
//    m_rightRectDiwne.Max.x = m_topRectDiwne.Max.x;
//    m_rightRectDiwne.Max.y = m_bottomRectDiwne_temp.Min.y;
//
//    m_middleRectDiwne.Min = m_middleRectDiwne_temp.Min;
//    m_middleRectDiwne.Max.x = m_rightRectDiwne.Min.x;
//    m_middleRectDiwne.Max.y = m_bottomRectDiwne_temp.Min.y;
//
//    m_bottomRectDiwne.Min = m_bottomRectDiwne_temp.Min;
//    m_bottomRectDiwne.Max.x = m_topRectDiwne.Max.x;
//    m_bottomRectDiwne.Max.y = m_bottomRectDiwne_temp.Max.y;
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

#ifdef DIWNE_DEBUG
        /* debug */
        ImGui::Text("Diwne ID: %i", m_idDiwne);
        ImGui::Separator();
#endif // DIWNE_DEBUG

        nodePopupContent();

		ImGui::EndPopup();
	}
	return interaction_happen;
}

void Node::nodePopupContent()
{
    if (ImGui::MenuItem("Override this method with content of your node popup menu")) {}
}

} /* namespace DIWNE */
