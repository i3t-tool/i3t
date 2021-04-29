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
    
    const char* keyword = World::tmpNode->getOperation()->keyWord.c_str();
    const glm::mat4& cp = World::tmpNode->getData().getMat4();
    
    if(strcmp(keyword,"Ortho")==0||strcmp(keyword,"Perspective")==0||strcmp(keyword,"Frustum")==0){
        m_cam->m_perspective = cp;
    }
    
    const glm::mat4& cp2 = World::tmpSequence2->getMatrices().at(0)->getData().getMat4();
    
    m_gameObject->transformation = glm::inverse(cp2);
}
