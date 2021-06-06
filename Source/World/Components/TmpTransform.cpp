#include "TmpTransform.h"
#include "../World.h"
#include "../Transforms.h"

#include "Source/GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "Source/Core/Nodes/GraphManager.h"

#include <iostream>
#include <typeinfo>

const char* TmpTransform::s_type = NULL;

TmpTransform::TmpTransform() {
    TmpTransform::s_type = typeid(TmpTransform).name();
    this->m_type = TmpTransform::s_type;

}

// void TmpTransform::start(){}

void TmpTransform::update() {
    if (World::tmpNode.get() == nullptr) { return; }

    const Core::Transform::DataMap* data = World::tmpNode->getDataMap(); //printf("a");
    const Operation* operation = World::tmpNode->getOperation(); //printf("b");
    const char* keyword = World::tmpNode->getOperation()->keyWord.c_str(); //printf("c");
    //const glm::mat4& cp = World::tmpNode->getData().getMat4();
    const glm::mat4& cp = World::tmpSequence->getMatrices().back()->getData().getMat4();

    if(/*strcmp(keyword,"LookAt")!=0&&*/strcmp(keyword,"Ortho")!=0&&strcmp(keyword,"Perspective")!=0&&strcmp(keyword,"Frustum")!=0){
        Ptr<Core::NodeBase>node= World::tmpSequence->getMatrices().back();
        m_gameObject->transformation = getNodeTransform(&node,&World::tmpSequence)*cp;
    }
    
}