#pragma once

#include <string>

#include "GUI/Elements/IWindow.h"

class Theme;

namespace UI
{
void showColors();
void showDimensions();

class StyleEditor : public IWindow
{
public:
	I3T_WINDOW(StyleEditor)

	StyleEditor() = default;

	void render() override;

private:
	void renderSaveRevertField();

	/// If empty, value from m_newThemeName is used.
	void saveCurrentTheme(const std::string& name = "");
	void revertChangesOnCurrentTheme();

	int m_currentThemeIdx = 0;
	std::string m_newThemeName;
	std::string m_infoMessage;
};
} // namespace UI
