#include "Scripting/Environment.h"

#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/UIModule.h"
#include "Tutorial/Tutorial.h"

LUA_REGISTRATION
{
	L.new_usertype<TutorialHeader>("TutorialHeader",
	                               // "filename", sol::readonly(&TutorialHeader::m_filename),
	                               "title", sol::readonly(&TutorialHeader::m_title), "description",
	                               sol::readonly(&TutorialHeader::m_description));

	L.new_usertype<TutorialStep>("TutorialStep", "scriptToRunWhenShown",
	                             sol::readonly(&TutorialStep::m_scriptToRunWhenShown));

	L.new_usertype<Tutorial>("Tutorial", "header", sol::readonly(&Tutorial::m_header), "steps",
	                         sol::readonly(&Tutorial::m_steps));

	L.new_usertype<std::monostate>(
	    "I3T", "tutorial", sol::property([]() -> Ptr<Tutorial> {
		    if (auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>())
		    {
			    return tutorialWindow->getTutorial();
		    }

		    return nullptr;
	    }));
}
