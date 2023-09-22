#pragma once

#include <memory>

#include "GUI/Elements/IWindow.h"

class TextureRegion;

class AboutWindow : public IWindow
{
public:
	I3T_WINDOW(AboutWindow)
	std::shared_ptr<TextureRegion> dcgiReg;

	AboutWindow();
	void render() override;
};
