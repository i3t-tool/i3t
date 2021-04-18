#include "CameraHandles.h"
#include "Core/Input/InputManager.h"
#include "../HardcodedMeshes.h"
#include "../Select.h"
#include "../Transforms.h"
#include "../World2.h"
#include <iostream>
#include <typeinfo>

const char* CameraHandles::typeStatic=NULL;

CameraHandles::CameraHandles(){
	CameraHandles::typeStatic=typeid(CameraHandles).name();
	this->type=CameraHandles::typeStatic;
		
	this->editedcam=Select::registerStencil(this);
	for(int i=0;i<6;i++){this->stencils.arr[i]=Select::registerStencil(this);}

	this->hposs[0]=glm::vec4(0.0f,0.0f,1.0f,1.0f);
	this->hposs[1]=glm::vec4(0.0f,0.0f,-1.0f,1.0f);
	this->hposs[2]=glm::vec4(0.0f,1.0f,0.0f,1.0f);
	this->hposs[3]=glm::vec4(0.0f,-1.0f,0.0f,1.0f);
	this->hposs[4]=glm::vec4(1.0f,0.0f,0.0f,1.0f);
	this->hposs[5]=glm::vec4(-1.0f,0.0f,0.0f,1.0f);
}
void CameraHandles::start(){
	this->cam = (Camera2*)this->owner->getComponent(Camera2::componentType());
	this->frustrum = new GameObject(unitcubeMesh, &World2::shaderProj, 0);
	this->frustrum->color=glm::vec4(0.5f,0.5f,0.5f,0.5f);
	this->frustruml = new GameObject(cubelinesMesh, &World2::shaderProj, 0);
	this->frustruml->primitive = GL_LINES;
	this->frustruml->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->cameraico = new GameObject(cameraicoMesh, &World2::shaderHandle, 0);
	this->cameraico->rotate(glm::vec3(1.0f,0.0f,0.0f),-90.0f);
	this->handle = new GameObject(unitcubeMesh, &World2::shaderHandle, 0);
}

void CameraHandles::render(glm::mat4*parent,bool renderTransparent){
	glm::mat4 projinv=glm::inverse(this->cam->perspective);
	glm::mat4 transform=(*parent)*this->owner->transformation;
	glm::vec4 pos=transform[3];transform=getRotation(transform,0);transform[3]=pos;
		
	glUseProgram(World2::shaderProj.program);
	glUniformMatrix4fv(glGetUniformLocation(World2::shaderProj.program, "P2Matrix"), 1, GL_FALSE, glm::value_ptr(projinv));
	if(renderTransparent){
		if(this->isEdit){
			glDisable(GL_CULL_FACE);
			this->frustrum->draw(transform);
			this->frustruml->draw(transform);
			glEnable(GL_CULL_FACE);
		}
	}
	else{
		glDepthRange(0.0, 0.01);
		glUseProgram(World2::shaderHandle.program);

		if(this->isEdit){
			glStencilMask(255);
			for(int i=0;i<6;i++){
				glStencilFunc(GL_ALWAYS, this->stencils.arr[i], 255);
				glm::vec3 col=glm::vec4((float)(this->activehandle==this->stencils.arr[i]));
				col[(5-i)/2]=1.0f;
					
				this->handle->color=glm::vec4(col[0],col[1],col[2],1.0f);
					
				glm::vec4 pos=projinv*this->hposs[i];
				pos/=pos[3];
					
				if(this->hposs[i][2]==0.0f&&(this->editmode==CameraHandles::EDIT_PERSPECTIVE||this->editmode==CameraHandles::EDIT_FRUSTUM)){//move side handles to middle - between far plane and near plane
					glm::vec4 f=glm::vec4(this->hposs[i][0],this->hposs[i][1],1.0f,1.0f);
					glm::vec4 n=glm::vec4(this->hposs[i][0],this->hposs[i][1],-1.0f,1.0f);
					glm::vec4 pn=(projinv*f)/(projinv*f)[3];
					glm::vec4 pf=(projinv*n)/(projinv*n)[3];
						
					pos=((pf-pn)*0.5f+pn);
				}

				float depth=(World2::perspective*World2::mainCamera*transform*pos)[2];
				this->handle->transformation=glm::mat4(depth*0.01f+0.1f);
				this->handle->transformation[3]=pos;

				this->handle->draw(transform);
				//printf("%d, %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3]);//getchar();
			}
			glStencilMask(0);
		}
		glm::vec4 mov=projinv*glm::vec4(0.0f,0.0f,-1.0f,1.0f);mov/=mov[3];mov[3]=0.0f;mov[2]+=1.5f;
		if(this->editmode==CameraHandles::EDIT_PERSPECTIVE||this->editmode==CameraHandles::EDIT_FRUSTUM){mov[2]=1.5f;}
		this->cameraico->transformation[3]+=mov;
		
		float color=(float)this->isEdit*0.2f+0.8f;
		this->cameraico->color=glm::vec4(color,color,color,1.0f);
		glStencilMask(255);
		glStencilFunc(GL_ALWAYS, this->editedcam, 255);
		this->cameraico->draw(transform);
		glStencilMask(0);
		glDepthRange(0.0, 1.0);
		this->cameraico->transformation[3]-=mov;
	}
	
}
	
