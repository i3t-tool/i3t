#pragma once
//----------------
/**
 * \file RenderTexture.h
 * \author Daniel Gruncl
 * \brief wrapper aroung GL framebuffers and textures.
 * Camera2 can render into RenderTexture.
 */
//---------------
#include "pgr.h"
class RenderTexture
{
public:
  RenderTexture(GLuint *colorAttachment, int w, int h);
  GLuint getColor();
  GLuint getFbo();
  int getWidth();
  int getHeight();
private:
  int width,height;
  GLuint colorAttachment;
  GLuint depthStencilRbo;
  GLuint fbo;
};
