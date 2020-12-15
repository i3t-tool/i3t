
#include "gldebug.h"

#include <iostream>
#if defined(_WIN32)
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp")
#elif defined(__linux__)
#include <execinfo.h>
#endif

namespace pgr {
namespace gldebug {

const char * sourceToString(GLenum src) {
  switch(src) {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION: return "Application";
    case GL_DEBUG_SOURCE_OTHER: return "Other";
    default: return "<unknown>";
  }
}

const char * typeToString(GLenum type) {
  switch(type) {
    case GL_DEBUG_TYPE_ERROR: return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
    case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
    case GL_DEBUG_TYPE_OTHER: return "Other";
    default: return "<unknown>";
  }
}

const char * severityToString(GLenum severity) {
  switch(severity) {
    case GL_DEBUG_SEVERITY_LOW: return "Low";
    case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
    case GL_DEBUG_SEVERITY_HIGH: return "High";
    default: return "<unknown>";
  }
}

void printBacktrace(std::ostream * os) {
  if(os == 0)
    os = &std::cout;

  const int SIZE = 100;
  const int NAME_SIZE = 256;
#if defined(_WIN32)
  HANDLE process = GetCurrentProcess();
  SymSetOptions(SYMOPT_LOAD_LINES);
  SymInitialize(process, NULL, TRUE);

  void * stack[SIZE];
  unsigned short frames = CaptureStackBackTrace(0, SIZE, stack, NULL);
  SYMBOL_INFO * symbol = (SYMBOL_INFO * )calloc(sizeof(SYMBOL_INFO) + NAME_SIZE * sizeof(char), 1);
  symbol->MaxNameLen = NAME_SIZE;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  for(unsigned short i = 0; i < frames; i++ ) {
    SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
    IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    DWORD  dwDisplacement;
    if(!strstr(symbol->Name, __FUNCTION__) && !strstr(symbol->Name,"pgr::debugCallback") && SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &dwDisplacement, &line)) {
      *os << symbol->Name << "():" << line.LineNumber << std::endl;
    }
    if(strcmp(symbol->Name, "main") == 0)
      break;
  }

  free(symbol);
#elif defined(__linux__)
  int j;
  void *buffer[SIZE];
  char **strings;

  int nptrs = backtrace(buffer, SIZE);
  strings = backtrace_symbols(buffer, nptrs);
  if(strings == NULL) {
    *os << "error in backtrace_symbols" << std::endl;
    return;
  }
  for (j = 0; j < nptrs; j++)
    *os << strings[j] << std::endl;
  free(strings);
#else
  *os << "backtrace not supported on this platform" << std::endl;
#endif
}

} // end namespace gldebug
} // end namespace pgr

