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
#include "GUI/Workspace/WorkspaceModule.h"
#include "Localization/Localization.h"
#include "Scripting/ScriptingModule.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

using namespace Core;

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

	Ptr<rapidjson::Document> configDoc = std::make_shared<rapidjson::Document>();
	JSON::parse(Configuration::configFile, *configDoc, "Data/Schemas/Config.schema.json");

	ResourceManager* resourceManager = createModule<Core::ResourceManager>();
	App::getModule<StateManager>().addOriginator(resourceManager);
	App::getModule<ResourceManager>().loadDefaultResources(*configDoc);

	Vp::Viewport* viewport = createModule<Vp::Viewport>();
	App::getModule<StateManager>().addOriginator(viewport);

	createModule<ScriptingModule>();
	createModule<WorkspaceModule>();
	UIModule* uiModule = createModule<UIModule>();
	App::getModule<StateManager>().addOriginator(uiModule);

	stateManager->loadGlobal();
	stateManager->newScene(true);

	createModule<TestModule>();

	// TODO: (DR)s Isn't this redundant? This is already called in StateManager
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
	return &App::get().getModule<Vp::Viewport>();
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
