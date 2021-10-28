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

    ImRect nodeRectDiwne = getNodeRectDiwne();

    if ( nodeRectDiwne.Overlaps( diwne.getWorkAreaDiwne() ) )
    {
        ImRect nodeOnWorkArea = ImRect( diwne.diwne2workArea_noZoom( m_topRectDiwne.Min )
                                    , diwne.diwne2workArea_noZoom( m_bottomRectDiwne.Max ) );


        /* debug - whole node */
        diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(0,255,0), 0, ImDrawCornerFlags_None, 2);


        /* Set cursor to position of node */
        if (!drawHere) ImGui::SetCursorScreenPos(diwne.workArea2screen(nodeOnWorkArea.Min));

        //putInvisibleButtonUnder(fmt::format("NodeIB{}", m_idDiwne), getNodeRectSizeDiwne());
//        if (ImGui::IsItemActive())
//        {
//            if (ImGui::IsMouseDragging(0))
//            {
//                translateNodePositionDiwne(ImGui::GetIO().MouseDelta/diwne.getWorkAreaZoomDiwne());
//                m_translated = true;
//                diwne.m_draged_node = this;
//                diwne.setDiwneAction(DiwneAction::DragNode);
//            }
//        }
//        else if (ImGui::IsItemHovered())
//        {
//            if (ImGui::IsMouseReleased(0))
//            {
//                if (m_translated){ m_translated = false; diwne.m_draged_node = nullptr; diwne.setDiwneAction(DiwneAction::None); }
//                else getSelected() ? setSelected(false) : setSelected(true);
//            }
//        }


        ImGui::PushID(fmt::format("Node{}", m_idDiwne).c_str());
        ImGui::BeginGroup(); /* Begin of node */

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

        /* debug */
        ImGui::Text(fmt::format( "NodeDiwne : {}-{}  -  {}-{}\nNodeWA    : {}-{}  -  {}-{}\nNodeScreen: {}-{}  -  {}-{}\n",
                                 nodeRectDiwne.Min.x, nodeRectDiwne.Min.y, nodeRectDiwne.Max.x, nodeRectDiwne.Max.y,
                                 diwne.diwne2workArea(nodeRectDiwne.Min).x, diwne.diwne2workArea(nodeRectDiwne.Min).y, diwne.diwne2workArea(nodeRectDiwne.Max).x, diwne.diwne2workArea(nodeRectDiwne.Max).y,
                                 diwne.diwne2screen(nodeRectDiwne.Min).x, diwne.diwne2screen(nodeRectDiwne.Min).y, diwne.diwne2screen(nodeRectDiwne.Max).x, diwne.diwne2screen(nodeRectDiwne.Max).y).c_str());

        ImGui::EndGroup(); /* End of node */
        ImGui::PopID();
        if (!inner_interaction_happen && ImGui::IsItemClicked(0))
        {
            m_isHeld = true;
        }

        if (!inner_interaction_happen && m_isHeld) /* be aware of same button for clicked and dragging and released*/
        {
            interaction_happen = true;
            if (ImGui::IsMouseDragging(0))
            {
                translateNodePositionDiwne(ImGui::GetIO().MouseDelta/diwne.getWorkAreaZoomDiwne());
                m_translated = true;
                diwne.m_draged_node = shared_from_this();
                diwne.setDiwneAction(DiwneAction::DragNode);
            }

            if (ImGui::IsMouseReleased(0))
            {
                if (m_translated){
                        m_translated = false;
                 diwne.m_draged_node = nullptr;
                  diwne.setDiwneAction(DiwneAction::None); }
                else getSelected() ? setSelected(false) : setSelected(true);

                m_isHeld = false;
            }
        }

        synchronizeSizeRectangles();



        if (getSelected())
        {
            diwne.AddRectDiwne(nodeRectDiwne.Min, nodeRectDiwne.Max, ImColor(255,255,0), 0, ImDrawCornerFlags_None, 5); /* \todo JH color and width from settings */
        }


        interaction_happen = ImGui::IsItemHovered(); /* for debug */
        if (!inner_interaction_happen)
        {
            inner_interaction_happen |= nodePopupDiwne(diwne, fmt::format("nodePopup{}", m_idDiwne));
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

		ImGui::PushID("Top");
    ImGui::BeginGroup();
        inner_interaction_happen |= topContent(diwne);
    ImGui::EndGroup();
		ImGui::PopID();

    m_topRectDiwne.Max = diwne.screen2diwne_noZoom( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}

bool Node::drawLeftDiwne(DIWNE::Diwne &diwne)
{
    /* debug - top of node */
    diwne.AddRectDiwne( m_leftRectDiwne.Min
                , m_leftRectDiwne.Max, ImColor(0,255,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

				ImGui::PushID("Left");
        ImGui::BeginGroup();
            inner_interaction_happen |= leftContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

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

				ImGui::PushID("Middle");
        ImGui::BeginGroup();
            inner_interaction_happen |= middleContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

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

				ImGui::PushID("Right");
        ImGui::BeginGroup();
            inner_interaction_happen |= rightContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

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

				ImGui::PushID("Bottom");
        ImGui::BeginGroup();
            inner_interaction_happen |= bottomContent(diwne);
        ImGui::EndGroup();
				ImGui::PopID();

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
    if (ImGui::MenuItem("Override this method with content of your node popup menu")) {}
}

} /* namespace DIWNE */
