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

#pragma once

#include "Core/Defs.h"
#include "Tutorial/Tutorial.h"

/// \todo Handle languages somewhere else for whole application.
enum class ETutorialLanguage
{
	Czech,
	English,
};

inline ETutorialLanguage toggle(ETutorialLanguage language)
{
	switch (language)
	{
	case ETutorialLanguage::Czech:
		return ETutorialLanguage::English;
	case ETutorialLanguage::English:
		return ETutorialLanguage::Czech;
	}
	return ETutorialLanguage::Czech;
}

class TutorialManager : public Singleton<TutorialManager>
{
public:
	using UTutorialHeaders = std::vector<std::shared_ptr<TutorialHeader>>;

	/// \brief searches through the tutorial directory and adds all found tutorial files to the window.
	/// \todo Use language enum or locale string.
	void reloadTutorials();

	const UTutorialHeaders& getTutorialHeaders() const
	{
		return m_tutorialHeaders;
	}

	Ptr<Tutorial>& getTutorial()
	{
		return m_currentTutorial;
	}

	void setTutorial(Ptr<Tutorial> tutorial);

	int getStep() const
	{
		return m_currentStep;
	}

	/**
	 * \brief Sets which step of currently set tutorial should be shown.
	 * \param stepNumber A step number counted from 0.
	 * \return true if successful, false if number out of range or no currently
	 * set tutorial
	 */
	void setStep(int stepNumber);

	ETutorialLanguage getLanguage() const
	{
		return m_language;
	}

	void setLanguage(ETutorialLanguage language);

	/// Switches the language between Czech and English and reloads the tutorials.
	void toggleLanguage();

	/// \brief Finds existing tutorial header by its filename.
	std::shared_ptr<TutorialHeader> findTutorialHeader(const std::string& tutorialName);

private:
	ETutorialLanguage m_language = ETutorialLanguage::Czech;

	/// \todo
	Ptr<Tutorial> m_currentTutorial;
	int m_currentStep;

	/// Metadata of all discovered tutorials.
	UTutorialHeaders m_tutorialHeaders;
};
