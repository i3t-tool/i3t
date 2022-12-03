#include "WorkspaceSequence.h"
#include "../Windows/WorkspaceWindow.h"

WorkspaceSequence::WorkspaceSequence(DIWNE::Diwne& diwne,
                                     Ptr<Core::NodeBase> nodebase /*= Core::Builder::createSequence()*/,
                                     bool drawPins /*=true*/, bool isCameraSequence /*=false*/)
    : WorkspaceNodeWithCoreDataWithPins(diwne, nodebase, false), m_drawPins(drawPins),
      m_isCameraSequence(isCameraSequence)
{
}

bool WorkspaceSequence::allowDrawing() { return m_isCameraSequence || WorkspaceNodeWithCoreData::allowDrawing(); }

bool WorkspaceSequence::isSequence() { return true; }

void WorkspaceSequence::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
	                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

int WorkspaceSequence::getInnerPosition(std::vector<ImVec2> points)
{
	ImRect rect = getNodeRectDiwne();
	bool any_in = false;
	ImVec2 in_point;
	for (auto const& point : points)
	{
		if (rect.Contains(point))
		{
			in_point = point;
			any_in = true;
		}
	}

	if (!any_in)
		return -1;

	rect.Max.x = rect.Min.x; /* squeeze rect at begin -> then in cycle shift rect
	                            and check point position */
	int i = 0;
	for (auto const& innerNode : getInnerWorkspaceNodes())
	{
		rect.Max.x = innerNode->getNodeRectDiwne().GetCenter().x;
		if (rect.Contains(in_point))
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

	auto node_iter =
	    std::find_if(m_workspaceInnerTransformations.begin(), m_workspaceInnerTransformations.end(),
	                 [node](Ptr<WorkspaceNodeWithCoreData> const& in_node) -> bool { return node == in_node; });

	if (node_iter != m_workspaceInnerTransformations.end())
	{
		int index = node_iter - m_workspaceInnerTransformations.begin();
		std::dynamic_pointer_cast<WorkspaceTransformation>(*node_iter)->setRemoveFromSequence(true);
		//		m_workspaceInnerTransformations.erase(node_iter); is done in next
		// frame based on setRemoveFromSequence(true)
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
		m_nodebase->as<Core::Sequence>()->addMatrix(node_t->getNodebase()->as<Core::Transformation>(), index);
	}
}

void WorkspaceSequence::moveNodeToSequence(Ptr<WorkspaceNodeWithCoreData> dragedNode, int index)
{
	pushNode(dragedNode, index); /*\ todo JH check if push is OK -> if not, not
	                                remove node from vector in window*/
	dragedNode->setRemoveFromWorkspace(true);
}

void WorkspaceSequence::moveNodeToWorkspace(Ptr<WorkspaceNodeWithCoreData> node)
{
	node->setRemoveFromWorkspace(false);
	dynamic_pointer_cast<WorkspaceTransformation>(node)->setRemoveFromSequence(true);
	//	node->m_selectable = true;
	dynamic_cast<WorkspaceDiwne&>(diwne).m_workspaceCoreNodes.push_back(node);
	//	popNode(node);
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const
{
	return m_workspaceInnerTransformations;
}

void WorkspaceSequence::setPostionOfDummyData(int positionOfDummyData)
{
	m_position_of_dummy_data = positionOfDummyData;
}

bool WorkspaceSequence::beforeContent()
{
	/* whole node background */

	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBgTransformation),
	                         I3T::getSize(ESize::Nodes_Sequence_Rounding), ImDrawCornerFlags_Top);
	return false;
}

bool WorkspaceSequence::afterContent()
{
	//    for (WorkspaceCoreInputPin & inputPin : getInputs())
	//    {
	//        if
	//        (inputPin.m_connection_changed){dynamic_cast<WorkspaceDiwne&>(diwne).m_reconnectCameraToSequence
	//        = true;}
	//    }
	return false;
}

