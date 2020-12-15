/**
 * \file gldebug.h
 * \author Tomas Barak
 * \brief Helper functions for opengl debugging, tightly connected to the opengl debug callback mechanism.
 * \date 2012-2013
 */

#ifndef GLDEBUG_H
#define GLDEBUG_H

#include <ostream>

#include "pgr.h"

namespace pgr {
/// helper functions for better opengl error handling
namespace gldebug {

/// returns human readable representation of debug source src
const char * sourceToString(GLenum src);
/// returns human readable representation of debug type
const char * typeToString(GLenum type);
/// returns human readable representation of debug severity
const char * severityToString(GLenum severity);

/** tries to print as nice as possible backtrace of the current call stack
 *
 * \warning The function will chop ou parts of the stack. For example the function itself will not appear in the backtrace.
 * \warning The format or verbosity may be platform dependent.
 * \param os prints to std::cout by default
 */
void printBacktrace(std::ostream * os = 0);

} // end namespace gldebug
} // end namespace pgr

#endif // GLDEBUG_H
