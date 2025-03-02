/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
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
#include "GUI/Theme/ThemeLoader.h"
#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "State/StateManager.h"
#include "Tutorial/TutorialLoader.h"
#include "Tutorial/TutorialManager.h"
#include "UserData.h"
#include "Utils/HSLColor.h"

#include "GUI/IconFonts/Icons.h"

using namespace UI;

UIModule::~UIModule()
{
	delete m_menu;
}


static void* LayoutStateReadOpenFn(ImGuiContext*, ImGuiSettingsHandler*, const char* name)
{
	return (void*) App::getModule<UIModule>().getWindowManager().findDockableWindowByID(name).get();
}

static void LayoutStateReadLineFn(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line)
{
	IWindow* window = static_cast<IWindow*>(entry);
	int isOpen = 0;
	if (sscanf(line, "IsOpen=%d", &isOpen) == 1)
	{
		auto& g = *ImGui::GetCurrentContext();
		if (!g.SettingsLoaded)
		{
			if (*window->getShowPtr() == false) // If window is not visible, do not open it
			{
				*window->getShowPtr() = isOpen;
			}
		}
		else
		{
			*window->getShowPtr() = isOpen;
		}
	}
}

static void LayoutStateWriteAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
	LOG_INFO("Saving Layout information to file.");
	if (App::getModule<UIModule>().getWindowManager().getDockableWindows().empty())
	{
		LOG_INFO("But there are no windows to save(handler name \"{}\").", handler->TypeName);
	}
	for (Ptr<IWindow> window : App::getModule<UIModule>().getWindowManager().getDockableWindows())
	{
		buf->appendf("[%s][%s]\n", handler->TypeName, window->getID());
		buf->appendf("IsOpen=%d\n", window->isVisible());
		buf->append("\n");
	}
}

