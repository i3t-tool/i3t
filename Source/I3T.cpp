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
#include "I3T.h"

#include "Config.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h" // TODO: (DR) Why is this include here? Some dependency spaghetti?
#include "GUI/Test/TestModule.h"
#include "GUI/Viewport/ViewportModule.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Localization/Localization.h"
#include "Scripting/ScriptingModule.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

using namespace Core;

ApplicationSettings I3TApplication::g_settings;

I3TApplication::~I3TApplication()
{
	if (!m_shouldClose)
	{
		close();
	}
}

void I3TApplication::onInit()
{
	Core::GraphManager::init();

	//

	InputManager::init();

	InputManager::bindGlobalAction("undo", EKeyState::Pressed, [&]() {
		LOG_INFO("undo triggered");
		App::getModule<StateManager>().undo();
	});
	InputManager::bindGlobalAction("redo", EKeyState::Pressed, [&]() {
		LOG_INFO("redo triggered");
		App::getModule<StateManager>().redo();
	});

	auto* stateManager = createModule<StateManager>();
	// Call loading of the localization file after the state manager is created
	LOCALIZATION.loadLanguage(LOCALIZATION.currentLanguageID);

	App::getModule<StateManager>().addOriginator(this);

	ResourceManager* resourceManager = createModule<Core::ResourceManager>();
	App::getModule<StateManager>().addOriginator(resourceManager);

	createModule<ViewportModule>();
	createModule<ScriptingModule>();
	createModule<WorkspaceModule>();
	UIModule* uiModule = createModule<UIModule>();
	App::getModule<StateManager>().addOriginator(uiModule);

	stateManager->loadGlobal();
	stateManager->newScene(true);

	// Clear the window with a dark color to prevent a bright white window popping up
	glClearColor(0.135f, 0.135f, 0.135f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_window->get());

	// Show the window immediately after loading global config
	// (Predominantly after loading window positions in Application loadGlobal)
	m_window->show();

	Ptr<rapidjson::Document> configDoc = std::make_shared<rapidjson::Document>();
	JSON::parse(Configuration::configFile, *configDoc, "Data/Schemas/Config.schema.json");
	App::getModule<ResourceManager>().loadDefaultResources(*configDoc);

	createModule<TestModule>();

	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});
}

void I3TApplication::onUpdate(double delta)
{
	Logger::getInstance().update();
	Core::GraphManager::update(delta);
}

void I3TApplication::onClose() {}

Memento I3TApplication::saveScene(Scene* scene)
{
	return emptyMemento();
}

Memento I3TApplication::saveGlobal()
{

	saveSettings();
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document appDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(g_settings, appDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("application", appDoc, doc.GetAllocator());

	return doc;
}

void I3TApplication::loadGlobal(const Memento& memento)
{
	if (!memento.HasMember("application"))
	{
		LOG_ERROR("Cannot load global application data! No 'application' entry found!");
		loadSettings();
		return;
	}
	LOG_INFO("Loaded global application data.");

	JSON::deserializeDocument(memento["application"], g_settings);
	loadSettings();
}

void I3TApplication::clearGlobal()
{
	g_settings.appLoopSettings = AppLoopSettings();
	m_appLoopManager = AppLoopManager(g_settings.appLoop().vsync, g_settings.appLoop().shouldLimitFPS,
	                                  g_settings.appLoop().targetFPS, g_settings.appLoop().shouldLimitFPSOnIdle,
	                                  g_settings.appLoop().targetFPSOnIdle, g_settings.appLoop().secondsBeforeIdle);
	m_window->setVSync(g_settings.appLoop().vsync);
}

void I3TApplication::saveSettings()
{
	g_settings.appLoop().vsync = m_appLoopManager.isVsync();
	g_settings.appLoop().shouldLimitFPS = m_appLoopManager.shouldLimitFPS();
	g_settings.appLoop().targetFPS = m_appLoopManager.getTargetFPS();
	g_settings.appLoop().shouldLimitFPSOnIdle = m_appLoopManager.shouldLimitFPSOnIdle();
	g_settings.appLoop().secondsBeforeIdle = m_appLoopManager.getSecondsBeforeIdle();
	g_settings.appLoop().targetFPSOnIdle = m_appLoopManager.getTargetFPSOnIdle();

	m_window->getWindowPosAndSize(g_settings.window().windowPosX, g_settings.window().windowPosY,
	                              g_settings.window().windowWidth, g_settings.window().windowHeight,
	                              g_settings.window().windowMaximized);
}

void I3TApplication::loadSettings()
{
	m_appLoopManager = AppLoopManager(g_settings.appLoop().vsync, g_settings.appLoop().shouldLimitFPS,
	                                  g_settings.appLoop().targetFPS, g_settings.appLoop().shouldLimitFPSOnIdle,
	                                  g_settings.appLoop().targetFPSOnIdle, g_settings.appLoop().secondsBeforeIdle);
	m_window->setVSync(g_settings.appLoop().vsync);
	m_window->setWindowPosAndSize(g_settings.window().windowPosX, g_settings.window().windowPosY,
	                              g_settings.window().windowWidth, g_settings.window().windowHeight,
	                              g_settings.window().windowMaximized);
}

//////////////////////////////////////////////

namespace I3T
{
I3TApplication& app()
{
	return static_cast<I3TApplication&>(App::get());
}
UIModule* getUI()
{
	return &App::get().getModule<UIModule>();
}
Vp::Viewport* getViewport()
{
	return &App::get().getModule<ViewportModule>().getViewport();
}
ViewportModule& getViewportModule()
{
	return App::get().getModule<ViewportModule>();
}
Core::ResourceManager& getResourceManager()
{
	return App::get().getModule<Core::ResourceManager>();
}
WorkspaceModule& getWorkspace()
{
	return App::get().getModule<WorkspaceModule>();
}
std::vector<Theme>& getThemes()
{
	return getUI()->getThemes();
}
Theme& getTheme()
{
	return getUI()->getTheme();
}
ImFont* getFont(EFont font)
{
	return getTheme().get(font);
}

const ImVec4& getColor(EColor color)
{
	return getTheme().get(color);
}

float getSize(ESize size)
{
	return getTheme().get(size);
}
ImVec2 getSize(ESizeVec2 size)
{
	return getTheme().get(size);
}
} // namespace I3T
