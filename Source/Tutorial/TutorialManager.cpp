/**
 * \file
 * \brief
 * \authors Martin Herich, Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "Tutorial/TutorialManager.h"

#include "Config.h"

#include "Scripting/ScriptingModule.h"
#include "Tutorial/TutorialLoader.h"

void TutorialManager::reloadTutorials()
{
	m_tutorialHeaders.clear();

	// preload all tutorials located in TUTORIALS_FOLDER recursively
	std::string path = Config::TUTORIALS_FOLDER;
	if (path[0] == '/')
	{
		path.erase(0, 1);
	}

	if (!std::filesystem::exists(path))
	{
		LOG_INFO("Path for tutorials not found. Searched path: " + path);
		return;
	}

	LOG_INFO("Searching for tutorials in: " + path);

	const bool english = m_language == ETutorialLanguage::English;

	// For all files in path recursively
	for (auto const& entry : std::filesystem::recursive_directory_iterator(
	         path, std::filesystem::directory_options::skip_permission_denied))
	{
		std::string filename = entry.path().stem().string();

		if (entry.path().extension() == ".tut" && ((english && filename.substr(filename.size() - 2) == "en") ||
		                                           (!english && filename.substr(filename.size() - 2) != "en")))
		{
			std::string pathString = entry.path().string();
			LOG_INFO(pathString);
			// Load header part of tutorial
			if (std::shared_ptr<TutorialHeader> header = TutorialLoader::loadTutorialHeader(pathString); header)
			{
				m_tutorialHeaders.push_back(std::move(header));
			}
			else
			{
				LOG_ERROR("Tutorial header " + pathString + " not loaded.");
			}
		}
	}
}

void TutorialManager::setTutorial(Ptr<Tutorial> tutorial)
{
	m_currentTutorial = tutorial;
	setStep(0);
}

void TutorialManager::setStep(int stepNumber)
{
	if (m_currentTutorial == nullptr || stepNumber < 0 || stepNumber >= m_currentTutorial->getStepCount())
	{
		LOG_FATAL("Trying to set an invalid step number or tutorial not active");
	}
	else
	{
		// set step
		m_currentStep = stepNumber;
		auto& step = m_currentTutorial->m_steps[m_currentStep];

		auto& scripting = App::getModule<ScriptingModule>();
		scripting.clearTimers();

		// run script
		if (!step.m_scriptToRunWhenShown.empty())
		{
			ConsoleCommand::dispatchImmediate(step.m_scriptToRunWhenShown);
		}
	}
}

void TutorialManager::setLanguage(ETutorialLanguage language)
{
	m_language = language;
	reloadTutorials();
}

void TutorialManager::toggleLanguage()
{
	m_language = toggle(m_language);
	reloadTutorials();
}

std::shared_ptr<TutorialHeader> TutorialManager::findTutorialHeader(const std::string& tutorialName)
{
	for (const auto& header : m_tutorialHeaders)
	{
		if (header->m_filename == tutorialName)
		{
			return header;
		}
	}
	return nullptr;
}
