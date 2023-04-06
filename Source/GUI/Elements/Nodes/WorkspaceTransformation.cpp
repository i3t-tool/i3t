#include "WorkspaceTransformation.h"
#include "Tools.h"

#include "../Windows/WorkspaceWindow.h"
#include "State/StateManager.h"

WorkspaceTransformation::WorkspaceTransformation(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase)
    : WorkspaceNodeWithCoreData(diwne, nodebase), aboveSequence(0), m_topOversizeSpace(0)
{
	setDataItemsWidth();
}

bool WorkspaceTransformation::allowDrawing() { return isInSequence() || WorkspaceNodeWithCoreData::allowDrawing(); }

void WorkspaceTransformation::updateSizes()
{
	/* right align - have to be computed before DIWNE::Node::updateSizes(),
	 * because after that are sizes updated */
	m_topOversizeSpace = std::max(
	    0.0f,
	    m_topRectDiwne.GetWidth() -
	        std::max(m_leftRectDiwne.GetWidth() + m_middleRectDiwne.GetWidth() + m_rightRectDiwne.GetWidth() +
	                     ImGui::GetStyle().ItemSpacing.x * 2 / diwne.getWorkAreaZoom() /* space is between left-middle
	                                                                                      and middle-right, spacing is
	                                                                                      scaled in begin of frame */
	                 ,
	                 m_bottomRectDiwne.GetWidth()));
	WorkspaceNodeWithCoreData::updateSizes();
}
bool WorkspaceTransformation::beforeBegin()
{
	aboveSequence = 0; /* 0 is none */
	return WorkspaceNodeWithCoreData::beforeBegin();
}

bool WorkspaceTransformation::beforeContent()
{
	/* whole node background */
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBgTransformation),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);
	return false;
}

bool WorkspaceTransformation::topContent()
{
	bool interaction_happen = false;
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeaderTranformation),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);

	interaction_happen |= WorkspaceNodeWithCoreData::topContent();
	ImGui::SameLine();

	if (!isMatrixValid())
	{
		ImVec2 iconSize = ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize());
		//     \todo JH Right Align
		//    ImGui::SetCursorPosX(ImGui::GetCursorPosX()-ImGui::GetStyle().ItemSpacing.x-1);
		//    /* remove spacing after Dummy in
		//    WorkspaceNodeWithCoreData::topContent() */
		//    /* right align */
		//    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max(0.0f,
		//    getNodeRectDiwne().Max.x - diwne.screen2diwne(ImGui::GetCursorPos()).x
		//    /*actual free space*/ - iconSize.x -
		//    m_topOversizeSpace)*diwne.getWorkAreaZoom());

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
		diwne.DrawIcon(DIWNE::IconType::Circle, I3T::getColor(EColor::Nodes_Transformation_ValidIcon_bgShape),
		               I3T::getColor(EColor::Nodes_Transformation_ValidIcon_bgInner), DIWNE::IconType::Cross,
		               I3T::getColor(EColor::Nodes_Transformation_ValidIcon_fgShape),
		               I3T::getColor(EColor::Nodes_Transformation_ValidIcon_fgInner), iconSize,
		               I3T::getColor(EColor::Nodes_Transformation_ValidIcon_padding), false);

		// case Core::ETransformState::Unknown:
		//	diwne.DrawIcon(DIWNE::IconType::Circle, ImColor(255, 0, 255),
		// ImColor(255, 0, 255), DIWNE::IconType::Cross, 	               ImColor(0,
		// 255, 255), ImColor(0, 255, 255), iconSize, ImVec4(5, 5, 5, 5),
		// false); /* \todo JH Icon setting from Theme? */
	}
	return interaction_happen;
}

bool WorkspaceTransformation::middleContent()
{
	bool inner_interaction_happen = false;

	switch (m_levelOfDetail)
	{
	case WorkspaceLevelOfDetail::Full:
		inner_interaction_happen = drawDataFull();
		break;
	case WorkspaceLevelOfDetail::SetValues:
		inner_interaction_happen = drawDataSetValues();
		break;
	case WorkspaceLevelOfDetail::Label:
		inner_interaction_happen = drawDataLabel();
		break;

	default:
		I3T_ABORT("drawData: Unknown m_levelOfDetail");
		inner_interaction_happen = drawDataFull();
	}

	return inner_interaction_happen;
}

