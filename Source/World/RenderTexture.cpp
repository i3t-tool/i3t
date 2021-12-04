#include "RenderTexture.h"


// TODO PF - This object is used for fiven size of the texture. Create a resize function for screen!
RenderTexture::RenderTexture(GLuint* colorAttachment, int w, int h) 
{ 
	m_width = w;
	m_height = h;
	
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_colorTexture);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

	glGenRenderbuffers(1, &m_depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// attach it to currently bound framebuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	GLenum g=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	printf("FBO ");
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_GL_ERROR();

	// return value
	*colorAttachment = m_colorTexture;  // pass the texture to ImGUI for preview
}

RenderTexture::~RenderTexture()
{
	if(m_colorTexture) 
		glDeleteTextures(1, &m_colorTexture);
	if(m_depthStencilRbo) 
		glDeleteRenderbuffers(1, &m_depthStencilRbo);
	if(m_fbo) 
		glDeleteFramebuffers(1, &(m_fbo));
}

GLuint RenderTexture::getColor(){return m_colorTexture;}
GLuint RenderTexture::getFbo()  {return m_fbo;}
int RenderTexture::getHeight()  {return m_height;}
int RenderTexture::getWidth()   {return m_width;}