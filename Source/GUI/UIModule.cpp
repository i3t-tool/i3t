/**
 * \file
 * \brief
 * \authors Martin Herich, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "UIModule.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Core/Result.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Windows/AboutWindow.h"
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

void UIModule::onInit()
{
	// Switch active InputController when window focus changes
	SetFocusedWindowCommand::addListener([](Ptr<IWindow> window) {
		InputManager::setActiveInput(&(window->getInput()));
	});

	Theme::initNames();

	ImGuiIO& io = ImGui::GetIO();

	// Load Themes to be usable in window initializations
	loadFonts();
	loadThemes();
	m_currentTheme->apply();

	// Create GUI Elements.
	m_menu = new MainMenuBar();
	m_windowManager.addWindow(std::make_shared<TutorialWindow>(false));
	m_windowManager.addWindow(std::make_shared<StartWindow>(true));
	m_windowManager.addWindow(std::make_shared<ViewportWindow>(true, I3T::getViewport()));
	m_windowManager.addWindow(std::make_shared<WorkspaceWindow>(true));
	m_windowManager.addWindow(std::make_shared<Console>(false));
	m_windowManager.addWindow(std::make_shared<LogWindow>());
	m_windowManager.addWindow(std::make_shared<StyleEditor>());
	m_windowManager.addWindow(std::make_shared<AboutWindow>());

	HideWindowCommand::addListener([this](const std::string& id) {
		m_windowManager.removeWindow(id);
	});

	BeforeCloseCommand::addListener([this]() {
		// m_windowManager.openModal<BeforeCloseModal>();
		// m_windowManager.openConfirmModal<BeforeCloseStrategy>();
		askBeforeExitScene([]() {
			CloseCommand::dispatch();
		});
	});

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

	// Apply theme to windows
	m_currentTheme->apply();
}

void UIModule::onBeginFrame()
{
	m_windowManager.updateFocus();

	buildDockspace();

	// Render UI elements
	m_menu->render();

	// Draw windows
	m_windowManager.draw();
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

	// Font v navrhu ma mensi mezery mezi pismeny - bez toho nevychazi na spravnou sirku
	ImFontConfig ubuntuBoldCfg;
	ubuntuBoldCfg.OversampleH = 2;
	ubuntuBoldCfg.OversampleV = 1;
	ubuntuBoldCfg.GlyphExtraSpacing.x = -0.5f;

	// NOTE: Oversampling is a technique that scales the loaded font by a specific factor in the X (horizontal) or Y
	// (vertical) direction (or both). In the process the font is also slightly blurred. Such modified font texture is
	// meant to be rendered at the original smaller size and the blurred edges of letters allow for subpixel
	// antialiasing. More info here: https://github.com/nothings/stb/blob/master/tests/oversample/README.md
	// This can be abused to make scaled up text look less pixelated when zoomed in. Although it also makes the zoomed
	// in text a little blurry.

	// Inspired by:
	// https://github.com/thedmd/imgui-node-editor/blob/af7fa51bb9d68c9b44477c341f13a7dadee3e359/examples/application/source/application.cpp#L97

	// In the future, dynamically replacing the font for a non-blurry bigger one would be the solution (or outright
	// always using a big font and scaling it down)
	// Another solution could be switching to freetype/vector-based fonts which ImGui might include soon. However, this
	// is a good enough solution for now. Note that using oversampling with high factors dramatically increases the size
	// of the font atlas texture, hogging video memory, especially if many fonts with large glyph ranges are used.

	// Default config, using lower horizontal oversampling (3 --> 2) than imgui to save some memory
	// (The difference is, as imgui docs mention, almost non-existent)
	ImFontConfig lqConfig;
	lqConfig.OversampleH = 2;
	lqConfig.OversampleV = 1;

	// More video memory intensive font config that uses high horizontal AND vertical oversampling.
	// This is ideal for fonts we zoom in on like in the workspace, albeit they become slightly blurry.
	ImFontConfig hqConfig;
	hqConfig.OversampleH = 4;
	hqConfig.OversampleV = 4;
	hqConfig.PixelSnapH = false;

	ImFontConfig mqConfig;
	mqConfig.OversampleH = 3;
	mqConfig.OversampleV = 2;
	mqConfig.PixelSnapH = false;

	// TODO: (DR) Do we really need all these fonts? Can't we use just a few (small/large variants) and them scale them?
	//   Currently we're filling the font atlas with huge fonts just to use them once.
	// As of right now the font atlas size is 2048x4096

	m_fonts = {
	    {"Roboto12",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 12.0f * fontScale, &lqConfig, ranges)}, //
	    {"Roboto14",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 14.0f * fontScale, &hqConfig, ranges)}, //
	    {"Roboto16",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 16.0f * fontScale, &lqConfig, ranges)}, //
	    {"Roboto17.5",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 17.5f * fontScale, &lqConfig, ranges)}, //
	    {"RobotoBold12",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Bold.ttf", 12.0f * fontScale, &lqConfig, ranges)}, //
	    {"RobotoBold16",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Bold.ttf", 16.0f * fontScale, &lqConfig, ranges)}, //
	    {"RobotoBold20",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Bold.ttf", 20.0f * fontScale, &mqConfig, ranges)}, //
	    {"RobotoItalic16",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Italic.ttf", 16.0f * fontScale, &lqConfig, ranges)}, //
	    {"UbuntuBold18",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Ubuntu-Bold.ttf", 18.0f * fontScale, &lqConfig, ranges)}, //
	    {"UbuntuBold24",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Ubuntu-Bold.ttf", 24.0f * fontScale, &lqConfig, ranges)}, //
	    {"UbuntuBold33.5",
	     io.Fonts->AddFontFromFileTTF("Data/Fonts/Ubuntu-Bold.ttf", 33.5f * fontScale, &ubuntuBoldCfg, ranges)}, //
	};
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
		ImGui::PushStyleColor(ImGuiCol_TabActive, I3T::getUI()->getTheme().get(EColor::DockTabActive));
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::PopStyleColor();
	}
	else
	{
		throw std::runtime_error("ImGui Docking is not enabled!");
	}
}
