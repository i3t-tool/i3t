/**
 * \file GUI/Elements/Modals/ImportedModelsModal.h
 * \author Dan Rakušan
 * \date 17.08.2023
 */
#pragma once

#include <filesystem>

#include "GUI/Elements/IWindow.h"

class ImportedModelsDialog : public IWindow
{
private:
	int m_selectedModelIndex{-1};

public:
	I3T_WINDOW(ImportedModelsDialog)

	ImportedModelsDialog();
	void render() override;

	static void importModel();
	static bool importContentDialog(std::filesystem::path& result, const std::string& title);

	static void maybeRemoveModel(const std::string& alias);
	static void removeModel(const std::string& alias);
};
