#pragma once
//----------------
/**
 * \file Renderer.h
 * \author Daniel Gruncl
 * \brief Wrapper of GameObject::draw(glm::mat4)
 * Allows to render it two passes - opaque, transparent.
 * Adds functionality of drawing m_stencil and drawing GL_LINES/GL_TRIANGLES
 */
//---------------
#include "../Component.h"

class Renderer : public Component
{
public:
	Renderer(unsigned int flags = 0);
	// void start();
	void               render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return Renderer::s_type; };
	const char*        getComponentType() { return Renderer::s_type; };

	bool                      m_isTransparent = false;
	bool                      m_useStencil    = false;
	bool                      m_drawLines     = false;
	unsigned char             m_stencil       = 0;
	static const unsigned int IS_TRANSPARENT = 1 << 0, USE_STENCIL = 1 << 1, DRAW_LINES = 1 << 2;
	// static Component*stencilRef[256];
	// static int registerStencil();
private:
	static const char* s_type;
};