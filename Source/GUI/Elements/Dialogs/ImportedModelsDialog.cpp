/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ImportedModelsDialog.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Defs.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Modals/RemoveModelModal.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/IconFonts/Icons.h"
#include "GUI/WindowManager.h"
#include "GUI/Workspace/Nodes/Model.h"
#include "State/StateManager.h"
#include "SystemDialogs.h"
#include "Viewport/entity/nodes/SceneModel.h"

ImportedModelsDialog::ImportedModelsDialog() : IWindow(ICON_I3T_MODELS " Manage Models") {}

void ImportedModelsDialog::render()
{
	bool modelIndexChanged = false;
	int newModelIndex;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
	ImVec2 windowSize = ImVec2(ImGui::GetFontSize() * 30, ImGui::GetFontSize() * 24);
	ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	ImGui::Begin(getName(), getShowPtr(), g_dialogFlags);
	{
		this->updateWindowInfo();
		std::vector<std::string> resourceAliases = RMI.getImportedResourceAliases();

		// Update the selected model
		if (m_selectedModelIndex >= 0 && resourceAliases.size() > 0)
		{
			if (m_selectedModelIndex >= resourceAliases.size())
			{
				m_selectedModelIndex = resourceAliases.size() - 1;
			}
			m_selectedModelAlias = resourceAliases.at(m_selectedModelIndex);
		}
		else
		{
			m_selectedModelIndex = -1;
			m_selectedModelAlias = "";
		}

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		ImGui::BeginChild("ModelList", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX,
		                  ImGuiWindowFlags_HorizontalScrollbar);

		if (resourceAliases.size() > 0)
		{
			int index = 0;
			for (auto& alias : resourceAliases)
			{
				if (ImGui::Selectable(alias.c_str(), m_selectedModelIndex == index))
				{
					modelIndexChanged = true;
					newModelIndex = index;
				}
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

		ImGui::BeginChild("ModelRightPane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

		if (ImGui::Button("Import", ImVec2(-FLT_MIN, 0)))
		{
			importModel(m_normalizeImportedModels);
		}
		ImGui::SetItemDefaultFocus();

		if (ImGui::Button("Remove", ImVec2(-FLT_MIN, 0)))
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
		ImGui::Checkbox("Normalize model size", &m_normalizeImportedModels);

		ImGui::Separator();

		ImGui::Text("Model details:");
		if (!m_selectedModelAlias.empty())
		{
			if (ImGui::BeginTabBar("##Tabs",
			                       ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton))
			{
				Core::Mesh* mesh = RMI.meshByAlias(m_selectedModelAlias);
				if (mesh)
				{
					int counter = 0;
					for (auto& meshPart : mesh->m_meshParts)
					{
						std::string meshPartName =
						    meshPart.name.empty() ? "Mesh " + std::to_string(counter) : meshPart.name;
						if (ImGui::BeginTabItem(meshPartName.c_str()))
						{
							ImGui::Text("Material:");
							ImGui::ColorEdit3("Diffuse", glm::value_ptr(meshPart.material.diffuse));
							ImGui::ColorEdit3("Specular", glm::value_ptr(meshPart.material.specular));
							ImGui::ColorEdit3("Ambient", glm::value_ptr(meshPart.material.ambient));
							ImGui::DragFloat("Shininess", &meshPart.material.shininess, 0.3f, 1.0f, 1000.0f, "%.1f",
							                 ImGuiSliderFlags_Logarithmic);

							ImGui::EndTabItem();
						}
						counter++;
					}
				}
			}
			ImGui::EndTabBar();
		}
		else
		{
			ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(IM_COL32(200, 200, 200, 255)), "No model selected");
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (modelIndexChanged)
		m_selectedModelIndex = newModelIndex;

	if (!isVisible())
	{
		HideWindowCommand::dispatch(ID);
	}
}

void ImportedModelsDialog::importModel(bool normalize)
{
	int counter = 0;
	int failCounter = 0;
	std::vector<fs::path> modelFiles;
	if (importContentDialog(modelFiles, "Import model(s)"))
	{
		StateManager& stateManager = Application::getModule<StateManager>();
		for (const auto& modelFile : modelFiles)
		{
			if (Application::getModule<Core::ResourceManager>().importModel(modelFile, normalize))
				counter++;
			else
				failCounter++;
		}

		// TODO: (DR) Is undoing model import a good idea? Does taking snapshots even make sens in
		//   the context of physical model files?
		stateManager.takeSnapshot();
	}
	LOG_INFO("[IMPORT] Imported {} resources.{}", std::to_string(counter),
	         (failCounter > 0 ? std::string(" Failed to import ") + std::to_string(failCounter) + " resource" +
	                                (failCounter == 1 ? "" : "s") + "."
	                          : ""));
	LOG_INFO("");
}

bool ImportedModelsDialog::importContentDialog(std::vector<fs::path>& result, const std::string& title)
{
	static std::vector<std::string> filter = {"All files", "*"};

	return SystemDialogs::OpenFilesDialog(result, title, "./", filter);
}

void ImportedModelsDialog::maybeRemoveModel(const std::string& alias)
{
	int usedCount = 0;
	for (const auto& model : WorkspaceWindow::g_editor->getAllModels())
	{
		if (model.viewportModel().lock()->m_modelAlias == alias)
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
