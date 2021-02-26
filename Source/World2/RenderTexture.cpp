#include "RenderTexture.h"

RenderTexture::RenderTexture(GLuint* colorAttachment, int w, int h){
  printf("w,h %d,%d\n",w,h);w=256;h=256;
  // Framebuffer is used in Viewport window.
  // generate a framebuffer for display function
  glGenFramebuffers(1, &this->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);


  // generate texture to draw on
  glGenTextures(1, colorAttachment);

  // create a renderbuffer to allow depth and stencil
  glGenRenderbuffers(1, &this->depthStencilRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRbo);
  // glPixelStorei(GL_PACK_ALIGNMENT, 1);
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_MULTISAMPLE);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  // glClearStencil(255);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);CHECK_GL_ERROR();






glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glBindTexture(GL_TEXTURE_2D, *colorAttachment);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);CHECK_GL_ERROR();
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *colorAttachment, 0);CHECK_GL_ERROR();

  // resize renderbuffer
  glBindRenderbuffer(GL_RENDERBUFFER, this->depthStencilRbo);CHECK_GL_ERROR();
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w + 16, h + 16);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  // attach it to currently bound framebuffer object
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depthStencilRbo);CHECK_GL_ERROR();





  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
  GLenum g=glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (g){
	case GL_FRAMEBUFFER_UNDEFINED:						printf("undefined\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			printf("incomplete attachment\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	printf("missing attachment\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			printf("incomplete draw buffer\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			printf("incomplete read buffer\n");break;
	case GL_FRAMEBUFFER_UNSUPPORTED:					printf("unsupported\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			printf("incomplete multisample\n");break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		printf("incomplete layer targets\n");break;
	case GL_FRAMEBUFFER_COMPLETE:						printf("complete\n");break;
	default:printf("--%d\n",g);break;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);CHECK_GL_ERROR();

  this->colorAttachment = *colorAttachment;
  this->width = w;
  this->height = h;
}
GLuint RenderTexture::getColor(){return this->colorAttachment;}
GLuint RenderTexture::getFbo(){return this->fbo;}
int RenderTexture::getHeight(){return this->height;}
int RenderTexture::getWidth(){return this->width;}