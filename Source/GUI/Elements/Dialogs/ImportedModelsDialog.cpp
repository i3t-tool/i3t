#include "ImportedModelsDialog.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Modals/RemoveModelModal.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/WindowManager.h"
#include "State/StateManager.h"
#include "SystemDialogs.h"
#include "Viewport/entity/nodes/SceneModel.h"

ImportedModelsDialog::ImportedModelsDialog() {}

void ImportedModelsDialog::render()
{
	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	ImVec2 windowSize = ImVec2(ImGui::GetFontSize() * 20, ImGui::GetFontSize() * 14);
	ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	ImGui::Begin(setName("Imported Models").c_str(), &m_show, g_dialogFlags | ImGuiWindowFlags_NoResize);
	{
		std::vector<std::string> resourceAliases = RMI.getImportedResourceAliases();

		// Could use a listbox here instead
		//		if (ImGui::BeginListBox("List", size))
		//		{
		//			ImGui::Selectable("Selected", true);
		//			ImGui::Selectable("Not Selected", false);
		//			ImGui::EndListBox();
		//		}

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		ImGui::BeginChild("ModelList", ImVec2(windowSize.x * 0.7f, windowSize.y * 0.65f), true);

		if (resourceAliases.size() > 0)
		{
			int index = 0;
			for (auto& alias : resourceAliases)
			{
				if (ImGui::Selectable(alias.c_str(), m_selectedModelIndex == index))
					m_selectedModelIndex = index;
				index++;
			}
		}
		else
		{
			ImGui::BeginDisabled(true);
			ImGui::Selectable("No models imported");
			ImGui::EndDisabled();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::SameLine();

		if (ImGui::BeginTable("buttonColumn", 1, ImGuiTableFlags_NoSavedSettings))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Button("Import", ImVec2(-FLT_MIN, 0.0f)))
			{
				importModel();
			}
			ImGui::SetItemDefaultFocus();

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Button("Remove", ImVec2(-FLT_MIN, 0.0f)))
			{
				if (m_selectedModelIndex >= 0)
				{
					maybeRemoveModel(resourceAliases.at(m_selectedModelIndex));
				}
				else
				{
					LOG_INFO("No model selected!");
				}
			}
			ImGui::EndTable();
		}


		//		ImGui::GetWindowDrawList()->AddRectFilled(
		//		    ImGui::GetCurrentWindow()->DC.CursorPos,
		//		    ImGui::GetCurrentWindow()->DC.CursorPos + ImGui::GetContentRegionAvail(), IM_COL32(255, 0, 0, 100));

		// Push bottom separator and button to the bottom, gotta calculate their size manually
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - (style.ItemSpacing.y * 3) - ImGui::GetFontSize() -
		                           (style.FramePadding.y * 2)));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 0));

		if (ImGui::Button("Close"))
		{
			m_show = false;
		}
	}
	ImGui::End();

	if (!m_show)
	{
		HideWindowCommand::dispatch(ID);
	}
}

void ImportedModelsDialog::importModel()
{
	std::filesystem::path modelFile;
	if (importContentDialog(modelFile, "Import model"))
	{
		StateManager& stateManager = Application::getModule<StateManager>();
		Application::getModule<Core::ResourceManager>().importModel(modelFile);
		stateManager.takeSnapshot();
		// TODO: (DR) Is undoing model import a good idea? Does taking snapshots even make sens in
		//   the context of physical model files?
	}
}

bool ImportedModelsDialog::importContentDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"All files", "*"};

	return SystemDialogs::OpenSingleFileDialog(result, title, "./", filter);
}

void ImportedModelsDialog::maybeRemoveModel(const std::string& alias)
{
	std::vector<Ptr<WorkspaceModel>> models = g_workspaceDiwne->getAllModels();
	int usedCount = 0;
	for (const auto& model : models)
	{
		if (model->viewportModel().lock()->m_modelAlias == alias)
		{
			usedCount++;
		}
	}
	if (usedCount > 0)
	{
		UPtr<IWindow> modal = std::make_unique<RemoveModelModal>(alias, usedCount, [alias]() {
			removeModel(alias);
		});
		App::getModule<UIModule>().getWindowManager().openModal(std::move(modal));
	}
	else
	{
		removeModel(alias);
	}
}

void ImportedModelsDialog::removeModel(const std::string& alias)
{
	StateManager& stateManager = Application::getModule<StateManager>();
	Application::getModule<Core::ResourceManager>().removeImportedModel(alias);
	stateManager.takeSnapshot();
}
