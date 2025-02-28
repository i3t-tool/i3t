/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "GUI/Elements/Windows/TutorialWindow.h"
#include "I3T.h"
#include "Scripting/ScriptingModule.h"
#include "Tutorial/TutorialManager.h"

#include "I3TUtil.h"

using TutorialScriptingTest = ApplicationTest;

TEST_F(TutorialScriptingTest, TutorialBase)
{
	using namespace std::string_literals;

	TutorialManager::instance().reloadTutorials();

	auto& tutorials = (TutorialManager::UTutorialHeaders&) TutorialManager::instance().getTutorialHeaders();
	EXPECT_TRUE(!tutorials.empty());

	auto tutorialWindow = App::getModule<UIModule>().getWindowManager().getWindowPtr<TutorialWindow>();
	EXPECT_TRUE(tutorialWindow != nullptr);

	tutorialWindow->setTutorial(tutorials[0]);

	auto& scripting = App::getModule<ScriptingModule>();

	/// \todo Tutorial is loaded in the next frame!
	/*
	auto maybeErr = scripting.runScript(R"(
	    tutorial = I3T.get_tutorial()
	    assert(tutorial ~= nil)
	)");
	EXPECT_FALSE(maybeErr);

	maybeErr = scripting.runScript(("assert(tutorial.header.title == \""s + tutorials[0]->m_title + "\")"s).c_str());
	EXPECT_FALSE(maybeErr);
	 */
}
