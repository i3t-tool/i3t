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
#include "SetupDialog.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "API.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/IconFonts/Icons.h"
#include "GUI/Theme/Theme.h"
#include "GUI/Toolkit.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

SetupDialog::SetupDialog() : IWindow(ICON_I3T_SETTINGS " Preferences") {}

void SetupDialog::render()
{
	static int cameraMode;
	static int angleUnits;

	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
	// style.FramePadding.y = 4;
	bool windowOpen = true;
	GUI::dockTabStylePush();
	if (ImGui::Begin(getName(), getShowPtr()))
	{
		this->updateWindowInfo();
		GUI::dockTabStylePop();

		Vp::Viewport* viewport = I3T::getViewport();
		Vp::ViewportSettings& stg = viewport->getSettings();

		if (ImGui::Button("Reset to defaults"))
		{
			App::getModule<StateManager>().resetGlobal();
		}

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

			if (ImGui::CollapsingHeader("Grid"))
			{
				ImGui::Indent();
				ImGui::ColorEdit3("Grid color", glm::value_ptr(stg.global().grid.color), ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("X axis color", glm::value_ptr(stg.global().grid.axisXColor),
				                  ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Y axis color", glm::value_ptr(stg.global().grid.axisYColor),
				                  ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Z axis color", glm::value_ptr(stg.global().grid.axisZColor),
				                  ImGuiColorEditFlags_Float);

				ImGui::SliderFloat("Size", &stg.global().grid.size, 0.01f, 2.f, "%.2f");
				ImGui::SliderFloat("Strength", &stg.global().grid.strength, 0.01f, 1.f, "%.2f");
				ImGui::SliderFloat("Line width", &stg.global().grid.lineWidth, 0.01f, 5.0f, "%.2f");
				ImGui::SliderFloat("Fade 1 start", &stg.global().grid.grid1FadeStart, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 1 end", &stg.global().grid.grid1FadeEnd, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 2 start", &stg.global().grid.grid2FadeStart, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 2 end", &stg.global().grid.grid2FadeEnd, 0.0f, 1.f, "%.2f");
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
