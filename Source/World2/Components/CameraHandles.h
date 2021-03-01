#pragma once
#include "../Component.h"
#include "Camera2.h"
#include "../Components.h"
#include "../GameObject.h"

class CameraHandles: public Component{
public:
	CameraHandles();
	void update();
	void start();
	void render(glm::mat4*parent,bool renderTransparent);
	static const char* componentType(){return CameraHandles::typeStatic;};
	const char* getComponentType(){return CameraHandles::typeStatic;};

	static const int EDIT_ORTHO=0,EDIT_PERSPECTIVE=1,EDIT_FRUSTUM=2;
	int editmode=EDIT_PERSPECTIVE;
	bool isEdit=false;
private:
	static const char* typeStatic;
		
	GameObject*frustrum,*frustruml,*cameraico,*handle;
	unsigned char editedcam;
	union Stencils{struct N{unsigned char f,n,hu,hb,wr,wl,a;}names;unsigned char arr[7];}stencils;
	glm::vec4 hposs[6];
	int activehandle,axisnum;
	Camera2*cam;
	//Renderer*icorend;
	float fnear=1.0f,ffar=50.0f,fleft=-1.0f,fright=1.0f,ftop=1.0f,fbottom=-1.0f;
	float pnear=0.5f,pfar=50.0f,pangle=60.0f,paspect=1.0f;
	float onear=-0.0f,ofar=20.0f,left=-10.0f,right=10.0f,top=10.0f,bottom=-10.0f;
};

