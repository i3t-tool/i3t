#include "WorkspaceSequence.h"
#include "../Windows/WorkspaceWindow.h"

WorkspaceSequence::WorkspaceSequence(Ptr<Core::NodeBase> nodebase/*= Core::Builder::createSequence()*/)
    :   WorkspaceNodeWithCoreData(nodebase)
{
        const auto& inputPins   = m_nodebase->getInputPins();
        const auto& outputPins  = m_nodebase->getOutputPins();

        m_workspaceInputs.reserve(inputPins.size());
        m_workspaceOutputs.reserve(outputPins.size());

        for (Core::Pin const& pin : inputPins)
        {
            if(pin.getType() == EValueType::MatrixMul)
            {
                m_workspaceInputs.push_back(
                    std::make_unique<WorkspaceCoreInputPinMatrixMul>(    pin.getId()
                                                                    ,   pin
                                                                    ,   *this));
            }else
            {
                m_workspaceInputs.push_back(
                    std::make_unique<WorkspaceCoreInputPin>(    pin.getId()
                                                            ,   pin
                                                            ,   *this));
            }

        }

        for (Core::Pin const& pin : outputPins)
        {
            switch (pin.getType())
            {
                case EValueType::Matrix:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinMatrix4x4>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Vec4:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinVector4>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Vec3:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinVector3>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Float:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinFloat>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Quat:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinQuaternion>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Pulse:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinPulse>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::MatrixMul:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinMatrixMul>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Screen:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinScreen>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Ptr:
                    /* Pin with type Ptr have no graphic representation */
                    break;
                default:
                    Debug::Assert(false , "Unknown Pin type in Sequence constructor");
            }

        }
    }

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
                                    [node](Ptr<WorkspaceNodeWithCoreData> const &in_node) -> bool { return node == in_node; }); /* \todo check adress of nodes directly */

    if (node_iter != m_workspaceInnerTransformations.end())
    {
        int index = node_iter-m_workspaceInnerTransformations.begin();
        m_workspaceInnerTransformations.erase(node_iter);
        m_nodebase->as<Core::Sequence>()->popMatrix(index);
    }
}

void WorkspaceSequence::pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index)
{
    Ptr<WorkspaceTransformation> node_t = std::dynamic_pointer_cast<WorkspaceTransformation>(node);
    if(node_t != nullptr && 0 <= index && index <= m_workspaceInnerTransformations.size())
    {
        node_t->setRemoveFromSequence(false);
        m_workspaceInnerTransformations.insert(m_workspaceInnerTransformations.begin()+index, node_t);
        /* \tod JH check return value if so */
        m_nodebase->as<Core::Sequence>()->addMatrix(node_t->getNodebase()->as<Core::Transformation>(), index);
    }
}

void WorkspaceSequence::moveNodeToSequenceWithCheck(DIWNE::Diwne &diwne, Ptr<WorkspaceNodeWithCoreData> dragedNode, int index)
{
    if (ImGui::IsMouseReleased(0))
    {
        pushNode(dragedNode, index); /*\ todo JH check if push is OK -> if not, not remove node from vector in window*/
        dragedNode->setRemoveFromWorkspaceWindow(true);
//        std::vector<Ptr<WorkspaceNodeWithCoreData>> &ww_nodes = static_cast<WorkspaceWindow&>(diwne).m_workspaceCoreNodes;
//        ww_nodes.erase(std::remove_if(ww_nodes.begin(),
//                                      ww_nodes.end(),
//                                      [this](Ptr<WorkspaceNodeWithCoreData> node){return node.get() == this;}),
//                       ww_nodes.end());
    }
}


std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const  { return m_workspaceInnerTransformations; }

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

bool WorkspaceSequence::middleContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
    int position_of_dummy_data = -1; /* -1 means not in Sequence */
    Ptr<WorkspaceTransformation> dragedNode;

    if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
    {
        diwne.AddRectFilledDiwne(m_middleRectDiwne.Min, m_middleRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().getBg())); /* \todo JH Sequence background from settings*/

        ImGui::TextUnformatted(m_middleLabel.c_str());
        return false;
    }

    if (diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode)
    {
        dragedNode = std::dynamic_pointer_cast<WorkspaceTransformation>(diwne.m_draged_node);
        if (dragedNode != nullptr) /* only transformation can be in Seqeuence*/
        {
            position_of_dummy_data = getInnerPosition( dragedNode->getInteractionPointsWithSequence() );
        }
    }

    int i = 0, push_index = -1, max = m_workspaceInnerTransformations.size()-1;
    for( auto const & transformation : m_workspaceInnerTransformations )
    {
        if(position_of_dummy_data == i)
        {
            ImGui::Dummy(m_sizeOfDummy); /* \todo JH size of dummy from settings */
            ImGui::SameLine();
            if (ImGui::IsMouseReleased(0))
            {
                push_index = i;
            }
        }

        inner_interaction_happen |= transformation->drawNodeDiwne(diwne, true);
        ImGui::SameLine();

        i++;
    }
    if (position_of_dummy_data == i) /* add dummy after last inner */
    {
        ImGui::Dummy(m_sizeOfDummy);
        if (ImGui::IsMouseReleased(0))
        {
            push_index = i;
        }
    }
    if (push_index >= 0)
    {
        moveNodeToSequenceWithCheck(diwne, std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(dragedNode), push_index);
    }

    return inner_interaction_happen;
}

bool WorkspaceSequence::leftContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
    for (auto const& pin : m_workspaceInputs) {
        inner_interaction_happen |= pin->drawPinDiwne(diwne);
        if (pin->isConnected())
        {
            Ptr<WorkspaceCoreInputPin> in = std::dynamic_pointer_cast<WorkspaceCoreInputPin>(pin);
            WorkspaceCoreLink * lin = &(in->getLink());
            inner_interaction_happen |= lin->drawLinkDiwne(diwne);
        }
    }
    return inner_interaction_happen;
}

bool WorkspaceSequence::rightContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
    for (auto const& pin : m_workspaceOutputs) {
        inner_interaction_happen |= pin->drawPinDiwne(diwne);
    }
    return inner_interaction_happen;
}

int WorkspaceSequence::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



