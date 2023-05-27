#include "LookAtManipulator.h"

#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/TransformImpl.h"
#include "ManipulatorUtil.h"
#include "Renderer.h"
#include "glm/gtx/norm.hpp"
#include "imgui.h"

const char* LookAtManipulator::s_type = nullptr;

LookAtManipulator::LookAtManipulator()
{
	LookAtManipulator::s_type = typeid(LookAtManipulator).name();
	m_type = LookAtManipulator::s_type;

	m_stencilx = ManipulatorUtil::getStencil(0);
	m_stencily = ManipulatorUtil::getStencil(1);
	m_stencilz = ManipulatorUtil::getStencil(2);
	m_stencilzy = ManipulatorUtil::getStencil(3);
	m_stencilzx = ManipulatorUtil::getStencil(4);
	m_stencilyx = ManipulatorUtil::getStencil(5);

	m_planeh = new GameObject(quadMesh, &World::shaderHandle, 0);
	m_arrowh = new GameObject(arrowMesh, &World::shaderHandle, 0);
	m_cameraico = new GameObject(cameraicoMesh, &World::shaderHandle, 0);
	m_cameraico->transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	m_cameraico->color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	m_threeaxis = new GameObject(three_axisMesh, &World::shader0, World::textures["axis"]);
	m_threeaxis->color = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
	m_threeaxis->primitive = GL_LINES;
	m_edited = glm::mat4(1.0f);
}
void LookAtManipulator::GUI()
{
	// if(m_activehandle!=-1){
	ManipulatorUtil::hint("Move center or eye of LookAt transformation matrix.\nUse keys C, E to "
	                      "switch between center and eye.");

	// char txt[100];
	// sprintf(txt,"X: %0.3f, Y: %0.3f, Z:
	// %0.3f",m_handlespace[3][0],m_handlespace[3][1],m_handlespace[3][2]);
	// ManipulatorUtil::hintAt("jj",world2screen((glm::vec3)m_handlespace[3])-glm::vec2(0.0f,20.0f));
	glm::vec2 pos = world2screen((glm::vec3)m_handlespace[3]) - glm::vec2(90.0f, 70.0f);
	if (pos.y < -5.0f)
	{
		return;
	}
	ImGui::SetCursorPos(ImVec2(pos.x, World::height - pos.y));

	auto* editedlookat = (Core::TransformImpl<Core::ETransformType::LookAt>*)m_editednode.get();
	glm::vec3 center = editedlookat->getDefaultValue("center").getVec3();
	glm::vec3 eye = editedlookat->getDefaultValue("eye").getVec3();

	if (m_editmode == LookAtManipulator::EDIT_CENTER)
	{
		ImGui::Text("X: %0.3f, Y: %0.3f, Z: %0.3f", center[0], center[1], center[2]);
	}
	else
	{
		ImGui::Text("X: %0.3f, Y: %0.3f, Z: %0.3f", eye[0], eye[1], eye[2]);
	}
	//}
}
void LookAtManipulator::render(glm::mat4* parent, bool renderTransparent)
{
	if (m_editednode == nullptr)
	{
		return;
	}
	if (!renderTransparent)
	{
		return;
	}

	// glm::mat4 ftransform=getFullTransform(m_edited);//TMP
	// glm::mat4 ftransform=m_edited;//full transform from nodebase
	glm::mat4 ftransform = getNodeTransform(&m_editednode, &m_parent) * m_edited;
	ftransform[0][3] = 0.0f;
	ftransform[1][3] = 0.0f;
	ftransform[2][3] = 0.0f;
	ftransform[3][3] = 1.0f;

	glUseProgram(World::shaderHandle.program);
	glDepthRange(0.01f, 0.02f);

	float depth = (World::perspective * World::mainCamera * ftransform[3])[2];
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(depth * 0.07f + 0.5f));

	m_threeaxis->draw(ftransform);
	m_cameraico->draw(ftransform * scale);
	glDepthRange(0.0f, 0.01f);

	depth = (World::perspective * World::mainCamera * m_handlespace[3])[2];
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(depth * 0.07f + 0.5f));

	m_arrowh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * scale;
	ManipulatorUtil::drawHandle(m_arrowh, getOrtho(m_handlespace, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), m_stencilx,
	                            m_activehandle, m_hoverhandle);
	m_arrowh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scale;
	ManipulatorUtil::drawHandle(m_arrowh, getOrtho(m_handlespace, 1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), m_stencily,
	                            m_activehandle, m_hoverhandle);
	m_arrowh->transformation = scale;
	ManipulatorUtil::drawHandle(m_arrowh, getOrtho(m_handlespace, 2), glm::vec4(0.1f, 0.4f, 1.0f, 1.0f), m_stencilz,
	                            m_activehandle, m_hoverhandle);

	m_planeh->transformation = scale;
	m_planeh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * scale;
	ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(0.0f, 1.0f, 1.0f, 0.7f), m_stencilzy, m_activehandle,
	                            m_hoverhandle);
	m_planeh->transformation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scale;
	ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(1.0f, 0.2f, 1.0f, 0.7f), m_stencilzx, m_activehandle,
	                            m_hoverhandle);
	m_planeh->transformation = scale;
	ManipulatorUtil::drawHandle(m_planeh, m_handlespace, glm::vec4(1.0f, 1.0f, 0.0f, 0.7f), m_stencilyx, m_activehandle,
	                            m_hoverhandle);

	glDepthRange(0.0f, 1.0f);
	CHECK_GL_ERROR();
}
void LookAtManipulator::update()
{
	if (m_editednode == nullptr)
	{
		return;
	}
	m_edited = glm::inverse(m_editednode->getData().getMat4());
	auto* editedlookat = (Core::TransformImpl<Core::ETransformType::LookAt>*)m_editednode.get();
	glm::vec3 center = editedlookat->getDefaultValue("center").getVec3();
	glm::vec3 eye = editedlookat->getDefaultValue("eye").getVec3();
	///
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

		if (InputManager::isKeyPressed(Keys::e))
		{
			m_editmode = LookAtManipulator::EDIT_EYE;
		}
		else if (InputManager::isKeyPressed(Keys::c))
		{
			m_editmode = LookAtManipulator::EDIT_CENTER;
		}
	}
	if (InputManager::isKeyJustPressed(Keys::mouseLeft))
	{
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
		if (m_activehandle != -1)
		{
			transactionBegin = true;
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

	// glm::vec4 pos=m_handlespace[3];
	// m_handlespace=getNormalized(getFullTransform(m_edited->parent));//TMP
	// glm::mat4 m=m_edited;*(glm::vec3*)&m[3]+=center;
	// m_handlespace[3]=getFullTransform(m_edited)[3];//TMP

	m_handlespace = getNodeTransform(&m_editednode, &m_parent);

	if (m_editmode == LookAtManipulator::EDIT_CENTER)
	{
		m_handlespace[3] = m_handlespace * (m_edited[3] - glm::vec4(eye - center, 0.0f));
	}
	else
	{
		m_handlespace[3] = m_handlespace * (m_edited[3]);
	}
	if (m_activehandle == -1)
	{
		return;
	}

	///
	glm::mat4 axes = glm::mat4(1.0f);
	axes[3] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	glm::mat2 mov = glm::mat2(1.0f);
	mov[0] = vecWorldscreen((glm::vec3)m_handlespace[3],
	                        (glm::vec3)(m_handlespace * axes[m_axisnum])); // the axis in screen space

	if (m_axisnum2 != -1)
	{
		mov[1] = vecWorldscreen((glm::vec3)m_handlespace[3], (glm::vec3)(m_handlespace * axes[m_axisnum2]));
	} // the axis in screen space}
	else
	{
		mov[1] = glm::vec2(mov[0][1], -mov[0][0]);
	}

	if (glm::length(mov[0]) < 0.01f)
	{
		mov[0][0] = 1.0f;
	} // axis length must not be zero
	if (glm::length(mov[1]) < 0.01f)
	{
		mov[1][1] = 1.0f;
	} // axis length must not be zero

	mov = glm::inverse(glm::mat2(glm::normalize(mov[0]), glm::normalize(mov[1])));

	glm::vec2 drag2 = mov * glm::vec2(InputManager::m_mouseXDelta, -InputManager::m_mouseYDelta);
	glm::vec3 drag3 = ((glm::vec3)axes[m_axisnum]) * drag2[0];
	if (m_axisnum2 != -1)
	{
		drag3 += ((glm::vec3)axes[m_axisnum2]) * drag2[1];
	}

	float depth = glm::length(World::mainCamPos + (glm::vec3)m_handlespace[3]); // add, not substract - moving camera is
	                                                                            // moving world in opposite direction
	drag3 *= depth * 0.5f;
	if (InputManager::isKeyPressed(Keys::shiftr))
	{
		drag3 *= 0.25f;
	}

	///
	if (m_axisnum2 != -1)
	{
		glm::vec3 pc = planeIntersect((glm::vec3)(m_handlespace[m_axisnum]), (glm::vec3)(m_handlespace[m_axisnum2]),
		                              (glm::vec3)(m_handlespace[3]));

		if (world2viewport(pc)[2] < 0.992f)
		{
			m_handlespace[3] = glm::vec4(pc[0], pc[1], pc[2], 1.0f);
		}
	}
	else
	{
		drag3 *= 0.006f;
		m_handlespace[3] += m_handlespace[m_axisnum] * drag3[m_axisnum];
	}
	// glm::mat4 m=getFullTransform(this->editedobj->parent);//TMP
	// glm::mat4 m=glm::mat4(1.0f);//TMP
	glm::mat4 m = getNodeTransform(&m_editednode, &m_parent);
	for (int i = 0; i < 4; i++)
	{
		if (glm::length2(m[0]) < 0.0001f)
		{
			m[i][i] = 1.0f;
		}
	} // singular matrix is not invertible
	glm::vec4 posh = (glm::inverse(m) * m_handlespace)[3];

	///
	if (m_editmode == LookAtManipulator::EDIT_CENTER)
	{
		editedlookat->setDefaultValue("center", (glm::vec3)posh);
		// printf("cur %f %f %f,, set %f %f
		// %f\n",center[0],center[1],center[2],posh[0],posh[1],posh[2]);
	}
	else
	{
		editedlookat->setDefaultValue("eye", (glm::vec3)posh);
		// printf("cur %f %f %f,, set %f %f
		// %f\n",eye[0],eye[1],eye[2],posh[0],posh[1],posh[2]);
	}
	m_edited = glm::inverse(m_editednode->getData().getMat4());
	center = editedlookat->getDefaultValue("center").getVec3();
	eye = editedlookat->getDefaultValue("eye").getVec3();

	m_handlespace = getNodeTransform(&m_editednode, &m_parent);
	if (m_editmode == LookAtManipulator::EDIT_CENTER)
	{
		m_handlespace[3] = m_handlespace * (m_edited[3] - glm::vec4(eye - center, 0.0f));
	}
	else
	{
		m_handlespace[3] = m_handlespace * (m_edited[3]);
	}
}