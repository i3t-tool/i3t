#include "Camera.h"
#include "../World.h"
#include "../Transforms.h"
#include "../Components.h"
#include <iostream>
#include <typeinfo>


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
    float viewportbkp[4];
    glGetFloatv(GL_VIEWPORT, viewportbkp);

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
    World::mainCamera = glm::inverse(getRotation(transform, 0));
    World::mainCamera[3] = World::mainCamera * glm::vec4(World::mainCamPos,1.0f);

    renderRecursive(m_sceneRoot, glm::mat4(1.0f),false);//render opaque
    renderRecursive(m_sceneRoot, glm::mat4(1.0f),true);//render transparent

    if (!this->m_mainCamera){
        glBindFramebuffer(GL_FRAMEBUFFER, fbobkp);
        glViewport((GLint)viewportbkp[0], (GLint)viewportbkp[1], (GLsizei)viewportbkp[2], (GLsizei)viewportbkp[3]);
        World::mainCamPos = posbkp;
        World::perspective = perspectivebkp;
        World::mainCamera = mainCamerabkp;
    }
}
