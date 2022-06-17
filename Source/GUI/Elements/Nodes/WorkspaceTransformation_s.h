#pragma once

#include "WorkspaceTransformation.h"

template<ETransformType T>
class WorkspaceTransformation_s : public WorkspaceTransformation
{
public:
	WorkspaceTransformation_s(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<T>())
	{
		setDataItemsWidth();
		if (ETransformType::Quat == T) setLevelOfDetail(WorkspaceLevelOfDetail::SetValues);
	}

	/**
	 * \brief helper function used for decision about showing the corrupted transform flag in topContent().
	 * Overriden, defined in WorkspaceTransformation.h
	 * Calls Core::TransformImpl isValid() methods of all variants of Transforms
	 *
	 * \return Core::ETransformState:: Valid, Invalid, or Unknown
	 */
	virtual Core::ETransformState isMatrixValid() override
	{
		return m_nodebase->as<Core::TransformImpl<T>>()->isValid();
	}

	int maxLenghtOfData()
	{
		//    if (ETransformType::Quat == T)
		//        return maxLenghtOfDataQuaternion( m_nodebase->getData().getQuat(), m_numberOfVisibleDecimal);
		//    else
		return maxLenghtOfData4x4(m_nodebase->getData().getMat4(), m_numberOfVisibleDecimal);
	}

	void drawMenuLevelOfDetail()
	{
		if (ETransformType::Free == T)
		{
			drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
			                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
		}
		else
		{
			drawMenuLevelOfDetail_builder(
			    std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
			    {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::SetValues, WorkspaceLevelOfDetail::Label});
		}

		//    drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
		//                                  (ETransformType::Free == T) ? {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label}
		//                                                              : {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::SetValues, WorkspaceLevelOfDetail::Label});
	}

	bool drawDataSetValues() override
	{
		bool inner_interaction_happen = false, value_changed = false;
		auto nodebase = m_nodebase->as<Core::TransformImpl<T>>();

		//  m_nodebase->as<Core::Transformation>()->enableSynergies(); // ??? JH should be here ?

		if (ImGui::BeginTable(fmt::format("##LoD{}", getIdDiwne()).c_str(), 2 /* label, value */,
		                      ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit))
		{
			for (auto& [key, valueStore] : nodebase->getDefaultValues())
			{
				switch (valueStore.opValueType)
				{
				case EValueType::Vec3:
				{
					auto localData = valueStore.getVec3();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()), fmt::format("{} Z", key.c_str())}
					    /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						setDataItemsWidth();
					}
					break;
				}
				case EValueType::Vec4:
				{
					auto localData = valueStore.getVec4();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()), fmt::format("{} Z", key.c_str()),
					     fmt::format("{} W", key.c_str())} /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2], &localData[3]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						setDataItemsWidth();
					}
					break;
				}
				case EValueType::Quat:
				{
					auto localData = valueStore.getQuat();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()), fmt::format("{} Z", key.c_str()),
					     fmt::format("{} W", key.c_str())} /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2], &localData[3]}, value_changed);
					if (value_changed)
					{
						//           if (m_nodebase->as<Core::Transformation>()->hasSynergies())
						//{
						//	if (glm::dot(localData, localData) == 0)
						//		localData[3] = 1.0f;  // to avoid undefined value and GLSL error
						//	else
						//		localData = glm::normalize(localData);
						//           }
						nodebase->setDefaultValue(key, localData);
						setDataItemsWidth();
					}
					break;
				}
				case EValueType::Matrix:
				{
					inner_interaction_happen |= drawDataFull();
					break;
				}
				case EValueType::Float:
				{
					auto localData = valueStore.getFloat();

					inner_interaction_happen |=
					    drawDataSetValues_InsideTablebuilder({fmt::format("{}", key.c_str())}, {&localData}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						setDataItemsWidth();
					}
					break;
				}
				default:
					inner_interaction_happen |= drawDataFull();
				}
			}
			ImGui::EndTable();
		}
		return inner_interaction_happen;
	}

	void drawMenuSetDataMap() override
	{
		WorkspaceTransformation::drawMenuSetDataMap();
	} /* thus we can specify it for ETransformType::Free  */
};

template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
void WorkspaceTransformation_s<ETransformType::Free>::drawMenuSetDataMap()
{
	ImGui::MenuItem("Lock", NULL, false, false); /* no change DataMap in Free transformation */
	ImGui::MenuItem("Enable synergies", NULL, false, false);
}

