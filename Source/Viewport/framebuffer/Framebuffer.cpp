#include "Framebuffer.h"

#include "Logger/Logger.h"

using namespace Vp;

#define FB_DEBUG false
#define FB_DEBUG_VERBOSE false

/*
 * IMPLEMENTATION NOTES, LIMITATIONS AND TODOS:
 * - Multisampled depth buffer can be resolved with multisampleResolveDepth()
 * - ColorAttachments ALWAYS use glTexImage2D
 * - DepthAttachment ALWAYS uses a renderbuffer
 * - Framebuffer manages its color attachments lifetime, they cannot be shared currently
 *  -- Framebuffer dispose disposes of its color attachments
 *  -- Attachments are also stored with values not pointers, so to share them they should be standalone objects
 *  -- However that might be tricky when mutlisample resolving comes into play, not sure
 * - glDrawBuffers is called for all color attachements, eg. it is expected you are using all of them
 */

Framebuffer* Framebuffer::createDefault(bool multisample, unsigned int samples, bool alpha)
{
	return createDefault(FBO_DEFAULT_WIDTH, FBO_DEFAULT_HEIGHT, multisample, samples, alpha);
}

Framebuffer* Framebuffer::createDefault(int width, int height, bool multisample, unsigned int samples, bool alpha)
{
	Framebuffer* framebuffer = new Framebuffer(width, height, multisample, samples);
	GLint format = (alpha ? GL_RGBA : GL_RGB);

	// Create default color attachment
	framebuffer->addColorAttachment(
	    ColorAttachment(format, format, framebuffer->m_width, framebuffer->m_height, GL_UNSIGNED_BYTE));
	// Create default depth attachment
	framebuffer->setDepthAttachment(
	    DepthAttachment(GL_DEPTH24_STENCIL8, true, framebuffer->m_width, framebuffer->m_height));

	return framebuffer;
}

Framebuffer::Framebuffer() : Framebuffer(m_multisample, m_samples)
{
	// Empty
}

Framebuffer::Framebuffer(bool multisample, unsigned int samples) : Framebuffer(m_width, m_height, multisample, samples)
{
	// Empty
}

Framebuffer::Framebuffer(int width, int height) : Framebuffer(width, height, m_multisample, m_samples)
{
	// Empty
}

Framebuffer::Framebuffer(int width, int height, bool multisample, unsigned int samples)
    : m_width(width), m_height(height), m_multisample(multisample), m_samples(samples)
{
	// Empty
}

Framebuffer::~Framebuffer() { dispose(); }

void Framebuffer::update(int width, int height)
{
	if (!isInitialized())
	{
		initImpl(width, height);
	}
	resize(width, height);
}

