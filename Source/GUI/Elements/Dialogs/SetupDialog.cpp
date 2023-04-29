#include "SetupDialog.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Theme.h"
#include "GUI/UIModule.h"
#include "Viewport/Viewport.h"

void SetupDialog::render()
{
	static int cameraMode;
	static int angleUnits;

	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
	// style.FramePadding.y = 4;
	bool windowOpen = true;
	ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
	ImGui::Begin(setName("Preferences").c_str(), &windowOpen);
	{
		Vp::Viewport* viewport = App::get().viewport();
		Vp::ViewportSettings& settings = viewport->getSettings();
		if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();
			ImGui::SliderFloat("Model preview FOV", &settings.preview_fov, 5, 120, "%f");
			ImGui::SliderFloat("Model preview radius", &settings.preview_radiusFactor, 0.1f, 10.0f, "%.2f");
			ImGui::SliderFloat("Model preview rotate speed", &settings.preview_rotateSpeed, 0.f, 100.f, "%f");

			if (ImGui::CollapsingHeader("Grid"))
			{
				ImGui::Indent();
				ImGui::ColorEdit3("Grid color", glm::value_ptr(settings.grid_color), ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("X axis color", glm::value_ptr(settings.grid_axisXColor), ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Y axis color", glm::value_ptr(settings.grid_axisYColor), ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("Z axis color", glm::value_ptr(settings.grid_axisZColor), ImGuiColorEditFlags_Float);

				ImGui::SliderFloat("Size", &settings.grid_size, 0.01f, 2.f, "%.2f");
				ImGui::SliderFloat("Strength", &settings.grid_strength, 0.01f, 1.f, "%.2f");
				ImGui::SliderFloat("Line width", &settings.grid_lineWidth, 0.01f, 5.0f, "%.2f");
				ImGui::SliderFloat("Fade 1 start", &settings.grid_grid1FadeStart, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 1 end", &settings.grid_grid1FadeEnd, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 2 start", &settings.grid_grid2FadeStart, 0.0f, 1.f, "%.2f");
				ImGui::SliderFloat("Fade 2 end", &settings.grid_grid2FadeEnd, 0.0f, 1.f, "%.2f");
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
	ImGui::PopStyleColor();
	if (!windowOpen)
	{
		HideWindowCommand::dispatch(ID);
	}
	ImGui::End();
}