void CameraHandles::update(){
	if (InputManager::isKeyJustUp(Keys::mouseLeft)) {
		unsigned char sel=Select::getStencilAt((int)InputManager::m_mouseX,(int)(World2::height- InputManager::m_mouseY),0,-1);
		if(sel==this->editedcam){this->isEdit=true;printf("is edit cam-handles\n");}
		else if(this->activehandle==-1){this->isEdit=false;}
			
		this->activehandle=-1;
	}
	if(!this->isEdit){return;}
		
	if(InputManager::isKeyPressed(Keys::o)){this->editmode=CameraHandles::EDIT_ORTHO;}
	if(InputManager::isKeyPressed(Keys::p)){this->editmode = CameraHandles::EDIT_PERSPECTIVE;}
	if(InputManager::isKeyPressed(Keys::f)){this->editmode=CameraHandles::EDIT_FRUSTUM;}
		
	if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){this->cam->perspective=glm::perspective(glm::radians(this->pangle),this->paspect,this->pnear,this->pfar);}
	else if(this->editmode==CameraHandles::EDIT_ORTHO){this->cam->perspective=glm::ortho(this->left,this->right,this->bottom,this->top,this->onear,this->ofar);}
	else if(this->editmode==CameraHandles::EDIT_FRUSTUM){this->cam->perspective=glm::frustum(this->fleft,this->fright,this->fbottom,this->ftop,this->fnear,this->ffar);}
		
	if (InputManager::isKeyJustPressed(Keys::mouseLeft)){
		unsigned char sel=Select::getStencilAt((int)InputManager::m_mouseX,(int)(World2::height- InputManager::m_mouseY),3,this->editedcam);
		this->activehandle=-1;
		for(int i=0;i<6;i++){if(sel==this->stencils.arr[i]){this->activehandle=this->stencils.arr[i];this->axisnum=i;}}
	}
		
	if(InputManager::isKeyJustUp(Keys::e)){
		printf("p params angle %3.2f, aspect %3.2f, far %3.2f, near %3.2f\n",this->pangle,this->paspect,this->pfar,this->pnear);
			
		glm::mat4 projinv=glm::inverse(this->cam->perspective);
		glm::vec4 pos=projinv*glm::vec4(0.0f,0.0f,-1.0f,1.0f);pos/=pos[3];float _near=-pos[2];
		pos=projinv*glm::vec4(0.0f,0.0f,1.0f,1.0f);pos/=pos[3];float _far=-pos[2];
		pos=projinv*glm::vec4(0.0f,1.0f,1.0f,1.0f);pos/=pos[3];float height=pos[1];
		pos=projinv*glm::vec4(1.0f,0.0f,1.0f,1.0f);pos/=pos[3];float width=pos[0];
		printf("p calc\t angle %3.2f, aspect %3.2f, far %3.2f, near %3.2f\n\n",glm::degrees(atan(height/(_far)))*2.0f,width/height,_far,_near);
	}
		
	if(this->activehandle!=-1){
		if(InputManager::isKeyJustUp(Keys::esc)){}
		glm::mat4 projinv=glm::inverse(this->cam->perspective);
		glm::vec3 drag3=glm::vec3(0.0f);
		glm::vec4 axis=this->hposs[this->axisnum];
		glm::vec4 pos=projinv*this->hposs[this->axisnum];
		//if(pos[2]==0.0f&&this->editmode==CameraHandles::EDIT_PERSPECTIVE){pos[2]=1.0f;}//move handles to front			
		pos/=pos[3];
			
		if(this->hposs[this->axisnum][2]==0.0f&&(this->editmode==CameraHandles::EDIT_PERSPECTIVE||this->editmode==CameraHandles::EDIT_FRUSTUM)){//move side handles to middle - between far plane and near plane
			glm::vec4 f=glm::vec4(this->hposs[this->axisnum][0],this->hposs[this->axisnum][1],1.0f,1.0f);
			glm::vec4 n=glm::vec4(this->hposs[this->axisnum][0],this->hposs[this->axisnum][1],-1.0f,1.0f);
			glm::vec4 pn=(projinv*f)/(projinv*f)[3];
			glm::vec4 pf=(projinv*n)/(projinv*n)[3];
				
			pos=((pf-pn)*0.5f+pn);
		}
			
		pos[3]=0.0f;
		axis[3]=0.0f;
		glm::mat4 handlespace=getFullTransform(this->owner);
			
		glm::vec2 spos1=world2screen((glm::vec3)(handlespace[3]+handlespace*pos));//position of transformated object on the screen
		glm::vec2 spos2=world2screen((glm::vec3)(handlespace[3]+handlespace*(pos+axis*axis)));//spos1,spos2 - project two points on screen - project axis on screen
		glm::vec2 dir=spos2-spos1;//the axis in screen space
		if(glm::length(dir)<0.01){dir[0]=1.0f;}//axis length must not be zero
			
		glm::mat2 mov=glm::mat2(dir,glm::vec2(dir[1],-dir[0]));			
		mov[0]=glm::normalize(mov[0]);
		mov[1]=glm::normalize(mov[1]);
			
		glm::vec2 drag,olddrag,dragfinal,mouse;

		mouse=glm::vec2(InputManager::m_mouseX,World2::height- InputManager::m_mouseY);
		drag=glm::inverse(mov)*(mouse-spos1);
		mouse=glm::vec2(InputManager::m_mouseX- InputManager::m_mouseXDelta,World2::height- InputManager::m_mouseY+ InputManager::m_mouseYDelta);
		olddrag=glm::inverse(mov)*(mouse-spos1);
		dragfinal=drag-olddrag;
		dragfinal*=0.05f;
			
		if(this->activehandle==this->stencils.names.n){
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				this->pnear-=dragfinal[0];
				if(this->pnear<0.1f){this->pnear=0.1f;}
				if(this->pnear>this->pfar-1.0f){this->pnear=this->pfar-1.0f;}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->onear-=dragfinal[0];
				if(this->onear>this->ofar-0.5f){this->onear=this->ofar-0.5f;}
			}
			else{
				if(this->fnear>this->ffar-0.5f){this->fnear=this->ffar-0.5f;}
				this->fnear-=dragfinal[0];
			}
		}
		else if(this->activehandle==this->stencils.names.f){
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				this->pfar-=dragfinal[0]*2.0f;
				if(this->pfar<this->pnear+1.0f){this->pfar=this->pnear+1.0f;}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->ofar-=dragfinal[0];
				if(this->ofar<this->onear+0.5f){this->ofar=this->onear+0.5f;}
			}
			else{
				if(this->ffar<this->fnear+0.5f){this->ffar=this->fnear+0.5f;}
				this->ffar-=dragfinal[0];
			}
		}
		else if(this->activehandle==this->stencils.names.wl){
			//printf("height %f\n",( this->pfar*tan(glm::radians(this->pangle*0.5f)) ));
			//printf("width %f\n",( this->pfar*tan(glm::radians(this->pangle*0.5f)) )/( this->pheight/this->paspect ));
			//printf("%f\n",glm::degrees(atan(1.0f)));
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				this->paspect+=-dragfinal[0]*0.09f;
				if(this->paspect<0.1f){this->paspect=0.1f;}
				else if(this->paspect>10.0f){this->paspect=10.0f;}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->left+=dragfinal[0];
				if(this->left>this->right-0.5f){this->left=this->right-0.5f;}
			}
			else{
				this->fleft+=dragfinal[0];
				if(this->fleft>this->fright-0.5f){this->fleft=this->fright-0.5f;}
			}
		}
		else if(this->activehandle==this->stencils.names.wr){
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				this->paspect+=+dragfinal[0]*0.09f;
				if(this->paspect<0.1f){this->paspect=0.1f;}
				else if(this->paspect>10.0f){this->paspect=10.0f;}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->right+=dragfinal[0];
				if(this->right<this->left+0.5f){this->right=this->left+0.5f;}
			}
			else{
				this->fright+=dragfinal[0];
				if(this->fright<this->fleft+0.5f){this->fright=this->fleft+0.5f;}
			}
		}
		else if(this->activehandle==this->stencils.names.hu){
			float sign=(float)(this->activehandle==this->stencils.names.hu)*2.0f-1.0f;
				
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				float amount=dragfinal[0]*5.0f*sign;
				if(this->pangle+amount<1.0f){amount=1.0f-this->pangle;}
				else if(this->pangle+amount>175.0f){amount=175.0f-this->pangle;}
					
				float f=tan(glm::radians((this->pangle+amount)*0.5f))/tan(glm::radians(this->pangle*0.5f));

				float f2=1.0f;
				if(this->paspect<0.1f*f){f2=10.0f*(this->paspect);}
				else if(this->paspect>10.0f*f){f2=0.1f*(this->paspect);}
				else{this->paspect/=f;this->pangle+=amount;}
				//if(f2!=1.0f){
				amount=glm::degrees(atan(f2*tan(glm::radians(this->pangle*0.5f))))*2.0f-this->pangle;
				this->paspect/=f2;
				this->pangle+=amount;
				//}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->top+=dragfinal[0]*sign;
				if(this->top<this->bottom+0.5f){this->top=this->bottom+0.5f;}
			}
			else{
				this->ftop+=dragfinal[0];
				if(this->ftop<this->fbottom+0.5f){this->ftop=this->fbottom+0.5f;}
			}
		}
		else if(this->activehandle==this->stencils.names.hb){
			//printf("hb\n");
			if(this->editmode==CameraHandles::EDIT_PERSPECTIVE){
				float amount=-dragfinal[0]*5.0f;
				if(this->pangle+amount<1.0f){amount=1.0f-this->pangle;}
				else if(this->pangle+amount>175.0f){amount=175.0f-this->pangle;}
					
				float f=tan(glm::radians((this->pangle+amount)*0.5f))/tan(glm::radians(this->pangle*0.5f));

				float f2=1.0f;
				if(this->paspect<0.1f*f){f2=10.0f*(this->paspect);}
				else if(this->paspect>10.0f*f){f2=0.1f*(this->paspect);}
				else{this->paspect/=f;this->pangle+=amount;}
				//if(f2!=1.0f){
				amount=glm::degrees(atan(f2*tan(glm::radians(this->pangle*0.5f))))*2.0f-this->pangle;
				this->paspect/=f2;
				this->pangle+=amount;
				//}
			}
			else if(this->editmode==CameraHandles::EDIT_ORTHO){
				this->bottom+=dragfinal[0];
				if(this->bottom>this->top-0.5f){this->bottom=this->top-0.5f;}
			}
			else{
				this->fbottom+=dragfinal[0];
				if(this->fbottom>this->ftop-0.5f){this->fbottom=this->ftop-0.5f;}
			}
		}
		//printf("%f\n",this->pheight);
	}
		
}
	

