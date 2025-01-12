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

#include "GUI/IconFonts/Icons.h"
#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "Logger/Logger.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

static DIWNE::SettingsDiwne settingsDiwne;

using namespace Workspace;

WorkspaceWindow::WorkspaceWindow(bool show)
    : IWindow(ICON_I3T_WORKSPACE " Workspace", show), m_wholeApplication(App::get())
{
	m_autoFocus = true;
	initDiwneFromTheme();
	g_diwne = new WorkspaceDiwne(&settingsDiwne);

	// TODO: These actions should be handled by the WorkspaceNodeEditor instance
	//  in its processInteractions() method so it can react to input blocking properly
	//  Eg. what if we press Ctrl+A in an ImGui text field? We're interacting with a
	//  text field which should be flagged in the FrameContext object passed along to
	//  processInteractions. Binding actions to the window ignores this outright.
	//  Also we should react to actions on a per frame basis using if(trigger) syntax
	//  rather than using bindings as its a more suitable approach in immediate mode UI

	// Input actions for workspace window.
	m_input->bindAction("selectAll", EKeyState::Pressed, [&]() {
		g_diwne->selectAll();
	});
	m_input->bindAction("invertSelection", EKeyState::Pressed, [&]() {
		g_diwne->invertSelection();
	});
	m_input->bindAction("zoomToAll", EKeyState::Pressed, [&]() {
		g_diwne->zoomToAll();
	});
	m_input->bindAction("zoomToSelected", EKeyState::Pressed, [&]() {
		g_diwne->zoomToSelected();
	});
	m_input->bindAction("delete", EKeyState::Pressed, [&]() {
		g_diwne->deleteCallback();
	});
	m_input->bindAction("copy", EKeyState::Pressed, [&]() {
		g_diwne->copySelectedNodes();
	});
	m_input->bindAction("paste", EKeyState::Pressed, [&]() {
		g_diwne->pasteSelectedNodes();
	});
	m_input->bindAction("cut", EKeyState::Pressed, [&]() {
		g_diwne->cutSelectedNodes();
	});
	m_input->bindAction("duplicate", EKeyState::Pressed, [&]() {
		g_diwne->duplicateClickedNode();
	});
	m_input->bindAction("duplicateSelected", EKeyState::Pressed, [&]() {
		g_diwne->duplicateSelectedNodes();
	});
	m_input->bindAction("trackingEscOff", EKeyState::Pressed, [&]() {
		g_diwne->trackingSwitchOff();
	});
	m_input->bindAction("trackingSmoothLeft", EKeyState::Pressed, [&]() {
		g_diwne->trackingSmoothLeft();
	});
	m_input->bindAction("trackingSmoothRight", EKeyState::Pressed, [&]() {
		g_diwne->trackingSmoothRight();
	});
	m_input->bindAction("trackingJaggedLeft", EKeyState::Pressed, [&]() {
		g_diwne->trackingJaggedLeft();
	});
	m_input->bindAction("trackingJaggedRight", EKeyState::Pressed, [&]() {
		g_diwne->trackingJaggedRight();
	});
	m_input->bindAction("trackingModeSwitch", EKeyState::Pressed, [&]() {
		g_diwne->trackingModeSwitch();
	});
	m_input->bindAction("trackingSwitch", EKeyState::Pressed, [&]() {
		g_diwne->trackingSwitch();
	});
	m_input->bindAction("trackingSwitchOn", EKeyState::Pressed, [&]() {
		g_diwne->trackingSwitchOn();
	});
	m_input->bindAction("trackingSwitchOff", EKeyState::Pressed, [&]() {
		g_diwne->trackingSwitchOff();
	});
	m_input->bindAction("toggleNodeWorkspaceVisibility", EKeyState::Pressed, [&]() {
		g_diwne->toggleSelectedNodesVisibility();
	});

	App::getModule<StateManager>().addOriginator(this);

	// Setup viewport selection callback
	I3T::getViewport()->getMainScene().lock()->addSelectionCallback([](Vp::Entity* newlySelectedEntity) {
		// Save information about this callback and perform actions based on it later while in workspace window context.
		// This is a workaround due to viewport selection occurring in unknown order at unknown time.
		g_diwne->m_viewportSelectionChanged = true;
		g_diwne->m_viewportLastSelectedEntity = newlySelectedEntity;
	});
}

WorkspaceWindow::~WorkspaceWindow()
{
	delete g_diwne;
}

