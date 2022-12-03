#include "Framebuffer.h"

#include "Logger/Logger.h"

using namespace Vp;

Framebuffer::Framebuffer(int width, int height) : Framebuffer(width, height, false)
{
	// Empty
}

Framebuffer::Framebuffer(int width, int height, bool multisample) : Framebuffer(width, height, multisample, false)
{
	// Empty
}

Framebuffer::Framebuffer(int width, int height, bool multisample, bool alpha)
    : m_width(width), m_height(height), m_multisample(multisample), m_alpha(alpha)
{
	// Empty
}

Framebuffer::~Framebuffer()
{
	if (m_colorTexture)
		glDeleteTextures(1, &m_colorTexture);
	if (m_depthStencilRbo)
		glDeleteRenderbuffers(1, &m_depthStencilRbo);
	if (m_fbo)
		glDeleteFramebuffers(1, &(m_fbo));
}

void Framebuffer::init(int width, int height)
{
	if (isInitialized())
	{
		Log::error("Called init() on framebuffer that is already initialized!");
	}

	m_width = width;
	m_height = height;

	if (m_multisample)
	{
		m_multisampleResolveFBO = std::make_unique<Framebuffer>(width, height, false, m_alpha);
		m_multisampleResolveFBO->init(width, height);
	}

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_colorTexture);
	createColorTexture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	                       (m_multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), m_colorTexture, 0);

	glGenRenderbuffers(1, &m_depthStencilRbo);
	createDepthStencilRenderBuffer(width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (!checkFramebuffer())
	{
		Log::error("Bad FBO status on init!");
	}

	Log::debug("Created {}FBO ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);
}

void Framebuffer::start() { return start(m_width, m_height); }

void Framebuffer::start(int width, int height)
{
	if (!isInitialized())
	{
		init(width, height);
	}
	resize(width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::end()
{
	if (m_multisample)
	{
		// Resolve current multisampled buffer into a single sampled intermediate
		// one
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_multisampleResolveFBO->m_fbo);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(int width, int height)
{
	assert(width > 0);
	assert(width > 0);

	if (m_multisample)
	{
		m_multisampleResolveFBO->resize(width, height);
	}

	if (width != m_width || height != m_height)
	{
		Log::debug("Resizing {}FBO ({} : {}) -> ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height, width, height);

		m_width = width;
		m_height = height;

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		createColorTexture(width, height);
		createDepthStencilRenderBuffer(width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (!checkFramebuffer())
		{
			Log::error("Bad FBO status after resizing!");
		}

		// TODO: (DR) Restore resize strategy, is it needed? Profile?
		// 	When real size != rendered size artefacts appear:
		//	Like gaps in lines that even AA won't fix, blinking and disappearing
		// lines. Doesn't really affect solid models.
		/*const float MIN_RATE = 0.5f;
		const float MAX_RATE = 1.2f;

		float rateX = static_cast<float>(newWidth) / static_cast<float>(m_width);
		float rateY = static_cast<float>(newHeight) / static_cast<float>(m_height);
		// printf("rateX = %5.3f, rateY = %5.3f\n", rateX, rateY);

		// TODO PF - find the best texture resize strategy
		// if( rateX < MIN_RATE || rateX > MAX_RATE || rateY < MIN_RATE || rateY >
		// MAX_RATE)   // both directions
		if (rateX > MAX_RATE || rateY > MAX_RATE) // only enlarge
		{
		  printf("(%5.3i, %5.3i) -> (%5.3i, %5.3i)\n", m_width, m_height,
		newWidth,newHeight);

		  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		  createColorTexture();
		  createDepthStencilRenderBuffer();
		  glBindRenderbuffer(GL_RENDERBUFFER, 0);

		  // checkFramebuffer();

		  m_width = newWidth;
		  m_height = newHeight;
		}*/
	}
}

bool Framebuffer::checkFramebuffer()
{
	bool result = false;
	GLenum g = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (g)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		Log::error("FBO undefined");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		Log::error("FBO incomplete attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		Log::error("FBO missing attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		Log::error("FBO incomplete draw buffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		Log::error("FBO incomplete read buffer");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		Log::error("FBO unsupported");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		Log::error("FBO incomplete multisample");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		Log::error("FBO incomplete layer targets");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		result = true;
		break;
	default:
		Log::error("FBO unknown framebuffer status");
		printf("--%d\n", g);
		break;
	}
	CHECK_GL_ERROR();

	return result;
}

bool Framebuffer::isInitialized() const { return m_fbo != 0; }

GLuint Framebuffer::getColorTexture() const
{
	if (m_multisample)
	{
		return m_multisampleResolveFBO->m_colorTexture;
	}
	else
	{
		return m_colorTexture;
	}
}

GLuint Framebuffer::getId() const { return m_fbo; }
void Framebuffer::createColorTexture(int width, int height)
{
	if (m_multisample)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, (m_alpha ? GL_RGBA : GL_RGB), m_width, m_height,
		                        GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, (m_alpha ? GL_RGBA : GL_RGB), m_width, m_height, 0, (m_alpha ? GL_RGBA : GL_RGB),
		             GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void Framebuffer::createDepthStencilRenderBuffer(int width, int height)
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRbo);
	if (m_multisample)
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
