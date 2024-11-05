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
#include "Tutorial/TutorialManager.h"

LUA_REGISTRATION
{
	auto api = L["I3T"];

	api["get_tutorial"] = []() -> Ptr<Tutorial> {
		if (auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>())
		{
			return tutorialWindow->getTutorial();
		}

		return nullptr;
	};

	api["set_tutorial"] = [](const Ptr<Tutorial>& tutorial) {
		if (auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>())
		{
			tutorialWindow->setTutorial(tutorial);
		}
	};
};
