#include "Camera2.h"
#include "../World2.h"
#include "../Transforms.h"
#include "../Components.h"
#include <iostream>
#include <typeinfo>


const char* Camera2::typeStatic = NULL;

Camera2::Camera2(float viewingAngle, GameObject* sceneRoot, RenderTexture* renderTarget){
    Camera2::typeStatic = typeid(Camera2).name();
    this->type = Camera2::typeStatic;

    assert(renderTarget!=NULL);
    this->angle = viewingAngle;
    this->sceneRoot = sceneRoot;
    float camw = (float)renderTarget->getWidth();
    float camh = (float)renderTarget->getHeight();
    this->perspective = glm::perspective(glm::radians(angle),camw/camh, 0.2f, 2000.0f);
    this->fbo = renderTarget;
    this->mainCamera = false;
}
Camera2::Camera2(float viewingAngle, GameObject* sceneRoot){
    Camera2::typeStatic = typeid(Camera2).name();
    this->type = Camera2::typeStatic;

    this->angle = viewingAngle;
    this->sceneRoot = sceneRoot;
    float camw = World2::width;
    float camh = World2::height;
    this->perspective = glm::perspective(glm::radians(angle), camw / camh, 0.2f, 2000.0f);
    this->fbo = NULL;
    this->mainCamera = true;
}

void Camera2::renderRecursive(GameObject* obj, glm::mat4 parent,bool isTranspartentPass){
    for (int i = 0; i < obj->components.size(); i++){if (obj->components[i]->isActive){obj->components[i]->render(&parent, isTranspartentPass);}}
    for (int i = 0; i < obj->children.size(); i++){renderRecursive(obj->children[i], parent * obj->transformation,isTranspartentPass);}
}

void Camera2::start(){}

void Camera2::update(){
    GLint fbobkp = 0;
    glm::vec3 posbkp = World2::mainCamPos;
    glm::mat4 perspectivebkp = World2::perspective;
    glm::mat4 mainCamera2bkp = World2::mainCamera;

    if (this->mainCamera){
        this->perspective = glm::perspective(glm::radians(angle), World2::width / World2::height, 0.2f, 2000.0f);
        glViewport(0, 0, (int)World2::width, (int)World2::height);
    }
    else{
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbobkp);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo->getFbo());
        glStencilMask(255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0);
    }

    glm::mat4 transform = getFullTransform(this->owner);

    World2::perspective = this->perspective;
    World2::mainCamPos = -(glm::vec3)transform[3];
    World2::mainCamera = glm::inverse(getRotation(transform, 0));
    World2::mainCamera[3] = World2::mainCamera * glm::vec4(World2::mainCamPos,1.0f);

    renderRecursive(sceneRoot, glm::mat4(1.0f),false);//render opaque
    renderRecursive(sceneRoot, glm::mat4(1.0f),true);//render transparent

    if (!this->mainCamera){
        glBindFramebuffer(GL_FRAMEBUFFER, fbobkp);
        World2::mainCamPos = posbkp;
        World2::perspective = perspectivebkp;
        World2::mainCamera = mainCamera2bkp;
    }
}
