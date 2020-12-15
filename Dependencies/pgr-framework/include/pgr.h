/**
 * \file pgr.h
 * \author Tomas Barak, Petr Felkel
 * \brief this includes all pgr headers + utility functions
 * \date 2011-2012
 */

/**
 * \namespace pgr
 * \author Tomas Barak, Petr Felkel, Vlastimil Havran
 * \date 2011-2012
 *
 * \brief Contains all framework functions/classes.
 *
 * The framework can help you with certain tedious tasks of OpenGL programmer's life.
 * For example, you can use the createTexture() function to load and create a texture in one line.
 * The CHECK_GL_ERROR() macro is a simple but powerful weapon if you deal with unexpected OpenGL behaviour.
 * Please read the reference to particular routines to find more info.
 */

#ifndef PGR_H
#define PGR_H

/// how do we include opengl, do we use glloadgen or glew?
#define USE_GLLOADGEN 1

#if USE_GLLOADGEN
#include "gl_core_4_4.h"
#else
#include <GL/glew.h>
#endif
//#include <GL/freeglut.h>

#include <string>
#include <math.h>

#include <glm/glm.hpp>                  // vec3 normalize cross
#include <glm/gtc/type_ptr.hpp>         // value_ptr
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Shader.h"  // createShaderFromSource, createProgram, deleteProgramAndShaders
#include "Image.h" // createTexture
#include "MeshData.h" // some built-in meshes (cube, teapot, ...)
#include "gldebug.h" // debugging helpers, opengl error catching

#ifndef M_PI
/// define Pi for compatibility issues (MSVC vs GCC)
#define M_PI 3.14159f
#endif

#ifndef DEGTORAD
/// degrees-to-radians conversion macro
#define DEGTORAD(a) ((a) * M_PI / 180.0)
#endif
#ifndef RADTODEG
/// radians-to-degrees conversion macro
#define RADTODEG(a) ((a) * 180.0 / M_PI)
#endif

namespace pgr {

/// defines verbosity of the debug system, DEBUG_OFF turns the system off
enum DebugLevel {
  DEBUG_OFF = 0, ///< turn of the debug callbacks
  DEBUG_LOW, ///< enables messages with hight severity
  DEBUG_MEDIUM, ///< enables messages with hight and medium severities,
  DEBUG_HIGH ///< enables messages with all severities
};

/** required opengl version - major number
 *
 * Use this constant as a first argument of glutInitContextVersion and pgr::initialize.
 */
const int OGL_VER_MAJOR = 3;
/** required opengl version - minor number
 *
 * Use this constant as a second argument of glutInitContextVersion and pgr::initialize.
 */
const int OGL_VER_MINOR = 1;

/** intializes gl functions, DevIL and checks opengl version
 *
 * Call this function after successful creation of OpenGL context (for example after glutCreateWindow).
 * \returns true if everything went ok, especially if opengl version is sufficient; false indicates error - you should exit the application
 * \param glVerMajor opengl major version to check (returns false if not available)
 * \param glVerMinor opengl minor version to check (returns false if not available)
 * \param debugLevel if not DEBUG_OFF initialize opengl debug callback (glDebugMessageCallback), works only with debug contexts!
 */
//bool initialize(int glVerMajor, int glVerMinor, DebugLevel debugLevel = DEBUG_MEDIUM);
bool initialize(int glVerMajor, int glVerMinor, DebugLevel debugLevel = DEBUG_OFF);

/** returns the value of the PGR_FRAMEWORK_ROOT system variable
 *
 * If the variable is not set, the function returns empty string.
 */
std::string frameworkRoot();

/** prints a message and exits the application
 *
 * Use this if you detect an error from which you dont want to recover.
 * The functions shows the errMsg string to the user
 * and terminates the application with an error return code.
 *
 * \return this function does not return
 */
void dieWithError(const std::string & errMsg);

/** prints out pending opengl error
 *
 * If there is nothing to print (no opengl error has occurred), the function prints nothing.
 * You can use where and line arguments to supply addtional information that will be printed out.
 *
 * \see CHECK_GL_ERROR
 */
void checkGLError(const char * where = 0, int line = 0);

/** helper macro involving the checkGLError function
 *
 * The macro uses __FUNCTION__ and __LINE__ as arguments to the checkGLError function.
 * Use it anywhere you like to catch opengl errors.
 */
#define CHECK_GL_ERROR() do { pgr::checkGLError(__FUNCTION__, __LINE__); } while(0)

} // end namespace pgr

#endif
