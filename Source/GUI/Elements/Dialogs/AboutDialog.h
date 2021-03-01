#pragma once

#include <memory>

#include "GUI/Elements/IWindow.h"

class TextureRegion;

class AboutDialog : public IWindow
{
public:
	I3T_WINDOW(AboutDialog)
	std::shared_ptr<TextureRegion> dcgiReg;

	AboutDialog();
	void render() override;
};
