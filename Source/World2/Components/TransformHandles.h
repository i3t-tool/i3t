#pragma once
#include "../Component.h"
#include "../GameObject.h"


class TransformHandles: public Component{
public:
	TransformHandles(GameObject*_editedobj);
	void start();
	void update();
	void render(glm::mat4*parent,bool renderTransparent);
	static const char* componentType(){return TransformHandles::typeStatic;};
	const char* getComponentType(){return TransformHandles::typeStatic;};

	static const int EDIT_LOCAL=0,EDIT_FREE=5,EDIT_ROTATION=7,EDIT_SCALE=8,EDIT_POSITION=9,EDIT_LOOKAT=10;
		
private:
	static const char* typeStatic;

	static void drawHandle(GameObject* _handle, glm::mat4 space, glm::vec4 color, int stencil, bool active);
		
	int editmode=EDIT_ROTATION;
	int editspace=EDIT_LOCAL;
	int editaxis=0;
	int clicked=0;
	bool isEdit=false;
	GameObject*editedobj,*circleh,*arrowh,*planeh,*scaleh,*uniscaleh,*lineh;
	char stencilx,stencily,stencilz,stencilzx,stencilzy,stencilyx,stencilxyz,stencilaxisx,stencilaxisy,stencilaxisz,stencilaxisw;
	int activehandle=-1,axisnum=-1,axisnum2=-1;
	glm::mat4 handlespace;///<transformation of handles - position+rotation - handles are not parent of any object
	glm::mat4 bkp;///<transform of editedobj before current operation started - escape - abort
	glm::mat4 rotfreebkp=glm::mat4(1.0f);
	glm::vec2 dirbkp=glm::vec2(0.0f);
};

