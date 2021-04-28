#include "TmpCamera.h"

#include "Source/GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "Source/Core/Nodes/GraphManager.h"

#include <iostream>
#include <typeinfo>

const char* TmpCamera::s_type = NULL;

TmpCamera::TmpCamera() {
    TmpCamera::s_type = typeid(TmpCamera).name();
    this->m_type = TmpCamera::s_type;

}

void TmpCamera::start(){
    m_cam=(Camera*)m_gameObject->getComponent(Camera::componentType());
}

void TmpCamera::update(){
    if (World::tmpNode.get() == nullptr) { return; }

    const Core::Transform::DataMap* data = World::tmpNode->getDataMap(); //printf("a");
    const Operation* operation = World::tmpNode->getOperation(); //printf("b");
    const char* keyword = World::tmpNode->getOperation()->keyWord.c_str(); //printf("c");
    const glm::mat4& cp = World::tmpNode->getData().getMat4();

    if(strcmp(keyword,"LookAt")==0){m_gameObject->transformation = cp;}
    if(strcmp(keyword,"Ortho")==0||strcmp(keyword,"Perspective")==0||strcmp(keyword,"Frustum")==0){
        m_cam->m_perspective = cp;
    }
    else{return;}
}
