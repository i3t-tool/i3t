#include "FrustumManipulator.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "Camera.h"
#include "ManipulatorUtil.h"

#include "Core/Input/InputManager.h"
#include "Core/Nodes/TransformImpl.h"
#include "imgui.h"
#include "pgr.h"

#include <typeinfo>

const char* FrustumManipulator::s_type = nullptr;

FrustumManipulator::FrustumManipulator()
{
	FrustumManipulator::s_type = typeid(FrustumManipulator).name();
	m_type = FrustumManipulator::s_type;

	for (int i = 0; i < 6; i++)
	{
		m_stencils.arr[i] = ManipulatorUtil::getStencil(i);
	}

	m_hposs[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_hposs[1] = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	m_hposs[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_hposs[3] = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	m_hposs[4] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_hposs[5] = glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);

	m_frustrum = new GameObject(unitcubeMesh, &World::shaderProj, 0);
	m_frustrum->color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	m_frustruml = new GameObject(cubelinesMesh, &World::shaderProj, 0);
	m_frustruml->primitive = GL_LINES;
	m_frustruml->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_cameraico = new GameObject(cameraicoMesh, &World::shaderHandle, 0);
	m_cameraico->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	m_cameraico->color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	m_handle = new GameObject(unitcubeMesh, &World::shaderHandle, 0);
}
void FrustumManipulator::start() {}

void FrustumManipulator::render(glm::mat4* parent, bool renderTransparent)
{
	if (m_editednode == nullptr)
	{
		return;
	}
	glm::mat4 projinv = glm::inverse(m_edited);
	;
	// glm::mat4 transform=(*parent)*m_gameObject->transformation;//TMP
	// glm::mat4 transform=glm::mat4(1.0f);
	glm::mat4 transform = getNodeTransform(&m_editednode, &m_parent, true);
	glm::vec4 pos = transform[3];
	transform = getRotation(transform, 0);
	transform[3] = pos;

	if (renderTransparent)
	{
		glUseProgram(World::shaderProj.program);
		glUniformMatrix4fv(
		    glGetUniformLocation(World::shaderProj.program, "P2Matrix"), 1,
		    GL_FALSE, glm::value_ptr(projinv));
		glDisable(GL_CULL_FACE);
		m_frustruml->draw(transform);
		m_frustrum->draw(transform);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDepthRange(0.0, 0.01);
		glUseProgram(World::shaderHandle.program);

		for (int i = 0; i < 6; i++)
		{
			glm::vec4 pos = projinv * m_hposs[i];
			pos /= pos[3];

			if (m_hposs[i][2] == 0.0f)
			{ // move side handles to middle - between far plane and near plane
				glm::vec4 f = glm::vec4(m_hposs[i][0], m_hposs[i][1], 1.0f, 1.0f);
				glm::vec4 n = glm::vec4(m_hposs[i][0], m_hposs[i][1], -1.0f, 1.0f);
				glm::vec4 pn = (projinv * f) / (projinv * f)[3];
				glm::vec4 pf = (projinv * n) / (projinv * n)[3];

				pos = ((pf - pn) * 0.5f + pn);
			}

			float depth =
			    (World::perspective * World::mainCamera * transform * pos)[2];
			m_handle->transformation = glm::mat4(depth * 0.01f + 0.1f);
			m_handle->transformation[3] = pos;

			ManipulatorUtil::drawHandle(m_handle, transform, m_hposs[i] * m_hposs[i],
			                            m_stencils.arr[i], m_activehandle,
			                            m_hoverhandle); // hposs*hposs=absolute value
		}

		glm::vec4 mov = projinv * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		mov /= mov[3];
		mov[2] += 1.5f;
		mov[2] = 1.5f;

		m_cameraico->transformation[3] = mov;
		m_cameraico->draw(transform);
		glDepthRange(0.0, 1.0);
	}
}
void FrustumManipulator::update()
{
	if (m_editednode == nullptr)
	{
		return;
	}
	auto* editedfrustum =
	    (Core::TransformImpl<ETransformType::Frustum>*)m_editednode.get();
	m_edited = m_editednode->getData().getMat4();

	unsigned char sel = Select::getStencilAt(
	    (int)InputManager::m_mouseX,
	    (int)(World::height - InputManager::m_mouseY), 3, -1);
	m_hoverhandle = -1;

	if (m_activehandle == -1)
	{
		for (int i = 0; i < 6; i++)
		{
			if (sel == m_stencils.arr[i])
			{
				m_hoverhandle = m_stencils.arr[i];
				m_axisnum = i;
			}
		}
	}
	if (InputManager::isKeyJustPressed(Keys::mouseLeft))
	{
		m_activehandle = -1;
		for (int i = 0; i < 6; i++)
		{
			if (sel == m_stencils.arr[i])
			{
				m_activehandle = m_stencils.arr[i];
				m_axisnum = i;
			}
		}
	}
	if (InputManager::isKeyJustUp(Keys::mouseLeft))
	{
		m_activehandle = -1;
	}

	if (m_hoverhandle != -1 || m_activehandle != -1)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	if (m_activehandle == -1)
	{
		return;
	}

	if (InputManager::isKeyJustUp(Keys::esc))
	{
	}
	glm::mat4 projinv = glm::inverse(m_edited);
	glm::vec3 drag3 = glm::vec3(0.0f);
	glm::vec4 axis = m_hposs[m_axisnum];
	glm::vec4 pos = projinv * m_hposs[m_axisnum];
	pos /= pos[3];

	glm::vec4 p = projinv * glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	m_left = p[0];
	p = projinv * glm::vec4(1.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	m_right = p[0];
	p = projinv * glm::vec4(0.0f, 1.0f, -1.0f, 1.0f);
	p /= p[3];
	m_top = p[1];
	p = projinv * glm::vec4(0.0f, -1.0f, -1.0f, 1.0f);
	p /= p[3];
	m_bottom = p[1];
	p = projinv * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	p /= p[3];
	m_near = -p[2];
	p = projinv * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	p /= p[3];
	m_far = -p[2];

	if (m_hposs[m_axisnum][2] == 0.0f)
	{ // move side handles to middle - between far plane and near plane
		glm::vec4 f =
		    glm::vec4(m_hposs[m_axisnum][0], m_hposs[m_axisnum][1], 1.0f, 1.0f);
		glm::vec4 n =
		    glm::vec4(m_hposs[m_axisnum][0], m_hposs[m_axisnum][1], -1.0f, 1.0f);
		glm::vec4 pn = (projinv * f) / (projinv * f)[3];
		glm::vec4 pf = (projinv * n) / (projinv * n)[3];

		pos = ((pf - pn) * 0.5f + pn);
	}

	pos[3] = 0.0f;
	axis[3] = 0.0f;
	// glm::mat4 handlespace=getFullTransform(m_owner);//TMP
	// glm::mat4 handlespace=glm::mat4(1.0f);
	glm::mat4 handlespace = getNodeTransform(&m_editednode, &m_parent, true);

	glm::vec2 spos1 = world2screen((glm::vec3)(
	    handlespace[3] +
	    handlespace * pos)); // position of transformated object on the screen
	glm::vec2 spos2 = world2screen((glm::vec3)(
	    handlespace[3] +
	    handlespace * (pos + axis * axis))); // spos1,spos2 - project two points
	                                         // on screen - project axis on screen
	glm::vec2 dir = spos2 - spos1;           // the axis in screen space
	if (glm::length(dir) < 0.01)
	{
		dir[0] = 1.0f;
	} // axis length must not be zero

	glm::mat2 mov = glm::mat2(dir, glm::vec2(dir[1], -dir[0]));
	mov = glm::inverse(glm::mat2(glm::normalize(mov[0]), glm::normalize(mov[1])));

	glm::vec2 dragfinal =
	    mov *
	    glm::vec2(InputManager::m_mouseXDelta, -InputManager::m_mouseYDelta) *
	    0.05f;

	if (m_hposs[m_axisnum][2] == 0.0f)
	{
		dragfinal *= 0.25f;
	} // decrease sensitivity for side handles

	if (m_activehandle == m_stencils.names.n)
	{
		m_near -= dragfinal[0];
		// if(m_near<0.01f){m_near=0.01f;}
		// if(m_near>m_far-0.5f){m_near=m_far-0.5f;}
	}
	else if (m_activehandle == m_stencils.names.f)
	{
		m_far -= dragfinal[0];
		// if(m_far<m_near+0.5f){m_far=m_near+0.5f;}
	}
	else if (m_activehandle == m_stencils.names.l)
	{
		m_left += dragfinal[0];
		// if(m_left>m_right-0.5f){m_left=m_right-0.5f;}
	}
	else if (m_activehandle == m_stencils.names.r)
	{
		m_right += dragfinal[0];
		if (m_right < m_left + 0.5f)
		{
			m_right = m_left + 0.5f;
		}
	}
	else if (m_activehandle == m_stencils.names.t)
	{
		float sign = (float)(m_activehandle == m_stencils.names.t) * 2.0f - 1.0f;

		m_top += dragfinal[0];
		// if(m_top<m_bottom+0.5f){m_top=m_bottom+0.5f;}
	}
	else if (m_activehandle == m_stencils.names.b)
	{
		// printf("b\n");
		m_bottom += dragfinal[0];
		// if(m_bottom>m_top-0.5f){m_bottom=m_top-0.5f;}
	}
	// printf("%f\n",m_height);
	// printf("%f\n",pheight);
	///
	editedfrustum->setDefaultValue("left", m_left);
	editedfrustum->setDefaultValue("right", m_right);
	editedfrustum->setDefaultValue("top", m_top);
	editedfrustum->setDefaultValue("bottom", m_bottom);
	editedfrustum->setDefaultValue("near", m_near);
	editedfrustum->setDefaultValue("far", m_far);

	m_edited = m_editednode->getData().getMat4();
}