void Framebuffer::dispose()
{
	if (FB_DEBUG)
		LOG_INFO("[FRAMEBUFFER DEBUG] Disposing {}FBO ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);
	for (auto& colorAttachment : m_colorAttachments)
	{
		colorAttachment.dispose();
	}
	if (m_depthAttachment)
	{
		m_depthAttachment->dispose();
	}
	if (m_fbo)
	{
		glDeleteFramebuffers(1, &(m_fbo));
		m_fbo = 0;
	}
	if (m_multisampleResolveFBO)
	{
		m_multisampleResolveFBO.reset(); // Triggers its destructor
	}
}

void Framebuffer::recreate()
{
	if (FB_DEBUG)
		LOG_INFO("[FRAMEBUFFER DEBUG] Recreating {}FBO ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);

	if (isInitialized())
	{
		// Free up existing OpenGL resources and destroy fbo
		dispose();
	}

	// (Re)initialize framebuffer
	update(m_width, m_height);
}

void Framebuffer::initImpl(int width, int height)
{
	if (isInitialized())
	{
		LOG_ERROR("Framebuffer: Called init() on framebuffer that is already initialized!");
		return;
	}

	m_width = width;
	m_height = height;

	if (m_multisample)
	{
		m_multisampleResolveFBO = std::make_shared<Framebuffer>(width, height, false, 0);
	}

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	for (int i = 0; i < m_colorAttachments.size(); i++)
	{
		// Init color attachment
		ColorAttachment& attachment = m_colorAttachments[i];
		if (attachment.m_syncSize)
		{
			attachment.m_width = m_width;
			attachment.m_height = m_height;
		}
		attachment.m_multisampled = m_multisample;
		attachment.m_samples = m_samples;

		attachment.create();
		attachment.bind();

		// Init resolve color attachment
		if (attachment.m_multisampled)
		{
			ColorAttachment resolveAttachment = ColorAttachment(attachment); // Explicit copy to be sure
			m_multisampleResolveFBO->addColorAttachment(resolveAttachment);
		}
	}
	setDrawBuffers();

	if (m_depthAttachment)
	{
		if (m_depthAttachment->m_syncSize)
		{
			m_depthAttachment->m_width = m_width;
			m_depthAttachment->m_height = m_height;
		}

		m_depthAttachment->m_multisampled = m_multisample;
		m_depthAttachment->m_samples = m_samples;

		m_depthAttachment->create();
		m_depthAttachment->bind();

		// Init resolve depth/stencil attachment
		if (m_depthAttachment->m_multisampled)
		{
			DepthAttachment resolveAttachment = DepthAttachment(m_depthAttachment.value()); // Explicit copy to be sure
			m_multisampleResolveFBO->setDepthAttachment(resolveAttachment);
		}
	}

	if (!checkFramebuffer())
	{
		throw std::runtime_error("Framebuffer: Bad status on init!");
	}

	if (FB_DEBUG)
		LOG_INFO("[FRAMEBUFFER DEBUG] Created {}FBO ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);
}

void Framebuffer::start() { return start(m_width, m_height); }

void Framebuffer::start(int width, int height)
{
	update(width, height);
	bind();
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::end(bool resolveMultisample)
{
	if (m_multisample && resolveMultisample && !m_colorAttachments.empty())
	{
		// Resolve current multisampled color buffers into a single sampled intermediate ones
		multisampleResolveColor(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Redundant if resolving multisampling but better safe than sorry
}

void Framebuffer::resize(int width, int height)
{
	if (!isInitialized())
	{
		throw std::runtime_error("Framebuffer: Cannot resize uninitialized framebuffer!");
	}

	assert(width > 0);
	assert(height > 0);

	if (width == m_width && height == m_height)
	{
		return;
	}

	if (FB_DEBUG)
		LOG_INFO("[FRAMEBUFFER DEBUG] Resizing {}FBO ({} : {}) -> ({} : {})", (m_multisample ? "AA " : ""), m_width,
		         m_height, width, height);

	m_width = width;
	m_height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	for (auto& colorAttachment : m_colorAttachments)
	{
		if (colorAttachment.m_syncSize)
		{
			colorAttachment.resize(width, height);
		}
	}

	if (m_depthAttachment)
	{
		m_depthAttachment->resize(width, height);
	}

	if (!checkFramebuffer())
	{
		throw std::runtime_error("Framebuffer: Bad status after resizing!");
	}

	// TODO: (DR) Idea: Create framebuffer flag that enables/disables automatic resizing, this flag can be put on
	//	a cooldown that will cause it to be false for some time saving resources during resize but eventually
	//	making sure the framebuffer regains its exact size after resizing operation is done

	// TODO: (DR) Restore resize strategy, is it needed? Profile?
	// 	When real size != rendered size artefacts appear:
	//	Like gaps in lines that even AA won't fix, blinking and disappearing
	// 	lines. Doesn't really affect solid models.

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

void Framebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }

bool Framebuffer::checkFramebuffer()
{
	bool result = false;
	GLenum g = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (g)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		LOG_ERROR("FBO undefined");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		LOG_ERROR("FBO incomplete attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOG_ERROR("FBO missing attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		LOG_ERROR("FBO incomplete draw buffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		LOG_ERROR("FBO incomplete read buffer");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		LOG_ERROR("FBO unsupported");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		LOG_ERROR("FBO incomplete multisample");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		LOG_ERROR("FBO incomplete layer targets");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		result = true;
		break;
	default:
		LOG_ERROR("FBO unknown framebuffer status");
		printf("--%d\n", g);
		break;
	}

	return result;
}

bool Framebuffer::isInitialized() const { return m_fbo != 0; }

std::weak_ptr<Framebuffer> Framebuffer::getResolvedFramebuffer()
{
	if (m_multisample)
	{
		if (!m_multisampleResolveFBO->isInitialized())
		{
			throw std::runtime_error("Framebuffer: getResolvedFramebuffer(): Multisampled buffer has not been resolved yet!");
		}
		return m_multisampleResolveFBO;
	}
	else
	{
		return shared_from_this();
	}
}

GLuint Framebuffer::getColorTexture(unsigned int index, bool multisampled) const
{
	if (m_colorAttachments.empty())
	{
		throw std::runtime_error("Framebuffer: getColorTexture(): Framebuffer has no color attachments!");
	}
	if (m_multisample && !multisampled)
	{
		if (!m_multisampleResolveFBO->isInitialized())
		{
			throw std::runtime_error("Framebuffer: getColorTexture(): Multisampled buffer has not been resolved yet!");
		}
		return m_multisampleResolveFBO->getColorTexture(index);
	}
	else
	{
		return m_colorAttachments[index].m_texture;
	}
}

GLuint Framebuffer::getId() const { return m_fbo; }

void Framebuffer::setMultisampled(bool multisample, unsigned int samples)
{
	// We need to recreate all textures and renderbuffers with same settings, but with different sample values
	bool recreateNeeded = false;

	if (this->m_multisample != multisample)
	{
		this->m_multisample = multisample;
		recreateNeeded = true;
	}
	if (this->m_samples != samples)
	{
		this->m_samples = samples;
		recreateNeeded = true;
	}

	if (recreateNeeded)
	{
		if (FB_DEBUG)
			LOG_INFO("[FRAMEBUFFER DEBUG] FBO recreate needed after multisampling change!");
		recreate();
	}
}

bool Framebuffer::isMultisampled() { return m_multisample; }

unsigned int Framebuffer::getSampleCount() const { return m_samples; }

void Framebuffer::multisampleResolveColors()
{
	for (int i = 0; i < m_colorAttachments.size(); i++)
	{
		multisampleResolveColor(i);
	}
	if (m_colorAttachments.empty())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind FBO just to stay consistent
	}
}

void Framebuffer::multisampleResolveColor(unsigned int colorAttachmentIndex)
{
	// TODO: (DR) Add attachment index error checking
	if (!m_multisample)
	{
		throw std::runtime_error("Framebuffer: Cannot resolve single sampled color FBO!");
	}

	m_multisampleResolveFBO->update(m_width, m_height);

	// Resolve multisampled buffer into a single sampled intermediate one
	if (FB_DEBUG && FB_DEBUG_VERBOSE)
		LOG_INFO("[FRAMEBUFFER DEBUG] Resolving {}FBO color ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, getId());
	glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_multisampleResolveFBO->getId());
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::multisampleResolveDepth()
{
	if (!m_multisample)
	{
		throw std::runtime_error("Framebuffer: Cannot resolve single sampled depth FBO!");
	}

	m_multisampleResolveFBO->update(m_width, m_height);

	// Resolve multisampled buffer into a single sampled intermediate one
	if (FB_DEBUG && FB_DEBUG_VERBOSE)
		LOG_INFO("[FRAMEBUFFER DEBUG] Resolving {}FBO depth ({} : {})", (m_multisample ? "AA " : ""), m_width, m_height);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, getId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_multisampleResolveFBO->getId());
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
	                  GL_DEPTH_BUFFER_BIT | (m_depthAttachment->m_stencil ? GL_STENCIL_BUFFER_BIT : 0x00), GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::removeAllColorAttachments()
{
	for (auto& colorAttachment : m_colorAttachments)
	{
		colorAttachment.dispose();
	}
	m_colorAttachments.clear();
}

void Framebuffer::addColorAttachment(ColorAttachment colorAttachment)
{
	colorAttachment.m_index = m_colorAttachments.size();
	// All FBO attachments must have the same sample count
	colorAttachment.m_multisampled = m_multisample;
	colorAttachment.m_samples = m_samples;
	m_colorAttachments.push_back(colorAttachment);
}

ColorAttachment& Framebuffer::getColorAttachment(unsigned int index)
{
	if (index < 0 || index >= m_colorAttachments.size())
	{
		throw std::runtime_error("Framebuffer: Color attachment index out of bounds");
	}
	return m_colorAttachments[index];
}

void Framebuffer::setDepthAttachment(DepthAttachment depthAttachment)
{
	// All FBO attachments must have the same sample count
	depthAttachment.m_multisampled = m_multisample;
	depthAttachment.m_samples = m_samples;
	m_depthAttachment = depthAttachment;
}

std::optional<DepthAttachment>& Framebuffer::getDepthAttachment() { return m_depthAttachment; }

void Framebuffer::removeDepthAttachment()
{
	if (m_depthAttachment)
	{
		m_depthAttachment->dispose();
	}
	m_depthAttachment.reset();
}

void Framebuffer::setDrawBuffers()
{
	std::vector<GLenum> colorBuffers;
	for (int i = 0; i < m_colorAttachments.size(); i++)
	{
		colorBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	if (!colorBuffers.empty())
	{
		glDrawBuffers(colorBuffers.size(), &colorBuffers[0]);
	}
}

void Framebuffer::setDrawBuffers(std::vector<unsigned int> indices)
{
	if (indices.empty() || indices.size() > m_colorAttachments.size())
	{
		throw std::invalid_argument("Framebuffer: setDrawBuffers(): Invalid indices array!");
	}
	std::vector<GLenum> colorBuffers;
	for (int i = 0; i < m_colorAttachments.size(); i++)
	{
		colorBuffers.push_back(GL_NONE);
	}
	for (unsigned int index : indices)
	{
		colorBuffers[index] = GL_COLOR_ATTACHMENT0 + index;
	}
	if (!colorBuffers.empty())
	{
		glDrawBuffers(colorBuffers.size(), &colorBuffers[0]);
	}
}

int Framebuffer::getWidth() const { return m_width; }
int Framebuffer::getHeight() const { return m_height; }
