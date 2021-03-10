#include <iostream>
#include <typeinfo>
#include <math.h>
#include "glm/gtx/norm.hpp"
#include "TransformHandles.h"
#include "Renderer.h"
#include "../HardcodedMeshes.h"
#include "../World2.h"
#include "../Transforms.h"
#include "../Select.h"
#include "../../Core/InputController.h"

void printMatrix2(glm::mat4 m){
  printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n",
	  m[0][0], m[1][0], m[2][0], m[3][0],
	  m[0][1], m[1][1], m[2][1], m[3][1],
	  m[0][2], m[1][2], m[2][2], m[3][2],
	  m[0][3], m[1][3], m[2][3], m[3][3]);
}

const char* TransformHandles::typeStatic=NULL;
	
void TransformHandles::drawHandle(GameObject*_handle,glm::mat4 space,glm::vec4 color,int stencil,bool active){
	glStencilMask(255*(stencil!=-1));
	glStencilFunc(GL_ALWAYS, (unsigned char)stencil, 255);
	glUseProgram(World2::shaderHandle.program);
	if (active) {_handle->color=glm::vec4(1.0f,1.0f,1.0f,color[3]);}
	else{_handle->color=glm::vec4(color[0],color[1],color[2],color[3]);}
	_handle->draw(space);
}
	
