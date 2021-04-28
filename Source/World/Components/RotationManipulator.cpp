#include "RotationManipulator.h"

#include "Core/Input/InputManager.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "Renderer.h"
#include "glm/gtx/norm.hpp"
#include "imgui.h"
#include "ManipulatorUtil.h"

const char* RotationManipulator::s_type = nullptr;

RotationManipulator::RotationManipulator() {
    RotationManipulator::s_type = typeid(RotationManipulator).name();
    m_type = RotationManipulator::s_type;

	m_stencilx = ManipulatorUtil::getStencil(0);
	m_stencily = ManipulatorUtil::getStencil(1);
	m_stencilz = ManipulatorUtil::getStencil(2);

	m_circleh =	new GameObject(unitcircleMesh,	&World::shaderHandle,	0);
	m_threeaxis=new GameObject(three_axisMesh,	&World::shader0,		World::axisTexture);		
	m_threeaxis->color=glm::vec4(2.0f,2.0f,2.0f,1.0f);
	m_threeaxis->primitive=GL_LINES;
	m_edited=glm::mat4(1.0f);
	
	m_editednode=Ptr<Core::NodeBase>();
	m_parent=Ptr<Core::Sequence>();
}
void RotationManipulator::render(glm::mat4* parent, bool renderTransparent) {
	if(m_editednode.get()==nullptr){return;}
	if(!renderTransparent){return;}

	float depth=(World::perspective*World::mainCamera*m_handlespace[3])[2];
	glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(depth*0.05f+0.5f));

	//glm::mat4 ftransform=getFullTransform(m_edited);//TMP
	//glm::mat4 ftransform=m_edited;//full transform from nodebase
	glm::mat4 ftransform=getNodeTransform(&m_editednode,&m_parent)*m_editednode->getData().getMat4();
	ftransform[0][3]=0.0f;
	ftransform[1][3]=0.0f;
	ftransform[2][3]=0.0f;
	ftransform[3][3]=1.0f;

	glUseProgram(World::shaderHandle.program);
	glDepthRange(0.0, 0.01);

	m_threeaxis->draw(ftransform);

	m_circleh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f))*scale;
	if(m_allowedaxis&s_x){
		ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,0),glm::vec4(1.0f,0.0f,0.0f,1.0f),m_stencilx,m_activehandle,m_hoverhandle);
	}
	else{ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,0),glm::vec4(0.5f,0.5f,0.5f,1.0f),-1,false,false);}

	m_circleh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f))*scale;
	if(m_allowedaxis&s_y){
		ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,1),glm::vec4(0.0f,1.0f,0.0f,1.0f),m_stencily,m_activehandle,m_hoverhandle);
	}
	else{ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,0),glm::vec4(0.5f,0.5f,0.5f,1.0f),-1,false,false);}

	m_circleh->transformation=glm::mat4(1.0f)*scale;
	if(m_allowedaxis&s_z){
		ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,2),glm::vec4(0.1f,0.4f,1.0f,1.0f),m_stencilz,m_activehandle,m_hoverhandle);
	}
	else{ManipulatorUtil::drawHandle(m_circleh,getOrtho(m_handlespace,0),glm::vec4(0.5f,0.5f,0.5f,1.0f),-1,false,false);}

	glDepthRange(0.0, 1.0);
	CHECK_GL_ERROR();
}
void RotationManipulator::update() {
	if(m_editednode.get()==nullptr){return;}
	
	m_allowedaxis=0;
	const char*oper= m_editednode->getOperation()->keyWord.c_str();
	if(strcmp(oper,"EulerX")==0)		{m_allowedaxis|=s_x;m_edited=m_editednode->getData().getMat4();}
	else if(strcmp(oper,"EulerY")==0)	{m_allowedaxis|=s_y;m_edited=m_editednode->getData().getMat4();}
	else if(strcmp(oper,"EulerZ")==0)	{m_allowedaxis|=s_z;m_edited=m_editednode->getData().getMat4();}
	else if(strcmp(oper,"AxisAngle")==0){m_allowedaxis|=s_x;m_edited=m_editednode->getData().getMat4();}
	else if(strcmp(oper,"Quat")==0){
		//m_allowedaxis|=s_x|s_y|s_z;
		//Core::QuatRot* editedquat = (Core::QuatRot*)m_editednode.get();
		
		//editedquat->
	}
	///
	bool transactionBegin=false;

	unsigned char sel =Select::getStencilAt((int)InputManager::m_mouseX, (int)(World::height - InputManager::m_mouseY), 3, -1);

	m_hoverhandle=-1;
	if(m_activehandle==-1){
		if(sel==m_stencilx&&(m_allowedaxis&s_x)){		m_hoverhandle=m_stencilx;}//manipulating handles clicked
		else if(sel==m_stencily&&(m_allowedaxis&s_y)){	m_hoverhandle=m_stencily;}
		else if(sel==m_stencilz&&(m_allowedaxis&s_z)){	m_hoverhandle=m_stencilz;}
	}
	if(InputManager::isKeyJustPressed(Keys::mouseLeft)){
		m_activehandle=-1;
		if(sel==m_stencilx&&(m_allowedaxis&s_x)){		m_activehandle=m_stencilx;	m_axisnum=0;}//manipulating handles clicked
		else if(sel==m_stencily&&(m_allowedaxis&s_y)){	m_activehandle=m_stencily;	m_axisnum=1;}
		else if(sel==m_stencilz&&(m_allowedaxis&s_z)){	m_activehandle=m_stencilz;	m_axisnum=2;}
		if(m_activehandle!=-1){transactionBegin=true;}
	}
	if(InputManager::isKeyJustUp(Keys::mouseLeft)){m_activehandle=-1;}

	if(m_hoverhandle!=-1||m_activehandle!=-1){ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);}

	//m_handlespace=getNormalized(getFullTransform(m_editedobj));//TMP
	//m_handlespace=m_edited;
	m_handlespace=getNormalized(getNodeTransform(&m_editednode,&m_parent)*m_editednode->getData().getMat4());
	m_handlespace[0][3]=0.0f;
	m_handlespace[1][3]=0.0f;
	m_handlespace[2][3]=0.0f;
	m_handlespace[3][3]=1.0f;

	if(m_activehandle==-1){return;}

	///
	glm::mat4 axes=glm::mat4(1.0f);axes[3]=glm::vec4(1.0f,1.0f,1.0f,0.0f);
	glm::mat2 mov =glm::mat2(1.0f);

	if (transactionBegin) {
		glm::mat4 ortho=getOrtho(m_handlespace,m_axisnum);
		glm::vec3 p0 = (glm::vec3)m_handlespace[3];
		glm::vec3 px = (glm::vec3)(ortho * axes[(m_axisnum+1)%3]);
		glm::vec3 py = (glm::vec3)(ortho * axes[(m_axisnum+2)%3]);
		glm::vec3 t0 = -World::mainCamPos;
		//glm::vec3 tz = mouseray(world2screen(p0) +glm::vec2(InputController::m_mouseXDelta, -InputController::m_mouseYDelta));
		glm::vec3 tz = mouseray(glm::vec2(InputManager::m_mouseX,World::height - InputManager::m_mouseY));
		glm::vec3 coef = glm::inverse(glm::mat3(-tz, px, py)) * (t0 - p0);

		glm::vec3 pc = px*coef[1]+py*coef[2];
		glm::vec3 dir3 = glm::normalize(glm::cross(pc, (glm::vec3)axes[m_axisnum]));
		m_dirbkp = glm::normalize(world2screen(p0)-world2screen(p0 + dir3));
	}
	mov[0]=m_dirbkp;
	mov[1]=glm::vec2(mov[0][1],-mov[0][0]);

	if(glm::length(mov[0])<0.01f){mov[0][0]=1.0f;}//axis length must not be zero
	if(glm::length(mov[1])<0.01f){mov[1][1]=1.0f;}//axis length must not be zero

	mov=glm::inverse(glm::mat2(glm::normalize(mov[0]),glm::normalize(mov[1])));
		
	glm::vec2 drag2=mov*glm::vec2(InputManager::m_mouseXDelta,-InputManager::m_mouseYDelta);
	glm::vec3 drag3=((glm::vec3)axes[m_axisnum])*drag2[0];

	if(InputManager::isKeyPressed(Keys::shiftr)){drag3*=0.25f;}

	///
	drag3*=1.0f;
	glm::mat4 rot=glm::rotate(glm::mat4(1.0f),glm::radians(drag3[m_axisnum]),(glm::vec3)m_edited[m_axisnum]);
	glm::vec4 bkp=m_edited[3];
	m_edited[3]=glm::vec4(0.0f,0.0f,0.0f,1.0f);
	m_edited=rot*m_edited;
	m_edited[3]=bkp;
	///

	float angle=0.0f;
	if(m_axisnum==0){
		glm::vec3 v=(glm::vec3)(m_edited*glm::vec4(0.0f,1.0f,0.0f,0.0f));
		angle=angle2(v[1],v[2]);
	}
	else if(m_axisnum==1){
		glm::vec3 v=(glm::vec3)(m_edited*glm::vec4(0.0f,0.0f,1.0f,0.0f));
		angle=angle2(v[2],v[0]);
	}
	else if(m_axisnum==2){
		glm::vec3 v=(glm::vec3)(m_edited*glm::vec4(1.0f,0.0f,0.0f,0.0f));
		angle=angle2(v[0],v[1]);
	}
	//printf("angle %f, mov %f\n",angle,drag3[m_axisnum]);
	if(strcmp(oper,"Quat")!=0){
		m_editednode->setValue(glm::radians(angle));
	}
	else {

	}
}