bool WorkspaceTransformation::afterContent()
{
	if (Core::GraphManager::isTrackingEnabled())
	{
		ImVec2 topleft = m_middleRectDiwne.Min;
		ImVec2 bottomright = m_middleRectDiwne.Max;
		bool trackingFromLeft = dynamic_cast<WorkspaceDiwne&>(diwne).m_trackingFromLeft;

		ImVec2 size = bottomright - topleft;
		float inactiveMark = this->getNodebase()->as<Core::Transformation>()->getActivePart();
		if (trackingFromLeft)
		{
			if (inactiveMark != 1) // Left tracking
			{
				bottomright.x -= (inactiveMark)*size.x;
				diwne.AddRectFilledDiwne(topleft, bottomright, ImColor(0.f, 0.f, 0.f, 0.3f));
			}
			else if (inactiveMark == 1)
			{ // accounting for yellow mark placement
				bottomright.x -= (inactiveMark)*size.x;
			}
		}
		else if (inactiveMark != 0) // RIGHT TRACKING
		{
			topleft.x += (1 - inactiveMark) * size.x;
			diwne.AddRectFilledDiwne(topleft, bottomright, ImColor(0.f, 0.f, 0.f, 0.3f));
		}
		else if (inactiveMark == 0)
		{
			topleft.x += (1 - inactiveMark) * size.x;
		}

		if (std::dynamic_pointer_cast<WorkspaceTransformation>(
		        findNodeById(dynamic_cast<WorkspaceDiwne&>(diwne).getAllNodesInnerIncluded(),
		                     dynamic_cast<WorkspaceDiwne&>(diwne).tracking->getInterpolatedTransformID())
		            .value())
		        .get() == this)
		{
			ImVec2 markCenter = ImVec2(trackingFromLeft ? bottomright.x : topleft.x, m_middleRectDiwne.GetCenter().y);
			ImVec2 markSize = I3T::getSize(ESizeVec2::Nodes_Transformation_TrackingMarkSize);

			diwne.AddRectFilledDiwne(markCenter - markSize / 2, markCenter + markSize / 2,
			                         I3T::getColor(EColor::Nodes_Transformation_TrackingColor));
		}
	}

	return false;
}

void WorkspaceTransformation::popupContent()
{
	WorkspaceNodeWithCoreData::drawMenuSetEditable();

	ImGui::Separator();

	drawMenuSetDataMap();
	drawMenuStorevalues();

	ImGui::Separator();

	WorkspaceNodeWithCoreData::drawMenuSetPrecision();
	drawMenuLevelOfDetail();

	ImGui::Separator();

	WorkspaceNodeWithCoreData::drawMenuDuplicate();

	ImGui::Separator();

	WorkspaceNode::popupContent();
}

void WorkspaceTransformation::drawMenuStorevalues()
{
	if (ImGui::BeginMenu("Value"))
	{
		if (ImGui::MenuItem("Reset"))
		{
			m_nodebase->as<Core::Transformation>()->initDefaults();
		} // reset()
		if (ImGui::MenuItem("Store"))
		{
			m_nodebase->as<Core::Transformation>()->saveValue();
		}
		if (ImGui::MenuItem("Restore", nullptr, false, m_nodebase->as<Core::Transformation>()->hasSavedValue()))
		{
			m_nodebase->as<Core::Transformation>()->reloadValue();
		}

		ImGui::EndMenu();
	}
}

/// used for translate, axisAngle, lookAt, and perspective
void WorkspaceTransformation::drawMenuSetDataMap()
{
	bool fullLOD = (getLevelOfDetail() == WorkspaceLevelOfDetail::Full);
	bool enableLockMenuItem = fullLOD;
	bool enableSynergiesMenuItem =
	    fullLOD && (m_nodebase->as<Core::Transformation>()->hasMenuSynergies()); // only some transformations have
	                                                                             // the possibility to set synergies

	if (m_nodebase->as<Core::Transformation>()->isLocked())
	{
		if (ImGui::MenuItem("Unlock", NULL, false, enableLockMenuItem))
		{
			m_nodebase->as<Core::Transformation>()->unlock();
		}
	}
	else
	{
		if (ImGui::MenuItem("Lock", NULL, false, enableLockMenuItem))
		{
			m_nodebase->as<Core::Transformation>()->lock();
		}
	}

	if (m_nodebase->as<Core::Transformation>()->hasSynergies())
	{
		if (ImGui::MenuItem("Disable synergies", NULL, false, enableSynergiesMenuItem))
		{
			m_nodebase->as<Core::Transformation>()->disableSynergies();
		}
	}
	else
	{
		if (ImGui::MenuItem("Enable synergies", NULL, false, enableSynergiesMenuItem))
		{
			m_nodebase->as<Core::Transformation>()->enableSynergies();
		}
	}
}

void WorkspaceTransformation::deleteAction()
{
	m_removeFromWorkspaceWindow = true;
	m_removeFromSequence = true;
	m_parentSequence.reset();
}

void WorkspaceTransformation::drawMenuDelete()
{
	if (ImGui::MenuItem("Delete"))
	{
		deleteActionDiwne();
	}
}

std::vector<ImVec2> WorkspaceTransformation::getInteractionPointsWithSequence()
{
	ImVec2 position = getNodePositionDiwne();
	ImVec2 size = getNodeRectSizeDiwne();
	ImVec2 topMiddle = position;
	topMiddle.x += size.x / 2;
	ImVec2 middle = position + size / 2;
	ImVec2 bottomMiddle = topMiddle;
	bottomMiddle.y += size.y;
	return {topMiddle, middle, bottomMiddle};
}

