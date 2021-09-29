#include "WorkspaceSequence.h"

WorkspaceSequence::WorkspaceSequence(Ptr<Core::NodeBase> nodebase)
    :   WorkspaceNodeWithCoreData(nodebase)

{}

bool WorkspaceSequence::isSequence()
{
    return true;
}

int WorkspaceSequence::getInnerPosition(ImVec2 point)
{
    ImRect rect = getNodeRectDiwne();
    if (!rect.Contains(point))
    {
        return -1;
    }

    rect.Max.x = rect.Min.x; /* squeeze rect at begin -> then in cycle shift rect and check point position */
    int i = 0;
    for (auto const & innerNode : getInnerWorkspaceNodes())
    {
        rect.Max.x = innerNode->getNodeRectDiwne().GetCenter().x;
        if(rect.Contains(point))
        {
            return i;
        }
        rect.Min.x = rect.Max.x;
        i++;
    }
    return i;
}

int WorkspaceSequence::getInnerPosition(std::vector<ImVec2> points)
{
    int position = -1; /* position of first found point that match */
    for(auto const & point : points)
    {
        position = getInnerPosition(point);
        if (position > -1){return position;}
    }
    return position;
}

void WorkspaceSequence::popNode(Ptr<WorkspaceNodeWithCoreData> node)
{

    auto node_iter = std::find_if(  m_workspaceTransformation.begin(),
                                    m_workspaceTransformation.end(),
                                    [node](Ptr<WorkspaceNodeWithCoreData> const &in_node) -> bool { return node->getId() == in_node->getId(); }); /* \todo check adress of nodes directly */

    if (node_iter != m_workspaceTransformation.end())
    {
        int index = node_iter-m_workspaceTransformation.begin();
        m_workspaceTransformation.erase(node_iter);
        m_nodebase->as<Core::Sequence>()->popMatrix(index);
    }
}

void WorkspaceSequence::pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index)
{
    if(node->isTransformation() && 0 <= index && index <= m_workspaceTransformation.size())
    {
        m_workspaceTransformation.insert(m_workspaceTransformation.begin()+index, node);
        m_nodebase->as<Core::Sequence>()->addMatrix(node->getNodebase()->as<Core::Transformation>(), index);
    }
}


std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const  { return m_workspaceTransformation; }

void WorkspaceSequence::setPostionOfDummyData(int positionOfDummyData) {m_position_of_dummy_data = positionOfDummyData;}

//void WorkspaceSequence::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
//{
//    builder.Begin(m_id);
//	drawHeader(builder);
//	drawInputs(builder, newLinkPin);
//    drawData(builder,0);
//	drawOutputs(builder, newLinkPin);
//	builder.End();
//
//        ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(I3T::getSize(ESizeVec2::Nodes_IconSize).x+5,builder.HeaderMax.y-builder.HeaderMin.y+1);  /* \todo JH add shift based on size of header and inputs pins */
//        m_dataRect = ImRect(dataLeftTop, dataLeftTop);
//
//        int i = 0;
//        for( auto const & transformation : m_workspaceTransformation )
//        {
//            if(m_position_of_dummy_data == i)
//            {
//                ne::SetNodePosition(transformation->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
//            }else
//            {
//                ne::SetNodePosition(transformation->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
//            }
//
//            Theme& t = I3T::getTheme();
//            t.transformationColorTheme();
//
//            transformation->drawNode(builder, nullptr, false);
//
//						ImVec2 sizeofNode = ImGui::GetItemRectSize();
//						float y = sizeofNode.y - 4* I3T::getSize(ESizeVec2::Nodes_IconSize).y +1;
//						float height = m_dataRect.Max.y - m_dataRect.Min.y;
//						if(y > height || i == 0){
//
//							m_dataRect.Max.y += y - height;
//						}
//						m_dataRect.Max.x += sizeofNode.x;
//
//
//            i++;
//        }
//        if (m_position_of_dummy_data == i) /* add dummy after last inner */
//        {
//            m_dataRect.Add(m_dataRect.Max + ImVec2(m_widthOfDummy, 0));
//        }
//}

bool WorkspaceSequence::drawDataFull(DIWNE::Diwne &diwne, int index)
{
    bool inner_interaction_happen = false;

    int i = 0;
    for( auto const & transformation : m_workspaceTransformation )
    {
        if(m_position_of_dummy_data == i)
        {
            ImGui::Dummy(m_sizeOfDummy);
        }

        inner_interaction_happen |= transformation->drawNodeDiwne(diwne);

        i++;
    }
    if (m_position_of_dummy_data == i) /* add dummy after last inner */
    {
        ImGui::Dummy(m_sizeOfDummy);
    }

    return inner_interaction_happen;
}

int WorkspaceSequence::maxLenghtOfData(int index)
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



