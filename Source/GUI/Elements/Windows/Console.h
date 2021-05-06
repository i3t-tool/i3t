#pragma once

#include <sstream>

#include "GUI/Elements/IWindow.h"
#include "Utils/Other.h"

#include <string.h> /* for strcpy_s */

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
	~Console() override = default;
	void render() override;

private:
	void onUpKey();
	void onDownKey();

	COutRedirect m_stdoutCapture;
};
} // namespace UI
