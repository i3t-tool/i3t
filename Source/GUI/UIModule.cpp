#include "UIModule.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "GUI/Elements/Windows/StyleEditor.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"
#include "Loader/ConfigLoader.h"
#include "Loader/ThemeLoader.h"
#include "Logger/Logger.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Utils/Filesystem.h"

using namespace UI;

UIModule::~UIModule() { delete m_menu; }

void UIModule::init()
{
	SetFocusedWindowCommand::addListener(
	    [](Ptr<IWindow> window)
	    { InputManager::setActiveInput(&(window->getInput())); });

	Theme::initNames();

	// Create GUI Elements.
	m_menu = new MainMenuBar();
	m_dockableWindows.push_back(std::make_shared<TutorialWindow>(false));
	m_dockableWindows.push_back(std::make_shared<StartWindow>(true));
	m_dockableWindows.push_back(
	    std::make_shared<Viewport>(true, App::get().world()));
	m_dockableWindows.push_back(std::make_shared<WorkspaceWindow>(true));
	m_dockableWindows.push_back(std::make_shared<Console>(false));
	m_dockableWindows.push_back(std::make_shared<LogWindow>());

	m_dockableWindows.push_back(std::make_shared<StyleEditor>());

	HideWindowCommand::addListener([this](const std::string& id)
	                               { popWindow(id); });

	// Setup Dear ImGui context after OpenGL context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |=
	    ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
	                                                    // Platform Windows
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform
	// windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f; // disable alpha
	}

	// Allocate path to the imgui ini file on heap.
	auto* path = new std::string(Config::getAbsolutePath("Data/imgui.ini"));
	io.IniFilename = path->c_str();

	loadFonts();
	loadThemes();
	m_currentTheme->apply();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(App::get().mainWindow(), true);
	ImGui_ImplOpenGL3_Init(ImGui_GLSLVersion);
}

void UIModule::beginFrame()
{
	// Start the Dear ImGui frame ----------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	buildDockspace();

	// Render UI elements
	m_menu->render();

	// Scene is rendered in the Viewport window.
	// TODO -> Do not render scene in the ViewportWindow class.
	for (auto element : m_dockableWindows)
	{
		if (element->isVisible())
		{
			element->render();
		}
		// if (InputController::isKeyJustPressed(Keys::f)) { printf("UP
		// %s\n",element->getID()); }
	}
	// if (InputController::isKeyJustPressed(Keys::f)) { printf("--- \n"); }
	// Render other windows.
	for (const auto& [id, w] : m_windows)
	{
		if (w->isVisible())
		{
			w->render();
		}
	}

	queryCameraState();

	setFocusedWindow();

	// ImGui rendering ----------------------------------------------------------
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we
	// save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call
	//  glfwMakeContextCurrent(window) directly)
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	auto& io = ImGui::GetIO();
	(void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UIModule::onClose()
{
	/// \todo MH - This may not be sufficient.
	auto workspace = I3T::getWindowPtr<WorkspaceWindow>();
	workspace->getNodeEditor().m_workspaceCoreNodes.clear();

	m_windows.clear();
	m_dockableWindows.clear();
}

void UIModule::loadThemes()
{
	std::string themesDir = Config::getAbsolutePath("Data/themes");

	m_allThemes.push_back(Theme::createDefaultClassic());
	m_allThemes.push_back(Theme::createDefaultModern());
	auto defaultThemesCount = m_allThemes.size();

	bool canLoadDefault = false;
	for (auto& entry : fs::directory_iterator(themesDir))
	{
		if (auto theme = loadTheme(entry))
		{
			// Check if theme name doesn't collides with default themes names.
			bool canLoadTheme = true;
			for (auto i = 0L; i < defaultThemesCount; ++i)
			{
				if (m_allThemes[i].getName() == (*theme).getName())
				{
					canLoadTheme = false;
				}
			}

			if (canLoadTheme)
			{
				m_allThemes.push_back(*theme);
				if (m_allThemes.back().getName() == Config::DEFAULT_THEME)
				{
					canLoadDefault = true;
					m_currentTheme = &m_allThemes.back();
					setTheme(m_allThemes.back());
				}
			}
		}
	}

	if (!canLoadDefault)
	{
		setTheme(m_allThemes.front());
	}
}

void UIModule::reloadThemes()
{
	m_allThemes.clear();
	loadThemes();
}

void UIModule::setTheme(const Theme& theme)
{
	m_currentTheme = (Theme*)&theme;
	m_currentTheme->apply();

	Config::DEFAULT_THEME = theme.getName();
	saveConfig();
}

void UIModule::loadFonts()
{
	auto& io = ImGui::GetIO();

	float fontScale = 1.2f;

	const ImWchar ranges[] = {
	    0x0020, 0x00FF, // Basic Latin + Latin Supplement
	    0x0080, 0x07FF, // Czech
	    0x25FC, 0x2BC8, // media buttons
	    0,
	};

	ImFontConfig fontCfg;
	fontCfg.GlyphExtraSpacing.x =
	    -0.5f; // Font v navrhu ma mensi mezery mezi pismeny - bez toho nevychazi
	           // na spravnou sirku

	m_fonts = {
	    // 0
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	        14.0f * fontScale, nullptr, ranges),
	    // 1
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
	        12.0f * fontScale, nullptr, ranges),
	    // 2
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	        12.0f * fontScale, nullptr, ranges),
	    // 3
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Ubuntu-Bold.ttf").c_str(),
	        24.0f * fontScale, nullptr, ranges),
	    // 4
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
	        16.0f * fontScale, nullptr, ranges),
	    // 5
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Bold.ttf").c_str(),
	        20.0f * fontScale, nullptr, ranges),
	    // 6
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Ubuntu-Bold.ttf").c_str(),
	        18.0f * fontScale, nullptr, ranges),
	    // 7
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Ubuntu-Bold.ttf").c_str(),
	        33.5f * fontScale, &fontCfg, ranges),
	    // 8
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	        17.5f * fontScale, nullptr, ranges),
	    // 9
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Regular.ttf").c_str(),
	        16.0f * fontScale, nullptr, ranges),
	    // 10
	    io.Fonts->AddFontFromFileTTF(
	        Config::getAbsolutePath("Data/fonts/Roboto-Italic.ttf").c_str(),
	        16.0f * fontScale, nullptr, ranges),
	};
	// io.FontDefault = I3T::getFont(EFont::MenuLarge);
	io.Fonts->Build();
}

