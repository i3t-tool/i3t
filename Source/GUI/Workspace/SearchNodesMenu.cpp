#include "GUI/Workspace/SearchNodesMenu.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h" 
#include "Viewport/entity/nodes/SceneModel.h"

#include <algorithm>

void SearchNodesMenu::init()
{
	if (!m_availableNodes.empty())
	{
		return;
	}

	auto spawn = [](auto nodeType) 
	{
		WorkspaceModule::g_editor->addNodeToPositionOfPopup<decltype(nodeType)>();
	};

	m_availableNodes = {
	    // --- BASIC ---
	    {"Matrix Sequence", "Basic", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Sequence>();}},
	    {"Camera", "Basic", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Camera>();}},
	    {"Screen", "Basic", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Screen>();}},
	    {"Scripting Node", "Basic", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::ScriptingNode>();}},

	    // --- ANIMATE ---
	    {"Pulse", "Animate", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::PulseToPulse>>();}},
	    {"Cycle", "Animate", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Cycle>();}},

	    // --- TRANSFORMATIONS ---
	    {"Free", "Transformation", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Free>>();}},
	    {"Translate", "Transformation", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Translation>>();}},
	    {"Scale", "Transformation", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Scale>>();}},
	    {"LookAt", "Transformation", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::LookAt>>();}},
	    {"Viewport", "Transformation", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Viewport>>();}},

	    {"EulerAngleX", "Transformation (Rotate)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::EulerX>>();}},
	    {"EulerAngleY", "Transformation (Rotate)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::EulerY>>();}},
	    {"EulerAngleZ", "Transformation (Rotate)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::EulerZ>>();}},
	    {"AxisAngle", "Transformation (Rotate)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::AxisAngle>>();}},
	    {"Quat", "Transformation (Rotate)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Quat>>(); }},

	    {"Ortho", "Transformation (Projection)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Ortho>>(); }},
	    {"Perspective", "Transformation (Projection)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Perspective>>(); }},
	    {"Frustum", "Transformation (Projection)", []() { WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Transformation<Core::ETransformType::Frustum>>(); }},

	    // --- OPERATORS (TRANSFORMATION) ---
	    {"Translate", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeTranslation>>();}},
	    {"EulerAngleX", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeEulerX>>();}},
	    {"EulerAngleY", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeEulerY>>();}},
	    {"EulerAngleZ", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeEulerZ>>();}},
	    {"Rotate", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeAxisAngle>>();}},
	    {"Scale", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeScale>>();}},
	    {"Ortho", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeOrtho>>();}},
	    {"Perspective", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakePerspective>>();}},
	    {"Frustum", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeFrustum>>();}},
	    {"LookAt", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeLookAt>>();}},
	    {"Viewport", "Operator (Transformation)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MakeViewport>>();}},

	    // --- OPERATORS (MATRIX) ---
	    {"Matrix", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixToMatrix>>();}},
	    {"Inversion", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Inversion>>();}},
	    {"Transpose", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Transpose>>();}},
	    {"Determinant", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Determinant>>();}},
	    {"Mat * Mat", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixMulMatrix>>();}},
	    {"Mat + Mat", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixAddMatrix>>();}},
	    {"Mat * Vec4", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixMulVector>>();}},
	    {"Vec4 * Mat", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorMulMatrix>>();}},
	    {"Float * Mat", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixMulFloat>>();}},
	    {"eigenvalues", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::EigenVals>>();}},
	    {"eigenvectors", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::EigenVecs>>();}},
	    {"svd", "Operator (Matrix)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::SVD>>();}},

	    // --- OPERATORS (VEC3) ---
	    {"Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3ToVector3>>();}},
	    {"Show Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::ShowVector3>>();}},
	    {"Vec3 x Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3CrossVector3>>();}},
	    {"Vec3 . Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3DotVector3>>();}},
	    {"Vec3 + Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3AddVector3>>();}},
	    {"Vec3 - Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3SubVector3>>();}},
	    {"Float * Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3MulFloat>>();}},
	    {"Normalize Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::NormalizeVector3>>();}},
	    {"Length(Vec3)", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3Length>>();}},
	    {"Mix Vec3", "Operator (Vec3)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MixVector3>>();}},

	    // --- OPERATORS (VEC4) ---
	    {"Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector4ToVector4>>();}},
	    {"Vec4 . Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorDotVector>>();}},
	    {"Vec4 + Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorAddVector>>();}},
	    {"Vec4 - Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorSubVector>>();}},
	    {"Float * Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorMulFloat>>();}},
	    {"Normalize Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::NormalizeVector>>();}},
	    {"Perspective Division", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorPerspectiveDivision>>();}},
	    {"Mix Vec4", "Operator (Vec4)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MixVector>>();}},	

	    // --- OPERATORS (QUAT) ---
	    {"Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToQuat>>();}},
	    {"Quat(float, vec3)", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatVecToQuat>>();}},
	    {"Quat(angle, axis)", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::AngleAxisToQuatOperator>();}},
	    {"Quat(vec3, vec3)", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VecVecToQuat>>();}},
	    {"Quat -> float, vec3", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToFloatVec>>();}},
	    {"Quat -> angle, axis", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToAngleAxis>>();}},
	    {"Float * Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatMulQuat>>();}},
	    {"Quat * Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatMulQuat>>();}},
	    {"Quat -> Euler", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToEuler>>();}},
	    {"Euler -> Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::EulerToQuat>>();}},
	    {"Slerp", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatSlerp>>();}},
	    {"Long Way Slerp", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatLongWaySlerp>>();}},
	    {"Lerp", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatLerp>>();}},
	    {"Quat Conjugate", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::ConjQuat>>();}},
	    {"qvq*", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatVecConjQuat>>();}},
	    {"Inverse Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatInverse>>();}},
	    {"Normalize Quat", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::NormalizeQuat>>();}},
	    {"Length(Quat)", "Operator (Quat)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatLength>>();}},

	    // --- OPERATORS (FLOAT) ---
	    {"Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatToFloat>>();}},
	    {"Clamp Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::ClampFloat>>();}},
	    {"Float * Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatMulFloat>>();}},
	    {"Float / Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatDivFloat>>();}},
	    {"Float + Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatAddFloat>>();}},
	    {"Float ^ Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatPowFloat>>();}},
	    {"Mix Float", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MixFloat>>();}},
	    {"Sin & Cos", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatSinCos>>();}},
	    {"ASin & ACos", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::ASinACos>>();}},
	    {"Signum", "Operator (Float)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Signum>>();}},

	    // --- OPERATORS (CONVERSION) ---
	    {"Mat -> TR", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixToTR>>();}},
	    {"TR -> Mat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::TRToMatrix>>();}},
	    {"Mat -> Vecs4", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixToVectors>>();}},
	    {"Mat -> Quat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixToQuat>>();}},
	    {"Mat -> Floats", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::MatrixToFloats>>();}},
	    {"Vecs4 -> Mat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorsToMatrix>>();}},
	    {"Vec4 -> Vec3", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorToVector3>>();}},
	    {"Vec4 -> Floats", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::VectorToFloats>>();}},
	    {"Vecs3 -> Mat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vectors3ToMatrix>>();}},
	    {"Vec3 -> Vec4", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3ToVector>>();}},
	    {"Vec3 -> Floats", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::Vector3ToFloats>>();}},
	    {"Quat -> Mat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToMatrix>>();}},
	    {"Quat -> Floats", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::QuatToFloats>>();}},
	    {"Floats -> Mat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatsToMatrix>>();}},
	    {"Floats -> Vec4", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatsToVector>>();}},
	    {"Floats -> Vec3", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatsToVector3>>();}},
	    {"Floats -> Quat", "Operator (Conversion)", []() {WorkspaceModule::g_editor->addNodeToPositionOfPopup<Workspace::Operator<Core::EOperatorType::FloatsToQuat>>();}},

		};

}

void SearchNodesMenu::open()
{
	m_wantsToOpen = true;
}

void SearchNodesMenu::render(ImVec2 spawnPos)
{
	if (m_wantsToOpen)
	{
		ImGui::OpenPopup("SearchNodesPopup");
		WorkspaceModule::g_editor->setPopupPosition(spawnPos);
		m_wantsToOpen = false;
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(450, 0), ImVec2(450, 400));

	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 8.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

	if (ImGui::BeginPopup("SearchNodesPopup"))
	{
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere();
			m_searchQuery.clear();
			m_selectedIndex = 0;
		}

		ImGui::PushItemWidth(-1);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
		bool enterPressed =
		    GUI::InputText("##search", &m_searchQuery, ImGuiInputTextFlags_EnterReturnsTrue, nullptr, nullptr);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		std::vector<SearchableNode*> filtered;
		std::string queryLower = m_searchQuery;
		std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

		if (!queryLower.empty())
		{
			for (auto& node : m_availableNodes)
			{
				std::string nameLower = node.name;
				std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

				if (nameLower.find(queryLower) != std::string::npos)
				{
					filtered.push_back(&node);
				}
			}

			std::sort(filtered.begin(), filtered.end(),
			          [&queryLower](const SearchableNode* a, const SearchableNode* b) {
				          std::string nameA = a->name;
				          std::string nameB = b->name;
				          std::transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
				          std::transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);

				          bool aStarts = (nameA.find(queryLower) == 0);
				          bool bStarts = (nameB.find(queryLower) == 0);

				          if (aStarts != bStarts)
					          return aStarts;
				          return nameA < nameB;
			          });
		}

		bool arrowPressed = false;
		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
		{
			m_selectedIndex++;
			arrowPressed = true;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
		{
			m_selectedIndex--;
			arrowPressed = true;
		}

		if (filtered.empty())
			m_selectedIndex = 0;
		else
			m_selectedIndex = std::clamp(m_selectedIndex, 0, (int) filtered.size() - 1);

		if (queryLower.empty())
		{
			ImGui::TextDisabled(" Type a node name to search...");
		}
		else if (filtered.empty())
		{
			ImGui::TextDisabled(" No nodes found...");
		}
		else
		{
			float itemHeight = ImGui::GetTextLineHeightWithSpacing();
			float visibleItems = std::min(5.25f, (float) filtered.size());
			float childHeight = itemHeight * visibleItems;

			ImGui::BeginChild("ScrollingRegion", ImVec2(0, childHeight), false, ImGuiWindowFlags_None);

			if (ImGui::BeginTable("##SearchResultsTable", 2))
			{
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed);

				for (int i = 0; i < filtered.size(); i++)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					bool isSelected = (i == m_selectedIndex);

					if (ImGui::Selectable(filtered[i]->name.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
					{
						filtered[i]->spawnFunc();
						ImGui::CloseCurrentPopup();
					}

					if (isSelected && arrowPressed)
					{
						ImGui::SetScrollHereY(0.5f);
					}

					if (isSelected && enterPressed)
					{
						filtered[i]->spawnFunc();
						ImGui::CloseCurrentPopup();
					}

					ImGui::TableNextColumn();
					ImGui::TextDisabled("%s", filtered[i]->category.c_str());
				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(2);
}

