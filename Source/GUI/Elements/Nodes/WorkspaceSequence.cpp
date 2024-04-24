/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "WorkspaceSequence.h"

#include "../Windows/WorkspaceWindow.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

WorkspaceSequence::WorkspaceSequence(DIWNE::Diwne& diwne,
                                     Ptr<Core::Node> nodebase /*= Core::GraphManager::createSequence()*/,
                                     /* bool drawPins, - (PF) was not used, remove */ /*=true*/
                                     bool isCameraSequence /*=false*/)
    : WorkspaceNodeWithCoreDataWithPins(diwne, nodebase, false),
      /* m_drawPinsNOT_USED(drawPins), \todo (PF) was not used - remove*/
      m_isCameraSequence(isCameraSequence)
{
	updateDataItemsWidth();
	m_headerMinWidth = 120;
}

bool WorkspaceSequence::allowDrawing()
{
	return m_isCameraSequence || WorkspaceNodeWithCoreData::allowDrawing();
}

bool WorkspaceSequence::isSequence()
{
	return true;
}

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
	auto node_iter = std::find_if(m_workspaceInnerTransformations.begin(), m_workspaceInnerTransformations.end(),
	                              [node](auto innerNode) -> bool {
		                              return node == innerNode;
	                              });

	if (node_iter != m_workspaceInnerTransformations.end())
	{
		int index = node_iter - m_workspaceInnerTransformations.begin();
		std::shared_ptr<WorkspaceTransformation> transformation =
		    std::dynamic_pointer_cast<WorkspaceTransformation>(*node_iter);
		transformation->setRemoveFromSequence(true);
		transformation->m_parentSequence.reset();
		// is done in next frame based on setRemoveFromSequence(true)
		// m_workspaceInnerTransformations.erase(node_iter);
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
		m_nodebase->as<Core::Sequence>()->pushMatrix(node_t->getNodebase()->as<Core::Transform>(), index);
		node_t->m_parentSequence = std::static_pointer_cast<WorkspaceSequence>(shared_from_this());
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

std::optional<Ptr<WorkspaceNodeWithCoreData>> WorkspaceSequence::getTransform(int index) const
{
	if (index >= m_workspaceInnerTransformations.size())
		return std::nullopt;

	return m_workspaceInnerTransformations[index];
}

void WorkspaceSequence::setPostionOfDummyData(int positionOfDummyData)
{
	m_position_of_dummy_data = positionOfDummyData;
}

void WorkspaceSequence::popupContentTracking()
{
	auto& workspaceDiwne = static_cast<WorkspaceDiwne&>(diwne);
	if (Core::GraphManager::isTrackingEnabled() &&
	    workspaceDiwne.tracking->getSequence()->getId() == this->getNodebase()->getId())
	{
		if (ImGui::MenuItem("Stop tracking", ""))
		{
			workspaceDiwne.trackingSwitchOff();
		}
		if (ImGui::MenuItem("Smooth tracking", "", workspaceDiwne.smoothTracking, true))
		{
			workspaceDiwne.trackingModeSwitch();
		}
	}
	else
	{
		if (ImGui::MenuItem("Start tracking from right", ""))
		{
			if (Core::GraphManager::isTrackingEnabled())
			{
				workspaceDiwne.trackingSwitchOff();
			}

			workspaceDiwne.trackingSwitchOn(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()), true);
		}
		if (ImGui::MenuItem("Start tracking from left", ""))
		{
			if (Core::GraphManager::isTrackingEnabled())
			{
				workspaceDiwne.trackingSwitchOff();
			}

			workspaceDiwne.trackingSwitchOn(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()), false);
		}
	}
}

void WorkspaceSequence::popupContent()
{
	WorkspaceNodeWithCoreDataWithPins::drawMenuSetEditable();

	ImGui::Separator();

	popupContentTracking();

	ImGui::Separator();

	drawMenuSetPrecision();
	drawMenuLevelOfDetail();

	ImGui::Separator();

	WorkspaceNodeWithCoreData::drawMenuDuplicate();

	ImGui::Separator();

	WorkspaceNode::popupContent();
}

bool WorkspaceSequence::beforeContent()
{
	/* whole node background */

	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max,
	                         I3T::getTheme().get(EColor::NodeBgTransformation),
	                         I3T::getSize(ESize::Nodes_Sequence_Rounding), ImDrawFlags_RoundCornersAll);
	return false;
}

