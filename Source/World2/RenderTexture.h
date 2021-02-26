#pragma once

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
