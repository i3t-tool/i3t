#pragma once

#include "GUI/Elements/IWindow.h"

class ConsoleWindow : public IWindow
{
public:
	I3T_WINDOW(ConsoleWindow)

	ConsoleWindow() = default;
	void render() override;
};
