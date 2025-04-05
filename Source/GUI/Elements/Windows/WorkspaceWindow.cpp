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
	/* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName(), getShowPtr(),
	                 g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar |
	                     ImGuiWindowFlags_NoScrollWithMouse))
	{
		GUI::dockTabStylePop();
		ImGui::PopStyleVar();
		this->updateWindowInfo();

		if (ImGui::BeginMenuBar())
		{
			showEditMenu();
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

		if (I3TGui::MenuItemWithLog(_t("Zoom all")))
		{
			WorkspaceModule::g_editor->zoomToAll();
		}

		ImGui::EndMenu();
	}
}
