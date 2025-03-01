#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "pgr.h"

#include "GLFW/glfw3.h"

namespace pgr {

static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, void* userParam) {
  std::ostream * os = reinterpret_cast<std::ostream*>(userParam);
  if(os == 0)
    os = &std::cerr;
  *os << "---- OGL DEBUG ----" << std::endl;
  *os << "message <" << id << ">: '" << gldebug::sourceToString(source) << "' reported '" << gldebug::typeToString(type)
      << "' with '" << gldebug::severityToString(severity) << "' severity: " << std::endl << message << std::endl;
  *os << "---- BACKTRACE ----" << std::endl;
  gldebug::printBacktrace(os);
  *os << "---- ---- ---- ----" << std::endl;
}

bool initialize(int glVerMajor, int glVerMinor, DebugLevel debugLevel) {
#if USE_GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "cannot initialize opengl" << std::endl;
    return false;
  }
  int major = 0, minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  if (major < glVerMajor || (major == glVerMajor && minor < glVerMinor)) {
    std::cerr << "OpenGL " << glVerMajor << "." << glVerMinor
              << " or higher not available" << std::endl;
    return false;
  }
#else
  // we have to sate experimental to work in forward comp mode
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    std::cerr << "glew init failed (right context not created?)" << std::endl;
    return false;
  }
  GLenum err = glGetError();
  // silently ignore error caused by glewInit() - happens in forward comp.
  //if(err != GL_NONE)
  //  std::cerr << "glErr in glewInit" << std::endl;

  std::stringstream vers;
  vers << "GL_VERSION_" << glVerMajor << "_" << glVerMinor;
  if(!glewIsSupported(vers.str().c_str())) {
    std::cerr << "OpenGL " << glVerMajor << "." << glVerMinor << " or higher not available" << std::endl;
    return false;
  }
#endif

  // enable debug if requested
  bool debugEnabled = false;
  if(debugLevel > 0 && glDebugMessageCallback != NULL) {
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)debugCallback, &std::cerr);
    debugEnabled = true;
    if(glDebugMessageControl) {
      // enable various severity levels according to debugLevel
      const int nSeverities = 3;
      GLenum severities[nSeverities] = { GL_DEBUG_SEVERITY_HIGH, GL_DEBUG_SEVERITY_MEDIUM, GL_DEBUG_SEVERITY_LOW };
      for(int i = 0; i < nSeverities; ++i) {
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
        glDebugMessageControl(GL_DEBUG_SOURCE_WINDOW_SYSTEM, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
        glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
        glDebugMessageControl(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
        glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
        glDebugMessageControl(GL_DEBUG_SOURCE_OTHER, GL_DONT_CARE, severities[i], 0, NULL, debugLevel > i ? GL_TRUE : GL_FALSE);
      }
    }
  }

  // initialize DevIL
  //ilInit();

  std::cout << "pgr: initialized, using OpenGL " << glVerMajor << "." << glVerMinor
      << " debug " << (debugEnabled ? "enabled" : "disabled") << std::endl;
  return true;
}

std::string frameworkRoot() {
  char * root = getenv("PGR_FRAMEWORK_ROOT");
  return root ? root : "";
}

void dieWithError(const std::string & errMsg) {
#if defined(_WIN32) || defined(WIN32)
  MessageBoxA(NULL, errMsg.c_str(), "Fatal error", MB_ICONERROR | MB_OK);
#else
  // will we show some nice graphic error messages on X11/Masox?
  std::cerr << "Fatal error: " << errMsg << std::endl;
#endif
  exit(1);
}

void checkGLError(const char *where, int line) {
  GLenum err = glGetError();
  if(err == GL_NONE)
    return;

  std::string errString = "<unknown>";
  switch(err) {
    case GL_INVALID_ENUM:
      errString = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      errString = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      errString = "GL_INVALID_OPERATION";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      errString = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      errString = "GL_OUT_OF_MEMORY";
      break;
    default:;
  }
  if(where == 0 || *where == 0)
    std::cerr << "GL error occurred: " << errString << std::endl;
  else
    std::cerr << "GL error occurred in " << where << ":" << line << ": " << errString << std::endl;
}

} // end namespace pgr
