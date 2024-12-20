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

#include "Tutorial.h"

class TutorialLoader
{
public:
	static std::shared_ptr<TutorialHeader> loadTutorialHeader(std::string& path);
	static std::shared_ptr<Tutorial> loadTutorial(std::shared_ptr<TutorialHeader> header);
	/**
	 * @brief simple wrapper that tries to load an image
	 * @param path whole path to the image
	 * @return ptr to image if successful, nullptr otherwise
	 */
	static std::shared_ptr<GUIImage> loadImage(const std::string& path);
	static std::string getDirectory(const std::filesystem::path& path);

private:
	TutorialLoader() = default;

	// enum state_t
	// {
	//   STATE_NO_ELEMENT, STATE_EXPLANATION, STATE_TASK, STATE_HINT,
	//   STATE_CHOICE, STATE_MULTICHOICE, STATE_INPUT
	// };

	enum class EBlockType
	{
		NOT_BLOCK = 0,
		EXPLANATION,
		TASK,
		HINT,
		CHOICE,
		MULTICHOICE,
		INPUT,
		SCRIPT,
		SCRIPT_FILE,
		HEADLINE
	};

	enum class ESingleLineType
	{
		NOT_SINGLE_LINE = 0,
		TASK_SINGLE,
		HINT_SINGLE,
		CORRECT_ANSWER,
		WRONG_ANSWER,
		ANSWER_LIST,
		SCRIPT_SINGLE,
		SCRIPT_FILE,
		HEADLINE_SINGLE
	};

	// enum keyword_t
	//{
	//   NO_KEYWORD, EMPTY, STEP_START, TASK, HINT, CHOICE, MULTICHOICE, INPUT,
	//   CORRECT_ANSWER, WRONG_ANSWER, ANSWER_LIST
	// };

	// static state_t m_state;
	// static int m_currentStep;
	// static std::shared_ptr<TutorialElement>& m_currentElement;
	// static int m_currentIndent;

	/**
	 * @brief returns according enum if such keyword was specified, EMPTY / 1 if
	 * the string is empty and NO_KEYWORD / 0 if none of the previous
	 */
	// static blockType_t isKeyword(const std::string& string);
	static void skipSpaces(std::istringstream& stream);
	static void skipSpaces(std::istringstream& stream, unsigned int maxCount);

	/**
	 * @brief create a new explanation element in the referenced step and return a
	 * also reference to it
	 * @param step a step at which the new element should be created
	 * @param string starting value of the element
	 * @return a reference to the shared_ptr of the newly created explanation
	 * element
	 */
	static std::shared_ptr<TutorialElement>& createExplanation(TutorialStep& step, const std::string& string);
	static std::shared_ptr<TutorialElement>& createHeadline(TutorialStep& step, const std::string& string);
	static std::shared_ptr<TutorialElement>& createTask(TutorialStep& step, const std::string& string);
	static std::shared_ptr<TutorialElement>& createHint(TutorialStep& step, const std::string& string);
	static std::shared_ptr<TutorialElement>& createChoice(TutorialStep& step, const std::string& question,
	                                                      const std::vector<std::string>& choices, int correctChoice);
	static std::shared_ptr<TutorialElement>& createMultichoice(TutorialStep& step, const std::string& question,
	                                                           std::vector<std::string>& choices,
	                                                           const std::vector<int>& correctChoices);
	static std::shared_ptr<TutorialElement>& createInput(TutorialStep& step, const std::string& question,
	                                                     const std::unordered_set<std::string>& correctAnswers);
	static void addScript(TutorialStep& step, const std::string& script);
	// todo possibly make this a templated function
	// todo maybe accept strings as rvalue references and move it

	// static void startTask();
};
