#include "I3T.h"

#include "Core/API.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "Scripting/ScriptingModule.h"
#include "State/StateManager.h"

void I3TApplication::onInit()
{
	auto* stateManager = createModule<StateManager>();
	createModule<ScriptingModule>();
	createModule<UIModule>();

	stateManager->createEmptyScene();

	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});
}
