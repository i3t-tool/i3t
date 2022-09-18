#pragma once

#include <vector>

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
	~StyleEditor() override = default;

	void render() override;

private:
	void renderSaveRevertField();
};
} // namespace UI
