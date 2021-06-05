#pragma once

#include <vector>

#include "GUI/Elements/IWindow.h"
#include "GUI/Theme.h"

class StyleEditor : public IWindow
{
public:
  I3T_WINDOW(StyleEditor)

  StyleEditor() = default;
	~StyleEditor() override = default;

	void render() override;
};
