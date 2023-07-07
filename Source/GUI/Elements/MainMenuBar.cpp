#include "MainMenuBar.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/AboutDialog.h"
#include "GUI/Elements/Dialogs/DescriptionDialog.h"
#include "GUI/Elements/Dialogs/SetupDialog.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/StyleEditor.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Windows/StartWindow.h"
// #include "RecentFiles.h"

using namespace UI;

static bool saveSceneDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"I3T scene files", "*.scene"};

	return SystemDialogs::SaveSingleFileDialog(result, title, "./", filter);
}

static bool openSceneDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"I3T scene files", "*.scene"};

	return SystemDialogs::OpenSingleFileDialog(result, title, "./", filter);
}

static bool importContentDialog(std::filesystem::path& result, const std::string& title)
{
	static std::vector<std::string> filter = {"All files", "*"};

	return SystemDialogs::OpenSingleFileDialog(result, title, "./", filter);
}

static void saveAs()
{
	std::filesystem::path filename;
	bool hasFilename = saveSceneDialog(filename, "Save I3T scene");
	if (hasFilename)
	{
		fs::path path(filename);
		if (path.extension().string() != ".scene")
		{
			filename += ".scene";
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

static void open()
{
	std::filesystem::path sceneFile;
	bool hasFile = openSceneDialog(sceneFile, "Open I3T scene");
	if (hasFile)
	{
		auto ww = I3T::getWindowPtr<WorkspaceWindow>();
		App::getModule<StateManager>().loadScene(sceneFile);
	}
}

static void importModel()
{
	std::filesystem::path modelFile;
	if (importContentDialog(modelFile, "Import model"))
	{
		Application::getModule<Core::ResourceManager>().importResource(modelFile);
		Application::getModule<StateManager>().takeSnapshot();
	}
}

//===----------------------------------------------------------------------===//

MainMenuBar::MainMenuBar()
{
	InputManager::bindGlobalAction("save", EKeyState::Pressed, [&]() {
		save();
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
		ImGui::ShowDemoWindow(&m_showDemoWindow);
}

void MainMenuBar::showFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			BeforeNewProjectCommand::dispatch();
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Recent"))
		{
			/// \todo Handle recent files.
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Open", "Ctrl+O"))
		{
			open();
		}

		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			save();
		}

		if (ImGui::MenuItem("Save As"))
		{
			saveAs();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Import Model"))
		{
			importModel();
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
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "Ctrl+Z", false, App::getModule<StateManager>().canUndo()))
		{
			InputManager::triggerAction("undo", EKeyState::Pressed);
		}
		if (ImGui::MenuItem("Redo", "Ctrl+Y", false, App::getModule<StateManager>().canRedo()))
		{
			InputManager::triggerAction("redo", EKeyState::Pressed);
		}
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
		ImGui::MenuItem("Tutorial window", nullptr, I3T::getWindowPtr<TutorialWindow>()->getShowPtr());
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

		if (ImGui::MenuItem("About"))
		{
			I3T::getUI()->getWindowManager().showUniqueWindow<AboutDialog>();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Show demo window", nullptr, &m_showDemoWindow))
		{}
		if (ImGui::MenuItem("Show style editor", nullptr, I3T::getWindowPtr<StyleEditor>()->getShowPtr()))
		{}

		ImGui::Separator();

		ImGui::MenuItem("Debug window manager", nullptr, &App::get().m_debugWindowManager);
		ImGui::MenuItem("Debug trackball camera", nullptr, &App::get().m_debugTrackball);

		ImGui::EndMenu();
	}
}
