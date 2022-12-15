#include "IWindow.h"

std::string IWindow::getName() const { return m_name; };

std::string IWindow::setName(const char* name)
{
	m_name = fmt::format("{}###{}", name, getID());
	return m_name;
};
