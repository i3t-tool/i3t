#pragma once

#include <sstream>

#include "GUI/Elements/IWindow.h"

class ConsoleWindow : public IWindow
{
public:
	I3T_WINDOW(ConsoleWindow)

	ConsoleWindow(bool show);
	~ConsoleWindow();
	void render() override;

private:
	std::ostringstream m_buffer;
};
