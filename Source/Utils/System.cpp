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
/*bool openFileDialog(char* filename, int bufsize) {
#ifdef WIN32
	char*filter= "C source file (*.c)\0*.c\0\0";
	char initfile[256]="\0";
	char*initdir=NULL;
	char*title="Open I3T script";
	OPENFILENAMEA openfile;
	openfile.lStructSize=sizeof(OPENFILENAMEA);
	openfile.hwndOwner=NULL;
	openfile.hInstance=NULL;
	openfile.lpstrFilter=filter;
	openfile.lpstrCustomFilter=NULL;
	openfile.nMaxCustFilter=0;
	openfile.nFilterIndex=0;
	openfile.lpstrFile=initfile;
	openfile.nMaxFile=256;
	openfile.lpstrTitle=NULL;
	openfile.nMaxFileTitle=0;
	openfile.lpstrInitialDir=initdir;
	openfile.lpstrTitle=title;
	openfile.Flags=OFN_CREATEPROMPT;
	if (GetOpenFileName(&openfile)) {
		if(strlen(openfile.lpstrFile)>=bufsize){return false;}
		strcpy(filename,openfile.lpstrFile);
		return true;
	}
	return false;
#endif
	return false;
}*/