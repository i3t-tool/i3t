#include "MainMenuBar.h"

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
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
#include "Scripting/Scripting.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Windows/StartWindow.h"
// #include "RecentFiles.h"

using namespace UI;

static bool saveSceneDialog(std::string& result, const std::string& title)
{
	std::string root = Config::getAbsolutePath("./");
	static std::vector<std::string> filter = {"I3T scene files", "*.scene"};

	return SystemDialogs::SaveSingleFileDialog(result, title, root, filter);
}

static bool openSceneDialog(std::string& result, const std::string& title)
{
	std::string root = Config::getAbsolutePath("./");
	static std::vector<std::string> filter = {"I3T scene files", "*.scene"};

	return SystemDialogs::OpenSingleFileDialog(result, title, root, filter);
}

static void saveAs()
{
	std::string filename;
	bool hasFilename = saveSceneDialog(filename, "Save I3T scene");
	if (hasFilename)
	{
		fs::path path(filename);
		if (path.extension().string() != ".scene")
		{
			filename += ".scene";
		}

		auto ww = I3T::getWindowPtr<WorkspaceWindow>();

		StateManager::instance().saveScene(filename);
		StateManager::instance().setScene(filename);
	}
}

static void save()
{
	auto& sm = StateManager::instance();

	if (sm.hasScene())
		sm.saveScene();
	else
		saveAs();
}

static void open()
{
	std::string sceneFile;
	bool hasFile = openSceneDialog(sceneFile, "Open I3T scene");
	if (hasFile)
	{
		auto ww = I3T::getWindowPtr<WorkspaceWindow>();
		StateManager::instance().loadScene(sceneFile);
	}
}

//===----------------------------------------------------------------------===//

MainMenuBar::MainMenuBar()
{
	InputManager::bindGlobalAction("save", EKeyState::Pressed, [&]() { save(); });
}

void MainMenuBar::render()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushFont(I3T::getFont(EFont::MenuLarge));

		showFileMenu();
		showEditMenu();
		showWindowsMenu();
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
			// TabSpace::onOpenScene(TabSpace::RESET);
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Recent"))
		{
			/// \todo Handle recent files.
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Open"))
		{
			open();
		}

		if (ImGui::MenuItem("Save"))
		{
			save();
		}

		if (ImGui::MenuItem("Save As"))
		{
			saveAs();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Import Content"))
		{
			/// \todo Import custom content? Use Utils/System.h.
			// Reader::openContentDialog();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Setup"))
		{
			I3T::getUI()->showUniqueWindow<SetupDialog>();
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
		if (ImGui::MenuItem("Undo", nullptr, false,
		                    StateManager::instance().canUndo()))
		{
			InputManager::triggerAction("undo", EKeyState::Pressed);
		}
		if (ImGui::MenuItem("Redo", nullptr, false,
		                    StateManager::instance().canRedo()))
		{
			InputManager::triggerAction("redo", EKeyState::Pressed);
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::showWindowsMenu()
{

	if (ImGui::BeginMenu("Windows"))
	{
		ImGui::MenuItem("Tutorial window", nullptr,
		                I3T::getWindowPtr<TutorialWindow>()->getShowPtr());
		ImGui::MenuItem("Scene view window", nullptr,
		                I3T::getWindowPtr<UI::Viewport>()->getShowPtr());
		ImGui::MenuItem("Workspace window", nullptr,
		                I3T::getWindowPtr<WorkspaceWindow>()->getShowPtr());
		ImGui::MenuItem("Console window", nullptr,
		                I3T::getUI()->getWindowPtr<Console>()->getShowPtr());
		ImGui::MenuItem("Log window", nullptr,
		                I3T::getUI()->getWindowPtr<LogWindow>()->getShowPtr());

		ImGui::EndMenu();
	}
}

void MainMenuBar::showHelpMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Description"))
		{
			I3T::getUI()->showUniqueWindow<DescriptionDialog>();
		}

		if (ImGui::MenuItem("About"))
		{
			I3T::getUI()->showUniqueWindow<AboutDialog>();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Show demo window", nullptr, &m_showDemoWindow))
		{
		}
		if (ImGui::MenuItem(
		        "Show style editor", nullptr,
		        I3T::getUI()->getWindowPtr<StyleEditor>()->getShowPtr()))
		{
		}

		ImGui::EndMenu();
	}
}
