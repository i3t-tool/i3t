/**
 * \file
 * \brief
 * \authors Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "Scripting/Environment.h"

#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/UIModule.h"
#include "Tutorial/Tutorial.h"

LUA_REGISTRATION
{
	// clang-format off
	L.new_usertype<TutorialHeader>("TutorialHeader",
	                               // "filename", sol::readonly(&TutorialHeader::m_filename),
	                               "title", sol::readonly(&TutorialHeader::m_title),
	                               "description", sol::readonly(&TutorialHeader::m_description));

	L.new_usertype<TutorialStep>("TutorialStep",
	                             "scriptToRunWhenShown", sol::readonly(&TutorialStep::m_scriptToRunWhenShown));

	L.new_usertype<Task>("TutorialTask",
						 "completed", &Task::m_completed);

	L.new_usertype<Tutorial>("Tutorial",
						     "header", sol::readonly(&Tutorial::m_header),
                             "steps", sol::readonly(&Tutorial::m_steps));

	L.new_usertype<std::monostate>(
	    "I3T", "tutorial", sol::property([]() -> Ptr<Tutorial> {
			if (auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>())
			{
				return tutorialWindow->getTutorial();
			}

			return nullptr;
	    }));
	// clang-format on
}
