#include "FreeManipulator.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "../World.h"
#include "Core/Input/InputManager.h"
#include "ManipulatorUtil.h"
#include "glm/gtx/norm.hpp"

#include "imgui.h"

#include <math.h>
#include <typeinfo>

void printMatrix4(glm::mat4 m)
{
	printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f "
	       "%0.3f %0.3f\n\t%0.3f %0.3f %0.3f "
	       "%0.3f\n\n",
	       m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2],
	       m[0][3], m[1][3], m[2][3], m[3][3]);
}

const char* FreeManipulator::s_type = nullptr;

FreeManipulator::FreeManipulator()
{
	FreeManipulator::s_type = typeid(FreeManipulator).name();
	m_type = FreeManipulator::s_type;

	m_stencilx = Select::registerStencil();
	m_stencily = Select::registerStencil();
	m_stencilz = Select::registerStencil();
	m_stencilzy = Select::registerStencil();
	m_stencilzx = Select::registerStencil();
	m_stencilyx = Select::registerStencil();
	stencilxyz = Select::registerStencil();
	m_stencilaxisx = Select::registerStencil();
	m_stencilaxisy = Select::registerStencil();
	m_stencilaxisz = Select::registerStencil();
	m_stencilaxisw = Select::registerStencil();

	m_circleh = new GameObject(unitcircleMesh, &World::shaderHandle, 0);
	m_arrowh = new GameObject(arrowMesh, &World::shaderHandle, 0);
	m_planeh = new GameObject(quadMesh, &World::shaderHandle, 0);
	m_scaleh = new GameObject(scalearrowMesh, &World::shaderHandle, 0);
	m_uniscaleh = new GameObject(unitcubeMesh, &World::shaderHandle, 0);
	m_lineh = new GameObject(lineMesh, &World::shaderHandle, 0);
	m_bkp = m_edited;
}
void FreeManipulator::start() {}
void FreeManipulator::GUI()
{
	ManipulatorUtil::hint("Use keys S, R, T to switch scale, rotation and translation.\nUse keys "
	                      "X, Y, Z, W or LMB to switch axis.");
}

