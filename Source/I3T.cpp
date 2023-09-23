#include "I3T.h"

#include "Config.h"
#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
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
		onClose();
	});

	InputManager::init();

	InputManager::bindGlobalAction("undo", EKeyState::Pressed, [&]() {
		LOG_INFO("undo triggered");
		App::getUI()->invokeLater([]() {
			App::getModule<StateManager>().undo();
		});
	});
	InputManager::bindGlobalAction("redo", EKeyState::Pressed, [&]() {
		LOG_INFO("redo triggered");
		App::getUI()->invokeLater([]() {
			App::getModule<StateManager>().redo();
		});
	});

	auto* stateManager = createModule<StateManager>();

	// Former initI3T member function
	const auto conf = loadConfig("Config.json");

	ResourceManager* resourceManager = createModule<Core::ResourceManager>();
	App::getModule<StateManager>().addOriginator(resourceManager);
	App::getModule<ResourceManager>().createDefaultResources(conf->Resources);

	m_viewport = new Vp::Viewport();
	m_viewport->init(Vp::ViewportSettings());
	App::getModule<StateManager>().addOriginator(m_viewport);

	createModule<ScriptingModule>();
	createModule<UIModule>();

	stateManager->loadGlobal();
	stateManager->newScene();

	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});
}
