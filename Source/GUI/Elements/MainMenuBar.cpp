#include "MainMenuBar.h"

#include "imgui.h"

#include "Scripting/Scripting.h"
#include "Core/API.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "Commands/ApplicationCommands.h"
#include "Core/API.h"
#include "Core/World.h"
#include "GUI/Elements/Dialogs/AboutDialog.h"
#include "GUI/Elements/Dialogs/DescriptionDialog.h"
#include "GUI/Elements/Dialogs/SetupDialog.h"
#include "GUI/Elements/Windows/ConsoleWindow.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
// #include "RecentFiles.h"

MainMenuBar::MainMenuBar()
{
	m_showDemoWindow = false;
}

void MainMenuBar::render()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushFont(I3T::getFont(EFont::MenuLarge));

		showFileMenu();
		showEditMenu();
		showWindowsMenu();
		showViewportsMenu();
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
			/*
			for (const std::string& recentFile : RecentFiles::getRecentFiles())
			{
			  std::filesystem::path path = std::filesystem::path(recentFile);
			  if (ImGui::MenuItem(path.filename().string().c_str()))
			  {
			    TabSpace::onOpenScene(path.string(), TabSpace::OPEN_PATH);
			  }
			}
			 */

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Open"))
		{
			std::string result;
			std::string title="Open I3T script...";
			std::string root=Config::getAbsolutePath("./");
			std::vector<std::string> filter; filter.push_back("C source files"); filter.push_back("*.c");
			bool b=SystemDialogs::OpenSingleFileDialog(result,title,root,filter);

			WorkspaceWindow*ww= (WorkspaceWindow*)I3T::getWindowPtr<WorkspaceWindow>();
			if(ww!=NULL){
				ww->WorkspaceNodes.clear();
				LoadWorkspace(result.c_str(), &ww->WorkspaceNodes);
			}
			else {fprintf(stderr, "Open failed:WorkspaceWindow not found\n");}
		}

		if (ImGui::MenuItem("Append"))
		{
			std::string result;
			std::string title = "Open I3T script...";
			std::string root = Config::getAbsolutePath("./");
			std::vector<std::string> filter; filter.push_back("C source files"); filter.push_back("*.c");
			bool b = SystemDialogs::OpenSingleFileDialog(result, title, root, filter);

			WorkspaceWindow* ww = (WorkspaceWindow*)I3T::getWindowPtr<WorkspaceWindow>();
			if (ww != NULL) {LoadWorkspace(result.c_str(), &ww->WorkspaceNodes);}
			else {fprintf(stderr, "Append failed:WorkspaceWindow not found\n");}
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Save"))
		{
			/// \todo SaveFileDialog, use Utils/System.h.
			// Reader::saveScene(FileMode::PROJECT);
		}

		if (ImGui::MenuItem("Save As"))
		{
			std::string result;
			std::string title="Save I3T script...";
			std::string root=Config::getAbsolutePath("./");
			std::vector<std::string> filter; filter.push_back("C source files"); filter.push_back("*.c");
			bool b= SystemDialogs::SaveSingleFileDialog(result,title,root,filter);

			WorkspaceWindow* ww = (WorkspaceWindow*)I3T::getWindowPtr<WorkspaceWindow>();
			if (ww != NULL) {SaveWorkspace(result.c_str(), &ww->WorkspaceNodes);}
			else {fprintf(stderr, "Save failed:WorkspaceWindow not found\n");}
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
			// TabSpace::showSetupForm();
			// App::get().enqueueWindow(SetupDialog::id, &m_setupDialog);
			// App::get().enqueueWindow(SetupDialog::id, std::make_unique<SetupDialog>());
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
		if (ImGui::MenuItem("Undo"))
		{
			// B
			/// \todo Undo.
			// UndoRedo::undo();
		}

		if (ImGui::MenuItem("Redo"))
		{
			// N
			/// \todo Redo.
			// UndoRedo::redo();
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::showWindowsMenu()
{
	if (ImGui::BeginMenu("Windows"))
	{
		ImGui::MenuItem("Workspace window", nullptr, I3T::getWindowPtr<WorkspaceWindow>()->getShowPtr());
		ImGui::MenuItem("Tutorial window", nullptr, I3T::getWindowPtr<TutorialWindow>()->getShowPtr());
		ImGui::MenuItem("Console window", nullptr, I3T::getUI()->getWindowPtr<ConsoleWindow>()->getShowPtr());
		ImGui::MenuItem("Log window", nullptr, I3T::getUI()->getWindowPtr<LogWindow>()->getShowPtr());
		ImGui::MenuItem("Scene view window", nullptr, I3T::getWindowPtr<ViewportWindow>()->getShowPtr());

		ImGui::EndMenu();
	}
}

void MainMenuBar::showViewportsMenu()
{
	if (ImGui::BeginMenu("Viewports"))
	{
		if (ImGui::MenuItem("View-x"))
		{
			// Num 1
			App::get().world()->scene->setCamToOrbitCenterX();
		}

		if (ImGui::MenuItem("View-y"))
		{
			// Num 2
			App::get().world()->scene->setCamToOrbitCenterY();
		}

		if (ImGui::MenuItem("View-z"))
		{
			// Num 3
			App::get().world()->scene->setCamToOrbitCenterZ();
		}

		if (ImGui::MenuItem("World-x"))
		{
			// Num 4
			App::get().world()->scene->setCamToCenterX();
		}

		if (ImGui::MenuItem("World-y"))
		{
			// Num 5
			App::get().world()->scene->setCamToCenterY();
		}

		if (ImGui::MenuItem("World-z"))
		{
			// Num 6
			App::get().world()->scene->setCamToCenterZ();
		}

		if (ImGui::MenuItem("Center"))
		{
			// Num 0
			App::get().world()->scene->setCamToCenter();
		}

		ImGui::EndMenu();
	}
}

void MainMenuBar::showHelpMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Description"))
		{
			// App::get().enqueueWindow(DescriptionDialog::id, &m_descriptionDialog);
			// App::get().enqueueWindow(DescriptionDialog::id, std::make_unique<DescriptionDialog>());
			I3T::getUI()->showUniqueWindow<DescriptionDialog>();
		}

		if (ImGui::MenuItem("About"))
		{
			// TabSpace::showAboutForm();
			// App::get().enqueueWindow(AboutDialog::id, &m_aboutDialog);
			// App::get().enqueueWindow(AboutDialog::id, std::make_unique<SetupDialog>());
			I3T::getUI()->showUniqueWindow<AboutDialog>();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Show demo window", nullptr, &m_showDemoWindow)) {}

		ImGui::EndMenu();
	}
}