void FreeManipulator::render(glm::mat4* parent, bool renderTransparent)
{
	if (m_editednode == nullptr)
	{
		return;
	}
	if (!renderTransparent)
	{
		return;
	}
	glUseProgram(World::shaderHandle.program);
	glDepthRange(0.0, 0.01);
	GameObject* handle = m_circleh;
	if (m_editmode == FreeManipulator::EDIT_ROTATION)
	{
		handle = m_circleh;
	}
	else if (m_editmode == FreeManipulator::EDIT_POSITION)
	{
		handle = m_arrowh;
	}
	else if (m_editmode == FreeManipulator::EDIT_SCALE)
	{
		handle = m_scaleh;
	}

	float depth = (World::perspective * World::mainCamera * m_handlespace[3])[2];
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(depth * 0.07f + 0.5f));

	glm::vec4 row4bkp = glm::vec4(m_edited[0][3], m_edited[1][3], m_edited[2][3], m_edited[3][3]);
	m_edited[0][3] = 0.0f;
	m_edited[1][3] = 0.0f;
	m_edited[2][3] = 0.0f;
	m_edited[3][3] = 1.0f;
	// glm::mat4 ftransform=getFullTransform(m_edited);//TMP
	// glm::mat4 ftransform=m_edited;
	glm::mat4 ftransform = getNodeTransform(&m_editednode, &m_parent) * m_editednode->getData().getMat4();
	m_edited[0][3] = row4bkp[0];
	m_edited[1][3] = row4bkp[1];
	m_edited[2][3] = row4bkp[2];
	m_edited[3][3] = row4bkp[3];

	glm::mat4 t;
	float selected;

	selected = 0.3f * (float)(m_editaxis == 0);
	t = getOrtho(ftransform, 0);
	setLen((glm::vec3*)&t[1], 1.0f + selected * 3.0f);
	setLen((glm::vec3*)&t[2], 1.0f + selected * 3.0f);
	m_lineh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ManipulatorUtil::drawHandle(m_lineh, t, glm::vec4(1.0f, selected * 1.5f, selected * 1.5f, 1.0f), m_stencilaxisx,
	                            false, m_hoverhandle);

	selected = 0.3f * (float)(m_editaxis == 1);
	t = getOrtho(ftransform, 1);
	setLen((glm::vec3*)&t[0], 1.0f + selected * 3.0f);
	setLen((glm::vec3*)&t[2], 1.0f + selected * 3.0f);
	m_lineh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ManipulatorUtil::drawHandle(m_lineh, t, glm::vec4(selected * 1.5f, 1.0f, selected * 1.5f, 1.0f), m_stencilaxisy,
	                            false, m_hoverhandle);

	selected = 0.3f * (float)(m_editaxis == 2);
	t = getOrtho(ftransform, 2);
	setLen((glm::vec3*)&t[0], 1.0f + selected * 3.0f);
	setLen((glm::vec3*)&t[1], 1.0f + selected * 3.0f);
	m_lineh->transformation = glm::mat4(1.0f);
	ManipulatorUtil::drawHandle(m_lineh, t, glm::vec4(selected * 0.5f + 0.1f, selected * 0.5f + 0.4f, 1.0f, 1.0f),
	                            m_stencilaxisz, false, m_hoverhandle);

	selected = 0.3f * (float)(m_editaxis == 3);
	// t=glm::mat4(ftransform[1],ftransform[2],getFullTransform(m_edited->parent)[3]-ftransform[3],
	// ftransform[3]);//TMP
	// t=glm::mat4(ftransform[1],ftransform[2],glm::mat4(1.0f)[3]-ftransform[3],
	// ftransform[3]);//TMP
	t = glm::mat4(ftransform[1], ftransform[2], getNodeTransform(&m_editednode, &m_parent)[3] - ftransform[3],
	              ftransform[3]); // TMP
	t = getOrtho(t, 2);
	setLen((glm::vec3*)&t[0], 1.0f + selected * 3.0f);
	setLen((glm::vec3*)&t[1], 1.0f + selected * 3.0f);
	m_lineh->transformation = glm::mat4(1.0f);
	ManipulatorUtil::drawHandle(m_lineh, t, glm::vec4(1.0f, selected + 0.2f, 1.0f, 1.0f), m_stencilaxisw, false,
	                            m_hoverhandle);

	if (m_editmode == FreeManipulator::EDIT_SCALE)
	{ // override scale handles for free edit
		scale = glm::scale(scale, glm::vec3(0.09f));
		if (m_editaxis == 0)
		{
			m_uniscaleh->transformation = glm::mat4(1.0f) * scale;
			ManipulatorUtil::drawHandle(m_uniscaleh, m_handlespace, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), m_stencilx,
			                            m_activehandle, m_hoverhandle);
		}
		else if (m_editaxis == 1)
		{
			m_uniscaleh->transformation = glm::mat4(1.0f) * scale;
			ManipulatorUtil::drawHandle(m_uniscaleh, m_handlespace, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), m_stencily,
			                            m_activehandle, m_hoverhandle);
		}
		else if (m_editaxis == 2)
		{
			m_uniscaleh->transformation = glm::mat4(1.0f) * scale;
			ManipulatorUtil::drawHandle(m_uniscaleh, m_handlespace, glm::vec4(0.1f, 0.4f, 1.0f, 1.0f), m_stencilz,
			                            m_activehandle, m_hoverhandle);
		}
		else if (m_editaxis == 3)
		{
			m_uniscaleh->transformation = glm::mat4(1.0f) * scale;
			ManipulatorUtil::drawHandle(m_uniscaleh, m_handlespace, glm::vec4(1.0f, 0.2f, 1.0f, 1.0f), m_stencilz,
			                            m_activehandle, m_hoverhandle);
		}
	}
	else
	{
		handle->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * scale;
		ManipulatorUtil::drawHandle(handle, getOrtho(m_handlespace, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), m_stencilx,
		                            m_activehandle, m_hoverhandle);
		handle->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scale;
		ManipulatorUtil::drawHandle(handle, getOrtho(m_handlespace, 1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), m_stencily,
		                            m_activehandle, m_hoverhandle);
		handle->transformation = glm::mat4(1.0f) * scale;
		ManipulatorUtil::drawHandle(handle, getOrtho(m_handlespace, 2), glm::vec4(0.1f, 0.4f, 1.0f, 1.0f), m_stencilz,
		                            m_activehandle, m_hoverhandle);

		if (m_editmode == FreeManipulator::EDIT_POSITION)
		{ // two-axis handles
			m_planeh->transformation = glm::mat4(1.0f) * scale;

			m_planeh->transformation =
			    glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * scale;
			ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(0.0f, 1.0f, 1.0f, 0.6f), m_stencilzy,
			                            m_activehandle, m_hoverhandle);
			m_planeh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scale;
			ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(1.0f, 0.2f, 1.0f, 0.6f), m_stencilzx,
			                            m_activehandle, m_hoverhandle);
			m_planeh->transformation = glm::mat4(1.0f) * scale;
			ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(1.0f, 1.0f, 0.0f, 0.6f), m_stencilyx,
			                            m_activehandle, m_hoverhandle);
		}
	}
	glDepthRange(0.0, 1.0);
	CHECK_GL_ERROR();
}

