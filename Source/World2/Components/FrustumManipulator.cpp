#include "FrustumManipulator.h"
#include "Core/Input/InputManager.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "Camera2.h"
#include "pgr.h"
#include "ManipulatorUtil.h"
#include <typeinfo>

const char* FrustumManipulator::typeStatic=NULL;

FrustumManipulator::FrustumManipulator(){
	FrustumManipulator::typeStatic=typeid(FrustumManipulator).name();
	type= FrustumManipulator::typeStatic;
		
	for(int i=0;i<6;i++){m_stencils.arr[i]=ManipulatorUtil::getStencil(i);}

	m_hposs[0]=glm::vec4(0.0f,0.0f,1.0f,1.0f);
	m_hposs[1]=glm::vec4(0.0f,0.0f,-1.0f,1.0f);
	m_hposs[2]=glm::vec4(0.0f,1.0f,0.0f,1.0f);
	m_hposs[3]=glm::vec4(0.0f,-1.0f,0.0f,1.0f);
	m_hposs[4]=glm::vec4(1.0f,0.0f,0.0f,1.0f);
	m_hposs[5]=glm::vec4(-1.0f,0.0f,0.0f,1.0f);

	m_frustrum = new GameObject(unitcubeMesh, &World2::shaderProj, 0);
	m_frustrum->color=glm::vec4(0.5f,0.5f,0.5f,0.5f);
	m_frustruml = new GameObject(cubelinesMesh, &World2::shaderProj, 0);
	m_frustruml->primitive = GL_LINES;
	m_frustruml->color = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	m_cameraico = new GameObject(cameraicoMesh, &World2::shaderHandle, 0);
	m_cameraico->rotate(glm::vec3(1.0f,0.0f,0.0f),-90.0f);
	m_handle = new GameObject(unitcubeMesh, &World2::shaderHandle, 0);
}
void FrustumManipulator::start(){}

