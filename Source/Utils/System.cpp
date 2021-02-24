#include "System.h"

#ifdef WIN32
#include "Windows.h"
#endif

void runShell(const char* sexe)
{
#ifdef WIN32
	ShellExecute(NULL, "open", sexe, NULL, NULL, SW_SHOWNORMAL);
#endif
}
