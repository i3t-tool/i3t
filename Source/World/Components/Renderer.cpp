#include "Renderer.h"
#include <iostream>
#include <typeinfo>
#include "../Select.h"

const char* Renderer::s_type = NULL;

Renderer::Renderer(unsigned int flags)
{
	Renderer::s_type = typeid(Renderer).name();
	this->m_type     = Renderer::s_type;

	this->m_isTransparent = (flags & Renderer::IS_TRANSPARENT) != 0;
	this->m_useStencil    = (flags & Renderer::USE_STENCIL) != 0;
	this->m_drawLines     = (flags & Renderer::DRAW_LINES) != 0;
	this->m_stencil       = 0;

	if (this->m_useStencil) { this->m_stencil = Select::registerStencil(); }
	// printf("is transparent %d, mask %d\n",this->m_isTransparent,);
}

// void Renderer::start(){}

void Renderer::render(glm::mat4* parent, bool renderTransparent)
{
	if (this->m_isTransparent == renderTransparent)
	{
		glStencilMask(255 * this->m_useStencil);
		glStencilFunc(GL_ALWAYS, this->m_stencil, 255);
		int primbkp = m_gameObject->primitive;
		if (this->m_drawLines)
		{
			m_gameObject->primitive = GL_LINES;
			m_gameObject->draw(*parent);
		}
		else
		{
			m_gameObject->primitive = GL_TRIANGLES;
			m_gameObject->draw(*parent);
		}
		m_gameObject->primitive = primbkp;
		glStencilMask(0);
	}
}