TransformHandles::TransformHandles(GameObject*_editedobj){
	TransformHandles::typeStatic=typeid(TransformHandles).name();
	this->type=TransformHandles::typeStatic;
		
	this->editedobj=_editedobj;
	this->stencilx=		Select::registerStencil(this);
	this->stencily=		Select::registerStencil(this);
	this->stencilz=		Select::registerStencil(this);
	this->stencilzy=	Select::registerStencil(this);
	this->stencilzx=	Select::registerStencil(this);
	this->stencilyx=	Select::registerStencil(this);
	this->stencilxyz=	Select::registerStencil(this);
	this->stencilaxisx=	Select::registerStencil(this);
	this->stencilaxisy=	Select::registerStencil(this);
	this->stencilaxisz=	Select::registerStencil(this);
	this->stencilaxisw=	Select::registerStencil(this);
}
void TransformHandles::start(){
	this->circleh =	new GameObject(unitcircleMesh,	&World2::shaderHandle, 0);
	this->arrowh =	new GameObject(arrowMesh,		&World2::shaderHandle, 0);
	this->planeh =	new GameObject(quadMesh,		&World2::shaderHandle, 0);
	this->scaleh =	new GameObject(scalearrowMesh,	&World2::shaderHandle, 0);
	this->uniscaleh=new GameObject(unitcubeMesh,	&World2::shaderHandle, 0);
	this->lineh =	new GameObject(lineMesh,		&World2::shaderHandle, 0);
	this->bkp=editedobj->transformation;
}
void TransformHandles::render(glm::mat4*parent,bool renderTransparent){
	if(!this->isEdit||!renderTransparent){return;}
	glDepthRange(0.0, 0.01);
	GameObject*handle=this->circleh;
	if(this->editmode==TransformHandles::EDIT_ROTATION){handle=this->circleh;}
	else if(this->editmode==TransformHandles::EDIT_POSITION||this->editmode==TransformHandles::EDIT_LOOKAT	){handle=this->arrowh;}
	else if(this->editmode==TransformHandles::EDIT_SCALE){handle=this->scaleh;}
		
	float depth=(World2::perspective*World2::mainCamera*this->handlespace[3])[2];
	glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(depth*0.05f+0.5f));
		
	glm::mat4 ftransform=getFullTransform(this->editedobj);
		
	float transparency=1.0f;//(float)(renderTransparent==false)*0.5f+0.5f;
	if(this->editspace==TransformHandles::EDIT_FREE||true){
		glm::mat4 t;
		float f,selected;
			
		selected=0.3f*(float)(this->editaxis==0);
		t=getRotation(ftransform,0);
		t=glm::mat4(t[0]*glm::length((glm::vec3)ftransform[0]),t[1]*(1.0f+selected*3.0f),t[2]*(1.0f+selected*3.0f),ftransform[3]);
		lineh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
		drawHandle(lineh,t,glm::vec4( 1.0f,selected,selected,transparency),this->stencilaxisx,false);
			
		selected=0.3f*(float)(this->editaxis==1);
		t=getRotation(ftransform,1);
		t=glm::mat4(t[0]*(1.0f+selected*3.0f),t[1]*glm::length((glm::vec3)ftransform[1]),t[2]*(1.0f+selected*3.0f),ftransform[3]);
		lineh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
		drawHandle(lineh,t,glm::vec4(selected,1.0f,selected,transparency),this->stencilaxisy,false);
			
		selected=0.3f*(float)(this->editaxis==2);	
		t=getRotation(ftransform,2);
		t=glm::mat4(t[0]*(1.0f+selected*3.0f),t[1]*(1.0f+selected*3.0f),t[2]*glm::length((glm::vec3)ftransform[2]),ftransform[3]);
		lineh->transformation=glm::mat4(1.0f);
		drawHandle(lineh,t,glm::vec4(selected*0.5f+0.1f,selected*0.5f+0.4f,1.0f,transparency),this->stencilaxisz,false);
			
		selected=0.3f*(float)(this->editaxis==3);
		glm::mat4 tt=glm::mat4(ftransform[1],ftransform[2],getFullTransform(this->editedobj->parent)[3]-ftransform[3],glm::vec4(0.0f));
		t=getRotation(tt,2);
		f=glm::length((glm::vec3)tt[2]);
		t=glm::mat4(t[0]*(1.0f+selected*3.0f),t[1]*(1.0f+selected*3.0f),t[2]*glm::length((glm::vec3)tt[2]),ftransform[3]);
		lineh->transformation=glm::mat4(1.0f);
		drawHandle(lineh,t,glm::vec4(1.0f,selected+0.2f,1.0f,transparency),this->stencilaxisw,false);
	}


	if(this->editspace==TransformHandles::EDIT_FREE&&this->editmode==TransformHandles::EDIT_SCALE){//override scale handles for free edit
		scale=glm::scale(scale, glm::vec3(0.09f));		
		if(this->editaxis==0){
			uniscaleh->transformation=glm::mat4(1.0f)*scale;
			drawHandle(uniscaleh,this->handlespace,glm::vec4( 1.0f,0.0f,0.0f,transparency),this->stencilx,this->activehandle==this->stencilx);	
		}
		else if(this->editaxis==1){
			uniscaleh->transformation=glm::mat4(1.0f)*scale;
			drawHandle(uniscaleh,this->handlespace,glm::vec4(0.0f,1.0f,0.0f,transparency),this->stencily,this->activehandle==this->stencily);
		}
		else if(this->editaxis==2){
			uniscaleh->transformation=glm::mat4(1.0f)*scale;
			drawHandle(uniscaleh,this->handlespace,glm::vec4(0.1f,0.4f,1.0f,transparency),this->stencilz,this->activehandle==this->stencilz);
		}
		else if(this->editaxis==3){
			uniscaleh->transformation=glm::mat4(1.0f)*scale;	
			drawHandle(uniscaleh,this->handlespace,glm::vec4(1.0f,0.2f,1.0f,transparency),this->stencilz,this->activehandle==this->stencilz);
		}
	}
	else{
		handle->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f))*scale;
		drawHandle(handle,getOrtho(this->handlespace,0),glm::vec4( 1.0f,0.0f,0.0f,transparency),this->stencilx,this->activehandle==this->stencilx);	
		handle->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f))*scale;
		drawHandle(handle,getOrtho(this->handlespace,1),glm::vec4(0.0f,1.0f,0.0f,transparency),this->stencily,this->activehandle==this->stencily);
		handle->transformation=glm::mat4(1.0f)*scale;
		drawHandle(handle,getOrtho(this->handlespace,2),glm::vec4(0.1f,0.4f,1.0f,transparency),this->stencilz,this->activehandle==this->stencilz);
			
		if (this->editmode == TransformHandles::EDIT_LOOKAT || this->editmode == TransformHandles::EDIT_POSITION ||this->editmode == TransformHandles::EDIT_SCALE){ // two-axis handles
			this->planeh->transformation=glm::mat4(1.0f)*scale;
				
			this->planeh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f))*scale;
			drawHandle(this->planeh,this->handlespace,glm::vec4(0.0f,1.0f,1.0f,transparency*0.6f),this->stencilzy,this->activehandle==this->stencilzy);
			this->planeh->transformation=glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f))*scale;
			drawHandle(this->planeh,this->handlespace,glm::vec4(1.0f,0.2f,1.0f,transparency*0.6f),this->stencilzx,this->activehandle==this->stencilzx);
			this->planeh->transformation=glm::mat4(1.0f)*scale;
			drawHandle(this->planeh,this->handlespace,glm::vec4(1.0f,1.0f,0.0f,transparency*0.6f),this->stencilyx,this->activehandle==this->stencilyx);
		}
		if(this->editmode==TransformHandles::EDIT_SCALE){//three-axis handle
			this->uniscaleh->transformation=glm::mat4(1.0f)*scale;
			this->uniscaleh->scale(glm::vec3(0.2f));
			float active=0.7f+0.3f*(float)(this->activehandle==this->stencilxyz);
			glStencilMask(255);
			glStencilFunc(GL_ALWAYS, this->stencilxyz, 255);
			this->uniscaleh->color=glm::vec4(active,active,active,transparency);
			this->uniscaleh->draw(this->handlespace);
			glStencilMask(0);
		}
	}
	glUseProgram(0);
	glDepthRange(0.0, 1.0);
	CHECK_GL_ERROR();
}
	
