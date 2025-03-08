/**
 * \file
 * \brief
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "Localization.h"

#include "Logger/Logger.h"

#include <fstream>
#include <ranges>

Localization& Localization::instance()
{
	static Localization instance;
	return instance;
}
bool Localization::loadLanguage(const std::string& langName)
{
	AppLanguage language = getLanguageByName(langName);
	if (loadFromFile(language.filePath))
	{
		currentLanguageID = language.id;
		return true;
	}
	return false;
}

bool Localization::loadLanguage(const int langID)
{
	const AppLanguage language = getLanguageById(langID);
	return loadFromFile(language.filePath);
}


bool Localization::loadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		LOG_ERROR("Unable to open file: " + filename + " for localization");
		return false;
	}

	m_translations.clear();

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		if (line[0] == '#')
		{
			continue;
		}

		std::size_t delimiterPos = line.find('=');
		if (delimiterPos != std::string::npos)
		{
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1);
			if (value.empty())
			{
				continue;
			}
			if (m_translations.contains(key))
			{
				LOG_ERROR("Duplicate key found in localization file: " + key);
				LOG_ERROR("Translation will be overwritten");
			}
			m_translations[key] = value;
		}
	}

	file.close();

	return true;
}

const std::string& Localization::translate(const std::string& key) const
{
	auto it = m_translations.find(key);
	if (it != m_translations.end())
	{
		return it->second;
	}
	return key;
}
std::vector<AppLanguage> Localization::getAvailableLanguages()
{
	std::vector<std::string> languages;
	for (const auto& lang : m_languages)
	{
		languages.push_back(lang.name);
	}

	return m_languages;
}
AppLanguage Localization::getLanguageById(int id) const
{
	for (const auto& language : m_languages)
	{
		if (language.id == id)
		{
			return language;
		}
	}
	return m_languages[0];
}

AppLanguage Localization::getLanguageByName(const std::string& name) const
{
	for (const auto& language : m_languages)
	{
		if (language.name == name)
		{
			return language;
		}
	}
	return m_languages[0];
}