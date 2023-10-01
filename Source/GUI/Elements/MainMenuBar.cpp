#include "MainMenuBar.h"

#include "imgui.h"

#include "API.h"
#include "Commands/ApplicationCommands.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/DescriptionDialog.h"
#include "GUI/Elements/Dialogs/ImportedModelsDialog.h"
#include "GUI/Elements/Dialogs/SetupDialog.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
#include "GUI/Elements/Windows/AboutWindow.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/StyleEditor.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Windows/StartWindow.h"

using namespace UI;

using namespace std::literals;

static bool saveSceneDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"I3T scene files", "*"s + I3T_SCENE_EXTENSION};

	return SystemDialogs::SaveSingleFileDialog(result, title, Configuration::sceneRoot.string(), filter);
}

static bool openSceneDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"I3T scene files", "*"s + I3T_SCENE_EXTENSION};

	return SystemDialogs::OpenSingleFileDialog(result, title, Configuration::sceneRoot.string(), filter);
}

//

static void showRecentFiles()
{
	const auto& recentFiles = getUserData().recentFiles;
	if (recentFiles.empty())
	{
		ImGui::Text("No recent files");
	}

	std::optional<std::filesystem::path> sceneToOpen;
	for (auto it = recentFiles.rbegin(); it != recentFiles.rend(); ++it)
	{
		if (ImGui::MenuItem(it->string().c_str()))
		{
			sceneToOpen = *it;
		}
	}

	if (sceneToOpen)
	{
		App::getModule<StateManager>().loadScene(*sceneToOpen);
	}
}

static void saveAs()
{
	std::filesystem::path filename;
	bool hasFilename = saveSceneDialog(filename, "Save I3T scene");
	if (hasFilename)
	{
		fs::path path(filename);
		if (path.extension().string() != I3T_SCENE_EXTENSION)
		{
			filename += I3T_SCENE_EXTENSION;
		}

		auto ww = I3T::getWindowPtr<WorkspaceWindow>();

		App::getModule<StateManager>().saveScene(filename);
	}
}

static void save()
{
	auto& sm = App::getModule<StateManager>();

	if (sm.hasScene())
		sm.saveScene();
	else
		saveAs();
}

Result<Void, Error> MenuBarDialogs::open()
{
	std::filesystem::path sceneFile;
	bool hasFile = openSceneDialog(sceneFile, "Open I3T scene");
	if (hasFile)
	{
		App::getModule<StateManager>().loadScene(sceneFile);
		return Void{};
	}

	return Err("No file selected");
}

//===----------------------------------------------------------------------===//

MainMenuBar::MainMenuBar()
{
	InputManager::bindGlobalAction("save", EKeyState::Pressed, [&]() {
		save();
	});
	InputManager::bindGlobalAction("open", EKeyState::Pressed, [&]() {
		MenuBarDialogs::open();
	});
}

void MainMenuBar::render()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushFont(I3T::getFont(EFont::MenuLarge));

		showFileMenu();
		showEditMenu();
		showWindowsMenu();
		showTutorialMenu();
		showHelpMenu();

		ImGui::EndMenuBar();
		ImGui::PopFont();
	}
	ImGui::End();

	if (m_showDemoWindow)
	{
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}
}

void MainMenuBar::showFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			App::getModule<UIModule>().openModal<BeforeNewModal>();
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Open", "Ctrl+O"))
		{
			MenuBarDialogs::open();
		}

		if (ImGui::BeginMenu("Recent"))
		{
			showRecentFiles();

			ImGui::EndMenu();
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Save", "Ctrl+S", false, !App::getModule<StateManager>().getCurrentScene()->m_readOnly))
		{
			save();
		}

		if (ImGui::MenuItem("Save As"))
		{
			saveAs();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Manage Models"))
		{
			App::getModule<UIModule>().getWindowManager().showUniqueWindow<ImportedModelsDialog>();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Exit"))
		{
			BeforeCloseCommand::dispatch();
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::showEditMenu()
{
	// See #297
	if (ImGui::BeginMenu("Edit"))
	{
		/*
		if (ImGui::MenuItem("Undo", "Ctrl+Z", false, App::getModule<StateManager>().canUndo()))
		{
		    InputManager::triggerAction("undo", EKeyState::Pressed);
		}
		if (ImGui::MenuItem("Redo", "Ctrl+Y", false, App::getModule<StateManager>().canRedo()))
		{
		    InputManager::triggerAction("redo", EKeyState::Pressed);
		}
		 */
		if (ImGui::MenuItem("Preferences"))
		{
			I3T::getUI()->getWindowManager().showUniqueWindow<SetupDialog>();
		}
		ImGui::EndMenu();
	}
}

void MainMenuBar::showWindowsMenu()
{

	if (ImGui::BeginMenu("Windows"))
	{
		ImGui::MenuItem("Start window", nullptr, I3T::getWindowPtr<StartWindow>()->getShowPtr());
		ImGui::MenuItem("Tutorial window", nullptr, I3T::getWindowPtr<TutorialWindow>()->getShowPtr());
		ImGui::MenuItem("Scene view window", nullptr, I3T::getWindowPtr<UI::ViewportWindow>()->getShowPtr());
		ImGui::MenuItem("Workspace window", nullptr, I3T::getWindowPtr<WorkspaceWindow>()->getShowPtr());
		ImGui::MenuItem("Console window", nullptr, I3T::getWindowPtr<Console>()->getShowPtr());
		ImGui::MenuItem("Log window", nullptr, I3T::getWindowPtr<LogWindow>()->getShowPtr());

		ImGui::EndMenu();
	}
}

void MainMenuBar::showTutorialMenu()
{
	if (ImGui::BeginMenu("Tutorials"))
	{
		ImGui::MenuItem("Start window", nullptr, I3T::getWindowPtr<StartWindow>()->getShowPtr());

		ImGui::EndMenu();
	}
}

void MainMenuBar::showHelpMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Description"))
		{
			I3T::getUI()->getWindowManager().showUniqueWindow<DescriptionDialog>();
		}

		ImGui::MenuItem("About", nullptr, I3T::getWindowPtr<AboutWindow>()->getShowPtr());

		ImGui::Separator();

		if (ImGui::MenuItem("Show demo window", nullptr, &m_showDemoWindow))
		{}
		if (ImGui::MenuItem("Show style editor", nullptr, I3T::getWindowPtr<StyleEditor>()->getShowPtr()))
		{}

		ImGui::Separator();

		ImGui::MenuItem("Debug window manager", nullptr, &I3T::app().m_debugWindowManager);
		ImGui::MenuItem("Debug trackball camera", nullptr, &I3T::app().m_debugTrackball);

		ImGui::EndMenu();
	}
}
