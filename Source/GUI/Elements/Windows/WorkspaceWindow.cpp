/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "WorkspaceWindow.h"

#include "GUI/Fonts/Icons.h"
#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "GUI/Workspace/Nodes/Basic/CoreNode.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Localization/Localization.h"
#include "Logger/Logger.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"

using namespace Workspace;

WorkspaceWindow::WorkspaceWindow(bool show)
    : IWindow(ICON_T(ICON_I3T_WORKSPACE " ", "Workspace"), show), m_wholeApplication(App::get())
{
	m_autoFocus = true;
}

void WorkspaceWindow::render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	GUI::dockTabStylePush();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getTheme().get(EColor::NodeEditorBackground));
	/* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName(), getShowPtr(),
	                 g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar |
	                     ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::PopStyleColor();
		GUI::dockTabStylePop();
		ImGui::PopStyleVar();
		this->updateWindowInfo();

		if (ImGui::BeginMenuBar())
		{
			showEditMenu();
			showAddMenu();
			showViewMenu();
#if DIWNE_DEBUG_ENABLED
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				I3TGui::MenuItemWithLog("Enable", nullptr, &(WorkspaceModule::g_editor->m_diwneDebug));
				I3TGui::MenuItemWithLog("Layout", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugLayout));
				I3TGui::MenuItemWithLog("Interaction", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugInteractions));
				I3TGui::MenuItemWithLog("Objects", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugObjects));
				ImGui::PopItemFlag();
				ImGui::EndMenu();
			}
#endif
			// TODO: Temporary, remove and move into I3T themes!
			if (ImGui::BeginMenu("Style"))
			{
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

				if (ImGui::BeginMenu("Pin style"))
				{
					ImGui::MenuItem("General", NULL, &WorkspaceModule::g_useSquarePins);
					ImGui::MenuItem("Matrix Mul", NULL, &WorkspaceModule::g_useSquarePinsMul);
					ImGui::MenuItem("Pulse", NULL, &WorkspaceModule::g_useSquarePinsPulse);
					ImGui::MenuItem("Screen", NULL, &WorkspaceModule::g_useSquarePinsScreen);
					ImGui::EndMenu();
				}
				ImGui::MenuItem(
				    "Drag by label", nullptr,
				    WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_ENABLE_DRAG_LABEL));
				ImGui::SliderFloat2(
				    "Pin spacing", &WorkspaceModule::g_editor->styleBase().getPtr<ImVec2>(DIWNE::Style::PIN_SPACING)->x,
				    0.0f, 20.f, "%.2f");
				ImGui::SliderFloat(
				    "Pin label spacing",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_LABEL_SPACING), 0.0f, 20.f,
				    "%.2f");

				ImGui::SeparatorText("Socket pin style");
				ImGui::SliderFloat(
				    "Pin socket offset",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_OFFSET), 0.0f, 20.f,
				    "%.2f");
				ImGui::SliderFloat(
				    "Pin socket thickness",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_THICKNESS), 0.0f,
				    20.f, "%.2f");
				ImGui::SliderFloat(
				    "Pin socket connected gap",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_CONNECTED_GAP), 0.0f,
				    20.f, "%.2f");
				ImGui::SliderFloat(
				    "Pin socket border width (0 for off)",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_BORDER_WIDTH), 0.0f,
				    20.f, "%.2f");
				ImGui::ColorEdit4(
				    "Pin socket border color",
				    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_SOCKET_BORDER_COLOR)->x,
				    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

				ImGui::SeparatorText("Square pin style");
				ImGui::SliderFloat(
				    "Pin square offset",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_OFFSET), 0.0f, 20.f,
				    "%.2f");

				ImGui::SliderFloat(
				    "Pin square rounding",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_ROUNDING), 0.0f, 20.f,
				    "%.2f");
				ImGui::SliderFloat(
				    "Pin square border width (0 for off)",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_BORDER_WIDTH), 0.0f,
				    20.f, "%.2f");
				ImGui::ColorEdit4(
				    "Pin square border color",
				    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_SQUARE_BORDER_COLOR)->x,
				    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

				ImGui::SeparatorText("Pin hover background");
				ImGui::MenuItem("Pin BG on hover", nullptr,
				                WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_ENABLE_HOVER_BG));
				ImGui::SliderFloat2(
				    "Pin BG spacing",
				    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec2>(DIWNE::Style::PIN_BG_SPACING)->x, 0.0f, 20.f,
				    "%.2f");
				ImGui::ColorEdit4("Pin BG color",
				                  &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_BG_COLOR)->x,
				                  ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
				ImGui::SliderFloat("Pin BG rounding",
				                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_BG_ROUNDING),
				                   0.0f, 20.f, "%.2f");

				ImGui::SeparatorText("Link style");
				ImGui::SliderFloat(
				    "Link border width (0 for off)",
				    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::LINK_BORDER_WIDTH), 0.0f, 20.f,
				    "%.2f");
				ImGui::ColorEdit4(
				    "Lin border color",
				    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::LINK_BORDER_COLOR)->x,
				    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

				ImGui::SeparatorText("Pin dimming");
				ImGui::MenuItem("Enabled", nullptr,
				                WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_DIM_ENABLED));
				ImGui::SliderFloat("Dim alpha",
				                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_DIM_ALPHA),
				                   0.0f, 1.f, "%.2f");

				ImGui::SeparatorText("Node icons");
				ImGui::MenuItem("Use LOD icons", NULL, &CoreNode_useLODIcons);
				if (ImGui::BeginMenu("LOD expand/collapse"))
				{
					if (ImGui::MenuItem("Chevron", NULL, !(CoreNode_useAngleLODIcon || CoreNode_useCaretLODIcon)))
					{
						CoreNode_useAngleLODIcon = false;
						CoreNode_useCaretLODIcon = false;
					}
					if (ImGui::MenuItem("Angle", NULL, &CoreNode_useAngleLODIcon))
					{
						CoreNode_useCaretLODIcon = false;
					}
					if (ImGui::MenuItem("Caret", NULL, &CoreNode_useCaretLODIcon))
					{
						CoreNode_useAngleLODIcon = false;
					}
					ImGui::EndMenu();
				}
				ImGui::MenuItem("Use three dots for multi LODs (>2 modes)", NULL, &CoreNode_useDotsForMultiLOD);
				if (ImGui::BeginMenu("Set value mode icon (when three dots are off)"))
				{
					ImGui::MenuItem("Pen in box", NULL, &CoreNode_usePenInBoxIcon);
					ImGui::EndMenu();
				}

				ImGui::PopItemFlag();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		DIWNE::DrawMode drawMode = DIWNE::DrawMode_JustDraw;
		// TODO: (DR) Make this consistent with ViewportWindow (check for active input rather than focus)
		if (I3T::getUI()->getWindowManager().isFocused<WorkspaceWindow>())
		{
			drawMode = DIWNE::DrawMode_Interactive;
		}
		WorkspaceModule::g_editor->draw(drawMode);
	}
	else
	{
		GUI::dockTabStylePop();
		ImGui::PopStyleVar();
	}
	ImGui::End();
}

void WorkspaceWindow::showEditMenu()
{
	if (I3TGui::BeginMenuWithLog(_t("Edit")))
	{
		/*
		if (I3TGui::MenuItemWithLog("Undo"))
		{
		    // B
		    /// \todo Undo.
		    // UndoRedo::undo();
		}

		if (I3TGui::MenuItemWithLog("Redo"))
		{
		    // N
		    /// \todo Redo.
		    // UndoRedo::redo();
		}
		 */

		if (I3TGui::MenuItemWithLog(_t("Select all")))
		{
			WorkspaceModule::g_editor->selectAllNodes();
			App::getModule<StateManager>().requestSnapshot();
		}

		ImGui::EndMenu();
	}
}

