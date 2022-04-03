#include "WorkspaceSequence.h"
#include "../Windows/WorkspaceWindow.h"

WorkspaceSequence::WorkspaceSequence(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase/*= Core::Builder::createSequence()*/, bool drawPins/*=true*/)
    :   WorkspaceNodeWithCoreDataWithPins(diwne, nodebase, false)
    ,   m_drawPins(drawPins)
{}

bool WorkspaceSequence::isSequence()
{
    return true;
}

void WorkspaceSequence::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

int WorkspaceSequence::getInnerPosition(std::vector<ImVec2> points)
{
    ImRect rect = getNodeRectDiwne();
    bool any_in = false;
    ImVec2 in_point;
    for(auto const & point : points)
    {
        if (rect.Contains(point))
        {
            in_point = point;
            any_in = true;
        }
    }

    if (!any_in) return -1;


    rect.Max.x = rect.Min.x; /* squeeze rect at begin -> then in cycle shift rect and check point position */
    int i = 0;
    for (auto const & innerNode : getInnerWorkspaceNodes())
    {
        rect.Max.x = innerNode->getNodeRectDiwne().GetCenter().x;
        if(rect.Contains(in_point))
        {
            return i;
        }
        rect.Min.x = rect.Max.x;
        i++;
    }
    return i;
}

void WorkspaceSequence::popNode(Ptr<WorkspaceNodeWithCoreData> node)
{

    auto node_iter = std::find_if(  m_workspaceInnerTransformations.begin(),
                                    m_workspaceInnerTransformations.end(),
                                    [node](Ptr<WorkspaceNodeWithCoreData> const &in_node) -> bool { return node == in_node; });

    if (node_iter != m_workspaceInnerTransformations.end())
    {
        int index = node_iter-m_workspaceInnerTransformations.begin();
        std::dynamic_pointer_cast<WorkspaceTransformation>(*node_iter)->setRemoveFromSequence(true);
        m_workspaceInnerTransformations.erase(node_iter);
        m_nodebase->as<Core::Sequence>()->popMatrix(index);
    }
}

void WorkspaceSequence::pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index)
{
	Ptr<WorkspaceTransformation> node_t = std::dynamic_pointer_cast<WorkspaceTransformation>(node);
	if (node_t != nullptr && 0 <= index && index <= m_workspaceInnerTransformations.size())
	{
		node_t->setRemoveFromSequence(false);
		m_workspaceInnerTransformations.insert(m_workspaceInnerTransformations.begin() + index, node_t);
		/* \todo JH check return value if so */
		m_nodebase->as<Core::Sequence>()->addMatrix(node_t->getNodebase()->as<Core::Transformation>(), index);
	}
}

void WorkspaceSequence::moveNodeToSequence(Ptr<WorkspaceNodeWithCoreData> dragedNode, int index)
{
    pushNode(dragedNode, index); /*\ todo JH check if push is OK -> if not, not remove node from vector in window*/
    dragedNode->setRemoveFromWorkspace(true);
}

void WorkspaceSequence::moveNodeToWorkspace(Ptr<WorkspaceNodeWithCoreData> node)
{
    node->setRemoveFromWorkspace(false);
    dynamic_cast<WorkspaceDiwne&>(diwne).m_workspaceCoreNodes.push_back(node);
    popNode(node);
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const  { return m_workspaceInnerTransformations; }

void WorkspaceSequence::setPostionOfDummyData(int positionOfDummyData) {m_position_of_dummy_data = positionOfDummyData;}


bool WorkspaceSequence::beforeContent()
{
    /* whole node background */
    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::NodeBgTransformation)), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/
    return false;
}

