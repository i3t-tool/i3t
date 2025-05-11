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
#pragma once

#include "GUI/Fonts/Bindings/BindingFontAwesome.h"
#include "GUI/Fonts/Bindings/IconsFontAwesome6_I3T.h"
#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"
#include "Localization/Localization.h"
#include "TransformationBase.h"

namespace Workspace
{
template <Core::ETransformType T>
class Transformation : public TransformationBase
{
	using Super = TransformationBase;

public:
	Transformation(DIWNE::NodeEditor& diwne) : TransformationBase(diwne, Core::Builder::createTransform<T>())
	{
		init();
	}

	/// Called in the constructor.
	void init()
	{
		// Used in templated specializations
	}

	void topRightHeaderContent(DIWNE::DrawInfo& context) override
	{
		// Used in template specializations
		Super::topRightHeaderContent(context);
	}

	/**
	 * \brief helper function used for decision about showing the corrupted
	 * transform flag in topContent(). Overriden, defined in
	 * WorkspaceTransformation.h Calls Core::TransformImpl isValid() methods of
	 * all variants of Transforms
	 *
	 * \return Core::ETransformState:: Valid, Invalid, or Unknown
	 */
	bool isMatrixValid() override
	{
		return m_nodebase->as<Core::TransformImpl<T>>()->isValid();
	}

	int maxLengthOfData() override
	{
		//    if (Core::ETransformType::Quat == T)
		//        return maxLengthOfDataQuaternion( m_nodebase->data().getQuat(),
		//        m_numberOfVisibleDecimal);
		//    else
		return GUI::maxLengthOfData4x4(m_nodebase->data().getMat4(), m_numberOfVisibleDecimal);
	}

	void drawMenuLevelOfDetail() override
	{
		if (Core::ETransformType::Free == T)
		{
			drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
			                              {LevelOfDetail::Full, LevelOfDetail::Label});
		}
		else
		{
			drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
			                              {LevelOfDetail::Full, LevelOfDetail::SetValues, LevelOfDetail::Label});
		}

		//    drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
		//                                  (Core::ETransformType::Free == T) ?
		//                                  {LevelOfDetail::Full,
		//                                  LevelOfDetail::Label}
		//                                                              :
		//                                                              {LevelOfDetail::Full,
		//                                                              LevelOfDetail::SetValues,
		//                                                              LevelOfDetail::Label});
	}

	int getLODCount() override
	{
		return Super::getLODCount();
	}

	bool drawDataSetValues(DIWNE::DrawInfo& context) override
	{
		bool inner_interaction_happen = false, value_changed = false;
		auto nodebase = m_nodebase->as<Core::TransformImpl<T>>();

		//  m_nodebase->as<Core::Transformation>()->enableSynergies(); // ??? JH
		//  should be here ?

		if (ImGui::BeginTable(fmt::format("##LoD{}", getId()).c_str(), 2 /* label, value */,
		                      ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit))
		{
			for (auto& [key, valueStore] : nodebase->getDefaultValues())
			{
				switch (valueStore.valueType)
				{
				case Core::EValueType::Vec3:
				{
					auto localData = valueStore.getVec3();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context,
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()),
					     fmt::format("{} Z", key.c_str())} /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						queueUpdateDataItemsWidth();
					}
					break;
				}
				case Core::EValueType::Vec4:
				{
					auto localData = valueStore.getVec4();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context,
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()),
					     fmt::format("{} Z", key.c_str()), fmt::format("{} W", key.c_str())}
					    /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2], &localData[3]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						queueUpdateDataItemsWidth();
					}
					break;
				}
				case Core::EValueType::Quat:
				{
					auto localData = valueStore.getQuat(); // the default "quat" value

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context,
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()),
					     fmt::format("{} Z", key.c_str()), fmt::format("{} W", key.c_str())}
					    /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2], &localData[3]}, value_changed);
					if (value_changed)
					{
						//           if
						//           (m_nodebase->as<Core::Transformation>()->hasSynergies())
						//{
						//	if (glm::dot(localData, localData) == 0)
						//		localData[3] = 1.0f;  // to avoid undefined value and GLSL
						// error 	else 		localData = glm::normalize(localData);
						//           }
						nodebase->setDefaultValue(key, localData);
						queueUpdateDataItemsWidth();
					}
					break;
				}
				case Core::EValueType::Matrix:
				{
					inner_interaction_happen |= drawDataFull(context);
					break;
				}
				case Core::EValueType::Float:
				{
					auto localData = valueStore.getFloat();

					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context, {fmt::format("{}", key.c_str())}, {&localData}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						queueUpdateDataItemsWidth();
					}
					break;
				}
				default:
					inner_interaction_happen |= drawDataFull(context);
				}
			}
			ImGui::EndTable();
		}
		return inner_interaction_happen;
	}

	void drawMenuSetDataMap() override
	{
		TransformationBase::drawMenuSetDataMap();
	} /* thus we can specify it for Core::ETransformType::Free  */

	void drawMenuExtra() override
	{
		TransformationBase::drawMenuExtra();
	}
};

