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

// TODO: (DR) It has long been a good idea on the backburner to remove the pgr framework dependency entirely because
//  most of the library is unused and the parts of ít that are used might be better off included in the project with
//  a hooked up logger etc. Furthermore matvei then edited the library anyway to use glad instead of glew, meaning
//  it is essentially already custom code and not an external "dependency". More reasons to remove it.
//  Some essential shader loading code was already extracted into GLUtils.h, but that file still depends on pgr.h to
//  bring in the glad/glew file. In the future we should extract these includes into a separate I3TGL.h (or such) file,
//  which would replace "pgr.h" in many parts of the I3T code.

/// how do we include opengl, do we use glad or glew?
#define USE_GLAD 1

#if USE_GLAD
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
// #include <GL/freeglut.h>

#include <math.h>
#include <string>

#include <glm/glm.hpp>                  // vec3 normalize cross
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale
#include <glm/gtc/type_ptr.hpp>         // value_ptr

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Image.h"    // createTexture
#include "MeshData.h" // some built-in meshes (cube, teapot, ...)
#include "Shader.h"   // createShaderFromSource, createProgram, deleteProgramAndShaders
#include "gldebug.h"  // debugging helpers, opengl error catching

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

namespace pgr
{

/// defines verbosity of the debug system, DEBUG_OFF turns the system off
enum DebugLevel
{
	// TODO: (DR) The comments/meaning is the wrong way around I swear, lower log level means MORE messages, not less
	DEBUG_OFF = 0,    ///< turn of the debug callbacks
	DEBUG_HIGH = 1,   ///< enables messages with high severity
	DEBUG_MEDIUM = 2, ///< enables messages with medium and higher severities
	DEBUG_LOW = 3,    ///< enables messages with low and higher severities
	DEBUG_ALL = 4,    ///< enables messages with all severities
};

/** required opengl version - major number
 *
 * Use this constant as a first argument of glutInitContextVersion and pgr::initialize.
 */
const int OGL_VER_MAJOR = 4;
/** required opengl version - minor number
 *
 * Use this constant as a second argument of glutInitContextVersion and pgr::initialize.
 */
const int OGL_VER_MINOR = 3;

/** intializes gl functions, DevIL and checks opengl version
 *
 * Call this function after successful creation of OpenGL context (for example after glutCreateWindow).
 * \returns true if everything went ok, especially if opengl version is sufficient; false indicates error - you should
 * exit the application
 * \param glVerMajor opengl major version to check (returns false if not available)
 * \param glVerMinor opengl minor version to check (returns false if not available)
 * \param debugLevel if not DEBUG_OFF initialize opengl debug callback (glDebugMessageCallback), works only with debug
 * contexts!
 */
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
void dieWithError(const std::string& errMsg);

/** prints out pending opengl error
 *
 * If there is nothing to print (no opengl error has occurred), the function prints nothing.
 * You can use where and line arguments to supply addtional information that will be printed out.
 *
 * \see CHECK_GL_ERROR
 */
void checkGLError(const char* where = 0, int line = 0);

/** helper macro involving the checkGLError function
 *
 * The macro uses __FUNCTION__ and __LINE__ as arguments to the checkGLError function.
 * Use it anywhere you like to catch opengl errors.
 */
#define CHECK_GL_ERROR()                                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		pgr::checkGLError(__FUNCTION__, __LINE__);                                                                     \
	} while (0)

} // end namespace pgr

#endif
