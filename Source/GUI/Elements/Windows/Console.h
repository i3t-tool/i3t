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

	void onUpKey();
	void onDownKey();

private:
	COutRedirect m_stdoutCapture;
};
} // namespace UI
