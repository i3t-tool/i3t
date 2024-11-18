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

#include "Scripting/Utils.h"
#include "Tutorial/Tutorial.h"
#include "Tutorial/TutorialManager.h"
#include "Utils/Math.h"

static TutorialStep& getCurrentStep([[maybe_unused]] Tutorial* self)
{
	auto currentStepIdx = TutorialManager::instance().getStep();

	return self->m_steps[currentStepIdx];
}

static void setStep(Tutorial* self, int luaStepIdx)
{
	const auto step = luaStepIdx - 1;
	if (step < 0 || step >= self->getStepCount())
	{
		print(fmt::format("index {} out of bounds", step));
		return;
	}

	TutorialManager::instance().setStep(std::clamp(step, 0, (int) self->getStepCount()));
}

static int showHint(Tutorial* self, std::string message)
{
	auto& currentStep = getCurrentStep(self);
	currentStep.m_content.push_back(std::make_shared<Hint>(std::move(message)));

	return (int) currentStep.m_content.size();
}

static void popElement(Tutorial* self, int luaElementIdx)
{
	auto index = luaElementIdx - 1;
	if (!Math::withinInterval(index, 0, (int) getCurrentStep(self).m_content.size()))
	{
		print(fmt::format("index {} out of bounds", luaElementIdx));
		return;
	}

	auto it = getCurrentStep(self).m_content.begin() + index;
	getCurrentStep(self).m_content.erase(it);
}

LUA_REGISTRATION
{
	// clang-format off
	L.new_usertype<TutorialHeader>("TutorialHeader",
	                               // "filename", sol::readonly(&TutorialHeader::m_filename),
	                               "title", sol::readonly(&TutorialHeader::m_title),
	                               "description", sol::readonly(&TutorialHeader::m_description));

	L.new_usertype<TutorialStep>("TutorialStep",
								 "completed", &TutorialStep::m_completed);

	L.new_usertype<Task>("TutorialTask",
						 "completed", &Task::m_completed);

	L.new_usertype<Tutorial>("Tutorial",
						     "header", sol::readonly(&Tutorial::m_header),
                             "steps", sol::readonly(&Tutorial::m_steps),
                             "get_current_step", &getCurrentStep,
                             "set_step", &setStep,
							 "show_hint", &showHint,
						     "pop_element", &popElement
	);
	// clang-format on
}
