#include "AboutDialog.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/API.h"
#include "Utils/TextureLoader.h"

AboutDialog::AboutDialog()
{
	/// \todo MH Load DCGI logo.
	// dcgiReg = std::make_shared<TextureRegion>(TextureLoader::getHCId("dcgiLogo"), 0.0f, 0.0f, 1.0f, 1.0f);
}

void AboutDialog::render()
{
	ImGui::Begin(getName("About").c_str(), nullptr, g_dialogFlags);
	{
		ImGui::Columns(2, NULL, false);

		// ImGui::Image((void*)(intptr_t)dcgiReg->getTexId(), ImVec2(243.0f, 66.0f), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::NextColumn();

		ImGui::TextWrapped(Config::INFO_TEXT.c_str());
		ImGui::Text("");

		ImGui::PushFont(I3T::getFont(EFont::TutorialAssignment));
		ImGui::Text("Supervisor:");
		ImGui::PopFont();

		ImGui::Text(Config::SUPERVISOR.c_str());
		ImGui::Text(Config::SUPERVISOR_MAIL.c_str());
		ImGui::Text("");

		/// \todo Add interactive link to http://www.i3t-tool.org/.

		ImGui::PushFont(I3T::getFont(EFont::TutorialAssignment));
		ImGui::Text("Authors:");
		ImGui::PopFont();

		ImGui::Text(Config::AUTHOR.c_str());
		ImGui::Text("...");
		ImGui::Text("...");
		ImGui::Text("...");
		ImGui::Text("...");
		ImGui::Text("");

		ImGui::Columns(1);

		ImGui::Separator();

		// ImGui::BeginHorizontal("AboutFooter", ImVec2{ImGui::GetWindowContentRegionMax().x, 0.0f});
		ImGui::Text(Config::VERSION.c_str());
		if (ImGui::Button("OK", ImVec2{100.0f, 0.0f}))
		{
			HideWindowCommand::dispatch(ID);
		}
		// ImGui::EndHorizontal();
	}
	ImGui::End();
}
