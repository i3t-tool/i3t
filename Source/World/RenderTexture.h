#pragma once
//----------------
/**
 * \file RenderTexture.h
 * \author Daniel Gruncl, Petr Felkel (refactoring) 
 * \brief wrapper aroung GL framebuffers and textures.
 * FBO with a color texture and a depth buffer. Camera can render into RenderTexture.
 */
//---------------
#include "pgr.h"
class RenderTexture
{
public:
  RenderTexture(GLuint *colorAttachment, int w, int h);
	~RenderTexture();
  GLuint getColor();   //todo PF- rename to getColorTexture()
  GLuint getFbo();
  int getWidth();
  int getHeight();
	
private:
  int m_width;              // rendered texture width
  int m_height;             // rendered texture height
  GLuint m_colorTexture;    // texture attached to GL_COLOR_ATTACHMENT0
  GLuint m_depthStencilRbo; // depthStencil renderbuffer
  GLuint m_fbo;             // the created framebufferobject
};
