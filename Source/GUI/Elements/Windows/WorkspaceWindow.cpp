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

#include "Core/Input/InputManager.h"
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
// #include "Utils/Statistics.h"

using namespace Workspace;

WorkspaceWindow::WorkspaceWindow(bool show)
    : IWindow(ICON_T(ICON_I3T_WORKSPACE " ", "Workspace"), show), m_wholeApplication(App::get())
{
	m_autoFocus = true;
}

void pinStyleSelection(int* ptr)
{
	static const char* pinStyleNames[4] = {"Square", "Socket", "SocketSquare", "Circle"};
	for (int i = 0; i < 4; i++)
	{
		if (ImGui::MenuItem(pinStyleNames[i], NULL, *ptr == i))
			*ptr = i;
	}
	ImGui::EndMenu();
}

void WorkspaceWindow::render()
{
	// Statistics::startTimer("WorkspaceWindow::render");

	GUI::dockTabStylePush();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f)); // Disable padding for node editor
	ImGui::PushStyleColor(ImGuiCol_WindowBg, I3T::getTheme().get(EColor::NodeEditorBackground));
	ImGui::PushStyleColor(ImGuiCol_Border, I3T::getTheme().get(EColor::BorderDim));
	/* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName(), getShowPtr(),
	                 g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar |
	                     ImGuiWindowFlags_NoScrollWithMouse))
	{
		this->updateWindowInfo();
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
		GUI::dockTabStylePop(); // MUST BE POPPED IN THE WINDOW BEGIN ELSE STATEMENT TOO!!

		showMenuBar();

		processTrackingInput();

		m_channelSplitter.Split(ImGui::GetWindowDrawList(), 2);
		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 1);

		bool menuInteraction = false;

		if (WorkspaceModule::g_settings.showQuickAddMenu)
			menuInteraction |= showQuickAddButtons();

		if (Core::GraphManager::isTracking())
		{
			menuInteraction |= showTrackingTimeline();
		}

		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 0);

		DIWNE::DrawMode drawMode = DIWNE::DrawMode_JustDraw;
		// TODO: (DR) Make this consistent with ViewportWindow (check for active input rather than focus)
		if (I3T::getUI()->getWindowManager().isFocused<WorkspaceWindow>() && !menuInteraction)
		{
			drawMode = DIWNE::DrawMode_Interactive;
		}

		WorkspaceModule::g_editor->draw(drawMode);

		m_channelSplitter.Merge(ImGui::GetWindowDrawList());
	}
	else
	{
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);
		GUI::dockTabStylePop();
	}
	ImGui::End();

	// Statistics::stopTimer("WorkspaceWindow::render");
}

void WorkspaceWindow::processTrackingInput()
{
	if (InputManager::isActionTriggered("trackingEscOff", EKeyState::Pressed))
		WorkspaceModule::g_editor->stopTracking();
	if (InputManager::isActionTriggered("trackingSmoothLeft", EKeyState::Pressed))
		WorkspaceModule::g_editor->trackingSmoothLeft();
	if (InputManager::isActionTriggered("trackingSmoothRight", EKeyState::Pressed))
		WorkspaceModule::g_editor->trackingSmoothRight();
	if (InputManager::isActionTriggered("trackingJaggedLeft", EKeyState::Pressed))
		WorkspaceModule::g_editor->trackingJaggedLeft();
	if (InputManager::isActionTriggered("trackingJaggedRight", EKeyState::Pressed))
		WorkspaceModule::g_editor->trackingJaggedRight();
	// if (InputManager::isActionTriggered("trackingModeSwitch", EKeyState::Pressed))
	// 	this->trackingModeSwitch();
	// if (InputManager::isActionTriggered("trackingSwitch", EKeyState::Pressed))
	// 	this->trackingSwitch();
	// if (InputManager::isActionTriggered("trackingSwitchOn", EKeyState::Pressed))
	// 	this->trackingSwitchOn();
	// if (InputManager::isActionTriggered("trackingSwitchOff", EKeyState::Pressed))
	// 	this->trackingSwitchOff();
	if (WorkspaceModule::g_editor->isTracking())
	{
		if (InputManager::isAxisActive("trackingSmoothLeft") != 0)
		{
			WorkspaceModule::g_editor->trackingSmoothLeft();
		}
		if (InputManager::isAxisActive("trackingSmoothRight") != 0)
		{
			WorkspaceModule::g_editor->trackingSmoothRight();
		}
	}
}

bool WorkspaceWindow::showQuickAddButtons()
{
	bool interacted = false;

	ImVec2 topLeftButtonsOffset;
	if (!WorkspaceModule::g_settings.quickAddMenuCollapsed)
		topLeftButtonsOffset = DIWNE_TRUNC_VEC(ImVec2(ImGui::GetFontSize() * 0.8f, ImGui::GetFontSize() * 1.9f));
	else
		topLeftButtonsOffset = DIWNE_TRUNC_VEC(ImVec2(0.f, ImGui::GetFontSize() * 2.5f));
	ImVec2 topLeftButtonsOrigin = GUI::glmToIm(this->m_windowMin) + topLeftButtonsOffset;

	ImVec2 originalPos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(topLeftButtonsOrigin);

	float dpiScale = I3T::getTheme().getDpiScale();
	bool groupStarted = false;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10.f * dpiScale));

	if (!WorkspaceModule::g_settings.quickAddMenuCollapsed)
	{
		ImGui::BeginGroup();
		groupStarted = true;

		// TODO: Implement buttons, set their icons, perhaps add some Theme variable for size / positioning

		ImVec2 btnSize = ImVec2(43.f * I3T::getTheme().getDpiScale(), 34.f * dpiScale);

		if (GUI::FloatingButton(ICON_FA_EYE "##Btn1", btnSize))
			interacted = true;

		if (GUI::FloatingButton(ICON_I3T_MODEL "##Btn2", btnSize))
			interacted = true;

		if (GUI::FloatingButton(ICON_FA_SUN "##Btn3", btnSize))
			interacted = true;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 4.f * dpiScale));
	float rounding = 0.f;
	if (!WorkspaceModule::g_settings.quickAddMenuCollapsed)
		rounding = I3T::getTheme().get(ESize::FloatingButtonRounding);
	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
	if (GUI::FloatingButtonDark(WorkspaceModule::g_settings.quickAddMenuCollapsed ? ICON_FA_ANGLE_RIGHT
	                                                                              : ICON_FA_ANGLE_LEFT
	                                "###CollapseQuickAdd",
	                            ImVec2(22.f, 22.f) * dpiScale, rounding))
	{
		interacted = true;
		WorkspaceModule::g_settings.quickAddMenuCollapsed = !WorkspaceModule::g_settings.quickAddMenuCollapsed;
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	if (groupStarted)
		ImGui::EndGroup();

	ImGui::SetCursorScreenPos(originalPos);

	return interacted;
}

bool WorkspaceWindow::showTrackingTimeline()
{
	bool interacted = false;

	Core::MatrixTracker* tracker = Core::GraphManager::getTracker();

	float dpiScale = I3T::getTheme().getDpiScale();
	ImVec2 padding = ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize());

	ImVec2 topLeft = GUI::glmToIm(this->m_windowMin) + padding;
	ImVec2 topRight = ImVec2(this->m_windowMax.x - padding.x, this->m_windowMin.y + padding.y);
	float timelineWidth = topRight.x - topLeft.x;

	ImGui::SetCursorScreenPos(topLeft);

	if (m_trackingBox.GetArea() > 0.f)
	{
		float rounding = 6;
		const ImVec2 boxPadding = ImVec2(ImGui::GetFontSize() * 0.5f, ImGui::GetFontSize() * 0.5f);
		ImGui::GetWindowDrawList()->AddRectFilled(m_trackingBox.Min - boxPadding, m_trackingBox.Max + boxPadding,
		                                          ImColor(0.f, 0.f, 0.f, 0.4f), rounding, ImDrawFlags_RoundCornersAll);
		ImGui::GetWindowDrawList()->AddRect(m_trackingBox.Min - boxPadding, m_trackingBox.Max + boxPadding,
		                                    ImGui::GetColorU32(ImGuiCol_Border, 0.7f), rounding,
		                                    ImDrawFlags_RoundCornersAll);
	}

	ImGui::BeginGroup();

	float trackingTitleScale = 1.2f;
	std::string trackingTitle = fmt::format(_tbd("Tracking") " {:.2f}%", tracker->getProgress() * 100.f);
	ImVec2 trackingTitleSize = ImGui::CalcTextSize(trackingTitle.c_str());

	ImVec2 btnSize = ImVec2(28.f * I3T::getTheme().getDpiScale(), 24.f * dpiScale);

	if (GUI::FloatingButton(ICON_FA_RIGHT_LEFT "##SwitchDirBtn", btnSize))
	{
		tracker->reverseDirection();
		interacted = true;
	}

	ImGui::SameLine();

	// TODO: Maybe use DIWNE layout here in the future
	float springWidth = (timelineWidth - trackingTitleSize.x * trackingTitleScale - btnSize.x * 2 -
	                     ImGui::GetStyle().ItemSpacing.x * 2) /
	                    2.f;
	ImGui::Dummy(ImVec2(springWidth, 0.f));
	ImGui::SameLine(0, 0);

	float oldScale = ImGui::GetCurrentWindow()->FontWindowScale;
	ImGui::SetWindowFontScale(trackingTitleScale);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(trackingTitle.c_str());
	ImGui::SetWindowFontScale(oldScale);

	ImGui::SameLine();

	ImGui::Dummy(ImVec2(springWidth, 0.f));
	ImGui::SameLine(0, 0);

	{
		const char* trackingSettingsPopup = "tracking_settings";
		if (GUI::FloatingButton(ICON_FA_GEAR "##TrackSettingsBtn", btnSize))
		{
			interacted = true;
			ImGui::OpenPopup(trackingSettingsPopup);
		}
		else
		{
			if (ImGui::IsItemHovered())
			{
				interacted = true;
				GUI::Tooltip(_tbd("Tracking settings"), "");
			}
		}

		if (ImGui::BeginPopup(trackingSettingsPopup))
		{
			interacted = true;
			ImGui::TextDisabled(_tbd("Tracking settings"));
			ImGui::Dummy({0.0f, ImGui::GetTextLineHeight() * 0.25f});
			I3TGui::MenuItemWithLog(_tbd("Track in world space"), nullptr, &tracker->m_trackInWorldSpace);

			ImGui::EndPopup();
		}
	}

	DIWNE::DGui::NewLine(0.2f);

	ImGui::SetNextItemWidth(timelineWidth);
	m_trackingSliderProgress = tracker->getProgress();
	if (TrackingSlider(tracker, "###trackingSlider", &m_trackingSliderProgress, tracker->getDirection()))
	{
		interacted = true;
		tracker->setProgress(m_trackingSliderProgress);
	}
	ImRect sliderRect = ImGui::GetCurrentContext()->LastItemData.Rect;
	bool sliderActive = ImGui::IsItemHovered() || ImGui::IsItemActive();

	ImGui::EndGroup();
	m_trackingBox = ImGui::GetCurrentContext()->LastItemData.Rect;

	if (sliderActive)
	{
		interacted = true;
		bool leftToRight = tracker->getDirection() == Core::TrackingDirection::LeftToRight;
		ImGui::GetWindowDrawList()->AddLine(
		    ImVec2(sliderRect.Min.x +
		               timelineWidth * (leftToRight ? m_trackingSliderProgress : 1.f - m_trackingSliderProgress),
		           sliderRect.GetCenter().y),
		    ImVec2(WorkspaceModule::g_editor->m_trackingCursorPos),
		    ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Tracking_Cursor) * ImVec4(1.f, 1.f, 1.f, 0.7f)));
	}

	return interacted;
}

void WorkspaceWindow::showMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		showEditMenu();
		showAddMenu();
		showViewMenu();
		showDebugMenu();
		showDiwneStyleMenu();
		ImGui::EndMenuBar();
	}
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

		if (I3TGui::BeginMenuWithLog(_t("Selection")))
		{
			if (I3TGui::MenuItemWithLog(_t("Select all"), "Ctrl+A"))
			{
				WorkspaceModule::g_editor->selectAllNodes();
				App::getModule<StateManager>().requestSnapshot();
			}
			if (I3TGui::MenuItemWithLog(_t("Invert"), "Ctrl+I"))
			{
				WorkspaceModule::g_editor->invertSelection();
				App::getModule<StateManager>().requestSnapshot();
			}
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Zoom")))
		{
			if (I3TGui::MenuItemWithLog(_t("To all"), "Ctrl+Alt+A"))
			{
				WorkspaceModule::g_editor->zoomToAll();
			}
			if (I3TGui::MenuItemWithLog(_t("To selection"), "Ctrl+Alt+S"))
			{
				WorkspaceModule::g_editor->zoomToSelected();
			}
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Delete")))
		{
			if (I3TGui::MenuItemWithLog(_t("Selected nodes")))
			{
				WorkspaceModule::g_editor->deleteSelectedNodes();
			}

			if (I3TGui::MenuItemWithLog(_t("All nodes")))
			{
				WorkspaceModule::g_editor->clear();
			}

			ImGui::Separator();

			if (I3TGui::MenuItemWithLog(_t("Selected links")))
			{
				for (auto& link : WorkspaceModule::g_editor->m_links)
				{
					if (link->getSelected())
					{
						link->destroy();
					}
				}
			}

			if (I3TGui::MenuItemWithLog(_t("All links")))
			{
				for (auto& link : WorkspaceModule::g_editor->m_links)
				{
					link->destroy();
				}
			}
			ImGui::EndMenu();
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

void WorkspaceWindow::showDebugMenu()
{
#if DIWNE_DEBUG_ENABLED
	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
		I3TGui::MenuItemWithLog("Enable", nullptr, &(WorkspaceModule::g_editor->m_diwneDebug));
		I3TGui::MenuItemWithLog("Layout", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugLayout));
		I3TGui::MenuItemWithLog("Interaction", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugInteractions));
		I3TGui::MenuItemWithLog("Objects", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugObjects));
		I3TGui::MenuItemWithLog("Core", nullptr, &(WorkspaceModule::g_editor->m_diwneDebugCustom));
		ImGui::PopItemFlag();
		ImGui::EndMenu();
	}
#endif
}

void WorkspaceWindow::showDiwneStyleMenu()
{
	// TODO: Temporary, remove and move into I3T themes / preferences!
	if (ImGui::BeginMenu("Style"))
	{
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

		ImGui::MenuItem("Create nodes on new scene", NULL, &WorkspaceModule::g_createNodesOnInit);
		if (ImGui::BeginMenu("Pin style"))
		{
			if (ImGui::BeginMenu("General"))
				pinStyleSelection(&WorkspaceModule::g_pinStyle);
			if (ImGui::BeginMenu("Matrix Mul"))
				pinStyleSelection(&WorkspaceModule::g_pinStyleMul);
			if (ImGui::BeginMenu("Pulse"))
				pinStyleSelection(&WorkspaceModule::g_pinStylePulse);
			if (ImGui::BeginMenu("Screen"))
				pinStyleSelection(&WorkspaceModule::g_pinStyleScreen);
			if (ImGui::BeginMenu("Seq model matrix"))
				pinStyleSelection(&WorkspaceModule::g_pinStyleModelMatrix);
			ImGui::EndMenu();
		}
		ImGui::MenuItem("Drag by label", nullptr,
		                WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_ENABLE_DRAG_LABEL));
		ImGui::SliderFloat2("Pin spacing",
		                    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec2>(DIWNE::Style::PIN_SPACING)->x, 0.0f,
		                    20.f, "%.2f");
		ImGui::SliderFloat("Pin label spacing",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_LABEL_SPACING), 0.0f,
		                   20.f, "%.2f");

		ImGui::SeparatorText("Socket pin style");
		ImGui::SliderFloat("Pin socket offset",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_OFFSET),
		                   -25.0f, 25.f, "%.2f");
		ImGui::SliderFloat("Pin socket thickness",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_THICKNESS),
		                   0.0f, 20.f, "%.2f");
		ImGui::SliderFloat("Pin socket connected gap",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_CONNECTED_GAP),
		                   0.0f, 20.f, "%.2f");
		ImGui::SliderFloat("Pin socket border width (0 for off)",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_BORDER_WIDTH),
		                   0.0f, 20.f, "%.2f");
		ImGui::ColorEdit4(
		    "Pin socket border color",
		    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_SOCKET_BORDER_COLOR)->x,
		    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
		ImGui::SliderFloat("Pin socket rounding",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_ROUNDING),
		                   0.0f, 20.f, "%.2f");
		ImGui::SliderFloat(
		    "Pin socket inner rounding",
		    WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SOCKET_INNER_ROUNDING), 0.0f, 20.f,
		    "%.2f");

		ImGui::SeparatorText("Square pin style");
		ImGui::SliderFloat("Pin square offset",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_OFFSET),
		                   -25.0f, 25.f, "%.2f");

		ImGui::SliderFloat("Pin square rounding",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_ROUNDING),
		                   0.0f, 20.f, "%.2f");
		ImGui::SliderFloat("Pin square border width (0 for off)",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_SQUARE_BORDER_WIDTH),
		                   0.0f, 20.f, "%.2f");
		ImGui::ColorEdit4(
		    "Pin square border color",
		    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_SQUARE_BORDER_COLOR)->x,
		    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::SeparatorText("Circle pin style");
		ImGui::SliderFloat("Pin circle offset",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_CIRCLE_OFFSET),
		                   -25.0f, 25.f, "%.2f");
		ImGui::SliderFloat("Pin circle border width (0 for off)",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_CIRCLE_BORDER_WIDTH),
		                   0.0f, 20.f, "%.2f");
		ImGui::ColorEdit4(
		    "Pin circle border color",
		    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_CIRCLE_BORDER_COLOR)->x,
		    ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::SeparatorText("Pin hover background");
		ImGui::MenuItem("Pin BG on hover", nullptr,
		                WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_ENABLE_HOVER_BG));
		ImGui::SliderFloat2("Pin BG spacing",
		                    &WorkspaceModule::g_editor->styleBase().getPtr<ImVec2>(DIWNE::Style::PIN_BG_SPACING)->x,
		                    0.0f, 20.f, "%.2f");
		ImGui::ColorEdit4("Pin BG color",
		                  &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::PIN_BG_COLOR)->x,
		                  ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
		ImGui::SliderFloat("Pin BG rounding",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_BG_ROUNDING), 0.0f,
		                   20.f, "%.2f");

		ImGui::SeparatorText("Link style");
		ImGui::SliderFloat("Link border width (0 for off)",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::LINK_BORDER_WIDTH), 0.0f,
		                   20.f, "%.2f");
		ImGui::ColorEdit4("Lin border color",
		                  &WorkspaceModule::g_editor->styleBase().getPtr<ImVec4>(DIWNE::Style::LINK_BORDER_COLOR)->x,
		                  ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::SeparatorText("Pin dimming");
		ImGui::MenuItem("Enabled", nullptr,
		                WorkspaceModule::g_editor->styleBase().getPtr<bool>(DIWNE::Style::PIN_DIM_ENABLED));
		ImGui::SliderFloat("Dim alpha",
		                   WorkspaceModule::g_editor->styleBase().getPtr<float>(DIWNE::Style::PIN_DIM_ALPHA), 0.0f, 1.f,
		                   "%.2f");

		ImGui::SeparatorText("Node icons");
		ImGui::MenuItem("Use LOD icons", NULL, &CoreNode_useLODIcons);
		ImGui::MenuItem("Use three dots for multi LODs (>2 modes)", NULL, &CoreNode_useDotsForMultiLOD);
		ImGui::MenuItem("Shift LOD icons", NULL, &CoreNode_shiftLODIcons);
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
		if (ImGui::BeginMenu("Set value mode icon (when three dots are off)"))
		{
			ImGui::MenuItem("Pen in box", NULL, &CoreNode_usePenInBoxIcon);
			ImGui::EndMenu();
		}

		ImGui::PopItemFlag();
		ImGui::EndMenu();
	}
}
bool WorkspaceWindow::TrackingSlider(Core::MatrixTracker* tracker, const char* label, void* p_data,
                                     Core::TrackingDirection direction)
{
	float p_min, p_max;
	if (direction == Core::TrackingDirection::RightToLeft)
	{
		p_min = 1.f;
		p_max = 0.f;
	}
	else
	{
		p_min = 0.f;
		p_max = 1.f;
	}
	float minHeight = ImGui::GetFontSize() * 1.5f + ImGui::GetStyle().FramePadding.y * 2.0f;
	return TrackingSlider(tracker, label, ImGuiDataType_Float, p_data, &p_min, &p_max, "%.3f",
	                      ImGuiSliderFlags_AlwaysClamp, minHeight);
}

// Custom modified copy of ImGui::SliderScalar()
// Will likely need to be updated sometimes during ImGui upgrades.
// Uses rendering modifications from unmerged pull request !3599 (https://github.com/ocornut/imgui/pull/3599)
// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a slider, they are all
// required. Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand
// how to use this function directly.
bool WorkspaceWindow::TrackingSlider(Core::MatrixTracker* tracker, const char* label, ImGuiDataType data_type,
                                     void* p_data, const void* p_min, const void* p_max, const char* format,
                                     ImGuiSliderFlags flags, float minHeight)
{
	float dpiScale = ImGui::GetWindowDpiScale();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = ImGui::CalcItemWidth();

	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	float frame_height = ImMax(minHeight > 0.f ? minHeight : 0.f, label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, frame_height));
	const ImRect total_bb(frame_bb.Min,
	                      frame_bb.Max +
	                          ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
		return false;

	// Default format string when passing NULL
	if (format == NULL)
		format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
	bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
	if (!temp_input_is_active)
	{
		// Tabbing or CTRL-clicking on Slider turns it into an input box
		const bool input_requested_by_tabbing =
		    temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
		const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
		const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id);
		if (make_active && clicked)
			ImGui::SetKeyOwner(ImGuiKey_MouseLeft, id);
		if (make_active && temp_input_allowed)
			if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) ||
			    (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
				temp_input_is_active = true;

		if (make_active && !temp_input_is_active)
		{
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}
	}

	if (temp_input_is_active)
	{
		// Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
		const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
		return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL,
		                              is_clamp_input ? p_max : NULL);
	}

	// Draw frame
	// ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

	// Slider behavior
	ImRect grab_bb;
	const bool value_changed =
	    ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
	if (value_changed)
		ImGui::MarkItemEdited(id);

	// Render track
	float SliderContrast = 0.5f; // TODO: Hookup to a parameter or something
	float innerThickness = 0.22f;
	float outerThickness = 0.36f;
	float rounding = 6 * dpiScale;
	float transformMargin = ImGui::GetFontSize() * 0.3f;

	ImRect tOuterRect = frame_bb;
	ImRect tInnerRect = frame_bb;
	if (innerThickness != 1.0f)
	{
		float shrink_amount = (float) (int) ((frame_bb.Max.y - frame_bb.Min.y) * 0.5f * (1.0f - innerThickness));
		tInnerRect.Min.y += shrink_amount;
		tInnerRect.Max.y -= shrink_amount;
	}
	if (outerThickness != 1.0f)
	{
		float shrink_amount = (float) (int) ((frame_bb.Max.y - frame_bb.Min.y) * 0.5f * (1.0f - outerThickness));
		tOuterRect.Min.y += shrink_amount;
		tOuterRect.Max.y -= shrink_amount;
	}

	const ImVec4& colorActive = I3T::getColor(EColor::Nodes_Tracking_ColorActive);
	const ImVec4& colorOverlay = I3T::getColor(EColor::Nodes_Tracking_OverlayActive);
	const ImVec4& cursorCol = I3T::getColor(EColor::Nodes_Tracking_Cursor);
	const ImVec4& cursorColHovered = I3T::getColor(EColor::Nodes_Tracking_CursorHovered);
	const ImVec4& cursorColActive = I3T::getColor(EColor::Nodes_Tracking_CursorActive);

	// const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id    ? ImGuiCol_FrameBgActive
	// 										   : g.HoveredId == id ? ImGuiCol_FrameBgHovered
	// 															   : ImGuiCol_FrameBg,
	// 										   1.0f + SliderContrast);
	// const ImU32 frame_col = ImGui::ColorConvertFloat4ToU32(g.ActiveId == id    ? cursorColActive
	//                                                        : g.HoveredId == id ? cursorColHovered
	//                                                                            : cursorCol);
	const ImU32 progressBg = ImColor(0.7f, 0.7f, 0.f, 1.0f);
	const ImU32 progressOverlay = ImColor(0.75, 0.75f, 0.f, 0.4f);
	const ImU32 frame_col_after = ImColor(0, 0, 0, 0);
	// const ImU32 frame_col_after = ImGui::GetColorU32(g.ActiveId == id    ? ImGuiCol_FrameBgActive
	//                                                  : g.HoveredId == id ? ImGuiCol_FrameBgHovered
	//                                                                      : ImGuiCol_FrameBg,
	//                                                  1.0f - SliderContrast);
	ImGui::RenderNavHighlight(frame_bb, id);

	Core::TrackingDirection dir = tracker->getDirection();
	bool leftToRight = dir == Core::TrackingDirection::LeftToRight;
	auto& trackedMatrices = tracker->getMatrices();
	ImVec4 tickColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImRect trackRect = frame_bb;
	ImVec2 trackSize = trackRect.GetSize();
	float tParam = tracker->getProgress();
	const int matricesCount = trackedMatrices.size();
	const float matStep = 1.0f / (float) matricesCount;

	static std::array spaceColors = {ImVec4(0, 0, 1, 1), ImVec4(0, 1, 0, 1), ImVec4(1, 0, 0, 1)};

	// Draw solid progress bg
	TrackingSlider_drawProgress(grab_bb, tOuterRect, rounding, progressBg, frame_col_after, leftToRight);

	// Draw a bar for each tracked matrix using color based on space type
	for (int i = leftToRight ? 0 : matricesCount - 1; leftToRight ? i < matricesCount : i >= 0; leftToRight ? i++ : i--)
	{
		auto& matrix = trackedMatrices[i];
		TrackingSlider_drawRect(i * matStep, matStep, tInnerRect, transformMargin, leftToRight,
		                        spaceColors[static_cast<int>(matrix->transform->data.space)], rounding, 1.f * dpiScale,
		                        I3T::getColor(EColor::BorderDim));
	}

	// Draw progress overlay
	TrackingSlider_drawProgress(grab_bb, tInnerRect, rounding, progressOverlay, frame_col_after, leftToRight);

	// Draw matrix ticks
	// for (int i = leftToRight ? 0 : matricesCount - 1; leftToRight ? i < matricesCount : i >= 0; leftToRight ? i++ :
	// i--)
	// {
	// 	auto& matrix = trackedMatrices[i];
	// 	float param = i * matStep;
	// 	TrackingSlider_drawTick(param, trackRect, tInnerRect.GetHeight(), leftToRight, tickColor);
	// }

	// Draw transform ticks
	for (auto& transform : tracker->getTrackedTransforms())
	{
		float param = transform->data.mIndex * matStep;
		TrackingSlider_drawTick(param, trackRect, tOuterRect.GetHeight(), leftToRight, tickColor);
	}

	// Draw sequence ticks
	for (auto& seq : tracker->getTrackedSequences())
	{
		if (seq->data.getChildCount() <= 0)
			continue;
		float param = seq->data.mIndex * matStep;
		TrackingSlider_drawTick(param, trackRect, frame_bb.GetHeight(), leftToRight, tickColor);
	}

	// Draw last tick and arrow
	TrackingSlider_drawTick(1.0f, trackRect, frame_bb.GetHeight(), leftToRight, tickColor);
	// TrackingSlider_drawArrow(1.0f, tInnerRect, leftToRight, tickColor);

	/////

	// Render grab
	if (grab_bb.Max.x > grab_bb.Min.x)
		window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max,
		                                ImGui::ColorConvertFloat4ToU32(g.ActiveId == id ? cursorColActive : cursorCol),
		                                rounding);

	return value_changed;
}

void WorkspaceWindow::TrackingSlider_drawTick(float tParam, const ImRect& trackRect, float height, bool leftToRight,
                                              const ImVec4& tickColor)
{
	float tickPosX = trackRect.Min.x + trackRect.GetSize().x * (leftToRight ? tParam : 1.f - tParam);
	float cy = trackRect.GetCenter().y;
	ImGui::GetWindowDrawList()->AddLine(ImVec2(tickPosX, cy - height / 2), ImVec2(tickPosX, cy + height / 2),
	                                    ImGui::ColorConvertFloat4ToU32(tickColor), 2.f * ImGui::GetWindowDpiScale());
}
void WorkspaceWindow::TrackingSlider_drawArrow(float tParam, const ImRect& trackRect, bool leftToRight,
                                               const ImVec4& color)
{
	float tickPosX = trackRect.Min.x + trackRect.GetSize().x * (leftToRight ? tParam : 1.f - tParam);
	float width = ImGui::GetFontSize();
	if (leftToRight)
	{
		ImGui::GetWindowDrawList()->AddTriangleFilled(
		    ImVec2(tickPosX, trackRect.GetCenter().y), ImVec2(tickPosX - width, trackRect.Max.y),
		    ImVec2(tickPosX - width, trackRect.Min.y), ImGui::ColorConvertFloat4ToU32(color));
	}
	else
	{
		ImGui::GetWindowDrawList()->AddTriangleFilled(
		    ImVec2(tickPosX, trackRect.GetCenter().y), ImVec2(tickPosX + width, trackRect.Min.y),
		    ImVec2(tickPosX + width, trackRect.Max.y), ImGui::ColorConvertFloat4ToU32(color));
	}
}
void WorkspaceWindow::TrackingSlider_drawRect(float tParam, float tStep, const ImRect& trackRect, float margin,
                                              bool leftToRight, const ImVec4& color, float rounding,
                                              float borderThickness, const ImVec4& borderCol)
{
	float posX1 = trackRect.Min.x + trackRect.GetSize().x * (leftToRight ? tParam : 1.f - tParam - tStep);
	float posX2 = posX1 + tStep * trackRect.GetSize().x;
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(posX1 + margin, trackRect.Min.y),
	                                          ImVec2(posX2 - margin, trackRect.Max.y),
	                                          ImGui::ColorConvertFloat4ToU32(color), rounding);
	if (borderThickness > 0.f)
	{
		ImGui::GetWindowDrawList()->AddRect(ImVec2(posX1 + margin, trackRect.Min.y),
		                                    ImVec2(posX2 - margin, trackRect.Max.y),
		                                    ImGui::ColorConvertFloat4ToU32(borderCol), rounding, 0, borderThickness);
	}
}

void WorkspaceWindow::TrackingSlider_drawProgress(const ImRect& grab_bb, const ImRect& tOuterRect, float rounding,
                                                  const ImU32 frame_col, const ImU32 frame_col_after, bool leftToRight)
{
	ImGui::GetWindowDrawList()->AddRectFilled(
	    tOuterRect.Min, ImVec2(grab_bb.Min.x + (grab_bb.Max.x - grab_bb.Min.x) * 0.5f, tOuterRect.Max.y),
	    leftToRight ? frame_col : frame_col_after, rounding, ImDrawFlags_RoundCornersLeft);
	ImGui::GetWindowDrawList()->AddRectFilled(
	    ImVec2(grab_bb.Max.x - (grab_bb.Max.x - grab_bb.Min.x) * 0.5f, tOuterRect.Min.y), tOuterRect.Max,
	    leftToRight ? frame_col_after : frame_col, rounding, ImDrawFlags_RoundCornersRight);
}