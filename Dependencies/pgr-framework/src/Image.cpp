#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "Image.h"
#include "pgr.h"

namespace pgr {

bool loadTexImage2D(const std::string & fileName, GLenum target) {
  /*
  // DevIL library has to be initialized (ilInit() must be called)

  // DevIL uses mechanism similar to OpenGL, each image has its ID (name)
  ILuint img_id;
  ilGenImages(1, &img_id); // generate one image ID (name)
  ilBindImage(img_id); // bind that generated id

  // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
  ilEnable(IL_ORIGIN_SET);
  ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

  // this will load image data to the currently bound image
  if(ilLoadImage(fileName.c_str()) == IL_FALSE) {
    ilDeleteImages(1, &img_id);
    std::cerr << __FUNCTION__ << " cannot load image " << fileName << std::endl;
    return false;
  }

  // if the image was correctly loaded, we can obtain some informatins about our image
  ILint width = ilGetInteger(IL_IMAGE_WIDTH);
  ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
  ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
  // there are many possible image formats and data types
  // we will convert all image types to RGB or RGBA format, with one byte per channel
  unsigned Bpp = ((format == IL_RGBA || format == IL_BGRA) ? 4 : 3);
  char * data = new char[width * height * Bpp];
  // this will convert image to RGB or RGBA, one byte per channel and store data to our array
  ilCopyPixels(0, 0, 0, width, height, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);
  // image data has been copied, we dont need the DevIL object anymore
  ilDeleteImages(1, &img_id);

  // bogus ATI drivers may require this call to work with mipmaps
  //glEnable(GL_TEXTURE_2D);

  glTexImage2D(target, 0, Bpp == 4 ? GL_RGBA : GL_RGB, width, height, 0, Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

  // free our data (they were copied to OpenGL)
  delete [] data;
  return true;
   */
  bool result = false;

  stbi_set_flip_vertically_on_load(false);

  int width, height, Bpp;
  unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &Bpp, STBI_rgb_alpha);

  if (data)
  {
    //printf("%s,bpp %d\n",fileName.c_str(),Bpp);
    glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    result = true;
  }
  else
  {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  return result;
}

GLuint createTexture(const std::string &fileName, bool mipmap) {
  // generate and bind one texture
  GLuint tex = 0;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  // set linear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // upload our image data to OpenGL
  if(!loadTexImage2D(fileName, GL_TEXTURE_2D)) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &tex);
    return 0;
  }
  // create mipmaps
  if(mipmap)
    glGenerateMipmap(GL_TEXTURE_2D);
  // unbind the texture (just in case someone will mess up with texture calls later)
  glBindTexture(GL_TEXTURE_2D, 0);
  CHECK_GL_ERROR();
  return tex;
}

} // end namespace pgr
