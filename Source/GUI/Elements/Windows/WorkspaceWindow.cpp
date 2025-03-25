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
#include "Localization/Localization.h"
#include "Logger/Logger.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

static DIWNE::SettingsDiwne settingsDiwne;

using namespace Workspace;

Ptr<WorkspaceDiwne> WorkspaceWindow::g_editor;

WorkspaceWindow::WorkspaceWindow(bool show)
    : IWindow(ICON_T(ICON_I3T_WORKSPACE " ", "Workspace"), show), m_wholeApplication(App::get())
{
	m_autoFocus = true;
	initDiwneFromTheme();
	g_editor = std::make_shared<WorkspaceDiwne>(&settingsDiwne);

	// TODO: These actions should be handled by the WorkspaceNodeEditor instance
	//  in its processInteractions() method so it can react to input blocking properly
	//  Eg. what if we press Ctrl+A in an ImGui text field? We're interacting with a
	//  text field which should be flagged in the FrameContext object passed along to
	//  processInteractions. Binding actions to the window ignores this outright.
	//  Also we should react to actions on a per frame basis using if(trigger) syntax
	//  rather than using bindings as its a more suitable approach in immediate mode UI

	// TODO: Workspace should be itself a module, so we don't fetch workspace nodes through the bloody window manager
	//  (viz NodeDeserializer) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	App::getModule<StateManager>().addOriginator(this);

	// Setup viewport selection callback
	I3T::getViewport()->getMainScene().lock()->addSelectionCallback([](Vp::Entity* newlySelectedEntity) {
		// Save information about this callback and perform actions based on it later while in workspace window context.
		// This is a workaround due to viewport selection occurring in unknown order at unknown time.
		g_editor->m_viewportSelectionChanged = true;
		g_editor->m_viewportLastSelectedEntity = newlySelectedEntity;
	});
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
	return *g_editor;
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
				ImGui::MenuItem("Enable", nullptr, &(g_editor->m_diwneDebug));
				ImGui::MenuItem("Layout", nullptr, &(g_editor->m_diwneDebugLayout));
				ImGui::MenuItem("Interaction", nullptr, &(g_editor->m_diwneDebugInteractions));
				ImGui::MenuItem("Objects", nullptr, &(g_editor->m_diwneDebugObjects));
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
		g_editor->draw(drawMode);
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
	if (ImGui::BeginMenu(_t("Edit")))
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

		if (ImGui::MenuItem(_t("Select all")))
		{
			g_editor->selectAllNodes();
			App::getModule<StateManager>().takeSnapshot();
		}

		if (ImGui::MenuItem(_t("Zoom all")))
		{
			g_editor->zoomToAll();
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
	visitor.dump(getNodeEditor().getAllCoreNodes().collect());

	JSON::addFloat(memento["workspace"], "zoom", g_editor->canvas().getZoom(), a);
	JSON::addRect(memento["workspace"], "workArea", g_editor->canvas().getViewportRectDiwne(), a);

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
		g_editor->setZoom(memento["workspace"]["zoom"].GetFloat());

	if (memento["workspace"].HasMember("workArea"))
		g_editor->canvas().setViewportRectDiwne(JSON::getRect(memento["workspace"]["workArea"]));
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

/////////////////////////////////////////
// NodeEditor management
/////////////////////////////////////////

bool Workspace::connectNodesNoSave(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin)
{
	bool success = std::static_pointer_cast<CoreNodeWithPins>(rhs)->getInputs().at(rhsPin)->plug(
	    std::static_pointer_cast<CoreNodeWithPins>(lhs)->getOutputs().at(lhsPin).get(), false);
	if (!success)
	{
		LOG_INFO("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSignature(),
		         rhs->getNodebase()->getSignature(), lhsPin, rhsPin);
	}
	rhs->updateDataItemsWidth();
	lhs->updateDataItemsWidth();
	return success;
}

bool Workspace::connectNodes(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin)
{
	const auto result = connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
	if (result)
	{
		App::getModule<StateManager>().takeSnapshot();
	}

	return result;
}