void UIModule::buildDockspace()
{
	// create dockspace -----------------------------
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags =
	    ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
	// not dockable into, because it would be confusing to have two docking
	// targets within each others.
	ImGuiWindowFlags window_flags =
	    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |=
		    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
	// our background and handle the pass-thru hole, so we ask Begin() to not
	// render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window
	// is collapsed). This is because we want to keep our DockSpace() active. If a
	// DockSpace() is inactive, all active windows docked into it will lose their
	// parent and become undocked. We cannot preserve the docking relationship
	// between an active window and an inactive docking, otherwise any change of
	// dockspace/settings would lead to windows being stuck in limbo and never
	// being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		// exit(EXIT_FAILURE);
	}
}

void UIModule::queryCameraState()
{
	/// \todo This code causes dockspace crash.
	return;

	if (!InputManager::isInputActive(getWindowPtr<UI::Viewport>()->getInputPtr()))
		return;

	// ORBIT camera rotation
	if (InputManager::isActionTriggered("rotate", EKeyState::Pressed))
	{
		InputManager::beginCameraControl();
	}
	if (InputManager::isActionTriggered("rotate", EKeyState::Released))
	{
		InputManager::endCameraControl();
	}

	// CAMERA PANNING - set a new orbit center
	if (InputManager::isActionTriggered("pan", EKeyState::Pressed))
	{
		InputManager::beginCameraControl();
	}
	if (InputManager::isActionTriggered("pan", EKeyState::Released))
	{
		InputManager::endCameraControl();
	}
}

/**
 * Not thread safe (using strtok).
 */
std::string makeIDNice(const char* ID)
{
	// Remove ## or ### from ImGui window name.
	// auto sanitizedWindowID = ID;
	// while (*sanitizedWindowID == '#')
	//  ++sanitizedWindowID;

	std::string IDCopy = std::string(ID);

	if (IDCopy.empty())
	{
		return IDCopy;
	}

	// Get fist part of "window/child-window" id.
	char* currID = std::strtok(const_cast<char*>(IDCopy.c_str()), "###");
	char* lastID;
	while (currID != nullptr)
	{
		lastID = currID;
		currID = std::strtok(nullptr, "###");
	}

	currID = std::strtok(const_cast<char*>(lastID), "/");

	return std::string(currID);
}

void UIModule::setFocusedWindow()
{
	// Get window ids.
	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const char* hoveredWindowID = g.HoveredWindow ? g.HoveredWindow->Name : "";
	const char* activeWindowID = g.ActiveIdWindow ? g.ActiveIdWindow->Name : "";
	const char* navWindowID = g.NavWindow ? g.NavWindow->Name : "";

	// Check for hovered window.
	if (strlen(hoveredWindowID) != 0)
	{
		auto activeID = makeIDNice(activeWindowID);
		auto hoveredID = makeIDNice(hoveredWindowID);
		auto navID = makeIDNice(navWindowID);

		auto window = findWindow(hoveredID.c_str(), m_dockableWindows);

		if (m_windows.count(hoveredID) != 0)
		{
			window = m_windows[hoveredID];
		};

		if (window != nullptr)
		{
			bool shouldSetFocus = true;

			// Check if window can be focused (no menu is active).
			if (String::contains(navID, "Menu_") ||
			    String::contains(navID, "Popup_") ||
			    String::contains(navID, "Combo_"))
			{
				shouldSetFocus = false;
			}
			if (!activeID.empty() && activeID != hoveredID)
			{
				shouldSetFocus = false;

				if (InputManager::isKeyJustPressed(Keys::mouseRight) ||
				    InputManager::isKeyJustPressed(Keys::mouseMiddle))
				{
					// Un-focus text input.
					ImGui::SetActiveID(0, g.HoveredWindow);
					shouldSetFocus = true;
				}
			}

			if (shouldSetFocus)
			{
				ImGui::SetWindowFocus(g.HoveredWindow->Name);

				// Set focused window input in next frame.
				SetFocusedWindowCommand::dispatch(window);
			}
		}
	}
}

void UIModule::popWindow(const std::string& windowId)
{
	if (hasWindow(windowId))
		m_windows.erase(windowId);
}

bool UIModule::hasWindow(const std::string& id) { return m_windows.count(id); }
