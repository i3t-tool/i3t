#include "ScaleManipulator.h"

#include "Core/Input/InputManager.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "Renderer.h"
#include "glm/gtx/norm.hpp"
#include "imgui.h"
#include "ManipulatorUtil.h"

#include <typeinfo>


void printMatrix3(glm::mat4 m){
  printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\n",
	  m[0][0], m[1][0], m[2][0], m[3][0],	m[0][1], m[1][1], m[2][1], m[3][1],
	  m[0][2], m[1][2], m[2][2], m[3][2],	m[0][3], m[1][3], m[2][3], m[3][3]);
}
const char* ScaleManipulator::typeStatic = NULL;

ScaleManipulator::ScaleManipulator() {
    ScaleManipulator::typeStatic = typeid(ScaleManipulator).name();
    type = ScaleManipulator::typeStatic;

	m_stencilx = ManipulatorUtil::getStencil(0);
	m_stencily = ManipulatorUtil::getStencil(1);
	m_stencilz = ManipulatorUtil::getStencil(2);
	m_stencilzy =ManipulatorUtil::getStencil(3);
	m_stencilzx =ManipulatorUtil::getStencil(4);
	m_stencilyx =ManipulatorUtil::getStencil(5);
	m_stencilxyz=ManipulatorUtil::getStencil(6);

	m_planeh =	new GameObject(quadMesh,		&World2::shaderHandle,	0);
	m_scaleh =	new GameObject(scalearrowMesh,	&World2::shaderHandle,	0);
	m_uniscaleh=new GameObject(unitcubeMesh,	&World2::shaderHandle,	0);
	m_threeaxis=new GameObject(three_axisMesh,	&World2::shader0,		World2::axisTexture);		
	m_threeaxis->color=glm::vec4(2.0f,2.0f,2.0f,1.0f);
	m_threeaxis->primitive=GL_LINES;
	m_edited=glm::mat4(1.0f);
}

void ScaleManipulator::render(glm::mat4* parent, bool renderTransparent) {
	if(!renderTransparent){return;}

	float depth=(World2::perspective*World2::mainCamera*m_handlespace[3])[2];
	glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(depth*0.05f+0.5f));

	//glm::mat4 ftransform=getFullTransform(m_edited);//TMP
	glm::mat4 ftransform=m_edited;//full transform from nodebase
	ftransform[0][3]=0.0f;
	ftransform[1][3]=0.0f;
	ftransform[2][3]=0.0f;
	ftransform[3][3]=1.0f;

	glUseProgram(World2::shaderHandle.program);
	glDepthRange(0.0, 0.01);

	m_threeaxis->draw(ftransform);

	m_uniscaleh->transformation=glm::mat4(1.0f)*scale;
	m_uniscaleh->scale(glm::vec3(0.2f));
	ManipulatorUtil::drawHandle(m_uniscaleh,getOrtho(m_handlespace, 2),glm::vec4(0.6f,0.6f,0.6f,1.0f), m_stencilxyz,m_activehandle,m_hoverhandle);

	m_scaleh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f))*scale;
	ManipulatorUtil::drawHandle(m_scaleh,getOrtho(m_handlespace,0),glm::vec4(1.0f,0.0f,0.0f,1.0f),m_stencilx,m_activehandle,m_hoverhandle);
	m_scaleh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f))*scale;
	ManipulatorUtil::drawHandle(m_scaleh,getOrtho(m_handlespace,1),glm::vec4(0.0f,1.0f,0.0f,1.0f),m_stencily,m_activehandle,m_hoverhandle);
	m_scaleh->transformation=glm::mat4(1.0f)*scale;
	ManipulatorUtil::drawHandle(m_scaleh,getOrtho(m_handlespace,2),glm::vec4(0.1f,0.4f,1.0f,1.0f),m_stencilz,m_activehandle,m_hoverhandle);
			
	m_planeh->transformation=glm::mat4(1.0f)*scale;
	m_planeh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f))*scale;
	ManipulatorUtil::drawHandle(m_planeh,m_handlespace,glm::vec4(0.0f,1.0f,1.0f,0.7f),m_stencilzy,m_activehandle,m_hoverhandle);
	m_planeh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f))*scale;
	ManipulatorUtil::drawHandle(m_planeh,m_handlespace,glm::vec4(1.0f,0.2f,1.0f,0.7f),m_stencilzx,m_activehandle,m_hoverhandle);
	m_planeh->transformation=glm::mat4(1.0f)*scale;
	ManipulatorUtil::drawHandle(m_planeh,m_handlespace,glm::vec4(1.0f,1.0f,0.0f,0.7f),m_stencilyx,m_activehandle,m_hoverhandle);

	glDepthRange(0.0, 1.0);
	CHECK_GL_ERROR();
}