bool WorkspaceTransformation::drawDataFull()
{
	bool valueChanged = false, interaction_happen = false;
	int rowOfChange, columnOfChange;
	float valueOfChange;

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::FloatBg)));

	interaction_happen = drawData4x4(diwne, getId(), m_numberOfVisibleDecimal, getDataItemsWidth(), m_floatPopupMode,
	                                 m_nodebase->getData().getMat4(),
	                                 {m_nodebase->as<Core::Transformation>()->getValueState({0, 0}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({1, 0}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({2, 0}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({3, 0}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({0, 1}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({1, 1}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({2, 1}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({3, 1}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({0, 2}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({1, 2}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({2, 2}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({3, 2}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({0, 3}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({1, 3}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({2, 3}),
	                                  m_nodebase->as<Core::Transformation>()->getValueState({3, 3})},
	                                 valueChanged, rowOfChange, columnOfChange, valueOfChange);

	ImGui::PopStyleColor();

	if (valueChanged)
	{
		m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
		//				/// TEST ///////////////////////////////////////////////// /* snap
		// is taken in the end of frame
		//*/ 				App::getModule<StateManager>().takeSnapshot();
		//				//////////////////////////////////////////////////////////
		setDataItemsWidth();
	}
	return interaction_happen;
}

int WorkspaceTransformation::maxLenghtOfData()
{
	return maxLenghtOfData4x4(m_nodebase->getData().getMat4(), m_numberOfVisibleDecimal);
}

bool WorkspaceTransformation::drawDataSetValues_InsideTablebuilder(
    std::vector<std::string> const& labels /* labels have to be unique in node - otherwise change label
                                              passed to drawDragFloatWithMap_Inline() below */
    ,
    std::vector<float*> const& local_data, bool& value_changed)
{
	int number_of_values = labels.size();
	I3T_ASSERT(number_of_values == local_data.size(), "drawDataSetValues_InsideTablebuilder: All vectors (labels, "
	                                                  "local_data) must have same size.");

	value_changed = false;
	bool inner_interaction_happen = false, actual_value_changed = false;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	ImGui::TableNextRow();

	for (int i = 0; i < number_of_values; i++)
	{
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(labels[i].c_str());
		ImGui::TableNextColumn();

		ImGui::PushItemWidth(getDataItemsWidth());
		inner_interaction_happen |= drawDragFloatWithMap_Inline(
		    diwne, getNumberOfVisibleDecimal(), getFloatPopupMode(), fmt::format("##{}:ch{}", m_labelDiwne, labels[i]),
		    *local_data[i],
		    m_nodebase->as<Core::Transformation>()->hasSynergies() ? Core::EValueState::EditableSyn
		                                                           : Core::EValueState::Editable,
		    actual_value_changed);
		value_changed |= actual_value_changed;
		ImGui::PopItemWidth();
	}

	ImGui::PopStyleVar(2);

	return inner_interaction_happen;
}

bool WorkspaceTransformation::drawDataSetValuesTable_builder(std::string const cornerLabel,
                                                             std::vector<std::string> const& columnLabels,
                                                             std::vector<std::string> const& rowLabels,
                                                             std::vector<float*> const& local_data, bool& value_changed,
                                                             int& index_of_change)
{
	int number_of_values = columnLabels.size() * rowLabels.size();
	I3T_ASSERT(number_of_values == local_data.size(), "drawDataSetValuesTable_builder: columnLabels.size() * "
	                                                  "rowLabels.size() must be equal to local_data.size()");

	value_changed = false;
	bool inner_interaction_happen = false, actual_value_changed = false;

	if (ImGui::BeginTable(fmt::format("##{}{}", cornerLabel, getIdDiwne()).c_str(), columnLabels.size() + 1,
	                      ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));
		/* header labels */
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(cornerLabel.c_str());
		for (int columns = 0; columns < columnLabels.size(); columns++)
		{
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(columnLabels[columns].c_str());
		}

		for (int rows = 0; rows < rowLabels.size(); rows++)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(rowLabels[rows].c_str());

			for (int columns = 0; columns < columnLabels.size(); columns++)
			{
				ImGui::TableNextColumn();

				ImGui::PushItemWidth(getDataItemsWidth());
				inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), getFloatPopupMode(),
				                                                        fmt::format("##{}:r{}c{}", m_labelDiwne, rows, columns),
				                                                        *(local_data[rows * columnLabels.size() + columns]),
				                                                        m_nodebase->as<Core::Transformation>()->hasSynergies()
				                                                            ? Core::EValueState::EditableSyn
				                                                            : Core::EValueState::Editable,
				                                                        actual_value_changed);

				ImGui::PopItemWidth();
				if (actual_value_changed)
				{
					index_of_change = rows * columnLabels.size() + columns;
					value_changed |= actual_value_changed;
				}
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::EndTable();
	}

	return inner_interaction_happen;
}

bool WorkspaceTransformation::isInSequence() { return m_nodebase->as<Core::Transformation>()->isInSequence(); }

Ptr<Core::NodeBase> WorkspaceTransformation::getNodebaseSequence()
{
	return m_nodebase->as<Core::Transformation>()->getCurrentSequence();
}
