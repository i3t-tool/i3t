#include "ManipulatorUtil.h"
#include "../Select.h"

bool ManipulatorUtil::s_stencilInit=false;
char ManipulatorUtil::s_stencilArr[7]={0};

void ManipulatorUtil::drawHandle(GameObject*_handle,glm::mat4 space,glm::vec4 color,int stencil,int active,int hover){
	float m=0.0f;
	if(stencil==active){m=1.0f;}
	else if(stencil==hover){m=0.5f;}
	glStencilMask(255*(stencil!=-1));
	glStencilFunc(GL_ALWAYS, (unsigned char)stencil, 255);
	_handle->color= glm::vec4(1.0f,1.0f,1.0f,color[3])*m + glm::vec4(color[0],color[1],color[2],color[3])*(1.0f-m);
	_handle->draw(space);
	glStencilMask(0);
}
int ManipulatorUtil::getStencil(char at){
	if(!s_stencilInit){
		for(int i=0;i<7;i++){s_stencilArr[i]=Select::registerStencil();}
		s_stencilInit=true;
	}
	if(at<0||at>7){return -1;}
	return s_stencilArr[at];
}