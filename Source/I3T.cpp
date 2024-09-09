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

#include "API.h"
#include "Config.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "Scripting/ScriptingModule.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

using namespace Core;

void I3TApplication::onInit()
{
	Core::GraphManager::init();

	//

	CloseCommand::addListener([this] {
		close();
	});

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

	Ptr<rapidjson::Document> configDoc = std::make_shared<rapidjson::Document>();
	JSON::parse(Configuration::configFile, *configDoc, "Data/Schemas/Config.schema.json");

	ResourceManager* resourceManager = createModule<Core::ResourceManager>();
	App::getModule<StateManager>().addOriginator(resourceManager);
	App::getModule<ResourceManager>().loadDefaultResources(*configDoc);

	Vp::Viewport* viewport = createModule<Vp::Viewport>();
	App::getModule<StateManager>().addOriginator(viewport);

	createModule<ScriptingModule>();
	createModule<UIModule>();

	stateManager->loadGlobal();
	stateManager->newScene(true);

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
} // namespace I3T