template <>
inline void Transformation<Core::ETransformType::Free>::drawMenuSetDataMap()
{
	I3TGui::MenuItemWithLog(_t("Lock"), NULL, false, false); /* no change DataMap in Free transformation */
	I3TGui::MenuItemWithLog(_t("Enable synergies"), NULL, false, false);
}

template <>
inline int Transformation<Core::ETransformType::Free>::getLODCount()
{
	return Super::getLODCount() - 1; // Free doesn't have SetValues mode
}

template <>
inline void Transformation<Core::ETransformType::Scale>::drawMenuSetDataMap()
{
	if (m_nodebase->as<Core::Transform>()->isLocked())
	{
		if (I3TGui::MenuItemWithLog(_t("Unlock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->unlock();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Lock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->lock();
		}
	}

	// the one of two with synergies in LevelOfDetail::FUll and SetValues
	if (m_nodebase->as<Core::Transform>()->hasSynergies())
	{
		if (I3TGui::MenuItemWithLog(_t("Disable synergies"), NULL, false, getLevelOfDetail() != LevelOfDetail::Label))
		{
			m_nodebase->as<Core::Transform>()->disableSynergies();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Enable synergies"), NULL, false, getLevelOfDetail() != LevelOfDetail::Label))
		{
			m_nodebase->as<Core::Transform>()->enableSynergies();
		}
	}
}

template <>
inline void Transformation<Core::ETransformType::Quat>::init()
{
	setFloatPopupMode(FloatPopupMode::Angle);
	setLevelOfDetail(LevelOfDetail::SetValues);
	CoreNode::setNumberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisibleQuaternionPrecision));
}

template <>
inline void Transformation<Core::ETransformType::Quat>::drawMenuSetDataMap()
{
	if (m_nodebase->as<Core::Transform>()->isLocked())
	{
		if (I3TGui::MenuItemWithLog(_t("Unlock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->unlock();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Lock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->lock();
		}
	}

	// the one of two with synergies in LevelOfDetail::FUll and SetValues
	// auto LOD		= getLevelOfDetail();
	// auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
	// if (hasSyn)
	//	int i = 7;
	// auto hasMenuSyn =
	// m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
	if (m_nodebase->as<Core::Transform>()->hasSynergies())
	{
		if (I3TGui::MenuItemWithLog(_t("Disable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->disableSynergies();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Enable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->enableSynergies();
		}
	}
}

template <>
inline void Transformation<Core::ETransformType::Ortho>::drawMenuSetDataMap()
{
	if (m_nodebase->as<Core::Transform>()->isLocked())
	{
		if (I3TGui::MenuItemWithLog(_t("Unlock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->unlock();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Lock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->lock();
		}
	}

	// the one of two with synergies in LevelOfDetail::FUll and SetValues
	// auto LOD		= getLevelOfDetail();
	// auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
	// if (hasSyn)
	//	int i = 7;
	// auto hasMenuSyn =
	// m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
	if (m_nodebase->as<Core::Transform>()->hasSynergies())
	{
		if (I3TGui::MenuItemWithLog(_t("Disable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->disableSynergies();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Enable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->enableSynergies();
		}
	}
}

template <>
inline void Transformation<Core::ETransformType::Frustum>::drawMenuSetDataMap()
{
	if (m_nodebase->as<Core::Transform>()->isLocked())
	{
		if (I3TGui::MenuItemWithLog(_t("Unlock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->unlock();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Lock"), NULL, false, getLevelOfDetail() == LevelOfDetail::Full))
		{
			m_nodebase->as<Core::Transform>()->lock();
		}
	}

	// the one of two with synergies in LevelOfDetail::FUll and SetValues
	// auto LOD		= getLevelOfDetail();
	// auto hasSyn = m_nodebase->as<Core::Transformation>()->hasSynergies();
	// if (hasSyn)
	//	int i = 7;
	// auto hasMenuSyn =
	// m_nodebase->as<Core::Transformation>()->hasMenuSynergies();
	if (m_nodebase->as<Core::Transform>()->hasSynergies())
	{
		if (I3TGui::MenuItemWithLog(_t("Disable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->disableSynergies();
		}
	}
	else
	{
		if (I3TGui::MenuItemWithLog(_t("Enable synergies"), NULL, false,
		                            getLevelOfDetail() == LevelOfDetail::SetValues))
		{
			m_nodebase->as<Core::Transform>()->enableSynergies();
		}
	}
}

inline void drawMenuCoordinateSystem(Core::Transform* transform, Core::CameraCoordSystem& coordinateSystem)
{
	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (I3TGui::BeginMenuWithLog(ICON_TBD(ICON_FA_I3T_COORD_SYSTEM " ", "Coordinate system")))
	{
		if (I3TGui::MenuItemWithLog(ICON_FA_I3T_OGL " "
		                                            "OpenGL",
		                            NULL, coordinateSystem == Core::g_openGL))
		{
			coordinateSystem = Core::g_openGL;
			transform->resetMatrixFromDefaults();
			if (Core::GraphManager::isTracking())
				Core::GraphManager::getTracker()->requestProgressUpdate();
		}
		if (I3TGui::MenuItemWithLog(ICON_FA_I3T_VULKAN " "
		                                               "Vulkan",
		                            NULL, coordinateSystem == Core::g_vulkan))
		{
			coordinateSystem = Core::g_vulkan;
			transform->resetMatrixFromDefaults();
			if (Core::GraphManager::isTracking())
				Core::GraphManager::getTracker()->requestProgressUpdate();
		}
		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	ImGui::Separator();
}

inline void drawCoordinateSystemIndicator(DIWNE::DrawInfo& context, Core::CameraCoordSystem& coordinateSystem)
{
	if (coordinateSystem == Core::g_vulkan)
	{
		// Undo the node label trailing padding, so that the indicator follows the text without extra gap
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorScreenPos(ImVec2(pos.x - ImGui::GetStyle().FramePadding.x, pos.y));

		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(ICON_FA_I3T_VULKAN);
		ImGui::SameLine();
	}
}

template <>
inline void Transformation<Core::ETransformType::Ortho>::drawMenuExtra()
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Ortho>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Ortho>>();
	drawMenuCoordinateSystem(coreTransform, coreTransform->m_coordinateSystem);
}

template <>
inline void Transformation<Core::ETransformType::Perspective>::drawMenuExtra()
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Perspective>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Perspective>>();
	drawMenuCoordinateSystem(coreTransform, coreTransform->m_coordinateSystem);
}

template <>
inline void Transformation<Core::ETransformType::Frustum>::drawMenuExtra()
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Frustum>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Frustum>>();
	drawMenuCoordinateSystem(coreTransform, coreTransform->m_coordinateSystem);
}

template <>
inline void Transformation<Core::ETransformType::Viewport>::drawMenuExtra()
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Viewport>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Viewport>>();
	drawMenuCoordinateSystem(coreTransform, coreTransform->m_coordinateSystem);
}

template <>
inline void Transformation<Core::ETransformType::Ortho>::topRightHeaderContent(DIWNE::DrawInfo& context)
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Ortho>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Ortho>>();
	drawCoordinateSystemIndicator(context, coreTransform->m_coordinateSystem);
	Super::topRightHeaderContent(context);
}

template <>
inline void Transformation<Core::ETransformType::Perspective>::topRightHeaderContent(DIWNE::DrawInfo& context)
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Perspective>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Perspective>>();
	drawCoordinateSystemIndicator(context, coreTransform->m_coordinateSystem);
	Super::topRightHeaderContent(context);
}

template <>
inline void Transformation<Core::ETransformType::Frustum>::topRightHeaderContent(DIWNE::DrawInfo& context)
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Frustum>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Frustum>>();
	drawCoordinateSystemIndicator(context, coreTransform->m_coordinateSystem);
	Super::topRightHeaderContent(context);
}

template <>
inline void Transformation<Core::ETransformType::Viewport>::topRightHeaderContent(DIWNE::DrawInfo& context)
{
	assert(dynamic_cast<Core::TransformImpl<Core::ETransformType::Viewport>*>(this->m_nodebase.get()));
	auto* coreTransform = m_nodebase->asRaw<Core::TransformImpl<Core::ETransformType::Viewport>>();
	drawCoordinateSystemIndicator(context, coreTransform->m_coordinateSystem);
	Super::topRightHeaderContent(context);
}

template <>
inline bool Transformation<Core::ETransformType::Scale>::drawDataSetValues(DIWNE::DrawInfo& context)
{
	bool inner_interaction_happen = false, value_changed = false;
	auto nodebase = m_nodebase->as<Core::TransformImpl<Core::ETransformType::Scale>>();
	if (ImGui::BeginTable(fmt::format("##LoD{}", getId()).c_str(), 2 /* label, value */,
	                      ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit))
	{
		for (auto& [key, valueStore] : nodebase->getDefaultValues())
		{
			switch (valueStore.valueType)
			{
			case Core::EValueType::Vec3:
			{
				auto localData = valueStore.getVec3();

				if (m_nodebase->as<Core::Transform>()->hasSynergies()) /* uniform */
				{
					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context, {fmt::format("{}", key.c_str())} /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0]}, value_changed);
					if (value_changed)
					{
						// localData[1] = localData[2] = localData[0]; /* \todo JH \todo JH
						// MH do this in core -> something like setUniformScale */
						// nodebase->setDefaultValue(key, localData);     // was
						// nodebase->setDefaultValue(key, localData[0]);  // Sets float
						// instead of vec3 - may be based on synergies??
						nodebase->setDefaultUniformScale(localData.x);
						queueUpdateDataItemsWidth();
					}
				}
				else /* non-uniform */
				{
					inner_interaction_happen |= drawDataSetValues_InsideTablebuilder(
					    context,
					    {fmt::format("{} X", key.c_str()), fmt::format("{} Y", key.c_str()),
					     fmt::format("{} Z", key.c_str())} /* \todo MH all/whole labels from core? */
					    ,
					    {&localData[0], &localData[1], &localData[2]}, value_changed);
					if (value_changed)
					{
						nodebase->setDefaultValue(key, localData);
						queueUpdateDataItemsWidth();
					}
				}
				break;
			}
			default:
				I3T_ASSERT(false, "Unknow data type in Scale Transform");
			}
		}
		ImGui::EndTable();
	}
	return inner_interaction_happen;
}

template <>
inline bool Transformation<Core::ETransformType::LookAt>::drawDataSetValues(DIWNE::DrawInfo& context)
{
	bool inner_interaction_happen = false, value_changed = false;
	auto nodebase = m_nodebase->as<Core::TransformImpl<Core::ETransformType::LookAt>>();
	int index_of_change;

	std::string cornerLabel = "";
	std::vector<std::string> rowLabels = {"X", "Y", "Z"};
	std::vector<std::string> columnLabels;
	std::vector<glm::vec3> local_data;

	/* \todo JH, \todo MH some way how to not compute this every frame? */
	for (auto& [key, valueStore] : nodebase->getDefaultValues())
	{
		columnLabels.push_back(key);
		local_data.push_back(valueStore.getVec3());
	}

	inner_interaction_happen |= drawDataSetValuesTable_builder(
	    context, cornerLabel, columnLabels, rowLabels,
	    {&local_data[0][0], &local_data[1][0], &local_data[2][0], &local_data[0][1], &local_data[1][1],
	     &local_data[2][1], &local_data[0][2], &local_data[1][2], &local_data[2][2]},
	    value_changed, index_of_change);
	if (value_changed)
	{
		index_of_change %= columnLabels.size(); /* move to index of nodebase data column */
		nodebase->setDefaultValue(columnLabels[index_of_change], local_data[index_of_change]);
		queueUpdateDataItemsWidth();
	}

	return inner_interaction_happen;
}

template <>
inline void Transformation<Core::ETransformType::LookAt>::init()
{
	setLevelOfDetail(LevelOfDetail::SetValues);
}

template <>
inline void Transformation<Core::ETransformType::EulerX>::init()
{
	setFloatPopupMode(FloatPopupMode::Angle);
}
template <>
inline void Transformation<Core::ETransformType::EulerY>::init()
{
	setFloatPopupMode(FloatPopupMode::Angle);
}
template <>
inline void Transformation<Core::ETransformType::EulerZ>::init()
{
	setFloatPopupMode(FloatPopupMode::Angle);
}
template <>
inline void Transformation<Core::ETransformType::AxisAngle>::init()
{
	setFloatPopupMode(FloatPopupMode::Angle);
}
} // namespace Workspace