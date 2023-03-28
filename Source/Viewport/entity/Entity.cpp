#include "Entity.h"

using namespace Vp;

Entity::Entity()
{
	m_backFaceCull = false;
	m_visible = true;
	m_opaque = true;
	m_wboit = false;
	m_wboitFunc = 0;
	m_explicitTransparencyOrder = 0;

	m_selectable = false;
	m_highlight = false;
	m_highlightUseDepth = true;
	m_highlightColor = glm::vec3(0.949, 0.682, 0.18);
}
