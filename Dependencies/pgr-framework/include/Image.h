/**
 * \file Image.h
 * \author Tomas Barak, Petr Felkel
 * \brief Helper functions for image loading
 * \date 2011-2013
 */

#ifndef _IMAGE_H
#define _IMAGE_H

#include <string>

#include "pgr.h"

namespace pgr {

/** Simple texture loading helper
 *
 * The function creates 1 2D texture (calling glGenTextures), loads its image using pgr::loadTexImage2D,
 * sets filtering (calling glTexParameteri) and also creates mipmaps if required (using glGenerateMipmap).
 *
 * The texture will be created with linear filtering and mip-mapping enabled by default.
 * You can disable mip-mapping and mipmap generation setting the mipmap parameter to false.
 *
 * \param fileName the texture file name
 * \param mipmap Enable and generate mipmaps for this texture
 * \return texture usable in OpenGL or 0 on failure
 */
GLuint createTexture(const std::string &fileName, bool mipmap = true);

/** Load texture from file and uplad texels to opengl using glTexImage2D
 *
 * The functions will load texels from fileName and call glTexImage2D,
 * so the data will replace image of the currently texture, currently bound to target.
 *
 * \note The DevIL library is used internally. Make sure ilInit() is called before this function (for example in pgr::initialize()).
 * \note The function has to be called after successful creation of the OpenGL context.
 * \warning Make sure ilInit() was called prior to this function (for example in pgr::initialize())
 * \param fileName file to open and load as a texture
 * \param target which texture target will be used as a parameter of glTexImage2D (GL_TEXTURE_2D for example)
 * \return true if texture was successfully loaded from fileName
 */
bool loadTexImage2D(const std::string & fileName, GLenum target);

} // end namespace pgr

#endif
