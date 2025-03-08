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

#pragma once

#include <string>
#include <unordered_map>

#define LOCALIZATION Localization::instance()

#define _ts(key) LOCALIZATION.translate(key)
#define _t(key) _ts(key).c_str()
#define ICON_T(icon, key) (std::string(icon) + _ts(key)).c_str()


struct AppLanguage
{
	int id;
	std::string name;
	std::string filePath;
};

class Localization
{
public:
	static Localization& instance();

	bool loadLanguage(const std::string& langName);
	bool loadLanguage(int langID);
	bool loadFromFile(const std::string& filename);

	const std::string& translate(const std::string& key) const;

	std::vector<AppLanguage> getAvailableLanguages();

	Localization(const Localization&) = delete;
	Localization& operator=(const Localization&) = delete;

	int currentLanguageID = 0;

private:
	Localization() = default;

	AppLanguage getLanguageById(int id) const;
	AppLanguage getLanguageByName(const std::string& name) const;


	std::unordered_map<std::string, std::string> m_translations;
	std::vector<AppLanguage> m_languages{
	    {.id = 0, .name = "English", .filePath = "Data/Localization/en.txt"},
	    {.id = 1, .name = "Čeština", .filePath = "Data/Localization/cz.txt"},
	    {.id = 2, .name = "Русский", .filePath = "Data/Localization/ru.txt"},
	};
};
