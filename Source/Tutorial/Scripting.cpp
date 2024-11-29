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

template <typename R>
static std::shared_ptr<TutorialElement> pushElement(Tutorial* self, std::shared_ptr<R> element)
{
	getCurrentStep(self).m_content.push_back(element);

	return element;
}

template <typename R>
static std::shared_ptr<TutorialElement> pushElementAt(Tutorial* self, std::shared_ptr<R> element, int luaIndex)
{
	auto index = luaIndex - 1;

	if (!Math::withinInterval(index, 0, (int) getCurrentStep(self).m_content.size()))
	{
		print(fmt::format("index {} out of bounds", luaIndex));
		return element;
	}

	getCurrentStep(self).m_content.insert(getCurrentStep(self).m_content.begin() + index, element);

	return element;
}

static void popElement(Tutorial* self, int luaElementIdx)
{
	auto index = luaElementIdx - 1;
	if (!Math::withinInterval(index, 0, (int) getCurrentStep(self).m_content.size()))
	{
		print(fmt::format("index {} out of bounds", luaElementIdx));
		return;
	}

	auto& step = getCurrentStep(self);

	if (step.m_content.empty())
	{
		return;
	}

	auto it = step.m_content.begin() + index;
	step.m_content.erase(it);
}

template <typename T>
static Ptr<T> cast(Ptr<TutorialElement> element)
{
	return std::dynamic_pointer_cast<T>(element);
}

LUA_REGISTRATION
{
	// clang-format off

	// Elements
	L.new_usertype<TutorialElement>(
		"TutorialElement",
		sol::no_constructor,
		"content", &TutorialElement::m_content,
		"as_explanation", &cast<Explanation>,
		"as_headline", &cast<Headline>,
		"as_task", &cast<Task>,
		"as_hint", &cast<Hint>,
		"as_choice_task", &cast<ChoiceTask>,
		"as_multi_choice_task", &cast<MultiChoiceTask>,
		"as_input_task", &cast<InputTask>
	);

	L.new_usertype<Explanation>(
		"Explanation",
		sol::meta_function::construct, [](std::string content) {
			return std::make_shared<Explanation>(content);
		},
		sol::base_classes, sol::bases<TutorialElement>()
	);

	L.new_usertype<Headline>(
		"Headline",
		sol::meta_function::construct, [](std::string content) {
			return std::make_shared<Headline>(content);
		},
		sol::base_classes, sol::bases<TutorialElement>()
	);

	L.new_usertype<Task>(
		"Task",
		sol::meta_function::construct, [](std::string content) {
			return std::make_shared<Task>(content);
		},
		sol::base_classes, sol::bases<TutorialElement>(),
		"completed", &Task::m_completed
	);

	L.new_usertype<Hint>(
		"Hint",
		sol::meta_function::construct, [](std::string content) {
			return std::make_shared<Hint>(content);
		},
		sol::base_classes, sol::bases<TutorialElement>(),
		"expanded", &Hint::m_expanded
	);

	L.new_usertype<ChoiceTask>(
		"ChoiceTask",
		sol::meta_function::construct, [](std::string question, std::vector<std::string> choices, int correctChoice) {
			return std::make_shared<ChoiceTask>(question, choices, correctChoice);
		},
		sol::base_classes, sol::bases<TutorialElement>(),
		"choices", &ChoiceTask::m_choices,
		"correct_choice", &ChoiceTask::m_correctChoice
	);

	L.new_usertype<MultiChoiceTask>(
		"MultiChoiceTask",
		sol::meta_function::construct, [](std::string question, std::vector<std::string> choices, std::vector<int> correctChoices) {
			return std::make_shared<MultiChoiceTask>(question, choices, correctChoices);
		},
		sol::base_classes, sol::bases<TutorialElement>(),
		"choices", &MultiChoiceTask::m_choices,
		"correct_choices", &MultiChoiceTask::m_correctChoices
	);

	L.new_usertype<InputTask>(
		"InputTask",
		sol::meta_function::construct, [](std::string question, std::unordered_set<std::string> correctAnswers) {
			return std::make_shared<InputTask>(question, correctAnswers);
		},
		sol::base_classes, sol::bases<TutorialElement>(),
		"correct_answers", &InputTask::m_correctAnswers
	);

	// Tutorial

	L.new_usertype<TutorialStep>(
		"TutorialStep",
		"completed", &TutorialStep::m_completed,
		"content", &TutorialStep::m_content
	);

	L.new_usertype<TutorialHeader>(
		"TutorialHeader",
		// "filename", sol::readonly(&TutorialHeader::m_filename),
		"title", sol::readonly(&TutorialHeader::m_title),
		"description", sol::readonly(&TutorialHeader::m_description)
	);

	L.new_usertype<Tutorial>(
		"Tutorial",
		"header", sol::readonly(&Tutorial::m_header),
        "steps", sol::readonly(&Tutorial::m_steps),
        "get_current_step", &getCurrentStep,
        "set_step", &setStep,
		"push_element", sol::overload(
			&pushElement<Explanation>,
			&pushElement<Task>,
			&pushElement<Hint>,
			&pushElement<ChoiceTask>,
			&pushElement<MultiChoiceTask>,
			&pushElement<InputTask>
		),
		"push_element_at", sol::overload(
			&pushElementAt<Explanation>,
			&pushElementAt<Task>,
			&pushElementAt<Hint>,
			&pushElementAt<ChoiceTask>,
			&pushElementAt<MultiChoiceTask>,
			&pushElementAt<InputTask>
		),
		"pop_element", &popElement
	);
	// clang-format on

	auto api = L["I3T"];

	api["get_tutorial"] = []() -> Ptr<Tutorial> {
		return TutorialManager::instance().getTutorial();
	};
}
