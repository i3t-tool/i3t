#include "ScaleHandles.h"

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
const char* ScaleHandles::typeStatic = NULL;

ScaleHandles::ScaleHandles() {
    ScaleHandles::typeStatic = typeid(ScaleHandles).name();
    type = ScaleHandles::typeStatic;

	m_stencilx = ManipulatorUtil::getStencil(0);
	m_stencily = ManipulatorUtil::getStencil(1);
	m_stencilz = ManipulatorUtil::getStencil(2);
	m_stencilzy =ManipulatorUtil::getStencil(3);
	m_stencilzx =ManipulatorUtil::getStencil(4);
	m_stencilyx =ManipulatorUtil::getStencil(5);
	m_stencilxyz=ManipulatorUtil::getStencil(6);
}

void ScaleHandles::start() {
	m_planeh =	new GameObject(quadMesh,		&World2::shaderHandle,	0);
	m_scaleh =	new GameObject(scalearrowMesh,	&World2::shaderHandle,	0);
	m_uniscaleh=new GameObject(unitcubeMesh,	&World2::shaderHandle,	0);
	m_threeaxis=new GameObject(three_axisMesh,	&World2::shader0,		World2::axisTexture);		
	m_threeaxis->color=glm::vec4(2.0f,2.0f,2.0f,1.0f);
	m_threeaxis->primitive=GL_LINES;
	m_editedobj=new GameObject();
	m_bkp=m_editedobj->transformation;
}

void ScaleHandles::render(glm::mat4* parent, bool renderTransparent) {
	if(!renderTransparent){return;}

	float depth=(World2::perspective*World2::mainCamera*m_handlespace[3])[2];
	glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(depth*0.05f+0.5f));

	glm::mat4 ftransform=getFullTransform(m_editedobj);//TMP
	//glm::mat4 ftransform=glm::mat4(1.0f);//full transform from nodebase
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

void ScaleHandles::update() {
	if(InputManager::isKeyPressed(Keys::p)){printMatrix3(m_editedobj->transformation);}//TMP
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

	m_handlespace=getNormalized(getFullTransform(m_editedobj->parent)*m_bkp);//TMP
	//m_handlespace=glm::mat4(1.0f);

	if(m_activehandle==-1){return;}

	glm::vec3 drag3=glm::vec3(0.0f);
	glm::mat4 axes=glm::mat4(1.0f);axes[3]=glm::vec4(1.0f,1.0f,1.0f,0.0f);
	glm::vec2 spos1=world2screen((glm::vec3)(m_handlespace[3]));//position of transformated object on the screen
	glm::vec2 spos2=world2screen((glm::vec3)(m_handlespace[3]+m_handlespace*axes[m_axisnum]));//spos1,spos2 - project two points on screen - project axis on screen
	glm::vec2 dir=spos2-spos1;//the axis in screen space

	if(glm::length(dir)<0.01f){dir[0]=1.0f;}//axis length must not be zero



	glm::mat2 mov=glm::mat2(dir,glm::vec2(dir[1],-dir[0]));

	if(m_axisnum2!=-1){
		glm::vec2 spos22=world2screen((glm::vec3)(m_handlespace[3]+m_handlespace*axes[m_axisnum2]));//project two points on screen - project axis on screen
		glm::vec2 dir2=spos22-spos1;//the axis in screen space
		if(glm::length(dir2)<0.01){dir2[1]=1.0f;}//axis length must not be zero
		mov[1]=dir2;
	}

	mov=glm::inverse(glm::mat2(glm::normalize(mov[0]),glm::normalize(mov[1])));
			
		
	glm::vec2 drag,olddrag,dragfinal,mouse;

	mouse = glm::vec2(InputManager::m_mouseX, World2::height - InputManager::m_mouseY);
	drag=mov*(mouse-spos1);
	mouse = glm::vec2(InputManager::m_mouseXPrev,World2::height - InputManager::m_mouseYPrev);
	olddrag=mov*(mouse-spos1);
	dragfinal=drag-olddrag;

	drag3+=((glm::vec3)axes[m_axisnum])*(dragfinal[0]);
	if(m_axisnum2!=-1){drag3+=((glm::vec3)axes[m_axisnum2])*(dragfinal[1]);}
			
	float depth=glm::length(World2::mainCamPos+(glm::vec3)m_handlespace[3]);//add, not substract - moving camera is moving world in opposite direction
	drag3*=depth*0.5f;
	if(InputManager::isKeyPressed(Keys::shiftr)){drag3*=0.25f;}

	///
	drag3*=0.005f;
	glm::vec3 scal=glm::vec3(glm::length((glm::vec3)m_bkp[0]),glm::length((glm::vec3)m_bkp[1]),glm::length((glm::vec3)m_bkp[2]));
					
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
					
	m_editedobj->transformation[0]+=m_bkp[0]*drag3[0]/scal[0];//TMP
	m_editedobj->transformation[1]+=m_bkp[1]*drag3[1]/scal[1];//TMP
	m_editedobj->transformation[2]+=m_bkp[2]*drag3[2]/scal[2];//TMP
}