void WorkspaceWindow::showAddMenu()
{
	if (I3TGui::BeginMenuWithLog(_t("Add")))
	{
		WorkspaceDiwne& editor = *WorkspaceModule::g_editor;
		ImRect viewport = editor.canvas().getViewportRectScreen();
		editor.setPopupPosition(viewport.Min + (viewport.GetSize() * ImVec2(0.24f, 0.31f)));
		editor.addMenu();

		ImGui::EndMenu();
	}
}

void WorkspaceWindow::showViewMenu()
{
	if (I3TGui::BeginMenuWithLog(_t("View")))
	{
		if (I3TGui::BeginMenuWithLog(_t("Zoom")))
		{
			if (I3TGui::MenuItemWithLog(_t("Zoom all")))
			{
				WorkspaceModule::g_editor->zoomToAll();
			}
			if (I3TGui::MenuItemWithLog(_t("Reset zoom")))
			{
				WorkspaceModule::g_editor->setZoom(1.0f);
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog(_t("Grid")))
		{
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			I3TGui::MenuItemWithLog(_t("Show"), NULL, &WorkspaceModule::g_editor->mp_settingsDiwne->showGrid);
			I3TGui::MenuItemWithLog(_t("Use dots"), NULL, &WorkspaceModule::g_editor->mp_settingsDiwne->useDotGrid);
			ImGui::PopItemFlag();
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}