void UIModule::onInit()
{
	// Switch active InputController when window focus changes
	SetFocusedWindowCommand::addListener([](Ptr<IWindow> window) {
		InputManager::setActiveInput(window ? window->getInput() : WPtr<InputController>());
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
			CloseCommand::dispatch(0);
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

	LoadWindowLayoutFromFileCommand::addListener([this](std::string path) {
		m_windowManager.loadLayout(path);
	});

	LoadWindowLayoutFromStringCommand::addListener([](std::string iniData) {
		ImGui::LoadIniSettingsFromMemory(iniData.c_str(), iniData.size());
	});

	// New settings handler for Dear ImGui to save window layout
	ImGuiSettingsHandler iniLayoutHandler;
	iniLayoutHandler.TypeName = "Layout";
	iniLayoutHandler.TypeHash = ImHashStr("Layout");
	iniLayoutHandler.ReadOpenFn = LayoutStateReadOpenFn;
	iniLayoutHandler.ReadLineFn = LayoutStateReadLineFn;
	iniLayoutHandler.WriteAllFn = LayoutStateWriteAllFn;

	ImGui::AddSettingsHandler(&iniLayoutHandler);
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
	// Save window layout before clearing windows
	ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);
	// and disable Dear ImGui saving
	ImGui::GetIO().IniFilename = nullptr;

	/// \todo MH - This may not be sufficient.
	auto workspace = I3T::getWindowPtr<WorkspaceWindow>();
	workspace->getNodeEditor().clear(); // We need to clear nodes here rather than later with destructors
	m_windowManager.clear();
}

void UIModule::loadThemes()
{
	constexpr auto MAX_THEMES_COUNT = 10;
	const std::string themesDir = "Data/Themes";

	m_allThemes.push_back(Theme::createDefaultClassic());

	// Load all themes at Data/Themes directory.
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
			// ignore nested directories and hidden files
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

		bool themeNameCollides =
		    std::find_if(m_allThemes.begin(), m_allThemes.end(), [&theme](const Theme& otherTheme) {
			    return theme.getName() == otherTheme.getName();
		    }) != m_allThemes.end();

		if (!themeNameCollides)
		{
			m_allThemes.push_back(theme);
		}
	}

	if (!getUserData().customThemeName.empty())
	{
		const auto maybeTheme = getThemeByName(getUserData().customThemeName);
		if (!maybeTheme)
		{
			setDefaultTheme();
		}
		else
		{
			const auto& theme = *maybeTheme.value();
			LOG_INFO("Set default theme: {}", theme.getName());
			setTheme(*maybeTheme.value());
		}
	}
	else
	{
		setDefaultTheme();
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

	getUserData().customThemeName = theme.getName();
	Application::getModule<StateManager>().saveUserData();
}

void UIModule::setDefaultTheme()
{
	const auto maybeLightTheme = getThemeByName(I3T_DEFAULT_THEME_LIGHT_NAME);
	const auto maybeDarkTheme = getThemeByName(I3T_DEFAULT_THEME_DARK_NAME);

	if (!maybeLightTheme.has_value() || !maybeDarkTheme.has_value())
	{
		LOG_ERROR("Failed to set default theme: {} or {} not found", I3T_DEFAULT_THEME_LIGHT_NAME,
		          I3T_DEFAULT_THEME_DARK_NAME);
		// Fallback to default "Folta" theme
		setTheme(m_allThemes.front());
	}
	else
	{
		Detail::isLightThemeSet() ? setTheme(*maybeLightTheme.value()) : setTheme(*maybeDarkTheme.value());
	}

	getUserData().customThemeName = "";
	App::getModule<StateManager>().saveUserData();
}

std::optional<Theme*> UIModule::getThemeByName(const std::string& name) const
{
	auto it = std::find_if(m_allThemes.begin(), m_allThemes.end(), [&name](const Theme& theme) {
		return theme.getName() == name;
	});

	if (it != m_allThemes.end())
	{
		return (Theme*) &(*it);
	}

	return std::nullopt;
}

void UIModule::loadFonts()
{
	auto& io = ImGui::GetIO();

	float fontScale = 1.2f;

	// https://www.unicode.org/roadmaps/bmp/
	// to show the atlas, use
	//  ImGuiIO& io = ImGui::GetIO();
	//  ImFontAtlas* atlas = io.Fonts;
	//  ImGui::ShowFontAtlas(atlas);
	// or
	// ImGui Demo / Configuration / Style / Fonts

	// (PF) minimalistic font set - 1024x2048 font atlas texture
	const ImWchar ranges[] = {
	    0x0020,
	    0x007F, // Basic Latin
	    0x0080,
	    0x017F, // Czech mini (Latin-1 Supplement + Latin extended A)
	    0x0400,
	    0x04FF, // Cyrillic
	    // 0x0080, 0x07FF, // Czech + lot of right to left characters -- too many never used characters
	    // 0x25FC, 0x2BC8, // Geometric shapes media buttons stop and play - are not in our fonts
	    // - this is a too long range instead of two characters
	    0,
	};
	const ImWchar symbolRanges[] = {
	    0x2300,
	    0x23E9,
	    0,
	};
	// https://symbl.cc/en/unicode/table/
	// These icons should be in unicode, but are not in our roboto fonts...
	// “User interface symbols” subblock of the “Miscellaneous Technical”
	//  ◼ 0x25FC
	//  ⯈ 0x2BC8 Black Medium Right-Pointing Triangle Centred
	// ▶ 25B6 Play Button, play.
	// ⏩ 23E9 Fast-Forward Button, fast forward.
	// ⏪ 23EA Fast Reverse Button, fast rewind, rewind.
	// ⏭ 23ED Next Track Button.
	// ⏮ 23EE Last Track Button.
	// ⏯ 23EF Play Or Pause Button.
	// ⏸ 23F8 Pause Button, pause.
	// ⏹ 23F9 Stop Button, stop.


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
	// (PF) reduced font ranges above -> font atlas 1024x2048

	m_fonts = {
	    {"Roboto12", loadFont("Data/Fonts/Roboto-Regular.ttf", 12.0f, fontScale, &lqConfig, ranges, false)},   //
	    {"Roboto14", loadFont("Data/Fonts/Roboto-Regular.ttf", 14.0f, fontScale, &hqConfig, ranges, true)},    //
	    {"Roboto16", loadFont("Data/Fonts/Roboto-Regular.ttf", 16.0f, fontScale, &lqConfig, ranges, false)},   //
	    {"Roboto17.5", loadFont("Data/Fonts/Roboto-Regular.ttf", 17.5f, fontScale, &lqConfig, ranges, false)}, //
	    {"RobotoBold12", loadFont("Data/Fonts/Roboto-Bold.ttf", 12.0f, fontScale, &lqConfig, ranges, false)},  //
	    {"RobotoBold16", loadFont("Data/Fonts/Roboto-Bold.ttf", 16.0f, fontScale, &lqConfig, ranges, false)},  //
	    {"RobotoBold20", loadFont("Data/Fonts/Roboto-Bold.ttf", 20.0f, fontScale, &mqConfig, ranges, false)},  //
	    {"RobotoMono14", loadFont("Data/Fonts/RobotoMono-Regular.ttf", 14.0f, fontScale, &lqConfig, ranges, false)},
	    {"RobotoItalic16", loadFont("Data/Fonts/Roboto-Italic.ttf", 16.0f, fontScale, &lqConfig, ranges, false)},    //
	    {"UbuntuBold18", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 18.0f, fontScale, &lqConfig, ranges, false)},        //
	    {"UbuntuBold24", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 24.0f, fontScale, &lqConfig, ranges, false)},        //
	    {"UbuntuBold33.5", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 33.5f, fontScale, &ubuntuBoldCfg, ranges, false)}, //
	};

	io.Fonts->Build();
}

