#include "SetupDialog.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"

void SetupDialog::render()
{
	static int cameraMode;
	static int angleUnits;

	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

	auto& style = ImGui::GetStyle();

	style.FramePadding.y = 4;

	ImGui::Begin(getName("Setup").c_str());
	{
		ImGui::Text("Camera control:");
		ImGui::RadioButton("orbit", &cameraMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("trackball", &cameraMode, 1);
		ImGui::SameLine();
		ImGui::NewLine();

		ImGui::Separator();

		ImGui::Text("Angle units:");
		ImGui::RadioButton("radians", &angleUnits, 0);
		ImGui::SameLine();
		ImGui::RadioButton("degrees", &angleUnits, 1);
		ImGui::SameLine();
		ImGui::NewLine();

		ImGui::Separator();

		static bool check = true;
		ImGui::Checkbox("console", &check);

		ImGui::Separator();

		/*
		if (cameraMode == 0)
		  TabSpace::world->setToOrbitControl();
		else if (cameraMode == 1)
		  TabSpace::world->setToTrackballControl();

		if (angleUnits == 0)
		  TabSpace::setToRadians();
		else if (angleUnits == 1)
		  TabSpace::setToDegrees();
		  */

		if (ImGui::Button("OK"))
		{
			HideWindowCommand::dispatch(ID);
		}
	}
	ImGui::End();

	style.FramePadding.y = 8;
}
