#include "Utils/System.h"

#include <cstdlib>

#ifdef WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif

void systemOpen(const std::string& path)
{
#if defined(WIN32)
	ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
#elif defined(__linux__)
	system(("xdg-open " + path).c_str());
#endif
}