void loadFontAwesomeIcons(float size_pixels, float fontScale)
{
	ImGuiIO& io = ImGui::GetIO();

	// Merge in icon fonts
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	float baseFontSize = size_pixels * fontScale;
	float iconFontSize = baseFontSize;

	icons_config.GlyphMinAdvanceX = iconFontSize;
	//	icons_config.GlyphMaxAdvanceX = iconFontSize;
	icons_config.GlyphOffset = ImVec2(0, 0);

	static const ImWchar icons_ranges_fa[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
	io.Fonts->AddFontFromFileTTF("Data/Fonts/fa-6-solid-900-i3t.ttf", iconFontSize, &icons_config, icons_ranges_fa);
}

ImFont* UIModule::loadFont(const char* filename, float size_pixels, float fontScale,
                           const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges, bool mergeIcons)
{
	ImFont* font =
	    ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels * fontScale, font_cfg_template, glyph_ranges);

	if (!mergeIcons)
		return font;

	loadFontAwesomeIcons(size_pixels, fontScale);

	return font;
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
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
		ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		// Active tab color is set to a special color for docked windows. It does not affect regular tabs.
		// This style should be pushed for the dockspace here as well as every docked window Begin()
		GUI::dockTabStylePush();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		GUI::dockTabStylePop();
	}
	else
	{
		throw std::runtime_error("ImGui Docking is not enabled!");
	}
}


Memento UIModule::saveScene(Scene* scene)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document::AllocatorType& a = doc.GetAllocator();

	rapidjson::Value layout(rapidjson::kObjectType);
	size_t iniDataSize = 0;
	const char* iniData = ImGui::SaveIniSettingsToMemory(&iniDataSize);

	rapidjson::Value iniDataValue(rapidjson::kStringType);
	iniDataValue.SetString(iniData, iniDataSize, a);
	layout.AddMember("iniData", iniDataValue, a);
	doc.AddMember("layout", layout, a);

	rapidjson::Value tutorialData(rapidjson::kObjectType);
	tutorialData.AddMember("LanguageId", (int) TutorialManager::instance().getLanguage(), a);
	if (TutorialManager::instance().getTutorial())
	{
		rapidjson::Value currentTutorial(rapidjson::kStringType);
		std::string currentTutorialName = TutorialManager::instance().getTutorial()->m_header->m_filename.string();
		currentTutorial.SetString(currentTutorialName.c_str(), currentTutorialName.size(), a);
		tutorialData.AddMember("CurrentTutorial", currentTutorial, a);
	}
	tutorialData.AddMember("CurrentStep", TutorialManager::instance().getStep(), a);
	doc.AddMember("tutorialData", tutorialData, a);

	return doc;
}

void UIModule::loadScene(const Memento& memento, Scene* scene)
{
	if (memento.HasMember("layout"))
	{
		const auto& layout = memento["layout"];
		if (layout.HasMember("iniData"))
		{
			std::string iniData = layout["iniData"].GetString();
			LoadWindowLayoutFromStringCommand::dispatch(iniData);
		}
	}
	else
	{
		LOG_WARN("Cannot load scene layout data! No 'layout' entry found!");
	}

	if (memento.HasMember("tutorialData"))
	{
		const auto& tutorialData = memento["tutorialData"];
		if (tutorialData.HasMember("LanguageId"))
		{
			TutorialManager::instance().setLanguage((ETutorialLanguage) tutorialData["LanguageId"].GetInt());
		}
		if (tutorialData.HasMember("CurrentTutorial"))
		{
			std::string currentTutorialName = tutorialData["CurrentTutorial"].GetString();
			auto header = TutorialManager::instance().findTutorialHeader(currentTutorialName);
			if (header)
			{
				auto tutorial = TutorialLoader::loadTutorial(header);
				SetTutorialCommand::dispatch(tutorial);
				LOG_INFO("Tutorial " + header->m_title + " loaded");
			}
		}
		if (tutorialData.HasMember("CurrentStep"))
		{
			SetTutorialStepCommand::dispatch(tutorialData["CurrentStep"].GetInt());
		}
	}
	else
	{
		LOG_WARN("Cannot load scene tutorial data! No 'tutorialData' entry found!");
	}
}
void UIModule::clearScene() {}

Memento UIModule::saveGlobal()
{
	return emptyMemento();
}
void UIModule::loadGlobal(const Memento& memento) {}
void UIModule::clearGlobal() {}
