/**
 * \file
 * \brief Localization system for handling translations in the application.
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
#include <vector>

/**
 * \def LOCALIZATION
 * Macro for accessing the singleton instance of Localization.
 */
#define LOCALIZATION Localization::instance()

/**
 * \def _ts(key)
 * Macro for translating a key to the current language.
 * \return std::string
 */
#define _ts(key) LOCALIZATION.translate(key)

/**
 * \def _t(key)
 * \return char*
 * Macro for translating a key and returning a C-style string.
 */
#define _t(key) _ts(key).c_str()

/**
 * \def _tbd(key)
 * No-op macro for a key that doesn't have translations filled in yet.
 */
#define _tbd(key) key

/**
 * \def ICON_T(icon, key)
 * Macro for concatenating an icon with a translated string.
 */
#define ICON_T(icon, key) (std::string(icon) + _ts(key)).c_str()

/**
 * \struct AppLanguage
 * \brief Represents a language option available in the application.
 */
struct AppLanguage
{
	int id;               /**< Unique identifier for the language. */
	std::string name;     /**< Display name of the language. */
	std::string filePath; /**< File path to the language translation file. */
};

/**
 * \class Localization
 * \brief Manages language translations for the application.
 */
class Localization
{
public:
	/**
	 * \brief Gets the singleton instance of the Localization class.
	 * \return Reference to the Localization instance.
	 */
	static Localization& instance();

	/**
	 * \brief Loads a language by name.
	 * \param langName Name of the language to load.
	 * \return True if the language was successfully loaded, false otherwise.
	 */
	bool loadLanguage(const std::string& langName);

	/**
	 * \brief Loads a language by its ID.
	 * \param langID ID of the language to load.
	 * \return True if the language was successfully loaded, false otherwise.
	 */
	bool loadLanguage(int langID);

	/**
	 * \brief Loads translations from a file.
	 * \param filename Path to the translation file.
	 * \return True if the file was successfully loaded, false otherwise.
	 */
	bool loadFromFile(const std::string& filename);

	/**
	 * \brief Translates a given key.
	 * \param key The key to be translated.
	 * \return Translated string corresponding to the key.
	 */
	const std::string& translate(const std::string& key) const;

	/**
	 * \brief Retrieves the list of available languages.
	 * \return A vector containing available language options.
	 */
	std::vector<AppLanguage> getAvailableLanguages();

	/**
	 * \brief Deleted copy constructor to enforce singleton pattern.
	 */
	Localization(const Localization&) = delete;

	/**
	 * \brief Deleted assignment operator to enforce singleton pattern.
	 */
	Localization& operator=(const Localization&) = delete;

	int currentLanguageID = 0; /**< ID of the currently selected language. */

private:
	/**
	 * \brief Private constructor to enforce singleton pattern.
	 */
	Localization() = default;

	/**
	 * \brief Retrieves a language by its ID.
	 * \param id The language ID.
	 * \return The corresponding AppLanguage object.
	 */
	AppLanguage getLanguageById(int id) const;

	/**
	 * \brief Retrieves a language by its name.
	 * \param name The language name.
	 * \return The corresponding AppLanguage object.
	 */
	AppLanguage getLanguageByName(const std::string& name) const;

	std::unordered_map<std::string, std::string> m_translations; /**< Map storing translation key-value pairs. */

	/**
	 * \brief List of predefined available languages.
	 */
	std::vector<AppLanguage> m_languages{
	    {.id = 0, .name = "English", .filePath = "Data/Localization/en.txt"},
	    {.id = 1, .name = "Čeština", .filePath = "Data/Localization/cz.txt"},
	    {.id = 2, .name = "Русский", .filePath = "Data/Localization/ru.txt"},
	    {.id = 3, .name = "Українська", .filePath = "Data/Localization/ua.txt"},
	};
};