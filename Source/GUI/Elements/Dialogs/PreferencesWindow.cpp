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
#include "PreferencesWindow.h"

#include "imgui.h"

#include "I3T.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Fonts/Icons.h"
#include "GUI/I3TGui.h"
#include "GUI/Theme/Theme.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Localization/Localization.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

PreferencesWindow::PreferencesWindow() : IWindow(ICON_T(ICON_I3T_SETTINGS " ", "Preferences")) {}

void PreferencesWindow::render()
{
	static int cameraMode;
	static int angleUnits;

	bool windowOpen = true;
	GUI::dockTabStylePush();
	if (ImGui::Begin(getName(), getShowPtr()))
	{
		this->updateWindowInfo();
		GUI::dockTabStylePop();

		if (I3TGui::ButtonWithLog(_t("Reset to defaults")))
		{
			App::getModule<StateManager>().resetGlobal();
		}

		showUISettings();
		showWorkspaceSettings();
		showViewportSettings();
	}
	else
	{
		GUI::dockTabStylePop();
	}
	if (!isVisible())
	{
		HideWindowCommand::dispatch(ID);
	}
	ImGui::End();
}

void PreferencesWindow::showUISettings()
{
	if (ImGui::CollapsingHeader("Loop settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		auto vsync = App::get().getAppLoopManager().isVsync();

		if (ImGui::Checkbox("Use VSync", &vsync))
		{
			App::get().setVSync(vsync);
		}
		if (!vsync)
		{
			auto shouldLimitFPS = App::get().getAppLoopManager().shouldLimitFPS();
			if (ImGui::Checkbox("Limit FPS", &shouldLimitFPS))
			{
				App::get().getAppLoopManager().enableFPSLimit(shouldLimitFPS);
			}

			if (shouldLimitFPS)
			{
				int fpsLimit = App::get().getAppLoopManager().getTargetFPS();
				if (ImGui::SliderInt("FPS Limit", &fpsLimit, 10, 500))
				{
					App::get().getAppLoopManager().setTargetFPS(fpsLimit);
				}
			}
		}

		auto shouldLimitFPSOnIdle = App::get().getAppLoopManager().shouldLimitFPSOnIdle();
		if (ImGui::Checkbox("Limit FPS during idle time", &shouldLimitFPSOnIdle))
		{
			App::get().getAppLoopManager().setShouldLimitFPSOnIdle(shouldLimitFPSOnIdle);
		}
		if (shouldLimitFPSOnIdle)
		{
			int fpsLimitOnIdle = App::get().getAppLoopManager().getTargetFPSOnIdle();
			if (ImGui::SliderInt("FPS Limit during idle time", &fpsLimitOnIdle, 5, 500))
			{
				App::get().getAppLoopManager().setTargetFPSOnIdle(fpsLimitOnIdle);
			}
		}

		ImGui::Unindent();
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("User interface", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();

		UIModule* uiModule = I3T::getUI();

		static float uiScaleTmp = 1.0f;
		if (ImGui::IsWindowAppearing())
		{
			uiScaleTmp = uiModule->getUiScale();
		}
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
		GUI::SliderFloatStepped(_tbd("UI Scale"), &uiScaleTmp, 0.05f, 1.0f, 4.0f, "{:.2f}");
		if (uiScaleTmp != uiModule->getUiScale())
		{
			ImGui::SameLine();
			if (I3TGui::ButtonWithLog(_tbd("Apply scale")))
			{
				uiModule->applyUIScalingNextFrame(uiScaleTmp);
			}
		}

		ImGui::Checkbox(_tbd("Show window tab buttons"), &uiModule->getSettings().useWindowMenuButtons);
		ImGui::SameLine();
		showHelpTip("Requires restart");

		ImGui::Checkbox(_tbd("Auto hide tab bar"), &uiModule->getSettings().autoHideTabBars);
		ImGui::SameLine();
		showHelpTip("Requires restart");

		ImGui::Unindent();
		ImGui::Spacing();
	}
}
void PreferencesWindow::showWorkspaceSettings()
{
	if (ImGui::CollapsingHeader("Workspace", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::Checkbox("Show quick add menu", &WorkspaceModule::g_settings.showQuickAddMenu);
		ImGui::SeparatorText("Tracking");
		ImGui::DragFloat("Smooth scroll speed", &WorkspaceModule::g_settings.tracking_smoothScrollSpeed, 0.01f, 0.001f);
		ImGui::DragFloat("Jagged scroll speed", &WorkspaceModule::g_settings.tracking_jaggedScrollSpeed, 0.01f, 0.001f);

		ImGui::Unindent();
		ImGui::Spacing();
	}
}

void PreferencesWindow::showViewportSettings()
{
	Vp::Viewport* viewport = I3T::getViewport();
	Vp::ViewportSettings& stg = viewport->getSettings();

	if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::SliderFloat("Model preview FOV", &stg.global().preview_fov, 5, 120, "%f");
		ImGui::SliderFloat("Model preview radius", &stg.global().preview_radiusFactor, 0.1f, 10.0f, "%.2f");
		ImGui::SliderFloat("Model preview rotate speed", &stg.global().preview_rotateSpeed, 0.f, 100.f, "%f");

		ImGui::Combo("Lighting model", &stg.global().lighting_lightingModel, "Phong\0Blinn-Phong\0\0");

		if (ImGui::CollapsingHeader("Highlight"))
		{
			ImGui::Indent();
			// ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
			ImGui::SliderFloat("Downscale factor", &stg.global().highlight.downscaleFactor, 0.01f, 1.0f, "%.2f");
			ImGui::SliderInt("Kernel size", &stg.global().highlight.kernelSize, 1, 10);
			ImGui::SliderFloat("Blur cutoff", &stg.global().highlight.outlineCutoff, 0.01f, 1.0f, "%.2f");
			ImGui::Checkbox("Use depth", &stg.global().highlight.useDepth);
			ImGui::SliderFloat("Darken factor", &stg.global().highlight.useDepth_darkenFactor, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("Desaturate factor", &stg.global().highlight.useDepth_desaturateFactor, 0.0f, 1.0f,
			                   "%.2f");
			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Grids"))
		{
			ImGui::Indent();
			ImGui::PushID("world grid");
			showGridSettings(stg.global().grid);
			ImGui::PopID();
			if (ImGui::CollapsingHeader("Local Grid"))
			{
				ImGui::Indent();
				ImGui::PushID("local grid");
				showGridSettings(stg.global().localGrid);
				ImGui::PopID();
				ImGui::Unindent();
			}
			ImGui::Unindent();
		}

		ImGui::Unindent();
		ImGui::Spacing();
	}

	//		if (ImGui::CollapsingHeader("Legacy", ImGuiTreeNodeFlags_None))
	//		{
	//			ImGui::Text("Camera control:");
	//			ImGui::RadioButton("orbit", &cameraMode, 0);
	//			ImGui::SameLine();
	//			ImGui::RadioButton("trackball", &cameraMode, 1);
	//			ImGui::SameLine();
	//			ImGui::NewLine();
	//
	//			ImGui::Separator();
	//
	//			ImGui::Text("Angle units:");
	//			ImGui::RadioButton("radians", &angleUnits, 0);
	//			ImGui::SameLine();
	//			ImGui::RadioButton("degrees", &angleUnits, 1);
	//			ImGui::SameLine();
	//			ImGui::NewLine();
	//
	//			ImGui::Separator();
	//
	//			static bool check = true;
	//			ImGui::Checkbox("console", &check);
	//		}
}

void PreferencesWindow::showGridSettings(Vp::GridSettings& grid)
{
	ImGui::SliderFloat("Size", &grid.size, 0.01f, 2.f, "%.2f");
	ImGui::SliderFloat("Line width", &grid.lineWidth, 0.01f, 5.0f, "%.2f");
	ImGui::SliderFloat("Fade 1 start", &grid.grid1FadeStart, 0.0f, 1.f, "%.2f");
	ImGui::SliderFloat("Fade 1 end", &grid.grid1FadeEnd, 0.0f, 1.f, "%.2f");
	ImGui::SliderFloat("Fade 2 start", &grid.grid2FadeStart, 0.0f, 1.f, "%.2f");
	ImGui::SliderFloat("Fade 2 end", &grid.grid2FadeEnd, 0.0f, 1.f, "%.2f");
}

void PreferencesWindow::showHelpTip(const char* text)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
