/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \date 17.08.2023
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <filesystem>

#include "GUI/Elements/IWindow.h"

class ImportedModelsDialog : public IWindow
{
private:
	int m_selectedModelIndex{-1};
	std::string m_selectedModelAlias{};
	bool m_normalizeImportedModels{false}; // TODO: (DR) This should be saved in a global config

public:
	I3T_WINDOW(ImportedModelsDialog)

	ImportedModelsDialog();

	void render() override;

	static void importModel(bool normalize);
	static bool importContentDialog(std::vector<std::filesystem::path>& result, const std::string& title);

	static void maybeRemoveModel(const std::string& alias);
	static void removeModel(const std::string& alias);
};
