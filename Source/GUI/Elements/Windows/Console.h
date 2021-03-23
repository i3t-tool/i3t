#pragma once

#include <sstream>

#include "GUI/Elements/IWindow.h"
#include "Utils/Other.h"

namespace UI
{
/**
 * The stdout is redirected to the console.
 */
class Console : public IWindow
{
public:
	I3T_WINDOW(Console)

	Console(bool show);
	~Console() override;
	void render() override;

private:
	void onUpKey();
	void onDownKey();

	COutRedirect m_stdoutCapture;
};
} // namespace UI