void FreeManipulator::update()
{
	if (m_editednode == nullptr)
	{
		return;
	}
	glm::mat4 changed = m_editednode.get()->getData().getMat4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (changed[i][j] != m_edited[i][j])
			{
				m_editaxis = i;
				break;
			}
		} // active axis that was changed from workspace
	}
	m_edited = m_editednode.get()->getData().getMat4();
	// printf("4\n");
	bool transactionBegin = false;
	unsigned char sel =
	    Select::getStencilAt((int)InputManager::m_mouseX, (int)(World::height - InputManager::m_mouseY), 3, -1);

	m_hoverhandle = -1;
	if (m_activehandle == -1)
	{
		if (sel == m_stencilx)
		{
			m_hoverhandle = m_stencilx;
		} // manipulating handles clicked
		else if (sel == m_stencily)
		{
			m_hoverhandle = m_stencily;
		}
		else if (sel == m_stencilz)
		{
			m_hoverhandle = m_stencilz;
		}
		else if (sel == m_stencilzy)
		{
			m_hoverhandle = m_stencilzy;
		}
		else if (sel == m_stencilzx)
		{
			m_hoverhandle = m_stencilzx;
		}
		else if (sel == m_stencilyx)
		{
			m_hoverhandle = m_stencilyx;
		}
		else if (sel == stencilxyz)
		{
			m_hoverhandle = stencilxyz;
		}
		else if (sel == m_stencilaxisx)
		{
			m_hoverhandle = m_stencilaxisx;
		}
		else if (sel == m_stencilaxisy)
		{
			m_hoverhandle = m_stencilaxisy;
		}
		else if (sel == m_stencilaxisz)
		{
			m_hoverhandle = m_stencilaxisz;
		}
		else if (sel == m_stencilaxisw)
		{
			m_hoverhandle = m_stencilaxisw;
		}
	}

	if (InputManager::isKeyJustPressed(Keys::mouseLeft))
	{
		unsigned char sel =
		    Select::getStencilAt((int)InputManager::m_mouseX, (int)(World::height - InputManager::m_mouseY), 3, -1);
		m_activehandle = -1;
		if (sel == m_stencilx)
		{
			m_activehandle = m_stencilx;
			m_axisnum = 0;
			m_axisnum2 = -1;
		} // manipulating handles clicked
		else if (sel == m_stencily)
		{
			m_activehandle = m_stencily;
			m_axisnum = 1;
			m_axisnum2 = -1;
		}
		else if (sel == m_stencilz)
		{
			m_activehandle = m_stencilz;
			m_axisnum = 2;
			m_axisnum2 = -1;
		}
		else if (sel == m_stencilzy)
		{
			m_activehandle = m_stencilzy;
			m_axisnum = 2;
			m_axisnum2 = 1;
		}
		else if (sel == m_stencilzx)
		{
			m_activehandle = m_stencilzx;
			m_axisnum = 2;
			m_axisnum2 = 0;
		}
		else if (sel == m_stencilyx)
		{
			m_activehandle = m_stencilyx;
			m_axisnum = 1;
			m_axisnum2 = 0;
		}
		else if (sel == stencilxyz)
		{
			m_activehandle = stencilxyz;
			m_axisnum = 3;
			m_axisnum2 = -1;
		}
		else if (sel == m_stencilaxisx)
		{
			m_editaxis = 0;
		}
		else if (sel == m_stencilaxisy)
		{
			m_editaxis = 1;
		}
		else if (sel == m_stencilaxisz)
		{
			m_editaxis = 2;
		}
		else if (sel == m_stencilaxisw)
		{
			m_editaxis = 3;
		}
		if (m_activehandle != -1)
		{
			transactionBegin = true;
		}
	}
	// printf("5\n");
	if (InputManager::isKeyJustUp(Keys::mouseLeft))
	{
		m_activehandle = -1;
	}

	if (m_hoverhandle != -1 || m_activehandle != -1)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	glm::vec4 row4bkp = glm::vec4(m_edited[0][3], m_edited[1][3], m_edited[2][3], m_edited[3][3]);
	m_edited[0][3] = 0.0f;
	m_edited[1][3] = 0.0f;
	m_edited[2][3] = 0.0f;
	m_edited[3][3] = 1.0f;

	if (m_activehandle == -1)
	{
		if (InputManager::isKeyPressed(Keys::r))
		{
			m_editmode = FreeManipulator::EDIT_ROTATION;
		}
		else if (InputManager::isKeyPressed(Keys::s))
		{
			m_editmode = FreeManipulator::EDIT_SCALE;
		}
		else if (InputManager::isKeyPressed(Keys::t))
		{
			m_editmode = FreeManipulator::EDIT_POSITION;
		}

		if (InputManager::isKeyPressed(Keys::x))
		{
			m_editaxis = 0;
		}
		else if (InputManager::isKeyPressed(Keys::y))
		{
			m_editaxis = 1;
		}
		else if (InputManager::isKeyPressed(Keys::z))
		{
			m_editaxis = 2;
		}
		else if (InputManager::isKeyPressed(Keys::w))
		{
			m_editaxis = 3;
		}
		m_bkp = m_edited;

		if (m_editaxis == 3)
		{
			glm::mat4 m = m_edited;
			m[2] = m[3];
			m_rotfreebkp = getRotation(m, 2);
		}
		else
		{
			m_rotfreebkp = getRotation(m_edited, m_editaxis);
		}
	}

	m_handlespace = glm::mat4(1.0f);
	// printf("7\n");
	glm::mat4 m = getNodeTransform(&m_editednode, &m_parent);
	if (m_editmode == FreeManipulator::EDIT_POSITION)
	{
		// glm::mat4 m=getFullTransform(m_edited->parent);//TMP
		// glm::mat4 m=glm::mat4(1.0f);
		m_handlespace = getNormalized(m);
		m = m * m_edited;
		m_handlespace[3] = m[3] + m[m_editaxis] * (float)(m_editaxis != 3);
		// printMatrix2(handlespace);
	}
	else if (m_editmode == FreeManipulator::EDIT_SCALE)
	{
		// glm::mat4 m=getFullTransform(m_edited->parent);//TMP
		// glm::mat4 m=glm::mat4(1.0f);
		if (m_editaxis == 3)
		{
			glm::vec4 dir = m[3] - (m * m_bkp)[3];
			m_handlespace = getRotation(glm::mat4(glm::vec4(0.0f), glm::vec4(0.0f), -dir, glm::vec4(0.0f)), 2);
		}
		else
		{
			m_handlespace = getRotation(m * m_bkp, m_editaxis);
		}
		m = m * m_edited;
		m_handlespace[3] = m[3] + m[m_editaxis] * (float)(m_editaxis != 3);
	}
	else if (m_editmode == FreeManipulator::EDIT_ROTATION)
	{
		// glm::mat4 m=getFullTransform(m_edited->parent);//TMP
		// glm::mat4 m=glm::mat4(1.0f);
		if (m_editaxis == 3)
		{
			m_handlespace = getRotation(m * m_rotfreebkp, 2);
			m_handlespace[3] = m[3];
		}
		else
		{
			m_handlespace = getRotation(m * m_rotfreebkp, m_editaxis);
			m_handlespace[3] = (m * m_edited)[3];
		}
	}

	m_handlespace[0][3] = 0.0f;
	m_handlespace[1][3] = 0.0f;
	m_handlespace[2][3] = 0.0f;
	m_handlespace[3][3] = 1.0f;

	if (m_activehandle == -1)
	{
		return;
	}
	// printf("hs %f %f %f\n\n", handlespace[3][0], handlespace[3][1],
	// handlespace[3][2]);
	// if(Controls::keysEventTable[27]==Controls::EVENT_UP){editedobj->transform=bkp;}

	glm::vec3 drag3 = glm::vec3(0.0f);
	glm::mat4 axes = glm::mat4(1.0f);
	axes[3] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	glm::vec2 spos1 = world2screen((glm::vec3)(m_handlespace[3])); // position of transformated object on the screen
	glm::vec2 spos2 = world2screen(
	    (glm::vec3)(m_handlespace[3] + m_handlespace * axes[m_axisnum])); // spos1,spos2 - project two points on
	                                                                      // screen - project axis on screen
	glm::vec2 dir = spos2 - spos1;                                        // the axis in screen space

	if (m_editmode == FreeManipulator::EDIT_ROTATION)
	{
		if (transactionBegin)
		{
			glm::mat4 ortho = getOrtho(m_handlespace, m_axisnum);
			glm::vec3 p0 = (glm::vec3)m_handlespace[3];
			glm::vec3 px = (glm::vec3)(ortho * axes[(m_axisnum + 1) % 3]);
			glm::vec3 py = (glm::vec3)(ortho * axes[(m_axisnum + 2) % 3]);
			glm::vec3 t0 = -World::mainCamPos;
			// glm::vec3 tz = mouseray(world2screen(p0)
			// +glm::vec2(InputController::m_mouseXDelta,
			// -InputController::m_mouseYDelta));
			glm::vec3 tz = mouseray(glm::vec2(InputManager::m_mouseX, World::height - InputManager::m_mouseY));
			glm::vec3 coef = glm::inverse(glm::mat3(-tz, px, py)) * (t0 - p0);

			glm::vec3 pc = px * coef[1] + py * coef[2];
			glm::vec3 dir3 = glm::normalize(glm::cross(pc, (glm::vec3)axes[m_axisnum]));
			m_dirbkp = glm::normalize(world2screen(p0) - world2screen(p0 + dir3));

			// glm::vec3 dir3 = glm::normalize(glm::cross(pc - (glm::vec3)ortho[3],
			// (glm::vec3)axes[axisnum])); dirbkp = glm::normalize(world2screen(p0) -
			// world2screen(p0 - dir3));
		}
		dir = m_dirbkp;
	}
	if (glm::length(dir) < 0.01f)
	{
		dir[0] = 1.0f;
	} // axis length must not be zero

	glm::mat2 mov = glm::mat2(dir, glm::vec2(dir[1], -dir[0]));

	if (m_axisnum2 != -1)
	{
		glm::vec2 spos22 =
		    world2screen((glm::vec3)(m_handlespace[3] + m_handlespace * axes[m_axisnum2])); // project two points on screen
		                                                                                    // - project axis on screen
		glm::vec2 dir2 = spos22 - spos1;                                                    // the axis in screen space
		if (glm::length(dir2) < 0.01)
		{
			dir2[1] = 1.0f;
		} // axis length must not be zero
		mov[1] = dir2;
	}

	mov = glm::inverse(glm::mat2(glm::normalize(mov[0]), glm::normalize(mov[1])));

	glm::vec2 drag, olddrag, dragfinal, mouse;

	mouse = glm::vec2(InputManager::m_mouseX, World::height - InputManager::m_mouseY);
	drag = mov * (mouse - spos1);
	mouse = glm::vec2(InputManager::m_mouseXPrev, World::height - InputManager::m_mouseYPrev);
	olddrag = mov * (mouse - spos1);
	dragfinal = drag - olddrag;

	drag3 += ((glm::vec3)axes[m_axisnum]) * (dragfinal[0]);
	if (m_axisnum2 != -1)
	{
		drag3 += ((glm::vec3)axes[m_axisnum2]) * (dragfinal[1]);
	}

	float depth = glm::length(World::mainCamPos + (glm::vec3)m_handlespace[3]); // add, not substract - moving camera is
	                                                                            // moving world in opposite direction
	if (m_editmode != FreeManipulator::EDIT_ROTATION)
	{
		drag3 *= depth * 0.5f;
	}
	if (InputManager::isKeyPressed(Keys::shiftr))
	{
		drag3 *= 0.25f;
	}
	// printf("9\n");

	///
	drag3 *= 1.2f;
	if (m_editmode == FreeManipulator::EDIT_ROTATION)
	{
		glm::mat4 r = glm::mat4(m_rotfreebkp[0], m_rotfreebkp[1], m_rotfreebkp[2],
		                        m_rotfreebkp[2]); // axes of rotation (0,1,2,2)
		glm::mat4 rot1 = glm::rotate(glm::mat4(1.0f), glm::radians(drag3[m_axisnum]), (glm::vec3)r[m_axisnum]);
		// printMatrix2(rotfreebkp);
		m_rotfreebkp = rot1 * m_rotfreebkp;
		// printMatrix2(rotfreebkp);printf("----------------\n");
		m_edited[m_editaxis] = rot1 * m_edited[m_editaxis];
		m_edited[3][3] = 1.0f;
	}
	else if (m_editmode == FreeManipulator::EDIT_POSITION)
	{
		if (m_axisnum2 != -1)
		{
			glm::vec3 pc = planeIntersect((glm::vec3)(m_handlespace[m_axisnum]), (glm::vec3)(m_handlespace[m_axisnum2]),
			                              (glm::vec3)(m_handlespace[3]));

			if (world2viewport(pc)[2] < 0.998f)
			{
				// glm::mat4 parent=getFullTransform(m_edited->parent);//TMP
				// glm::mat4 parent=glm::mat4(1.0f);
				glm::mat4 parent = getNodeTransform(&m_editednode, &m_parent);
				glm::vec4 editedo = glm::inverse(parent) * glm::vec4(pc, 1.0f);
				if (m_editaxis != 3)
				{
					editedo -= m_edited[3];
				}

				m_edited[m_editaxis] = editedo;
			}
		}
		else
		{
			drag3 *= 0.008f;
			m_edited[m_editaxis][m_axisnum] += drag3[m_axisnum];
		}
		// glm::mat4 ftransform=getFullTransform(m_edited);//TMP
		// glm::mat4 ftransform=m_edited;
		glm::mat4 ftransform = getNodeTransform(&m_editednode, &m_parent) * m_editednode->getData().getMat4();
		m_handlespace[3] = ftransform[3] + ftransform[m_editaxis] * (float)(m_editaxis != 3);
		m_handlespace[3][3] = 1.0f;
	}
	else if (m_editmode == FreeManipulator::EDIT_SCALE)
	{
		drag3 *= 0.005f;

		if (m_editaxis == 3 && glm::length2((glm::vec3)m_bkp[3]) > 0.0005f)
		{
			m_edited[3] += m_bkp[3] * drag3[2] / (glm::length((glm::vec3)m_bkp[3]));
			m_edited[3][3] = 1.0f;
		}
		else if (glm::length2((glm::vec3)m_bkp[m_editaxis]) > 0.0005f)
		{
			m_edited[m_editaxis] += m_bkp[m_editaxis] * drag3[m_editaxis] / (glm::length((glm::vec3)m_bkp[m_editaxis]));
		}
		// glm::mat4 ftransform=getFullTransform(m_edited);//TMP
		// glm::mat4 ftransform=m_edited;
		glm::mat4 ftransform = getNodeTransform(&m_editednode, &m_parent) * m_editednode->getData().getMat4();
		m_handlespace[3] = ftransform[3] + ftransform[m_editaxis] * (float)(m_editaxis != 3);
		m_handlespace[3][3] = 1.0f;
	}

	m_edited[0][3] = row4bkp[0];
	m_edited[1][3] = row4bkp[1];
	m_edited[2][3] = row4bkp[2];
	m_edited[3][3] = row4bkp[3];
	///
	m_editednode->setValue(m_edited);
}
