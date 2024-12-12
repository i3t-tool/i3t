/**
 * \file
 * \brief
 * \authors Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "GUIImage.h"
#include "TutorialRenderer.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Commands/ApplicationCommands.h"

// forward declaration from TutorialRenderer.h to avoid cyclic dependency
class ITutorialRenderer;

/// Append new languages at the end of the list!
enum class Language
{
	English = 0,
	Czech
};

struct TutorialElement
{
	TutorialElement()
	{
		m_content = "";
	}
	explicit TutorialElement(std::string content) : m_content(std::move(content)) {}
	virtual ~TutorialElement() = default;

	std::string m_content;
	// std::string m_id; // not used

	virtual void acceptRenderer(ITutorialRenderer* tutorialRenderer) = 0;
};

struct Explanation : TutorialElement
{
	explicit Explanation(std::string explanation) : TutorialElement(std::move(explanation)) {}

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderExplanation(this);
	}
};

struct Headline : TutorialElement
{
	explicit Headline(std::string headline) : TutorialElement(std::move(headline)) {}

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderHeadline(this);
	}
};

struct Task : TutorialElement
{
	explicit Task(std::string task) : TutorialElement(std::move(task)), m_completed(false) {}

	bool m_completed; // todo future feature

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderTask(this);
	}
};

struct Hint : TutorialElement
{
	explicit Hint(std::string hint) : TutorialElement(std::move(hint)), m_expanded(false) {}
	bool m_expanded;
	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderHint(this);
	}
};

struct TestQuestion : TutorialElement
{
	explicit TestQuestion(std::string question) : TutorialElement(std::move(question)) {}

	void submit()
	{
		m_isSubmitted = true;
		onSubmit();
	}

	/// Set m_isCorrect based on the answer.
	virtual void onSubmit() = 0;

	bool m_isSubmitted = false;
	bool m_isCorrect = false;
};

struct ChoiceTask : TestQuestion
{
	ChoiceTask(std::string question, std::vector<std::string> choices, int correctChoice)
	    : TestQuestion(std::move(question)), m_choices(std::move(choices)), m_correctChoice(correctChoice)
	{}

	std::vector<std::string> m_choices;
	int m_correctChoice;
	int m_selected = -1;

	void onSubmit() override
	{
		m_isCorrect = m_selected == m_correctChoice;
	}

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderChoiceTask(this);
	}
};

struct MultiChoiceTask : TestQuestion
{
	MultiChoiceTask(std::string question, std::vector<std::string> choices, std::vector<int> correctChoices)
	    : TestQuestion(std::move(question)), m_choices(std::move(choices)), m_correctChoices(std::move(correctChoices))
	{
		m_selected = std::vector<bool>(m_choices.size(), false);
	}

	std::vector<std::string> m_choices;
	std::vector<int> m_correctChoices;
	std::vector<bool> m_selected;

	void onSubmit() override
	{
		m_isCorrect = true;
		for (auto i = 0L; i < m_selected.size(); ++i)
		{
			const auto shouldBeSelected =
			    m_correctChoices.end() != std::find(m_correctChoices.begin(), m_correctChoices.end(), i);
			if (m_selected[i] != shouldBeSelected)
			{
				m_isCorrect = false;
				return;
			}
		}
	}

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderMultiChoiceTask(this);
	}
};

struct InputTask : TestQuestion
{
	InputTask(std::string question, std::unordered_set<std::string> correctAnswers)
	    : TestQuestion(std::move(question)), m_correctAnswers(std::move(correctAnswers))
	{}

	std::unordered_set<std::string> m_correctAnswers;
	std::array<char, 64> m_input = {0};

	void onSubmit() override
	{
		m_isSubmitted = true;
		m_isCorrect = m_correctAnswers.contains(m_input.data());
	}

	void acceptRenderer(ITutorialRenderer* tutorialRenderer) override
	{
		tutorialRenderer->renderInputTask(this);
	}
};

struct TutorialStep
{
	TutorialStep() = default;

	std::vector<std::shared_ptr<TutorialElement>> m_content; // NOTE: need a pointer to avoid object slicing
	std::string m_scriptToRunWhenShown;

	/// Fall back to true for now.
	bool m_completed = true;

	// todo
	// maybe call task?
	// tasks - ptrs to all task widgets
	// questions - ptrss to all question widgets
	// isCompleted - true if all tasks and questions completed (also check in each
	// update vs check after every change)
};

struct TutorialHeader
{
	TutorialHeader(std::string filename, std::string title, std::string description, std::string scene,
	               std::shared_ptr<GUIImage> thumbnail, std::string layout)
	    : m_filename(std::move(filename)), m_title(std::move(title)), m_description(std::move(description)),
	      m_scene(std::move(scene)), m_thumbnailImage(std::move(thumbnail)), m_layout(std::move(layout))
	{}

	fs::path m_filename;
	std::string m_title;
	std::string m_description;
	std::string m_scene;
	std::shared_ptr<GUIImage> m_thumbnailImage;
	std::string m_layout;
};

/**
 * \brief Structure for holding information need for showing a specific
 * tutorial. Should be created by \fn TutorialLoader::loadFile() function.
 */
class Tutorial
{
public:
	Tutorial(std::shared_ptr<TutorialHeader> header, std::vector<TutorialStep> steps,
	         std::unordered_map<std::string, std::shared_ptr<GUIImage>> filenameToImageMap)
	    : m_header(std::move(header)), m_steps(std::move(steps)), m_filenameToImage(std::move(filenameToImageMap))
	{}

	// other properties
	auto getStepCount() const
	{
		return m_steps.size();
	}

	// general
	std::shared_ptr<TutorialHeader> m_header;
	// step content
	std::vector<TutorialStep> m_steps;
	/// support structures, filename to GUIImage (including GLuint id)
	std::unordered_map<std::string, std::shared_ptr<GUIImage>> m_filenameToImage;
};