template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
void WorkspaceTransformation_s<ETransformType::Scale>::drawMenuSetDataMap()
{
    if(m_nodebase->as<Core::Transformation>()->isLocked()){ if(ImGui::MenuItem("Unlock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->unlock();} }
    else                                                  { if(ImGui::MenuItem("Lock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->lock();} }

		// the one of two with synergies in WorkspaceLevelOfDetail::FUll and SetValues
    if(m_nodebase->as<Core::Transformation>()->hasSynergies()){ if(ImGui::MenuItem("Disable synergies", NULL, false, getLevelOfDetail()!=WorkspaceLevelOfDetail::Label)){m_nodebase->as<Core::Transformation>()->disableSynergies();} }
    else                                                      { if(ImGui::MenuItem("Enable synergies", NULL, false, getLevelOfDetail()!=WorkspaceLevelOfDetail::Label)){m_nodebase->as<Core::Transformation>()->enableSynergies();} }

}

template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
void WorkspaceTransformation_s<ETransformType::Quat>::drawMenuSetDataMap()
{
    if(m_nodebase->as<Core::Transformation>()->isLocked()){ if(ImGui::MenuItem("Unlock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->unlock();} }
    else                                                  { if(ImGui::MenuItem("Lock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->lock();} }

		// the one of two with synergies in WorkspaceLevelOfDetail::FUll and SetValues
		//auto LOD		= getLevelOfDetail();
		//auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
		//if (hasSyn) 
		//	int i = 7;
		//auto hasMenuSyn = m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
		if (m_nodebase->as<Core::Transformation>()->hasSynergies())
    {
	    if(ImGui::MenuItem("Disable synergies", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->disableSynergies();}
    }
    else
    {
			if (ImGui::MenuItem("Enable synergies", NULL, false, getLevelOfDetail() == WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->enableSynergies();}
    }

}
template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
void WorkspaceTransformation_s<ETransformType::Ortho>::drawMenuSetDataMap()
{
    if(m_nodebase->as<Core::Transformation>()->isLocked()){ if(ImGui::MenuItem("Unlock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->unlock();} }
    else                                                  { if(ImGui::MenuItem("Lock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->lock();} }

		// the one of two with synergies in WorkspaceLevelOfDetail::FUll and SetValues
		//auto LOD		= getLevelOfDetail();
		//auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
		//if (hasSyn) 
		//	int i = 7;
		//auto hasMenuSyn = m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
		if (m_nodebase->as<Core::Transformation>()->hasSynergies())
    {
	    if(ImGui::MenuItem("Disable synergies", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->disableSynergies();}
    }
    else
    {
			if (ImGui::MenuItem("Enable synergies", NULL, false, getLevelOfDetail() == WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->enableSynergies();}
    }

}
template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
void WorkspaceTransformation_s<ETransformType::Frustum>::drawMenuSetDataMap()
{
    if(m_nodebase->as<Core::Transformation>()->isLocked()){ if(ImGui::MenuItem("Unlock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->unlock();} }
    else                                                  { if(ImGui::MenuItem("Lock", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::Full)){m_nodebase->as<Core::Transformation>()->lock();} }

		// the one of two with synergies in WorkspaceLevelOfDetail::FUll and SetValues
		//auto LOD		= getLevelOfDetail();
		//auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
		//if (hasSyn) 
		//	int i = 7;
		//auto hasMenuSyn = m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
		if (m_nodebase->as<Core::Transformation>()->hasSynergies())
    {
	    if(ImGui::MenuItem("Disable synergies", NULL, false, getLevelOfDetail()==WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->disableSynergies();}
    }
    else
    {
			if (ImGui::MenuItem("Enable synergies", NULL, false, getLevelOfDetail() == WorkspaceLevelOfDetail::SetValues))
			{m_nodebase->as<Core::Transformation>()->enableSynergies();}
    }

}
template <>
inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
    bool
    WorkspaceTransformation_s<ETransformType::Scale>::drawDataSetValues()
{
	bool inner_interaction_happen = false, value_changed = false;
	auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Scale>>();
	if (ImGui::BeginTable(fmt::format("##LoD{}", getIdDiwne()).c_str(), 2 /* label, value */,
	                      ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit))
	{
		for (auto& [key, valueStore] : nodebase->getDefaultValues())
		{
			switch (valueStore.opValueType)
			{
			case EValueType::Vec3:
			{
				auto localData = valueStore.getVec3();

				if (m_nodebase->as<Core::Transformation>()->hasSynergies()) /* uniform */
				{
					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder({fmt::format("{}", key.c_str())}
					                                                                 /* \todo MH all/whole labels from core? */
					                                                                 ,
					                                                                 {&localData[0]}, value_changed);
					if (value_changed)
					{
						//localData[1] = localData[2] = localData[0]; /* \todo JH MH do this in core -> something like setUniformScale */
						//nodebase->setDefaultValue(key, localData);     // was
						//nodebase->setDefaultValue(key, localData[0]);  // Sets float instead of vec3 - may be based on synergies??
						nodebase->setDefaultUniformScale(localData.x);
						setDataItemsWidth();
					}
				}
				else /* non-uniform */
				{
					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()), fmt::format("{} Z", key.c_str())}
					    /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						setDataItemsWidth();
					}
				}
				break;
			}
			default:
				I3T_ASSERT(false && "Unknow data type in Scale Transform");
			}
		}
		ImGui::EndTable();
	}
	return inner_interaction_happen;
}


template <> inline /* inline for ability to compile https://stackoverflow.com/questions/4445654/multiple-definition-of-template-specialization-when-using-different-objects */
bool WorkspaceTransformation_s<ETransformType::LookAt>::drawDataSetValues()
{
    bool inner_interaction_happen = false, value_changed = false;
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::LookAt>>();
    int index_of_change;

    std::string cornerLabel = "";
    std::vector<std::string> rowLabels = {"X", "Y", "Z"};
    std::vector<std::string> columnLabels;
    std::vector<glm::vec3> local_data;

    /* \todo JH, MH some way how to not compute this every frame? */
    int i = 0;
    for (auto& [key, valueStore] : nodebase->getDefaultValues())
    {
        columnLabels.push_back(key);
        local_data.push_back( valueStore.getVec3());
    }


    inner_interaction_happen |= drawDataSetValuesTable_builder( cornerLabel,
                                                                columnLabels,
                                                                rowLabels,
                                                                {   &local_data[0][0], &local_data[1][0], &local_data[2][0],
                                                                    &local_data[0][1], &local_data[1][1], &local_data[2][1],
                                                                    &local_data[0][2], &local_data[1][2], &local_data[2][2]},
                                                                value_changed,
                                                                index_of_change);
    if (value_changed)
    {
        index_of_change %= columnLabels.size(); /* move to index of nodebase data column */
        nodebase->setDefaultValue(columnLabels[index_of_change], local_data[index_of_change]);
        setDataItemsWidth();
    }

    return inner_interaction_happen;
}