void ScaleManipulator::update() {
	if(m_editednode!=NULL){m_edited=m_editednode->getData().getMat4();}
	///
	if(InputManager::isKeyPressed(Keys::p)){printMatrix3(m_edited);}//TMP
	bool transactionBegin=false;

	unsigned char sel =Select::getStencilAt((int)InputManager::m_mouseX, (int)(World2::height - InputManager::m_mouseY), 3, -1);
	m_hoverhandle=-1;
	if(m_activehandle==-1){
		if(sel==m_stencilx){		m_hoverhandle=m_stencilx;}//manipulating handles clicked
		else if(sel==m_stencily){	m_hoverhandle=m_stencily;}
		else if(sel==m_stencilz){	m_hoverhandle=m_stencilz;}
		else if(sel==m_stencilzy){	m_hoverhandle=m_stencilzy;}
		else if(sel==m_stencilzx){	m_hoverhandle=m_stencilzx;}
		else if(sel==m_stencilyx){	m_hoverhandle=m_stencilyx;}
		else if(sel==m_stencilxyz){	m_hoverhandle=m_stencilxyz;}
	}
	if(InputManager::isKeyJustPressed(Keys::mouseLeft)){
		m_activehandle=-1;
		if(sel==m_stencilx){		m_activehandle=m_stencilx;	m_axisnum=0;m_axisnum2=-1;}//manipulating handles clicked
		else if(sel==m_stencily){	m_activehandle=m_stencily;	m_axisnum=1;m_axisnum2=-1;}
		else if(sel==m_stencilz){	m_activehandle=m_stencilz;	m_axisnum=2;m_axisnum2=-1;}
		else if(sel==m_stencilzy){	m_activehandle=m_stencilzy;	m_axisnum=2;m_axisnum2=1;}
		else if(sel==m_stencilzx){	m_activehandle=m_stencilzx;	m_axisnum=2;m_axisnum2=0;}
		else if(sel==m_stencilyx){	m_activehandle=m_stencilyx;	m_axisnum=1;m_axisnum2=0;}
		else if(sel==m_stencilxyz){	m_activehandle=m_stencilxyz;m_axisnum=3;m_axisnum2=-1;}
		if(m_activehandle!=-1){transactionBegin=true;}
	}
	if(InputManager::isKeyJustUp(Keys::mouseLeft)){m_activehandle=-1;}

	if(m_hoverhandle!=-1||m_activehandle!=-1){ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);}

	
	//m_handlespace=getNormalized(getFullTransform(m_edited->parent)/**m_bkp*/);//TMP
	m_handlespace=glm::mat4(1.0f);

	if(m_activehandle==-1){/*m_bkp=m_edited->transformation;*/return;}

	///
	glm::mat4 axes=glm::mat4(1.0f);axes[3]=glm::vec4(1.0f,1.0f,1.0f,0.0f);
	glm::mat2 mov =glm::mat2(1.0f);
	mov[0]=vecWorld2screen((glm::vec3)m_handlespace[3],(glm::vec3)(m_handlespace*axes[m_axisnum]));//the axis in screen space

	if(m_axisnum2!=-1){mov[1]=vecWorld2screen((glm::vec3)m_handlespace[3],(glm::vec3)(m_handlespace*axes[m_axisnum2]));}//the axis in screen space}
	else{mov[1]=glm::vec2(mov[0][1],-mov[0][0]);}

	if(glm::length(mov[0])<0.01f){mov[0][0]=1.0f;}//axis length must not be zero
	if(glm::length(mov[1])<0.01f){mov[1][1]=1.0f;}//axis length must not be zero

	mov=glm::inverse(glm::mat2(glm::normalize(mov[0]),glm::normalize(mov[1])));
		
	glm::vec2 drag2=mov*glm::vec2(InputManager::m_mouseXDelta,-InputManager::m_mouseYDelta);
	glm::vec3 drag3=((glm::vec3)axes[m_axisnum])*drag2[0];
	if(m_axisnum2!=-1){drag3+=((glm::vec3)axes[m_axisnum2])*drag2[1];}

	float depth=glm::length(World2::mainCamPos+(glm::vec3)m_handlespace[3]);//add, not substract - moving camera is moving world in opposite direction
	drag3*=depth*0.5f;
	if(InputManager::isKeyPressed(Keys::shiftr)){drag3*=0.25f;}

	///
	drag3*=0.004f;
	glm::vec3 scal=glm::vec3(
		glm::length((glm::vec3)m_edited[0]),
		glm::length((glm::vec3)m_edited[1]),
		glm::length((glm::vec3)m_edited[2])
	);
					
	if(m_activehandle==m_stencilxyz){//if uniform scale, scale of other axes proportionate to ref axis
		int ref=0;
		if(scal[0]!=0.0f){//find axis not scaled by 0 - that is reference axis
			drag3[1]=(scal[1]/scal[0])*drag3[0];
			drag3[2]=(scal[2]/scal[0])*drag3[0];
		}
		else if(scal[1]!=0.0f){
			drag3[0]=(scal[0]/scal[1])*drag3[1];
			drag3[2]=(scal[2]/scal[1])*drag3[1];
		}
		else if(scal[2]!=0.0f){
			drag3[1]=(scal[1]/scal[2])*drag3[2];
			drag3[0]=(scal[0]/scal[2])*drag3[2];
		}
		else{//if no ref axis found, then no changes to drag vector needed - the axis is x,x,x as default which is proportionate to scale by 0,0,0
		}
	}
					
	m_edited[0][0]+=drag3[0];//TMP
	m_edited[1][1]+=drag3[1];//TMP
	m_edited[2][2]+=drag3[2];//TMP
	///
	//if(m_editednode!=NULL){ValueSetResult v=m_editednode->setValue(glm::vec3(m_edited[0][0], m_edited[1][1], m_edited[2][2]));}
	if(m_editednode!=NULL){ValueSetResult v=m_editednode->setValue(glm::vec3(m_edited[0][0]));}
}
