#include "Camera.h"
#include "../World.h"
#include "../Transforms.h"
#include "../Components.h"
#include <iostream>
#include <typeinfo>

void printMatrix5(glm::mat4 m){
  printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\n",
	  m[0][0], m[1][0], m[2][0], m[3][0],
	  m[0][1], m[1][1], m[2][1], m[3][1],
	  m[0][2], m[1][2], m[2][2], m[3][2],
	  m[0][3], m[1][3], m[2][3], m[3][3]);
}

const char* Camera::s_type = NULL;

Camera::Camera(float viewingAngle, GameObject* sceneRoot, RenderTexture* renderTarget){
    Camera::s_type = typeid(Camera).name();
    this->m_type = Camera::s_type;

    assert(renderTarget!=NULL);
    this->m_angle = viewingAngle;
    this->m_sceneRoot = sceneRoot;
    float camw = (float)renderTarget->getWidth();
    float camh = (float)renderTarget->getHeight();
    this->m_perspective = glm::perspective(glm::radians(m_angle),camw/camh, 0.2f, 2000.0f);
    this->m_fbo = renderTarget;
    this->m_mainCamera = false;
}
Camera::Camera(float viewingAngle, GameObject* sceneRoot){
    Camera::s_type = typeid(Camera).name();
    this->m_type = Camera::s_type;

    this->m_angle = viewingAngle;
    this->m_sceneRoot = sceneRoot;
    float camw = World::width;
    float camh = World::height;
    this->m_perspective = glm::perspective(glm::radians(m_angle), camw / camh, 0.2f, 2000.0f);
    this->m_fbo = NULL;
    this->m_mainCamera = true;
}

void Camera::renderRecursive(GameObject* obj, glm::mat4 parent,bool isTranspartentPass){
    for (int i = 0; i < obj->components.size(); i++){if (obj->components[i]->m_isActive){obj->components[i]->render(&parent, isTranspartentPass);}}
    for (int i = 0; i < obj->children.size(); i++){renderRecursive(obj->children[i], parent * obj->transformation,isTranspartentPass);}
}

void Camera::start(){}

void Camera::update(){
    GLint fbobkp = 0;
    glm::vec3 posbkp = World::mainCamPos;
    glm::mat4 perspectivebkp = World::perspective;
    glm::mat4 mainCamerabkp = World::mainCamera;
    int viewportbkp[4];
    glGetIntegerv(GL_VIEWPORT, viewportbkp);

    if (this->m_mainCamera){
        this->m_perspective = glm::perspective(glm::radians(m_angle), World::width / World::height, 0.2f, 2000.0f);
        glStencilMask(255);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearStencil(0);
    }
    else{
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbobkp);
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo->getFbo());
        glViewport(0, 0, this->m_fbo->getWidth(), this->m_fbo->getHeight());
        glStencilMask(255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0);
    }

    glm::mat4 transform = getFullTransform(this->m_gameObject);

    World::perspective = this->m_perspective;
    World::mainCamPos = -(glm::vec3)transform[3];
    if(m_processView){World::mainCamera = glm::inverse(getRotation(transform, 0));}
    else{ World::mainCamera = transform; }

    World::mainCamera[3] = World::mainCamera * glm::vec4(-(glm::vec3)transform[3],1.0f);
    //*(glm::vec3*)(&World::mainCamera[3]) = -(glm::vec3)transform[3];

    renderRecursive(m_sceneRoot, glm::mat4(1.0f),false);//render opaque
    renderRecursive(m_sceneRoot, glm::mat4(1.0f), true);//render transparent
    /*glDepthMask(GL_FALSE);//render overlaping transparent (replace "render transparent"), naive solution
    renderRecursive(m_sceneRoot, glm::mat4(1.0f),true);//render color, dont write depth - because of overlapping
    glDepthMask(GL_TRUE);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    renderRecursive(m_sceneRoot, glm::mat4(1.0f), true);//write depth, color is already rendered
    glColorMask(GL_TRUE,GL_TRUE, GL_TRUE, GL_TRUE);*/
    if (!this->m_mainCamera){
        //printMatrix5(World::mainCamera);
        glBindFramebuffer(GL_FRAMEBUFFER, fbobkp);
        glViewport(viewportbkp[0], viewportbkp[1], viewportbkp[2], viewportbkp[3]);
        World::mainCamPos = posbkp;
        World::perspective = perspectivebkp;
        World::mainCamera = mainCamerabkp;
    }
}
