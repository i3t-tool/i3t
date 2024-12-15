/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "MainMenuBar.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Dialogs/DescriptionDialog.h"
#include "GUI/Elements/Dialogs/ImportedModelsDialog.h"
#include "GUI/Elements/Dialogs/SetupDialog.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/Elements/Modals/ConfirmModal.h"
#include "GUI/Elements/Windows/AboutWindow.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/StyleEditor.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/IconFonts/Icons.h"
#include "I3T.h"
#include "Logger/Logger.h"
#include "State/StateManager.h"
#include "Windows/StartWindow.h"

using namespace UI;

using namespace std::literals;

/// \todo Move to some common place, generalize with event?
static void beforeExitScene()
{
	if (const auto startWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<StartWindow>())
	{
		startWindow->hide();
	}

	if (const auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>())
	{
		if (tutorialWindow->hasTutorial())
		{
			tutorialWindow->emptyTutorial();
		}
		tutorialWindow->hide();
	}
}

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
		askBeforeExitScene([sceneToOpen]() {
			App::getModule<StateManager>().loadScene(*sceneToOpen);

			beforeExitScene();
		});
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

		App::getModule<StateManager>().saveScene(filename);
	}
}

static void save()
{
	auto& stateManager = App::getModule<StateManager>();

	if (stateManager.hasScene())
	{
		if (stateManager.getCurrentScene()->m_readOnly)
		{
			saveAs();
		}
		else
		{
			stateManager.saveScene();
		}
	}
	else
	{
		saveAs();
	}
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

	InputManager::bindGlobalAction("new", EKeyState::Pressed, [&]() {
		askBeforeExitScene([]() {
			NewProjectCommand::dispatch();

			beforeExitScene();
		});
	});

	InputManager::bindGlobalAction("open", EKeyState::Pressed, [&]() {
		askBeforeExitScene([]() {
			if (MenuBarDialogs::open())
			{
				beforeExitScene();
			}
		});
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
		if (ImGui::MenuItem(ICON_I3T_NEW_FILE " New"))
		{
			InputManager::triggerAction("new", EKeyState::Pressed);
		}
		ImGui::Separator();

		if (ImGui::MenuItem(ICON_I3T_OPEN_FILE " Open", "Ctrl+O"))
		{
			InputManager::triggerAction("open", EKeyState::Pressed);
		}

		if (ImGui::BeginMenu(ICON_I3T_CLOCK " Recent"))
		{
			showRecentFiles();

			ImGui::EndMenu();
		}
		ImGui::Separator();

		if (ImGui::MenuItem(ICON_I3T_SAVE " Save", "Ctrl+S", false,
		                    !App::getModule<StateManager>().getCurrentScene()->m_readOnly))
		{
			save();
		}

		if (ImGui::MenuItem(ICON_I3T_SAVE " Save As"))
		{
			saveAs();
		}

		ImGui::Separator();

		if (ImGui::MenuItem(ICON_I3T_MODELS " Manage Models"))
		{
			App::getModule<UIModule>().getWindowManager().showUniqueWindow<ImportedModelsDialog>();
		}

		ImGui::Separator();

		if (ImGui::MenuItem(ICON_I3T_EXIT " Exit"))
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
		if (ImGui::MenuItem("Undo", "Ctrl+Z", false, App::getModule<StateManager>().canUndo()))
		{
			InputManager::triggerAction("undo", EKeyState::Pressed);
		}
		if (ImGui::MenuItem("Redo", "Ctrl+Y", false, App::getModule<StateManager>().canRedo()))
		{
			InputManager::triggerAction("redo", EKeyState::Pressed);
		}
		if (ImGui::MenuItem(ICON_I3T_STYLE " Style editor", nullptr, I3T::getWindowPtr<StyleEditor>()->getShowPtr()))
		{}
		if (ImGui::MenuItem(ICON_I3T_SETTINGS " Preferences"))
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
		ImGui::MenuItem(ICON_I3T_HOME " Start window", nullptr, I3T::getWindowPtr<StartWindow>()->getShowPtr());
		ImGui::MenuItem(ICON_I3T_TUTORIAL " Tutorial window", nullptr,
		                I3T::getWindowPtr<TutorialWindow>()->getShowPtr());
		ImGui::MenuItem(ICON_I3T_SCENE " Scene view window", nullptr,
		                I3T::getWindowPtr<UI::ViewportWindow>()->getShowPtr());
		ImGui::MenuItem(ICON_I3T_WORKSPACE " Workspace window", nullptr,
		                I3T::getWindowPtr<WorkspaceWindow>()->getShowPtr());
#ifdef I3T_DEBUG
		ImGui::MenuItem(ICON_I3T_CONSOLE " Console window", nullptr, I3T::getWindowPtr<Console>()->getShowPtr());
		ImGui::MenuItem(ICON_I3T_LOG " Log window", nullptr, I3T::getWindowPtr<LogWindow>()->getShowPtr());
#endif

		ImGui::EndMenu();
	}
}

void MainMenuBar::showTutorialMenu()
{
	if (ImGui::BeginMenu(ICON_I3T_TUTORIAL " Tutorials"))
	{
		ImGui::MenuItem(ICON_I3T_HOME " Start window", nullptr, I3T::getWindowPtr<StartWindow>()->getShowPtr());

		if (ImGui::MenuItem(ICON_I3T_REFRESH " Reload", nullptr))
		{
			auto tutorialWindow = I3T::getWindowPtr<TutorialWindow>();
			if (tutorialWindow->hasTutorial())
			{
				tutorialWindow->reloadTutorial();
			}
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::showHelpMenu()
{
	if (ImGui::BeginMenu(ICON_I3T_HELP " Help"))
	{
#ifdef I3T_DEBUG
		if (ImGui::MenuItem("Description"))
		{
			I3T::getUI()->getWindowManager().showUniqueWindow<DescriptionDialog>();
		}
#endif
		ImGui::MenuItem("About", nullptr, I3T::getWindowPtr<AboutWindow>()->getShowPtr());

		ImGui::Separator();

#ifdef I3T_DEBUG
		if (ImGui::MenuItem("Show ImGui demo window", nullptr, &m_showDemoWindow))
		{}
#endif

#ifdef I3T_DEBUG
		ImGui::Separator();
		if (ImGui::BeginMenu(ICON_I3T_DEBUG " Debug"))
		{
			if (ImGui::BeginMenu("Log level"))
			{
				Logger& logger = Logger::getInstance();
				spdlog::level::level_enum logLevel = logger.getAppLogger()->level();
				spdlog::level::level_enum* newLogLevel = nullptr;

				if (ImGui::MenuItem("Trace", nullptr, logLevel == spdlog::level::trace))
					logger.getAppLogger()->set_level(spdlog::level::trace);
				if (ImGui::MenuItem("Debug", nullptr, logLevel == spdlog::level::debug))
					logger.getAppLogger()->set_level(spdlog::level::debug);
				if (ImGui::MenuItem("Info", nullptr, logLevel == spdlog::level::info))
					logger.getAppLogger()->set_level(spdlog::level::info);
				if (ImGui::MenuItem("Warn", nullptr, logLevel == spdlog::level::warn))
					logger.getAppLogger()->set_level(spdlog::level::warn);
				if (ImGui::MenuItem("Error", nullptr, logLevel == spdlog::level::err))
					logger.getAppLogger()->set_level(spdlog::level::err);
				if (ImGui::MenuItem("Critical", nullptr, logLevel == spdlog::level::critical))
					logger.getAppLogger()->set_level(spdlog::level::critical);
				if (ImGui::MenuItem("Off", nullptr, logLevel == spdlog::level::off))
					logger.getAppLogger()->set_level(spdlog::level::off);

				ImGui::EndMenu();
			}
#ifdef DIWNE_DEBUG
			ImGui::MenuItem("Debug node editor", nullptr, &(Workspace::g_diwne->m_diwneDebug));
#endif
			ImGui::MenuItem("Debug window manager", nullptr, &I3T::app().m_debugWindowManager);
			ImGui::MenuItem("Debug trackball camera", nullptr, &I3T::app().m_debugTrackball);
			ImGui::EndMenu();
		}

#endif

		ImGui::EndMenu();
	}
}