void TransformHandles::update(){ 
	//if(Controls::mouseKeysEventTable[GLUT_LEFT_BUTTON]==Controls::EVENT_DOWN){
	bool transactionBegin=false;
	if(InputController::isKeyJustPressed(Keys::mouseLeft)){
		//printf("0x%p\n", (void*)this->editedobj->getComponent(Renderer::componentType()));
		unsigned char stencile=((Renderer*)this->editedobj->getComponent(Renderer::componentType()))->stencil;
		unsigned char sel =Select::getStencilAt((int)InputController::m_mouseX, (int)(World2::height - InputController::m_mouseY), 3, stencile);
		unsigned char sele =Select::getStencilAt((int)InputController::m_mouseX, (int)(World2::height - InputController::m_mouseY), 3, -1);
		this->clicked++;
		this->activehandle=-1;
		if(sel==this->stencilx){this->activehandle=this->stencilx;this->axisnum=0;this->axisnum2=-1;}//manipulating handles clicked
		else if(sel==this->stencily){this->activehandle=this->stencily;this->axisnum=1;this->axisnum2=-1;}
		else if(sel==this->stencilz){this->activehandle=this->stencilz;this->axisnum=2;this->axisnum2=-1;}
		else if(sel==this->stencilzy){this->activehandle=this->stencilzy;this->axisnum=2;this->axisnum2=1;}
		else if(sel==this->stencilzx){this->activehandle=this->stencilzx;this->axisnum=2;this->axisnum2=0;}
		else if(sel==this->stencilyx){this->activehandle=this->stencilyx;this->axisnum=1;this->axisnum2=0;}
		else if(sel==this->stencilxyz){this->activehandle=this->stencilxyz;this->axisnum=3;this->axisnum2=-1;}
		else if(sel==this->stencilaxisx){this->editaxis=0;}
		else if(sel==this->stencilaxisy){this->editaxis=1;}
		else if(sel==this->stencilaxisz){this->editaxis=2;}
		else if(sel==this->stencilaxisw){this->editaxis=3;}
		else if(sele==stencile){}
		else{this->clicked--;}//click outside editedobj or handle
		if(this->activehandle!=-1){transactionBegin=true;}
	}
		
	if (InputController::isKeyJustUp(Keys::mouseLeft)){
		unsigned char stencile=((Renderer*)editedobj->getComponent(Renderer::componentType()))->stencil;
		unsigned char sel =Select::getStencilAt((int)InputController::m_mouseX, (int)(World2::height - InputController::m_mouseY), 0, -1);
		if(sel==stencile){clicked++;}//click inside editedobj
			
		if(clicked==2){this->isEdit=true;printf("is edit\n");}
		else if(clicked==0){this->isEdit=false;}
			
		this->activehandle=-1;
		this->clicked=0;
	}
		
	if(!this->isEdit){return;}
		
	if(this->activehandle==-1){
		if(			InputController::isKeyPressed(Keys::l)){this->editspace =TransformHandles::EDIT_LOCAL;}
		else if (	InputController::isKeyPressed(Keys::f)){this->editspace =TransformHandles::EDIT_FREE;}

		if (		InputController::isKeyPressed(Keys::a)){this->editmode = TransformHandles::EDIT_LOOKAT;}
		else if (	InputController::isKeyPressed(Keys::r)){this->editmode = TransformHandles::EDIT_ROTATION;}
		else if (	InputController::isKeyPressed(Keys::s)){this->editmode = TransformHandles::EDIT_SCALE;}
		else if (	InputController::isKeyPressed(Keys::p)){this->editmode = TransformHandles::EDIT_POSITION;}
		
		if (		InputController::isKeyPressed(Keys::x)){this->editaxis = 0;}
		else if (	InputController::isKeyPressed(Keys::y)){this->editaxis = 1;}
		else if (	InputController::isKeyPressed(Keys::z)){this->editaxis = 2;}
		else if (	InputController::isKeyPressed(Keys::w)){this->editaxis = 3;}
		this->bkp=editedobj->transformation;
		this->rotfreebkp=this->editedobj->transformation;
	}
		
	if(this->editspace==TransformHandles::EDIT_FREE){//free editing - editing axes of matix directly
		this->handlespace=glm::mat4(1.0f);

		if(this->editmode==TransformHandles::EDIT_POSITION){
			glm::mat4 m=getFullTransform(this->editedobj->parent);
			this->handlespace=getNormalized(m);
			m=m*this->editedobj->transformation;
			this->handlespace[3]=m[3]+m[this->editaxis]*(float)(this->editaxis!=3);

		}
		else if(this->editmode==TransformHandles::EDIT_SCALE){
			glm::mat4 m=getFullTransform(this->editedobj->parent);
			if(this->editaxis==3){
				glm::vec4 dir=m[3]-(m*this->bkp)[3];
				this->handlespace=getRotation(glm::mat4(glm::vec4(0.0f),glm::vec4(0.0f),-dir,glm::vec4(0.0f)),2);
			}
			else{
				this->handlespace=getRotation(m*this->bkp,this->editaxis);
			}
			m=m*this->editedobj->transformation;
			this->handlespace[3]=m[3]+m[this->editaxis]*(float)(this->editaxis!=3);
		}
		else if(this->editmode==TransformHandles::EDIT_ROTATION){
			this->rotfreebkp=getRotation(this->rotfreebkp,this->editaxis);
			glm::mat4 m=getFullTransform(this->editedobj->parent);
			if(this->editaxis==3){
				this->handlespace=getRotation(m*this->rotfreebkp,2);
				this->handlespace[3]=getFullTransform(this->editedobj->parent)[3];
			}
			else{
				this->handlespace=getRotation(m*this->rotfreebkp,this->editaxis);
				this->handlespace[3]=(m*this->editedobj->transformation)[3];
			}
		}
	}
	else{
		if (this->editmode == TransformHandles::EDIT_LOOKAT) {
			glm::vec4 pos=this->handlespace[3];
			this->handlespace = getNormalized(getFullTransform(this->editedobj->parent));
			this->handlespace[3]=pos;

			glm::mat4 t=getFullTransform(this->editedobj);
			glm::vec4 pose=t[3];
			glm::vec4 posh=this->handlespace[3];
			glm::vec4 dir=posh-pose;
			if(glm::length2(dir)<0.05f){this->handlespace[3] = pose + glm::normalize(t[2]);}//non-zero distance between editedobj and position to look at
		}
		else{
			this->handlespace = glm::mat4(1.0f);
			
			if(this->editmode==TransformHandles::EDIT_POSITION){
				this->handlespace=getNormalized(getFullTransform(this->editedobj->parent));
			}
			else if(this->editmode==TransformHandles::EDIT_SCALE){
				this->handlespace=getNormalized(getFullTransform(this->editedobj->parent)*this->bkp);
			}
			else if(this->editmode==TransformHandles::EDIT_ROTATION){
				this->handlespace=getNormalized(getFullTransform(this->editedobj));
			}
			this->handlespace[3]=getFullTransform(this->editedobj)[3];
		}
	}
	
	if(this->activehandle!=-1){
		//printf("hs %f %f %f\n\n", this->handlespace[3][0], this->handlespace[3][1], this->handlespace[3][2]);
		//if(Controls::keysEventTable[27]==Controls::EVENT_UP){editedobj->transform=this->bkp;}
			
		glm::vec3 drag3=glm::vec3(0.0f);
		glm::mat4 axes=glm::mat4(1.0f);axes[3]=glm::vec4(1.0f,1.0f,1.0f,0.0f);
		glm::vec2 spos1=world2screen((glm::vec3)(this->handlespace[3]));//position of transformated object on the screen
		glm::vec2 spos2=world2screen((glm::vec3)(this->handlespace[3]+this->handlespace*axes[this->axisnum]));//spos1,spos2 - project two points on screen - project axis on screen
		glm::vec2 dir=spos2-spos1;//the axis in screen space

		if (this->editmode == TransformHandles::EDIT_ROTATION) {
			if (transactionBegin) {
				glm::mat4 ortho=getOrtho(this->handlespace,this->axisnum);
				glm::vec3 p0 = (glm::vec3)this->handlespace[3];
				glm::vec3 px = (glm::vec3)(ortho * axes[(axisnum+1)%3]);
				glm::vec3 py = (glm::vec3)(ortho * axes[(axisnum+2)%3]);
				glm::vec3 t0 = -World2::mainCamPos;
				//glm::vec3 tz = mouseray(world2screen(p0) +glm::vec2(InputController::m_mouseXDelta, -InputController::m_mouseYDelta));
				glm::vec3 tz = mouseray(glm::vec2(InputController::m_mouseX,World2::height -InputController::m_mouseY));
				glm::vec3 coef = glm::inverse(glm::mat3(-tz, px, py)) * (t0 - p0);

				glm::vec3 pc = px*coef[1]+py*coef[2];
				glm::vec3 dir3 = glm::normalize(glm::cross(pc, (glm::vec3)axes[this->axisnum]));
				this->dirbkp = glm::normalize(world2screen(p0)-world2screen(p0 + dir3));
			}
			dir=this->dirbkp;
		}
		if(glm::length(dir)<0.01f){dir[0]=1.0f;}//axis length must not be zero



		glm::mat2 mov=glm::mat2(dir,glm::vec2(dir[1],-dir[0]));

		if(this->axisnum2!=-1){
			glm::vec2 spos22=world2screen((glm::vec3)(this->handlespace[3]+this->handlespace*axes[this->axisnum2]));//project two points on screen - project axis on screen
			glm::vec2 dir2=spos22-spos1;//the axis in screen space
			if(glm::length(dir2)<0.01){dir2[0]=1.0f;}//axis length must not be zero
			mov[1]=dir2;
		}

		mov=glm::inverse(glm::mat2(glm::normalize(mov[0]),glm::normalize(mov[1])));
			
		
		glm::vec2 drag,olddrag,dragfinal,mouse;

		mouse = glm::vec2(InputController::m_mouseX, World2::height - InputController::m_mouseY);
		drag=mov*(mouse-spos1);
		mouse = glm::vec2(InputController::m_mouseXPrev,World2::height - InputController::m_mouseYPrev);
		olddrag=mov*(mouse-spos1);
		dragfinal=drag-olddrag;

		drag3+=((glm::vec3)axes[axisnum])*(dragfinal[0]);
		if(axisnum2!=-1){drag3+=((glm::vec3)axes[this->axisnum2])*(dragfinal[1]);}
			
		float depth=glm::length(World2::mainCamPos+(glm::vec3)this->handlespace[3]);//add, not substract - moving camera is moving world in opposite direction
		if(this->editmode!=TransformHandles::EDIT_ROTATION){drag3*=depth*0.5f;}
		if(InputController::isKeyPressed(Keys::shiftr)){drag3*=0.25f;}
			
		if(this->editspace==TransformHandles::EDIT_FREE){
			drag3*=1.2f;
			if(this->editmode==TransformHandles::EDIT_ROTATION){
				glm::mat3 r=glm::mat3((glm::vec3)this->rotfreebkp[0],(glm::vec3)this->rotfreebkp[1],(glm::vec3)this->rotfreebkp[2]);//axes of rotation
				glm::mat4 rot1=glm::rotate(glm::mat4(1.0f),glm::radians(drag3[this->axisnum]),r[this->axisnum]);
				this->rotfreebkp=rot1*this->rotfreebkp;
				this->editedobj->transformation[this->editaxis]=rot1*this->editedobj->transformation[this->editaxis];
				this->editedobj->transformation[3][3]=1.0f;
					
			}
			else if(this->editmode==TransformHandles::EDIT_POSITION){
				if (axisnum2 != -1){
					glm::vec3 pc = planeIntersect((glm::vec3)(this->handlespace[axisnum]), (glm::vec3)(this->handlespace[axisnum2]), (glm::vec3)(this->handlespace[3]));

					if (world2viewport(pc)[2] < 0.992f) {
						glm::mat4 parent = getFullTransform(this->editedobj->parent);
						glm::vec4 editedo = glm::inverse(parent) * glm::vec4(pc, 1.0f);
						if(this->editaxis!=3){editedo-=this->editedobj->transformation[3]; }
						
						this->editedobj->transformation[this->editaxis] = editedo;
					}
				}
				else{
					drag3*=0.008f;
					this->editedobj->transformation[this->editaxis][this->axisnum]+=drag3[this->axisnum];
				}
				glm::mat4 ftransform=getFullTransform(this->editedobj);
				this->handlespace[3]=ftransform[3]+ftransform[this->editaxis]*(float)(this->editaxis!=3);
				this->handlespace[3][3]=1.0f;
			}
			else if(this->editmode==TransformHandles::EDIT_SCALE){
				drag3*=0.005f;
					
				if(this->editaxis==3){
					this->editedobj->transformation[3]+=this->bkp[3]*drag3[2]/glm::length((glm::vec3)this->bkp[3]);
					this->editedobj->transformation[3][3]=1.0f;
				}
				else{
					this->editedobj->transformation[this->editaxis]+=this->bkp[this->editaxis]*drag3[this->editaxis]/glm::length((glm::vec3)this->bkp[this->editaxis]);
				}
				glm::mat4 ftransform=getFullTransform(this->editedobj);
				this->handlespace[3]=ftransform[3]+ftransform[this->editaxis]*(float)(this->editaxis!=3);
				this->handlespace[3][3]=1.0f;
			}
		}
		else {
			if(this->editmode==TransformHandles::EDIT_ROTATION){
				drag3*=1.0f;
				glm::mat4 rot=glm::rotate(glm::mat4(1.0f),glm::radians(drag3[this->axisnum]),(glm::vec3)this->editedobj->transformation[this->axisnum]);
				glm::vec4 bkp=this->editedobj->transformation[3];
				this->editedobj->transformation[3]=glm::vec4(0.0f,0.0f,0.0f,1.0f);
				this->editedobj->transformation=rot*this->editedobj->transformation;
				this->editedobj->transformation[3]=bkp;
			}
			else if (this->editmode == TransformHandles::EDIT_LOOKAT){
				if (axisnum2 != -1){
					glm::vec3 pc = planeIntersect((glm::vec3)(this->handlespace[axisnum]), (glm::vec3)(this->handlespace[axisnum2]), (glm::vec3)(this->handlespace[3]));

					if (world2viewport(pc)[2] < 0.992f){
						glm::mat4 parent = getFullTransform(this->editedobj->parent);
						glm::vec4 result = glm::vec4(pc[0], pc[1], pc[2], 1.0f);
						glm::vec4 editedo = glm::inverse(parent) * result;
						//this->editedobj->transform[3] = editedo;
						this->handlespace[3]=glm::vec4(pc[0],pc[1],pc[2],1.0f);
					}
				}
				else{
					drag3 *= 0.008f;
					glm::vec3 mov = (glm::vec3)( getFullTransform(this->editedobj->parent) * glm::vec4(drag3[0], drag3[1], drag3[2], 0.0f) );
					if (glm::length2(mov) != 0.0f){mov = glm::normalize(mov) * glm::length(drag3);}
					*((glm::vec3*)&this->handlespace[3]) += mov;
				}
				glm::mat4 m=getFullTransform(this->editedobj->parent);
				for (int i = 0; i < 4; i++) {if(glm::length2(m[0])<0.0001f){m[i][i]=1.0f;}}//singular matrix is not invertible
				glm::vec4 posh=(glm::inverse(m)*this->handlespace)[3];
				glm::vec4 pose=this->editedobj->transformation[3];
				glm::vec4 dir=posh-pose;
				this->editedobj->transformation=getRotation(glm::mat4(glm::vec4(0.0f),glm::vec4(0.0f,1.0f,0.0f,0.0f),dir,glm::vec4(0.0f)),2);
				this->editedobj->transformation[3]=pose;
			}
			else if(this->editmode==TransformHandles::EDIT_POSITION){
				if(axisnum2!=-1){
					glm::vec3 pc = planeIntersect((glm::vec3)(this->handlespace[axisnum]), (glm::vec3)(this->handlespace[axisnum2]), (glm::vec3)(this->handlespace[3]));
						
					if(world2viewport(pc)[2]<0.992f){
						glm::mat4 parent=getFullTransform(this->editedobj->parent);
						glm::vec4 result=glm::vec4(pc[0],pc[1],pc[2],1.0f);
						glm::vec4 editedo=glm::inverse(parent)*result;
						this->editedobj->transformation[3]=editedo;
					}
				}
				else{
					drag3*=0.008f;
					this->editedobj->transformation[3][this->axisnum]+=drag3[this->axisnum];
				}
				this->handlespace[3]=getFullTransform(this->editedobj)[3];
			}
			else if(this->editmode==TransformHandles::EDIT_SCALE){
				drag3*=0.005f;
				glm::vec3 scal=glm::vec3(glm::length((glm::vec3)this->bkp[0]),glm::length((glm::vec3)this->bkp[1]),glm::length((glm::vec3)this->bkp[2]));
					
				if(this->activehandle==this->stencilxyz){//if uniform scale, scale of other axes proportionate to ref axis
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
					
				this->editedobj->transformation[0]+=this->bkp[0]*drag3[0]/scal[0];
				this->editedobj->transformation[1]+=this->bkp[1]*drag3[1]/scal[1];
				this->editedobj->transformation[2]+=this->bkp[2]*drag3[2]/scal[2];
			}
		}
	}
		
		
		
		

}


