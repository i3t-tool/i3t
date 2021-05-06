#pragma once

#include "GUI/Elements/IWindow.h"

class StyleEditor : public IWindow
{
public:
  I3T_WINDOW(StyleEditor)

  StyleEditor() = default;
	~StyleEditor() override = default;

	void render() override;
};