void FrustumManipulator::render(glm::mat4*parent,bool renderTransparent){
	if(m_editednode==NULL){return;}
	glm::mat4 projinv=glm::inverse(m_edited);;
	//glm::mat4 transform=(*parent)*owner->transformation;//TMP
	glm::mat4 transform=glm::mat4(1.0f);
	glm::vec4 pos=transform[3];transform=getRotation(transform,0);transform[3]=pos;

	if(renderTransparent){
		glUseProgram(World2::shaderProj.program);
		glUniformMatrix4fv(glGetUniformLocation(World2::shaderProj.program, "P2Matrix"), 1, GL_FALSE, glm::value_ptr(projinv));
		glDisable(GL_CULL_FACE);
		m_frustrum->draw(transform);
		m_frustruml->draw(transform);
		glEnable(GL_CULL_FACE);
	}
	else{
		glDepthRange(0.0, 0.01);
		glUseProgram(World2::shaderHandle.program);

		glStencilMask(255);
		for(int i=0;i<6;i++){
			glStencilFunc(GL_ALWAYS, m_stencils.arr[i], 255);
			glm::vec3 col=glm::vec4((float)(m_activehandle==m_stencils.arr[i]));
			col[(5-i)/2]=1.0f;

			m_handle->color=glm::vec4(col[0],col[1],col[2],1.0f);

			glm::vec4 pos=projinv*m_hposs[i];
			pos/=pos[3];

			if(m_hposs[i][2]==0.0f){//move side handles to middle - between far plane and near plane
				glm::vec4 f=glm::vec4(m_hposs[i][0],m_hposs[i][1],1.0f,1.0f);
				glm::vec4 n=glm::vec4(m_hposs[i][0],m_hposs[i][1],-1.0f,1.0f);
				glm::vec4 pn=(projinv*f)/(projinv*f)[3];
				glm::vec4 pf=(projinv*n)/(projinv*n)[3];
						
				pos=((pf-pn)*0.5f+pn);
			}

			float depth=(World2::perspective*World2::mainCamera*transform*pos)[2]; 
			m_handle->transformation=glm::mat4(depth*0.01f+0.1f);
			m_handle->transformation[3]=pos;

			m_handle->draw(transform);
			//printf("%d, %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3]);//getchar();
		}
		glStencilMask(0);

		glm::vec4 mov=projinv*glm::vec4(0.0f,0.0f,-1.0f,1.0f);mov/=mov[3];mov[3]=0.0f;mov[2]+=1.5f;
		mov[2] = 1.5f;

		m_cameraico->transformation[3]+=mov;
		m_cameraico->color=glm::vec4(0.7f,0.7f,0.7f,1.0f);
		m_cameraico->draw(transform);
		m_cameraico->transformation[3]-=mov;
		glDepthRange(0.0, 1.0);
	}
}
void FrustumManipulator::update(){
	if(m_editednode==NULL){return;}
	Core::Frustum*editedfrustum=(Core::Frustum*)m_editednode->get();
	m_edited=m_editednode->get()->getData().getMat4();
	m_left=		editedfrustum->getLeft();
	m_right=	editedfrustum->getRight();
	m_top=		editedfrustum->getTop();
	m_bottom=	editedfrustum->getBottom();
	m_near=		editedfrustum->getNear();
	m_far=		editedfrustum->getFar();
	//m_edited=glm::perspective(glm::radians(m_angle),m_aspect,m_near,m_ar);

	if (InputManager::isKeyJustUp(Keys::mouseLeft)) {m_activehandle=-1;}
		
	if (InputManager::isKeyJustPressed(Keys::mouseLeft)){
		unsigned char sel=Select::getStencilAt((int)InputManager::m_mouseX,(int)(World2::height- InputManager::m_mouseY),3,-1);
		m_activehandle=-1;
		for(int i=0;i<6;i++){if(sel==m_stencils.arr[i]){m_activehandle=m_stencils.arr[i];m_axisnum=i;}}
	}
		
	if(m_activehandle==-1){return;}

	if(InputManager::isKeyJustUp(Keys::esc)){}
	glm::mat4 projinv=glm::inverse(m_edited);
	glm::vec3 drag3=glm::vec3(0.0f);
	glm::vec4 axis=m_hposs[m_axisnum];
	glm::vec4 pos=projinv*m_hposs[m_axisnum];

	pos/=pos[3];
			
	if(m_hposs[m_axisnum][2]==0.0f){//move side handles to middle - between far plane and near plane
		glm::vec4 f=glm::vec4(m_hposs[m_axisnum][0],m_hposs[m_axisnum][1],1.0f,1.0f);
		glm::vec4 n=glm::vec4(m_hposs[m_axisnum][0],m_hposs[m_axisnum][1],-1.0f,1.0f);
		glm::vec4 pn=(projinv*f)/(projinv*f)[3];
		glm::vec4 pf=(projinv*n)/(projinv*n)[3];
				
		pos=((pf-pn)*0.5f+pn);
	}
			
	pos[3]=0.0f;
	axis[3]=0.0f;
	//glm::mat4 handlespace=getFullTransform(m_owner);//TMP
	glm::mat4 handlespace=glm::mat4(1.0f);
			
	glm::vec2 spos1=world2screen((glm::vec3)(handlespace[3]+handlespace*pos));//position of transformated object on the screen
	glm::vec2 spos2=world2screen((glm::vec3)(handlespace[3]+handlespace*(pos+axis*axis)));//spos1,spos2 - project two points on screen - project axis on screen
	glm::vec2 dir=spos2-spos1;//the axis in screen space
	if(glm::length(dir)<0.01){dir[0]=1.0f;}//axis length must not be zero
		
	glm::mat2 mov=glm::mat2(dir,glm::vec2(dir[1],-dir[0]));
	mov=glm::inverse(glm::mat2(glm::normalize(mov[0]), glm::normalize(mov[1])));

	glm::vec2 dragfinal=mov*glm::vec2(InputManager::m_mouseXDelta,-InputManager::m_mouseYDelta)*0.05f;

	if(m_hposs[m_axisnum][2]==0.0f){dragfinal*=0.25f;}//decrease sensitivity for side handles
			
	if(m_activehandle==m_stencils.names.n){
		m_near-=dragfinal[0];
		if(m_near<0.01f){m_near=0.01f;}
		if(m_near>m_far-0.5f){m_near=m_far-0.5f;}
	}
	else if(m_activehandle==m_stencils.names.f){
		m_far-=dragfinal[0];
		if(m_far<m_near+0.5f){m_far=m_near+0.5f;}
	}
	else if(m_activehandle==m_stencils.names.wl){
		//printf("height %f\n",( m_pfar*tan(glm::radians(m_pangle*0.5f)) ));
		//printf("width %f\n",( m_pfar*tan(glm::radians(m_pangle*0.5f)) )/( m_pheight/m_paspect ));
		//printf("%f\n",glm::degrees(atan(1.0f)));
		m_left+=dragfinal[0];
		if(m_left>m_right-0.5f){m_left=m_right-0.5f;}
	}
	else if(m_activehandle==m_stencils.names.wr){
		m_right+=dragfinal[0];
		if(m_right<m_left+0.5f){m_right=m_left+0.5f;}
	}
	else if(m_activehandle==m_stencils.names.hu){
		float sign=(float)(m_activehandle==m_stencils.names.hu)*2.0f-1.0f;

		m_top+=dragfinal[0];
		if(m_top<m_bottom+0.5f){m_top=m_bottom+0.5f;}
	}
	else if(m_activehandle==m_stencils.names.hb){
		//printf("hb\n");
		m_bottom+=dragfinal[0];
		if(m_bottom>m_top-0.5f){m_bottom=m_top-0.5f;}
	}
		//printf("%f\n",m_height);
	//printf("%f\n",pheight);
	///
	editedfrustum->setLeft(m_left);
	editedfrustum->setRight(m_right);
	editedfrustum->setTop(m_top);
	editedfrustum->setBottom(m_bottom);
	editedfrustum->setNear(m_near);
	editedfrustum->setFar(m_far);
	m_edited=m_editednode->get()->getData().getMat4();
}