bool WorkspaceSequence::afterContent()
{
	if (!Core::GraphManager::isTrackingEnabled())
	{
		return false;
	}

	auto& workspaceDiwne = static_cast<WorkspaceDiwne&>(diwne);

	ImVec2 topleft = m_middleRectDiwne.Min;
	ImVec2 bottomright = m_middleRectDiwne.Max;
	bool trackingFromLeft = workspaceDiwne.m_trackingFromLeft;

	Core::TrackingResult t = workspaceDiwne.tracking->result();

	ImVec2 size = bottomright - topleft;
	float inactiveMark = t.trackingProgress[this->getId()];

	if (!trackingFromLeft && (inactiveMark != 0 || this->getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected()))
	{
		bottomright.x = topleft.x;
		bottomright.x += (1 - inactiveMark) * size.x;
		diwne.AddRectFilledDiwne(topleft, bottomright, I3T::getColor(EColor::Nodes_Transformation_TrackingColor));
	}
	else if (inactiveMark != 0 || this->getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{ // Left tracking, top left moving left
		topleft.x = bottomright.x;
		topleft.x -= (1 - inactiveMark) * size.x;
		diwne.AddRectFilledDiwne(topleft, bottomright, I3T::getColor(EColor::Nodes_Transformation_TrackingColor));
	}
	else
	{
		return false;
	}

	auto maybeInterpolatedSequence = findNodeById(workspaceDiwne.getAllNodes(), t.interpolatedTransformID);

	if (!maybeInterpolatedSequence)
	{
		maybeInterpolatedSequence =
		    findNodeById(workspaceDiwne.getAllNodes(), t.transformInfo[t.interpolatedTransformID].sequence->getId());
	}

	if (!maybeInterpolatedSequence)
	{
		Core::GraphManager::stopTracking();
		return false;
	}

	auto interpolatedSequence = std::dynamic_pointer_cast<WorkspaceSequence>(maybeInterpolatedSequence.value());
	if (interpolatedSequence.get() == this)
	{
		ImVec2 markCenter = ImVec2(!trackingFromLeft ? bottomright.x : topleft.x, m_middleRectDiwne.GetCenter().y);
		ImVec2 markSize = ImVec2(I3T::getSize(ESize::Nodes_Transformation_TrackingMarkSize), topleft.y - bottomright.y);

		diwne.AddRectFilledDiwne(markCenter - markSize / 2, markCenter + markSize / 2,
		                         I3T::getColor(EColor::Nodes_Transformation_TrackingMarkColor));
	}

	return false;
}

bool WorkspaceSequence::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
	                         I3T::getTheme().get(EColor::NodeHeaderTranformation) * m_tint,
	                         I3T::getSize(ESize::Nodes_Sequence_Rounding), ImDrawFlags_RoundCornersTop);

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

	const auto matrixInput = getInputs().at(Core::I3T_SEQ_IN_MAT);
	if (matrixInput->isConnected())
	{
		if (matrixInput->connectionChanged())
		{
			// Ensure that width is recalculated the first time data is shown prompted by a new input connection
			updateDataItemsWidth();
		}
		bool valueChanged = false;
		int rowOfChange, columnOfChange;
		float valueOfChange;
		const auto inputMatrix = m_nodebase->getInput(Core::I3T_SEQ_IN_MAT).data().getMat4();

		return drawData4x4(diwne, getId(), m_numberOfVisibleDecimal, getDataItemsWidth(), m_floatPopupMode, inputMatrix,
		                   {Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                    Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                    Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                    Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                    Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                    Core::EValueState::Locked},
		                   valueChanged, rowOfChange, columnOfChange, valueOfChange);
	}

	if (diwne.getDiwneAction() == DIWNE::DiwneAction::DragNode ||
	    diwne.getDiwneActionPreviousFrame() == DIWNE::DiwneAction::DragNode)
	{
		dragedNode = diwne.getLastActiveNode<WorkspaceTransformation>();
		if (dragedNode != nullptr &&
		    (dragedNode->aboveSequence == 0 ||
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

	// Check if transform is marked for removal.
	{
		auto it = m_workspaceInnerTransformations.begin();
		while (it != m_workspaceInnerTransformations.end())
		{
			bool removeFromSeq = std::dynamic_pointer_cast<WorkspaceTransformation>(*it)->getRemoveFromSequence();
			if (removeFromSeq)
			{
				popNode(*it);
				it = m_workspaceInnerTransformations.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	//
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
		    DIWNE::DrawModeNodePosition::OnCursorPosition, m_isHeld || m_drawMode == DIWNE::DrawMode::JustDraw
		                                                       ? DIWNE::DrawMode::JustDraw
		                                                       : DIWNE::DrawMode::Interacting);

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
		updateDataItemsWidth();
	}
	else
	{
		for (auto const& transformation : m_workspaceInnerTransformations)
		{
			transformation->setNumberOfVisibleDecimal(value);
		}
	}
}

int WorkspaceSequence::maxLengthOfData()
{
	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		return maxLengthOfData4x4(m_nodebase->data(0).getMat4() /*\todo JM HM better selection (index) of data*/,
		                          m_numberOfVisibleDecimal);
	}
	return 0;
}
