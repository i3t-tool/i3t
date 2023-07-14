#include "I3T.h"

#include "Core/API.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
#include "GUI/Elements/Modals/BeforeNewTutModal.h"
#include "Scripting/ScriptingModule.h"
#include "State/StateManager.h"

void I3TApplication::onInit()
{
	createModule<StateManager>();
	createModule<ScriptingModule>();
	createModule<UIModule>();

	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});

	BeforeNewProjectCommand::addListener([this]() {
		I3T::getUI()->getWindowManager().showUniqueWindow<BeforeNewModal>();
	});

	BeforeNewTutCommand::addListener([this]() {
		I3T::getUI()->getWindowManager().showUniqueWindow<BeforeNewTutModal>();
	});
}
