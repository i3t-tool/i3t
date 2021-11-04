#include "WorkspaceTransformation.h"

WorkspaceTransformation::WorkspaceTransformation(Ptr<Core::NodeBase> nodebase)
    :   WorkspaceNodeWithCoreData(nodebase)
{}

bool WorkspaceTransformation::topContent(DIWNE::Diwne &diwne)
{
    /* \todo JH get Transformation header Theme here */
    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().getHeader()), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/

    WorkspaceNodeWithCoreData::topContent(diwne);

    if (!dataAreValid())
    {
        diwne.DrawIcon(DIWNE::IconType::Circle, ImColor(255,0,0), ImColor(255,255,255),
                    DIWNE::IconType::Cross, ImColor(255,0,0), ImColor(255,255,255),
                    ImVec2(20,20), ImVec4(5,5,5,5), false ); /* \todo JH Icon setting from Theme? */
    }
	return false;
}

bool WorkspaceTransformation::middleContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
	switch (m_levelOfDetail)
	{
	case WorkspaceLevelOfDetail::Full:
		inner_interaction_happen = drawDataFull(diwne);
		break;
	case WorkspaceLevelOfDetail::SetValues:
		inner_interaction_happen = drawDataSetValues(diwne);
		break;
	case WorkspaceLevelOfDetail::Label:
		inner_interaction_happen = drawDataLabel(diwne);
		break;

	default:
		Debug::Assert(false , "drawData: Unknown m_levelOfDetail");
        inner_interaction_happen = drawDataFull(diwne);
	}

	/*if (m_inactiveMark != 0)
    {
        ImVec2 start = ne::GetNodePosition(m_id);
        ImVec2 size = ne::GetNodeSize(m_id);
        ImVec2 end = start + size;
        if(m_inactiveMark > 0)
        {
            end.x -= (1-m_inactiveMark)*size.x;
        }
        else
        {
            start.x += m_inactiveMark*size.x;
        }

        //mGui::PushStyleColor(ImGuiColor, ImVec4(164, 171, 190, 1));
        //ImGui::Dummy(const ImVec2& size);
        ImGui::GetWindowDrawList()->AddRectFilled( start, end, ImColor(0,0,0,0.5) );
    }*/
    return inner_interaction_happen;
}

void WorkspaceTransformation::nodePopupContent()
{
    drawMenuSetDataMap();

    WorkspaceNodeWithCoreData::nodePopupContent();
}

void WorkspaceTransformation::drawMenuSetDataMap()
{
	if (ImGui::BeginMenu("Set datamap"))
	{
		for (Core::Transform::DataMap const* datamap : m_nodebase->getValidDataMaps())
		{
			if (ImGui::MenuItem(WorkspaceDatamapName[datamap].c_str())) { setDataMap(datamap); }
		}

		ImGui::EndMenu();
	}
}

void WorkspaceTransformation::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::SetValues, WorkspaceLevelOfDetail::Label});
}

void WorkspaceTransformation::drawMenuDelete()
{
    if (ImGui::MenuItem("Delete")) {
            m_removeFromWorkspaceWindow = true;
            m_removeFromSequence = true;
    }
}

std::vector<ImVec2> WorkspaceTransformation::getInteractionPointsWithSequence()
{
    ImVec2 position = getNodePositionDiwne();
    ImVec2 size = getNodeRectSizeDiwne();
    ImVec2 topMiddle = position; topMiddle.x += size.x/2;
    ImVec2 middle = position + size/2;
    ImVec2 bottomMiddle = topMiddle; bottomMiddle.y += size.y;
    return {topMiddle, middle, bottomMiddle};
}

bool WorkspaceTransformation::drawDataFull(DIWNE::Diwne &diwne)
{
     bool valueChanged = false, interaction_happen = false;
     int rowOfChange, columnOfChange;
     float valueOfChange;
     interaction_happen = drawData4x4(diwne, getId(), m_numberOfVisibleDecimal, m_floatPopupMode,
                                        m_nodebase->getData().getMat4(), m_nodebase->getDataMapRef(),
                                        getDataItemsWidth(diwne), valueChanged, rowOfChange, columnOfChange, valueOfChange );
    if (valueChanged)
    {
        m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
        setDataItemsWidth();
    }
    return interaction_happen;
}

int WorkspaceTransformation::maxLenghtOfData()
{
    return maxLenghtOfData4x4( m_nodebase->getData().getMat4(), m_numberOfVisibleDecimal);
}

bool WorkspaceTransformation::drawDataSetValues_builder(  DIWNE::Diwne &diwne
                                                        ,   std::vector<std::string> const& labels
                                                        ,   std::vector<getter_function_pointer> const& getters
                                                        ,   std::vector<setter_function_pointer> const& setters
                                                        /*,   std::vector<unsigned char> const& datamap_values*/)
{
    int number_of_values = labels.size();
	Debug::Assert(number_of_values==getters.size()
               && number_of_values==setters.size()
               /*&& number_of_values==datamap_values.size()*/ , "drawDataSetValues_builder: All vectors (labels, getters, setters) must have same size.");

	bool	valueChanged = false, actual_value_changed = false, inner_interaction_happen = false;
	int		index_of_change;
	float valueOfChange, localData; /* user can change just one value at the moment */

	ImGui::PushItemWidth(getDataItemsWidth(diwne));
	for (int i = 0; i < number_of_values; i++)
	{
		ImGui::TextUnformatted(labels[i].c_str());

		ImGui::SameLine(50); /* \todo JH why 50? what is it */

		localData = getters[i]();

		inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, getNumberOfVisibleDecimal(), getFloatPopupMode(), actual_value_changed, localData, 1, fmt::format("##{}:ch{}", getId(), i));
		if (actual_value_changed){

            valueChanged = true;
			index_of_change = i;
			valueOfChange = localData;
		}
	}
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		setters[index_of_change](valueOfChange); /* \todo JH react to different returned value of setter */
		setDataItemsWidth();
	}

	// ImGui::Spring(0);
	return inner_interaction_happen;
}

bool WorkspaceTransformation::isInSequence()
{
	return m_nodebase->as<Core::Transformation>()->isInSequence();
}

Ptr<Core::NodeBase> WorkspaceTransformation::getNodebaseSequence()
{
	return m_nodebase->as<Core::Transformation>()->getCurrentSequence();
}