bool WorkspaceSequence::topContent()
{
    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
                         ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::NodeHeaderTranformation)), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/

    return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceSequence::middleContent()
{
    bool inner_interaction_happen = false;
    int position_of_draged_node_in_sequence = -1; /* -1 means not in Sequence */
    Ptr<WorkspaceTransformation> dragedNode;

    if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
    {
        return false;
    }

    if (getInputs().at(1)->isConnected() ) /* \todo JH MH better selection of copy pin? */
    {
        bool valueChanged = false;
        int rowOfChange, columnOfChange;
        float valueOfChange;
        return drawData4x4(diwne, getId(), m_numberOfVisibleDecimal, getDataItemsWidth(), m_floatPopupMode,
                                    m_nodebase->getData(0).getMat4(), {Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
                                                                       Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
                                                                       Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
                                                                       Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked} ,
                                    valueChanged, rowOfChange, columnOfChange, valueOfChange );
    }

    if (diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode || diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::DragNode)
    {
        dragedNode = diwne.getLastActiveNode<WorkspaceTransformation>();
        if (dragedNode != nullptr && (dragedNode->aboveSequence == 0 || dragedNode->aboveSequence == getIdDiwne())) /* only transformation can be in Sequence && not above other sequence */
        {
            position_of_draged_node_in_sequence = getInnerPosition( dragedNode->getInteractionPointsWithSequence() );
            if (position_of_draged_node_in_sequence >= 0)
            {
                dragedNode->aboveSequence = getIdDiwne(); /* reset in transformation beforeBegin */
            }
#ifdef WORKSPACE_DEBUG
            ImGui::Text(fmt::format("Draged node in Sequence: {}", position_of_draged_node_in_sequence).c_str());
#endif // WORKSPACE_DEBUG
        }
    }

    m_workspaceInnerTransformations.erase(std::remove_if(m_workspaceInnerTransformations.begin(), m_workspaceInnerTransformations.end(),
                                                          [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool { return std::dynamic_pointer_cast<WorkspaceTransformation>(node)->getRemoveFromSequence();}
                                                          ),
                                            m_workspaceInnerTransformations.end());

    int i = 0, push_index = -1;
    for( auto const & transformation : m_workspaceInnerTransformations )
    {
        if(position_of_draged_node_in_sequence == i)
        {
            ImGui::Dummy(m_sizeOfDummy*diwne.getWorkAreaZoom());
            ImGui::SameLine();
            if (dragedNode->bypassUnholdAction())
            {
                push_index = i;
            }
        }

        /* \todo some better selected transformation/nodes politic (when dragging, pushing, poping) -> use dynamic_cast<WorkspaceDiwne&>(diwne) and mark action to do and in WorkspaceDiwne react to this action  */
        transformation->setSelected(false);

        inner_interaction_happen |= transformation->drawNodeDiwne<WorkspaceTransformation>(DIWNE::DrawModeNodePosition::OnCoursorPosition, m_drawMode);
        ImGui::SameLine();

        i++;
    }
    if ( i==0 || position_of_draged_node_in_sequence == i ) /* add dummy after last inner or if empty */
    {
        ImGui::Dummy(position_of_draged_node_in_sequence == i ? m_sizeOfDummy*diwne.getWorkAreaZoom() : m_sizeOfDummy*diwne.getWorkAreaZoom()/2 ); /* smaller dummy if dragged node is not over Sequence */
        if (dragedNode && dragedNode->bypassUnholdAction() && position_of_draged_node_in_sequence >= 0 )
        {
            push_index = i;
        }
    }

    if ( push_index >= 0 /*&& !dragedNode->isInSequence()*/) /* already in sequence if second drawing or other sequence */
    {
        moveNodeToSequence(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(dragedNode), push_index);
        inner_interaction_happen |= true;
    }

    /* pop NodeFrom Sequence */
    if ( m_drawMode == DIWNE::DrawMode::Interacting &&  diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode)
    {
        dragedNode = diwne.getLastActiveNode<WorkspaceTransformation>();
        if (dragedNode != nullptr) /* only transformation can be in Sequence*/
        {
            if (dragedNode->isInSequence() && dragedNode->getNodebaseSequence() == m_nodebase)
            {
                moveNodeToWorkspace(dragedNode);
                inner_interaction_happen |= true;
            }
        }
    }



    return inner_interaction_happen;
}

void WorkspaceSequence::setNumberOfVisibleDecimal(int value)
{
    for( auto const & transformation : m_workspaceInnerTransformations )
    {
        transformation->setNumberOfVisibleDecimal(value);
    }
}


int WorkspaceSequence::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



