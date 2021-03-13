#pragma once

#include "GUI/Elements/IWindow.h"

class LogWindow : public IWindow
{
public:
	I3T_WINDOW(LogWindow)

	LogWindow() = default;
	void render() override;
};
