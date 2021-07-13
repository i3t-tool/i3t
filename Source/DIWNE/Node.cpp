#include "Node.h"

namespace DIWNE
{


Node::Node(DIWNE::Diwne *diwne)
    : m_topRectDiwne(ImRect(100,100,0,0))
    , m_leftRectDiwne(ImRect(0,0,0,0))
    , m_middleRectDiwne(ImRect(0,0,0,0))
    , m_rightRectDiwne(ImRect(0,0,0,0))
    , m_bottomRectDiwne(ImRect(0,0,300,300))
    , m_diwne(diwne)
{
    m_pin = new DIWNE::Pin();
}

Node::~Node()
{
    //dtor
    delete m_pin;
}

Node::Node(const Node& other)
{
    //copy ctor
}

Node& Node::operator=(const Node& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Node::drawNodeDiwne()
{
    bool inner_interaction_happen = false;
    bool interaction_happen = false;
    ImRect workAreaDiwne = m_diwne->getWorkAreaDiwne();
    if ( this->getNodeRect().Overlaps(workAreaDiwne) )
    {
        ImRect nodeOnScreen = ImRect( m_diwne->diwne2screen( m_topRectDiwne.Min )
                                    , m_diwne->diwne2screen( m_bottomRectDiwne.Max ) );


        /* debug - whole node */
        ImGui::GetWindowDrawList()->AddRect(nodeOnScreen.Min, nodeOnScreen.Max, ImColor(0,255,0), 0, ImDrawCornerFlags_None, 2);


        /* Set cursor to position of node */
        ImGui::SetCursorScreenPos(nodeOnScreen.Min);


        ImGui::BeginGroup(); /* Begin of node */

            inner_interaction_happen |= drawTopDiwne();

            ImGui::BeginHorizontal("LeftMiddleRightDiwne");
                inner_interaction_happen |= drawLeftDiwne();
                inner_interaction_happen |= drawMiddleDiwne();
                inner_interaction_happen |= drawRightDiwne();
            ImGui::EndHorizontal(); /* Left-Middle-Right */

            inner_interaction_happen |= drawBottomDiwne();

        ImGui::EndGroup(); /* End of node */
        interaction_happen = ImGui::IsItemHovered();

        synchronizeSizeRectangles();

        if (!inner_interaction_happen && interaction_happen)
        {
            /* debug - whole node */
            ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_topRectDiwne.Min)
                                            , m_diwne->diwne2screen(m_bottomRectDiwne.Max), ImColor(0,0,0), 0, ImDrawCornerFlags_None, 2);
        }

    }
    return inner_interaction_happen || interaction_happen;
}

bool Node::drawTopDiwne()
{
    /* debug - top of node */
    ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_topRectDiwne.Min)
                                        , m_diwne->diwne2screen(m_topRectDiwne.Max), ImColor(0,0,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

    ImGui::BeginHorizontal("TopDiwne");
        inner_interaction_happen |= drawTop();
    ImGui::EndHorizontal();

    m_topRectDiwne.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}

bool Node::drawLeftDiwne()
{
    /* debug - top of node */
    ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_leftRectDiwne.Min)
                                        , m_diwne->diwne2screen(m_leftRectDiwne.Max), ImColor(0,255,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("LeftDiwne");
            inner_interaction_happen |= drawLeft();
        ImGui::EndVertical();

        m_leftRectDiwne.Min = m_diwne->screen2diwne( ImGui::GetItemRectMin() );
        m_leftRectDiwne.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawMiddleDiwne()
{
    /* debug - top of node */
    ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_middleRectDiwne.Min)
                                        , m_diwne->diwne2screen(m_middleRectDiwne.Max), ImColor(255,0,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("MiddleDiwne");
            inner_interaction_happen |= drawMiddle();
        ImGui::EndVertical();

        m_middleRectDiwne.Min = m_diwne->screen2diwne( ImGui::GetItemRectMin() );
        m_middleRectDiwne.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );
    return inner_interaction_happen;
}
bool Node::drawRightDiwne()
{
    /* debug - top of node */
    ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_rightRectDiwne.Min)
                                        , m_diwne->diwne2screen(m_rightRectDiwne.Max), ImColor(255,255,0), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginVertical("RightDiwne");
            inner_interaction_happen |= drawRight();
        ImGui::EndVertical();

        m_rightRectDiwne.Min = m_diwne->screen2diwne( ImGui::GetItemRectMin() );
        m_rightRectDiwne.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen;

}
bool Node::drawBottomDiwne()
{
    /* debug - top of node */
    ImGui::GetWindowDrawList()->AddRect( m_diwne->diwne2screen(m_bottomRectDiwne.Min)
                                        , m_diwne->diwne2screen(m_bottomRectDiwne.Max), ImColor(255,255,255), 0, ImDrawCornerFlags_None, 2);
    bool inner_interaction_happen = false;

        ImGui::BeginHorizontal("BottomDiwne");
            inner_interaction_happen |= drawBottom();
        ImGui::EndHorizontal();

        m_bottomRectDiwne.Min = m_diwne->screen2diwne( ImGui::GetItemRectMin() );
        m_bottomRectDiwne.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );

    return inner_interaction_happen;
}

void Node::synchronizeSizeRectangles()
{
    m_bottomRectDiwne.Max.x = std::max(m_bottomRectDiwne.Max.x, m_rightRectDiwne.Max.x);

    m_rightRectDiwne.Max.x = m_bottomRectDiwne.Max.x;
    m_rightRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_middleRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_leftRectDiwne.Max.y = m_bottomRectDiwne.Min.y;

    m_topRectDiwne.Max.x = m_bottomRectDiwne.Max.x;
}

bool Node::drawTop(){return false;}
bool Node::drawLeft()
{
    bool interaction_happen = false;
    ImGui::Spring(1, 3);


    interaction_happen |= m_pin->drawPinDiwne();
    interaction_happen |= m_pin->drawPinDiwne();

    ImGui::Spring(2, 0);
    return interaction_happen;
}

bool Node::drawRight()
{
    bool interaction_happen = false;
    ImGui::Spring(1, 0);
    ImGui::Text("Zkušební Right");
    ImGui::Text("Zkušební Right");
    ImGui::Spring(2, 0);
    return interaction_happen;
}

bool Node::drawMiddle()
{
    bool interaction_happen = false;
    ImGui::Spring(1, 0);
    ImGui::Text("Zkušební Middle");
    ImGui::Text("Zkušební Middle");
    ImGui::Spring(2, 0);
    return interaction_happen;
}

bool Node::drawBottom(){return false;}

} /* namespace DIWNE */