bool WorkspaceSequence::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeaderTranformation),
	                         I3T::getSize(ESize::Nodes_Sequence_Rounding), ImDrawCornerFlags_Top);

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

	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		bool valueChanged = false;
		int rowOfChange, columnOfChange;
		float valueOfChange;
		return drawData4x4(

		    diwne, getId(), m_numberOfVisibleDecimal, getDataItemsWidth(), m_floatPopupMode,
		    m_nodebase->getData(0).getMat4() /* \todo JM \todo HM better selection (index) of data*/,
		    {Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		     Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		     Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		     Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked},
		    valueChanged, rowOfChange, columnOfChange, valueOfChange);
	}

	if (diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode ||
	    diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::DragNode)
	{
		dragedNode = diwne.getLastActiveNode<WorkspaceTransformation>();
		if (dragedNode != nullptr && (dragedNode->aboveSequence == 0 ||
		                              dragedNode->aboveSequence == getIdDiwne())) /* only transformation can be in Sequence
		                                                                             && not above other sequence */
		{
			position_of_draged_node_in_sequence = getInnerPosition(dragedNode->getInteractionPointsWithSequence());
			if (position_of_draged_node_in_sequence >= 0)
			{
				dragedNode->aboveSequence = getIdDiwne(); /* reset in transformation beforeBegin */
			}
#ifdef WORKSPACE_DEBUG
			ImGui::Text(fmt::format("Draged node in Sequence: {}", position_of_draged_node_in_sequence).c_str());
#endif // WORKSPACE_DEBUG
		}
	}

	// Better deleting from Sequence -> popMatrix() in popNode() is crucial
	m_workspaceInnerTransformations.erase(
	    std::remove_if(m_workspaceInnerTransformations.begin(), m_workspaceInnerTransformations.end(),
	                   [this](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool
	                   {
		                   bool remove_from_seq =
		                       std::dynamic_pointer_cast<WorkspaceTransformation>(node)->getRemoveFromSequence();
		                   if (remove_from_seq)
		                   {
			                   popNode(node);
		                   }
		                   return remove_from_seq;
	                   }),
	    m_workspaceInnerTransformations.end());

	int i = 0, push_index = -1;
	bool interaction_with_transformation_happen = false;
	for (auto const& transformation : m_workspaceInnerTransformations)
	{
		if (position_of_draged_node_in_sequence == i)
		{
			ImGui::Dummy(I3T::getSize(ESizeVec2::Nodes_Sequence_DummySpaceSize) * diwne.getWorkAreaZoom());
			ImGui::SameLine();
			if (dragedNode->bypassUnholdAction())
			{
				push_index = i;
			}
		}
		/* with no selection manipulator not work
		    transformation->m_selectable = false;
		    transformation->setSelected(false);
		*/

		/* \todo some better selected transformation/nodes politic (when dragging,
		 * pushing, poping) -> use dynamic_cast<WorkspaceDiwne&>(diwne) and mark
		 * action to do and in WorkspaceDiwne react to this action  */
		interaction_with_transformation_happen |= transformation->drawNodeDiwne<WorkspaceTransformation>(
		    DIWNE::DrawModeNodePosition::OnCoursorPosition,
		    m_isHeld || m_drawMode == DIWNE::DrawMode::JustDraw ? DIWNE::DrawMode::JustDraw : DIWNE::DrawMode::Interacting);

		ImGui::SameLine();

		i++;
	}
	if (interaction_with_transformation_happen)
	{
		inner_interaction_happen = false; /* do not set sequence as interacting node
		              -> selected transformation should be only interacting */
	}
	if (i == 0 || position_of_draged_node_in_sequence == i) /* add dummy after last inner or if empty */
	{
		ImGui::Dummy(position_of_draged_node_in_sequence == i
		                 ? I3T::getSize(ESizeVec2::Nodes_Sequence_DummySpaceSize) * diwne.getWorkAreaZoom()
		                 : I3T::getSize(ESizeVec2::Nodes_Sequence_DummySpaceSize) * diwne.getWorkAreaZoom() /
		                       2); /* smaller dummy if dragged node is not over Sequence */
		if (dragedNode && dragedNode->bypassUnholdAction() && position_of_draged_node_in_sequence >= 0)
		{
			push_index = i;
		}
	}

	if (push_index >= 0)
	{
		moveNodeToSequence(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(dragedNode), push_index);
		inner_interaction_happen |= true;
	}

	/* pop NodeFrom Sequence */
	if (m_drawMode == DIWNE::DrawMode::Interacting && diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode)
	{
		dragedNode = diwne.getLastActiveNode<WorkspaceTransformation>();
		if (dragedNode != nullptr) /* only transformation can be in Sequence*/
		{
			if (dragedNode->isInSequence() && dragedNode->getNodebaseSequence() == m_nodebase)
			{
				moveNodeToWorkspace(dragedNode);
				inner_interaction_happen = false; /* do not set sequence as interacting node -> draged
				                                     transformation should be only interacting */
			}
		}
	}

	return inner_interaction_happen;
}

void WorkspaceSequence::setNumberOfVisibleDecimal(int value)
{
	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		m_numberOfVisibleDecimal = value;
		setDataItemsWidth();
	}
	else
	{
		for (auto const& transformation : m_workspaceInnerTransformations)
		{
			transformation->setNumberOfVisibleDecimal(value);
		}
	}
}

int WorkspaceSequence::maxLenghtOfData()
{
	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		return maxLenghtOfData4x4(m_nodebase->getData(0).getMat4() /*\todo JM HM better selection (index) of data*/,
		                          m_numberOfVisibleDecimal);
	}
	return 0;
}