// TODO - Make diwne change settings on theme switch (when Theme::apply() is called)
void WorkspaceWindow::initDiwneFromTheme()
{
	// TODO: (DR) Port to DiwneStyle or refactor DiwneSettings
	settingsDiwne.selectionRounding = I3T::getUI()->getTheme().get(ESize::Nodes_Rounding);
	settingsDiwne.itemSelectedBorderColor = I3T::getUI()->getTheme().get(EColor::Workspace_SelectedBorder);
	settingsDiwne.itemSelectedBorderThicknessDiwne =
	    I3T::getUI()->getTheme().get(ESize::Workspace_SelectedBorderThickness);
	settingsDiwne.objectHoverBorderColor = I3T::getUI()->getTheme().get(EColor::Workspace_FocusBorder);
	settingsDiwne.objectHoverBorderThicknessDiwne = I3T::getUI()->getTheme().get(ESize::Workspace_FocusBorderThickness);
	settingsDiwne.objectFocusForInteractionBorderColor =
	    I3T::getUI()->getTheme().get(EColor::Workspace_InteractionFocusBorder);
	settingsDiwne.objectFocusForInteractionBorderThicknessDiwne =
	    I3T::getUI()->getTheme().get(ESize::Workspace_InteractionFocusBorderThickness);
}

//
// Node builder functions.
//

WorkspaceDiwne& WorkspaceWindow::getNodeEditor()
{
	return *g_diwne;
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
				ImGui::MenuItem("Enable", nullptr, &(Workspace::g_diwne->m_diwneDebug));
				ImGui::MenuItem("Layout", nullptr, &(Workspace::g_diwne->m_diwneDebugLayout));
				ImGui::MenuItem("Interaction", nullptr, &(Workspace::g_diwne->m_diwneDebugInteractions));
				ImGui::MenuItem("Objects", nullptr, &(Workspace::g_diwne->m_diwneDebugObjects));
				ImGui::PopItemFlag();
				ImGui::EndMenu();
			}
#endif
			ImGui::EndMenuBar();
		}

		DIWNE::DrawMode drawMode = DIWNE::DrawMode::JustDraw;
		// TODO: (DR) Make this consistent with ViewportWindow (check for active input rather than focus)
		if (I3T::getUI()->getWindowManager().isFocused<WorkspaceWindow>())
		{
			drawMode = DIWNE::DrawMode::Interacting;
		}
		static bool firstFrame = false;
		if (!firstFrame)
		{
			firstFrame = true;
			g_diwne->addNodeToPosition<Model>(ImVec2(200, 200));
		}
		g_diwne->draw(drawMode);
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
	if (ImGui::BeginMenu("Edit"))
	{
		/*
		if (ImGui::MenuItem("Undo"))
		{
		    // B
		    /// \todo Undo.
		    // UndoRedo::undo();
		}

		if (ImGui::MenuItem("Redo"))
		{
		    // N
		    /// \todo Redo.
		    // UndoRedo::redo();
		}
		 */

		if (ImGui::MenuItem("Select all"))
		{
			g_diwne->selectAll();
		}

		if (ImGui::MenuItem("Zoom all"))
		{
			g_diwne->zoomToAll();
		}

		ImGui::EndMenu();
	}
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento WorkspaceWindow::saveScene(Scene* scene)
{
	Memento memento;
	rapidjson::Value::AllocatorType& a = memento.GetAllocator();

	SerializationVisitor visitor(memento);
	visitor.dump(getNodeEditor().m_workspaceCoreNodes);

	JSON::addFloat(memento["workspace"], "zoom", g_diwne->getWorkAreaZoom(), a);
	JSON::addRect(memento["workspace"], "workArea", g_diwne->getWorkAreaDiwne(), a);

	return memento;
}

void WorkspaceWindow::loadScene(const Memento& memento, Scene* scene)
{
	clearScene();

	if (!memento.HasMember("workspace"))
	{
		LOG_ERROR("Failed to load workspace! No 'workspace' member found.");
		return;
	}

	NodeDeserializer::createFrom(memento);

	if (memento["workspace"].HasMember("zoom"))
		g_diwne->setWorkAreaZoom(memento["workspace"]["zoom"].GetFloat());

	if (memento["workspace"].HasMember("workArea"))
		g_diwne->setWorkAreaDiwne(JSON::getRect(memento["workspace"]["workArea"]));
}

void WorkspaceWindow::clearScene()
{
	getNodeEditor().clear();
}

Memento WorkspaceWindow::saveGlobal()
{
	return emptyMemento();
}

void WorkspaceWindow::loadGlobal(const Memento& memento) {}

void WorkspaceWindow::clearGlobal() {}
