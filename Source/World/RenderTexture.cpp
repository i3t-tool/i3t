#include "RenderTexture.h"

RenderTexture::RenderTexture(GLuint* colorAttachment, int w, int h){
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


	glGenTextures(1, colorAttachment);
	glBindTexture(GL_TEXTURE_2D, *colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *colorAttachment, 0);

	glGenRenderbuffers(1, &this->depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->depthStencilRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach it to currently bound framebuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/*glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	GLenum g=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (g){
		case GL_FRAMEBUFFER_UNDEFINED:									printf("undefined\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			printf("incomplete attachment\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	printf("missing attachment\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			printf("incomplete draw buffer\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			printf("incomplete read buffer\n");break;
		case GL_FRAMEBUFFER_UNSUPPORTED:								printf("unsupported\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			printf("incomplete multisample\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		printf("incomplete layer targets\n");break;
		case GL_FRAMEBUFFER_COMPLETE:										printf("complete\n");break;
		default:printf("--%d\n",g);break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);CHECK_GL_ERROR();*/

	this->colorAttachment = *colorAttachment;
	this->width = w;
	this->height = h;
}
GLuint RenderTexture::getColor(){return this->colorAttachment;}
GLuint RenderTexture::getFbo(){return this->fbo;}
int RenderTexture::getHeight(){return this->height;}
int RenderTexture::getWidth(){return this->width;}