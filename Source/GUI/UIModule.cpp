#include "UIModule.h"

#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Core/Result.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeCloseModal.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/StartWindow.h"
#include "GUI/Elements/Windows/StyleEditor.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/WindowManager.h"
#include "Loader/ThemeLoader.h"
#include "Utils/HSLColor.h"

using namespace UI;

UIModule::~UIModule()
{
	delete m_menu;
}

void UIModule::init()
{
	// Switch active InputController when window focus changes
	SetFocusedWindowCommand::addListener([](Ptr<IWindow> window) {
		InputManager::setActiveInput(&(window->getInput()));
	});

	Theme::initNames();

	// Setup Dear ImGui context after OpenGL context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;

	// Load Themes to be usable in window initializations
	loadFonts();
	loadThemes();
	m_currentTheme->apply();

	// Create GUI Elements.
	m_menu = new MainMenuBar();
	m_windowManager.addWindow(std::make_shared<TutorialWindow>(false));
	m_windowManager.addWindow(std::make_shared<StartWindow>(true));
	m_windowManager.addWindow(std::make_shared<ViewportWindow>(true, App::get().viewport()));
	m_windowManager.addWindow(std::make_shared<WorkspaceWindow>(true));
	m_windowManager.addWindow(std::make_shared<Console>(false));
	m_windowManager.addWindow(std::make_shared<LogWindow>());
	m_windowManager.addWindow(std::make_shared<StyleEditor>());

	HideWindowCommand::addListener([this](const std::string& id) {
		m_windowManager.removeWindow(id);
	});

	BeforeCloseCommand::addListener([this]() {
		m_windowManager.openModal<BeforeCloseModal>();
	});

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
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
	io.IniFilename = "Data/imgui.ini";

	// Apply theme to windows
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

	m_windowManager.updateFocus();

	buildDockspace();

	// Render UI elements
	m_menu->render();

	// Draw windows
	m_windowManager.draw();

	// TODO: (DR) Unused, resolve, old camera handling
	queryCameraState();

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
	(void) io;
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

	m_windowManager.clear();
}

void UIModule::loadThemes()
{
	constexpr auto MAX_THEMES_COUNT = 10;
	const std::string themesDir = "Data/Themes";

	m_allThemes.push_back(Theme::createDefaultClassic());
	m_allThemes.push_back(Theme::createDefaultModern());

	// Load all themes at Data/Themes directory.
	bool canLoadDefault = false;
	const auto entries = fs::directory_iterator(themesDir);

	//
	m_allThemes.reserve(MAX_THEMES_COUNT);

	for (auto& entry : entries)
	{
		if (m_allThemes.size() >= MAX_THEMES_COUNT)
		{
			LOG_WARN("Maximum number ({}) of loaded themes reached.", MAX_THEMES_COUNT)
			break;
		}

		if (entry.path().filename().string()[0] == '.' || entry.is_directory())
		{
			continue;
		}

		if (!entry.path().filename().string().ends_with(".yml"))
		{
			continue;
		}

		auto result = loadTheme(entry);

		if (!result)
		{
			LOG_WARN("Failed to load theme {}: {}", entry.path().string(), result.error());
			continue;
		}

		const auto& theme = result.value();

		// Check if theme name doesn't collide with default Themes names.
		bool canLoadTheme = true;
		for (const auto& otherTheme : m_allThemes)
		{
			if (otherTheme.getName() == theme.getName())
			{
				canLoadTheme = false;
			}
		}

		if (canLoadTheme)
		{
			m_allThemes.push_back(theme);
			if (theme.getName() == getUserData().themeName)
			{
				canLoadDefault = true;
				m_currentTheme = &m_allThemes.back();
				setTheme(m_allThemes.back());

				LOG_INFO("Set default theme: {}", theme.getName());
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
	m_currentTheme = (Theme*) &theme;
	m_currentTheme->apply();

	getUserData().themeName = theme.getName();
	Application::getModule<StateManager>().saveUserData();
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
	fontCfg.GlyphExtraSpacing.x = -0.5f; // Font v navrhu ma mensi mezery mezi pismeny - bez toho nevychazi
	                                     // na spravnou sirku

	m_fonts = {
	    // 0
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Regular.ttf", 14.0f * fontScale, nullptr, ranges),
	    // 1
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Bold.ttf", 12.0f * fontScale, nullptr, ranges),
	    // 2
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Regular.ttf", 12.0f * fontScale, nullptr, ranges),
	    // 3
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Ubuntu-Bold.ttf", 24.0f * fontScale, nullptr, ranges),
	    // 4
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Bold.ttf", 16.0f * fontScale, nullptr, ranges),
	    // 5
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Bold.ttf", 20.0f * fontScale, nullptr, ranges),
	    // 6
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Ubuntu-Bold.ttf", 18.0f * fontScale, nullptr, ranges),
	    // 7
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Ubuntu-Bold.ttf", 33.5f * fontScale, &fontCfg, ranges),
	    // 8
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Regular.ttf", 17.5f * fontScale, nullptr, ranges),
	    // 9
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Regular.ttf", 16.0f * fontScale, nullptr, ranges),
	    // 10
	    io.Fonts->AddFontFromFileTTF("Data/fonts/Roboto-Italic.ttf", 16.0f * fontScale, nullptr, ranges),
	};
	// io.FontDefault = I3T::getFont(EFont::MenuLarge);
	io.Fonts->Build();
}

void UIModule::buildDockspace()
{
	// create dockspace -----------------------------
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
	// not dockable into, because it would be confusing to have two docking
	// targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		                ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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
		// Active tab color is set to a special color for docked windows. It does not affect regular tabs.
		// This style should be pushed for the dockspace here as well as every docked window Begin()
		ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::PopStyleColor();
	}
	else
	{
		throw std::runtime_error("ImGui Docking is not enabled!");
	}
}

void UIModule::queryCameraState()
{
	/// \todo This code causes dockspace crash.
	// TODO: (DR) Old code used for camera mouse drag handling, unused, probably delete in the future
	return;

	//	if (!InputManager::isInputActive(getWindowManager().getWindowPtr<UI::ViewportWindow>()->getInputPtr()))
	//		return;
	//
	//	// ORBIT camera rotation
	//	if (InputManager::isActionTriggered("rotate", EKeyState::Pressed))
	//	{
	//		InputManager::beginCameraControl();
	//	}
	//	if (InputManager::isActionTriggered("rotate", EKeyState::Released))
	//	{
	//		InputManager::endCameraControl();
	//	}
	//
	//	// CAMERA PANNING - set a new orbit center
	//	if (InputManager::isActionTriggered("pan", EKeyState::Pressed))
	//	{
	//		InputManager::beginCameraControl();
	//	}
	//	if (InputManager::isActionTriggered("pan", EKeyState::Released))
	//	{
	//		InputManager::endCameraControl();
	//	}
}
