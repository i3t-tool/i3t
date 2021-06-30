#include "libraryI3T.h"

#include <stdio.h>

//#include "picoc.h"

#include "Core/API.h"
///////// #include "Core/Nodes/Cycle.h"
#include "GUI/Elements/Nodes/SingleInclude.h"  //?????
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Scripting/Scripting.h"

ScriptingData scriptingData;

ScriptingData*getScriptingData(){return &scriptingData;}
void clearScriptingData(){
    scriptingData.nodeData.clear();
}
void mat4oper(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type=param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x=param[1]->Val->Integer;
        y=param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	if(type==scriptingData.mat4Types.determinant){
		workspace->push_back(std::make_unique<WorkspaceDeterminant>((ImTextureID)0, l));
	}
    else if(type==scriptingData.arithmeticOperators.inverse){
		workspace->push_back(std::make_unique<WorkspaceMatrixInversion>((ImTextureID)0, l));
	}
    else if(type==scriptingData.arithmeticOperators.mul){
        workspace->push_back(std::make_unique<WorkspaceMatrixMulMatrix>((ImTextureID)0, l));
	}
    else if(type==scriptingData.arithmeticOperators.add){
        workspace->push_back(std::make_unique<WorkspaceMatrixAddMatrix>((ImTextureID)0, l));
	}
    else if (type == scriptingData.mat4Types.matmulvec) {
        workspace->push_back(std::make_unique<WorkspaceMatrixMulVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.vecmulmat) {
        workspace->push_back(std::make_unique<WorkspaceVectorMulMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.floatmulmat) {
        workspace->push_back(std::make_unique<WorkspaceMatrixMulFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.free) {
        workspace->push_back(std::make_unique<WorkspaceMatrixFree>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.trackball) {
        returnValue->Val->Integer = -1; return;
    }
    else if (type == scriptingData.mat4Types.transpose) {
        workspace->push_back(std::make_unique<WorkspaceMatrixTranspose>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.ortho) {
        workspace->push_back(std::make_unique<WorkspaceMakeOrtho>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.perspective) {
        workspace->push_back(std::make_unique<WorkspaceMakePerspective>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.frustrum) {
        workspace->push_back(std::make_unique<WorkspaceMakeFrustum>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.axisangle) {
        workspace->push_back(std::make_unique<WorkspaceMakeAxisAngle>((ImTextureID)0, l));//?
    }
    else if (type == scriptingData.mat4Types.rotatex) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerX>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.rotatey) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerY>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.rotatez) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerZ>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.scale) {
        workspace->push_back(std::make_unique<WorkspaceMakeScale>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.lookAt) {
        workspace->push_back(std::make_unique<WorkspaceMakeLookAt>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Types.translate) {
        workspace->push_back(std::make_unique<WorkspaceMakeTranslation>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1;return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));

    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void mat4(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type=param[0]->Val->Integer;
    int dataindex=param[1]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==5){
        x=param[2]->Val->Integer;
        y=param[3]->Val->Integer;
        if(param[4]->Val->Pointer!=nullptr){l = (char*)param[4]->Val->Pointer;}
    }

    bool datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	if (type == scriptingData.mat4Types.scale) {
		workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue((glm::vec3)mat[0]);}
	}
    else if (type == scriptingData.mat4Types.uniscale) {
        workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue((glm::vec3)mat[0]);}
    }
	else if (type == scriptingData.mat4Types.translate) {
		workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue((glm::vec3)mat[0]);}
	}
    else if (type == scriptingData.mat4Types.free) {
        workspace->push_back(std::make_unique<WorkspaceTransformationFree>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue(mat);}
    }
    else if (type == scriptingData.mat4Types.lookAt) {
        workspace->push_back(std::make_unique<WorkspaceLookAt>((ImTextureID)0, l));
        Core::LookAt* lookat=(Core::LookAt*)(workspace->back()->getNodebase().get());
        if(datavalid){lookat->setCenter((glm::vec3)mat[0]);lookat->setEye((glm::vec3)mat[1]);lookat->setUp((glm::vec3)mat[2]);}
    }
    else if (type == scriptingData.mat4Types.ortho) {
        workspace->push_back(std::make_unique<WorkspaceOrtho>((ImTextureID)0, l));
        Core::OrthoProj* ff = (Core::OrthoProj*)workspace->back()->getNodebase().get();
        if (datavalid){
            ff->setLeft(mat[0][0]); ff->setRight(mat[0][1]);
            ff->setTop(mat[1][0]);  ff->setBottom(mat[1][1]);
            ff->setNear(mat[2][0]); ff->setFar(mat[2][1]);
        }
    }
    else if (type == scriptingData.mat4Types.perspective) {
        workspace->push_back(std::make_unique<WorkspacePerspective>((ImTextureID)0, l));
        Core::PerspectiveProj*  ff = (Core::PerspectiveProj*)workspace->back()->getNodebase().get();
        if (datavalid){
            ff->setZNear(mat[2][0]);ff->setZFar(mat[2][1]);
            ff->setFOW(glm::radians(mat[3][0]));  ff->setAspect(mat[3][1]);
        }
    }
    else if (type == scriptingData.mat4Types.frustrum) {
        workspace->push_back(std::make_unique<WorkspaceFrustum>((ImTextureID)0, l));
        Core::Frustum*ff=(Core::Frustum*)workspace->back()->getNodebase().get();
        if (datavalid){
            ff->setLeft(mat[0][0]); ff->setRight(mat[0][1]);
            ff->setTop(mat[1][0]);  ff->setBottom(mat[1][1]);
            ff->setNear(mat[2][0]); ff->setFar(mat[2][1]);
        }
    }
    else if (type == scriptingData.mat4Types.rotatex) {
        workspace->push_back(std::make_unique<WorkspaceEulerX>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue(glm::radians(mat[0][0]));}
    }
    else if (type == scriptingData.mat4Types.rotatey) {
        workspace->push_back(std::make_unique<WorkspaceEulerY>((ImTextureID)0, l));
        if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase()->setValue(glm::radians(mat[0][0]));}
    }
    else if (type == scriptingData.mat4Types.rotatez) {
        workspace->push_back(std::make_unique<WorkspaceEulerZ>((ImTextureID)0, l));
        if (datavalid) {ValueSetResult result = (workspace->back().get())->getNodebase()->setValue(glm::radians(mat[0][0]));}
    }
    else if (type == scriptingData.mat4Types.axisangle) {
        workspace->push_back(std::make_unique<WorkspaceAxisAngle>((ImTextureID)0, l));
        if (datavalid) {ValueSetResult result = (workspace->back().get())->getNodebase()->setValue(glm::radians(mat[0][0]));}
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==2){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
    //    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void vec4(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int dataindex=param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x=param[1]->Val->Integer;
        y=param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }

    float datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	workspace->push_back(std::make_unique<WorkspaceVectorFree>((ImTextureID)0, l));
    if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec4)mat[0]);}

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void vec3(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int dataindex=param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x=param[1]->Val->Integer;
        y=param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }


    float datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	workspace->push_back(std::make_unique<WorkspaceVector3Free>((ImTextureID)0, l));
    if(datavalid){ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec3)mat[0]);}

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void vec4oper(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type = param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if (type == scriptingData.vecOperators.dot) {
        workspace->push_back(std::make_unique<WorkspaceVectorDotVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.vecOperators.perspdiv) {
        workspace->push_back(std::make_unique<WorkspaceVectorPerspectiveDivision>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.norm) {
        workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.vecOperators.vecmulfloat) {
        workspace->push_back(std::make_unique<WorkspaceVectorMulFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.add) {
        workspace->push_back(std::make_unique<WorkspaceVectorAddVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.sub) {
        workspace->push_back(std::make_unique<WorkspaceVectorSubVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.mix) {
        workspace->push_back(std::make_unique<WorkspaceMixVector>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void vec3oper(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type = param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if(type == scriptingData.vecOperators.cross){
        workspace->push_back(std::make_unique<WorkspaceVector3CrossVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.vecOperators.dot) {
        workspace->push_back(std::make_unique<WorkspaceVector3DotVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.norm) {
        workspace->push_back(std::make_unique<WorkspaceNormalizeVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.length) {
        workspace->push_back(std::make_unique<WorkspaceVector3Length>((ImTextureID)0,l));
    }
    else if (type == scriptingData.vecOperators.vecmulfloat) {
        workspace->push_back(std::make_unique<WorkspaceVector3MulFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.add) {
        workspace->push_back(std::make_unique<WorkspaceVector3AddVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.sub) {
        workspace->push_back(std::make_unique<WorkspaceVector3SubVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.show) {
        workspace->push_back(std::make_unique<WorkspaceShowVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.mix) {
        workspace->push_back(std::make_unique<WorkspaceMixVector3>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void scalar(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int dataindex = param[0]->Val->Integer;
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 4) {
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if (param[3]->Val->Pointer != nullptr) { l = (char*)param[3]->Val->Pointer; }
    }

    float datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    workspace->push_back(std::make_unique<WorkspaceFloatFree>((ImTextureID)0, l));
    if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue(mat[0][0]);}

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void cycle(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int dataindex=param[0]->Val->Integer;
    int x=0,y=0;
    const char*l="-";
    if(numArgs==4){
        x=param[1]->Val->Integer;
        y=param[2]->Val->Integer;
        if(param[3]->Val->Pointer!=nullptr){l = (char*)param[3]->Val->Pointer;}
    }
    float datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	workspace->push_back(std::make_unique<WorkspaceCycle>((ImTextureID)0, WorkspaceCycleArgs()));
    if(datavalid){
        Core::Cycle* cycle= (Core::Cycle*)(workspace->back().get())->getNodebase().get();
        if(mat[0][0]<1.0f){cycle->setMode(Core::Cycle::EMode::Once);}
        else if(mat[0][0]<2.0f){ cycle->setMode(Core::Cycle::EMode::Repeat);}
        else {cycle->setMode(Core::Cycle::EMode::PingPong);}
        cycle->setFrom(mat[1][0]);
		cycle->setMultiplier(mat[1][1]);
		cycle->setManualStep(mat[1][2]);
		cycle->setTo(mat[1][3]);
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void scalaroper(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type = param[0]->Val->Integer;
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 4) {
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if (param[3]->Val->Pointer != nullptr) { l = (char*)param[3]->Val->Pointer; }
    }
    
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if (type == scriptingData.floatOperators.asinacos) {
        workspace->push_back(std::make_unique<WorkspaceASinACos>((ImTextureID)0, l));
    }
    else if (type == scriptingData.floatOperators.pow) {
        workspace->push_back(std::make_unique<WorkspaceFloatPowFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.floatOperators.sincos) {
        workspace->push_back(std::make_unique<WorkspaceFloatSinCos>((ImTextureID)0, l));
    }
    else if (type == scriptingData.floatOperators.clamp) {
        workspace->push_back(std::make_unique<WorkspaceClampFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.floatOperators.signum) {
        workspace->push_back(std::make_unique<WorkspaceSignum>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.mul) {
        workspace->push_back(std::make_unique<WorkspaceFloatMulFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.add) {
        workspace->push_back(std::make_unique<WorkspaceFloatAddFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.div) {
        workspace->push_back(std::make_unique<WorkspaceFloatDivFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.mix) {
        workspace->push_back(std::make_unique<WorkspaceMixFloat>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void convertor(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type = param[0]->Val->Integer;
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 4) {
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if (param[3]->Val->Pointer != nullptr) { l = (char*)param[3]->Val->Pointer; }
    }
    
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if (type == scriptingData.convertors.mat_tr) {
        workspace->push_back(std::make_unique<WorkspaceMatrixToTR>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.tr_mat) {
        workspace->push_back(std::make_unique<WorkspaceTRToMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.mat_vecs4) {
        workspace->push_back(std::make_unique<WorkspaceMatrixToVectors>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.mat_quat) {
        workspace->push_back(std::make_unique<WorkspaceMatrixToQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.mat_scalars) {
        workspace->push_back(std::make_unique<WorkspaceMatrixToFloats>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vecs4_mat) {
        workspace->push_back(std::make_unique<WorkspaceVectorsToMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vec4_vec3) {
        workspace->push_back(std::make_unique<WorkspaceVectorToVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vec4_scalars) {
        workspace->push_back(std::make_unique<WorkspaceVectorToFloats>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vecs3_mat) {
        workspace->push_back(std::make_unique<WorkspaceVectors3ToMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vec3_vec4) {
        workspace->push_back(std::make_unique<WorkspaceVector3ToVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.vec3_scalars) {
        workspace->push_back(std::make_unique<WorkspaceVector3ToFloats>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.quat_mat) {
        workspace->push_back(std::make_unique<WorkspaceQuatToMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.quat_scalars) {
        workspace->push_back(std::make_unique<WorkspaceQuatToFloats>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.scalars_mat) {
        workspace->push_back(std::make_unique<WorkspaceFloatsToMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.scalars_vec3) {
        workspace->push_back(std::make_unique<WorkspaceFloatsToVector3>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.scalars_vec4) {
        workspace->push_back(std::make_unique<WorkspaceFloatsToVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.convertors.scalars_quat) {
        workspace->push_back(std::make_unique<WorkspaceFloatsToQuat>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void quat(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int dataindex = param[0]->Val->Integer;
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 4) {
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if (param[3]->Val->Pointer != nullptr) { l = (char*)param[3]->Val->Pointer; }
    }

    float datavalid=false;
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex];datavalid=true; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    returnValue->Val->Integer =- 1; return;
    //workspace->push_back(std::make_unique<WorkspaceQuatFree>((ImTextureID)0, l));
    if (datavalid){ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue(mat[0]);}

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void quatoper(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int type = param[0]->Val->Integer;
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 4) {
        x = param[1]->Val->Integer;
        y = param[2]->Val->Integer;
        if (param[3]->Val->Pointer != nullptr) { l = (char*)param[3]->Val->Pointer; }
    }
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if (type == scriptingData.quatOperators.scalarvec3_quat) {
        workspace->push_back(std::make_unique<WorkspaceFloatVecToQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.angleaxis_quat) {
        workspace->push_back(std::make_unique<WorkspaceAngleAxisToQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.vec3vec3_quat) {
        workspace->push_back(std::make_unique<WorkspaceVecVecToQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.quat_scalarvec3) {
        workspace->push_back(std::make_unique<WorkspaceQuatToFloatVec>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.quat_angleaxis) {
        workspace->push_back(std::make_unique<WorkspaceQuatToAngleAxis>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.scalarmulquat) {
        workspace->push_back(std::make_unique<WorkspaceFloatMulQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.quat_euler) {
        workspace->push_back(std::make_unique<WorkspaceQuatToEuler>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.euler_quat) {
        workspace->push_back(std::make_unique<WorkspaceEulerToQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.slerp) {
        workspace->push_back(std::make_unique<WorkspaceQuatSlerp>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.longslerp) {
        workspace->push_back(std::make_unique<WorkspaceQuatLongWaySlerp>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.lerp) {
        workspace->push_back(std::make_unique<WorkspaceQuatLerp>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.conjugate) {
        workspace->push_back(std::make_unique<WorkspaceConjQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.quatOperators.qvq) {
        workspace->push_back(std::make_unique<WorkspaceQuatVecConjQuat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.inverse) {
        workspace->push_back(std::make_unique<WorkspaceQuatInverse>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.length) {
        workspace->push_back(std::make_unique<WorkspaceQuatLength>((ImTextureID)0, l));
    }
    else if (type == scriptingData.arithmeticOperators.norm) {
        workspace->push_back(std::make_unique<WorkspaceNormalizeQuat>((ImTextureID)0, l));
    }
    else {
        returnValue->Val->Integer = -1; return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
}
void sequence(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int x = 0, y = 0;
    const char* l = "-";
    if (numArgs == 3) {
        x = param[0]->Val->Integer;
        y = param[1]->Val->Integer;
        if (param[2]->Val->Pointer != nullptr) { l = (char*)param[2]->Val->Pointer; }
    }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    workspace->push_back(std::make_unique<WorkspaceSequence>((ImTextureID)0, nullptr,l));

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 0) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;//...
    }

    returnValue->Val->Integer = workspace->back()->getNodebase()->getId();
    //returnValue->Val->Integer = (int)workspace->size() - 1;
}
void plugNodes(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int ida=param[0]->Val->Integer;
    int idb=param[1]->Val->Integer;
    int outputindex=param[2]->Val->Integer;
    int inputindex= param[3]->Val->Integer;
    int indexa=-1;
    int indexb=-1;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==ida){indexa=i;break;}
    }
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==idb){indexb=i;break;}
    }

    if(indexa<0||indexa>=workspace->size()){returnValue->Val->Integer=false;return;}
    if(indexb<0||indexb>=workspace->size()){returnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->getNodebase();
	Ptr<Core::NodeBase> pcb= (workspace->at(indexb).get())->getNodebase();

    if(outputindex>=pca->getOutputPins().size()){returnValue->Val->Integer=false;return;}
    if(inputindex>= pcb->getInputPins().size()){ returnValue->Val->Integer=false;return;}

	ENodePlugResult p = Core::GraphManager::plug(pca,pcb, outputindex, inputindex);
    returnValue->Val->Integer =(int)p==0;
}
void seqAdd(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int seqid = param[0]->Val->Integer;
    int nodeid =param[1]->Val->Integer;
    int seqindex=-1,nodeindex=-1;
    //int at =param[2]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==seqid){seqindex=i;break;}
    }
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==nodeid){nodeindex=i;break;}
    }

    if(seqindex<0||seqindex>=workspace->size()){returnValue->Val->Integer=false;return;}
    if(nodeindex<0||nodeindex>=workspace->size()){returnValue->Val->Integer=false;return;}
    if(!workspace->at(seqindex)->isSequence()){returnValue->Val->Integer=false;return;}
    if(!workspace->at(nodeindex)->isTransformation()){returnValue->Val->Integer=false;return;}

    WorkspaceSequence*seq=(WorkspaceSequence*)workspace->at(seqindex).get();
    WorkspaceNodeWithCoreData*node=workspace->at(nodeindex).get();
    //ne::DeleteNode(workspace->at(nodeindex)->getId());
    
    //printf("push  %s\n",workspace->at(nodeindex)->getNodebase()->getOperation()->keyWord.c_str());
    //printf("erase %s\n",(workspace->begin()+nodeindex)->get()->getNodebase()->getOperation()->keyWord.c_str());
    seq->pushNode(workspace->at(nodeindex),seq->getInnerWorkspaceNodes().size());
    workspace->erase(workspace->begin()+nodeindex);

    returnValue->Val->Integer = true;
}
void unplugInput(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int ida=param[0]->Val->Integer;
    int inputindex= param[1]->Val->Integer;
    int indexa=-1;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==ida){indexa=i;break;}
    }
    if(indexa<0||indexa>=workspace->size()){returnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->getNodebase();
    if(inputindex>= pca->getInputPins().size()){ returnValue->Val->Integer=false;return;}
    Core::GraphManager::unplugInput(pca, inputindex);

    returnValue->Val->Integer=true;
}
void getNodeByName(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    char* label=(char*)param[0]->Val->Pointer;
    int occurances=0;
    returnValue->Val->Integer=-1;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if (strcmp(workspace->at(i).get()->getHeaderLabel().c_str(),label)==0) {occurances++;}
    }
    if(occurances==0){
        std::cout<<"No matches found."<<std::endl;
        return;
    }
    else if(occurances>1){
        std::cout<<"Multiple matches found:"<<std::endl;
    }
    for(int i=0;i<workspace->size();i++){
        if (strcmp(workspace->at(i).get()->getHeaderLabel().c_str(),label)==0) {
            returnValue->Val->Integer= workspace->at(i)->getNodebase()->getId();
            if(occurances>1){std::cout<<"Id: "<< workspace->at(i)->getNodebase()->getId() <<", Name: "<<label<<std::endl;}
        }
    }

}
void deleteNode(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int id=param[0]->Val->Integer;
    int index=-1;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==id){index=i;break;}
    }
    if(index<0||index>=workspace->size()){returnValue->Val->Integer=false;std::cout<<"Node does not exist."<<std::endl;return;}
    WorkspaceNodeWithCoreData* cc= (workspace->at(index).get());
    Ptr<WorkspaceWindow> ww = I3T::getWindowPtr<WorkspaceWindow>();
    ww->NodeDelete(cc->getId());

    returnValue->Val->Integer=true;
}
void confNode(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int id=param[0]->Val->Integer;
    int precision=param[1]->Val->Integer;
    int lod=param[2]->Val->Integer;
    int index=-1;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if(workspace->at(i)->getNodebase()->getId()==id){index=i;break;}
    }
    if(index<0||index>=workspace->size()){returnValue->Val->Integer=false;return;}
    //if(precision<0||precision>4){precision==0;}
    if(lod<0||lod>3){lod=0;}

    WorkspaceNodeWithCoreData* cc= (workspace->at(index).get());
    cc->setNumberOfVisibleDecimal(precision);
    cc->setLevelOfDetail((WorkspaceLevelOfDetail)lod);

    returnValue->Val->Integer=true;
}
void dataMat4(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    glm::mat4 m;
    for(int i=0;i<4;i++){for(int j=0;j<4;j++){m[i][j]=(float)param[j+4*i]->Val->FP; }}
    scriptingData.nodeData.push_back(m);
    returnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataVec4(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    glm::mat4 m=glm::mat4(1.0f);
    m[0][0]=(float)param[0]->Val->FP;m[0][1]= (float)param[1]->Val->FP;m[0][2]=(float)param[2]->Val->FP; m[0][3] = (float)param[3]->Val->FP;
    scriptingData.nodeData.push_back(m);
    returnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataVec3(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)param[0]->Val->FP; m[0][1] = (float)param[1]->Val->FP; m[0][2] = (float)param[2]->Val->FP;
    scriptingData.nodeData.push_back(m);
    returnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataScalar(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)param[0]->Val->FP;
    scriptingData.nodeData.push_back(m);
    returnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void loadW(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    std::string filename= Config::getAbsolutePath((char*)param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != nullptr) {ww->m_workspaceCoreNodes.clear(); status=loadWorkspace(filename.c_str()); }
    returnValue->Val->Integer=status;
}
void appendW(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    std::string filename= Config::getAbsolutePath((char*)param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    int len=(int)ww->m_workspaceCoreNodes.size();
    if (ww != nullptr) {status=loadWorkspace(filename.c_str()); }

    ne::ClearSelection();
    for(int i=len;i<(int)ww->m_workspaceCoreNodes.size();i++){
        ne::SelectNode(ww->m_workspaceCoreNodes[i]->getId(),true);
    }

    returnValue->Val->Integer=status;
}
void saveW(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    std::string filename = Config::getAbsolutePath((char*)param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != nullptr) {status=saveWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    returnValue->Val->Integer = status;
}
void saveSelW(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    std::string filename = Config::getAbsolutePath((char*)param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status = false;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>ws= ww->getSelectedWorkspaceCoreNodes();
    if (ww != nullptr) { status = saveWorkspace(filename.c_str(), &ws); }
    returnValue->Val->Integer = status;
}
void runScript(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    std::string filename = Config::getAbsolutePath((char*)param[0]->Val->Pointer);
    returnValue->Val->Integer=picocRunFile(filename.c_str())==0;
}
void help(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    scriptingHelp(0);
}
void help2(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    scriptingHelp(1);
}
void help3(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    scriptingHelp(2);
}
/* list of all library functions and their prototypes */
struct LibraryFunction platformLibraryI3T[] =
{
	{ mat4oper,     "int mat4oper(int,int,int,char*);" },         { mat4oper,     "int mat4operc(int);" },
	{ mat4,         "int mat4(int,int,int,int,char*);" },         { mat4,         "int mat4c(int,int);" },
    { vec4oper,     "int vec4oper(int,int,int,char*);" },         { vec4oper,     "int vec4operc(int);" },
	{ vec4,         "int vec4(int,int,int,char*);"     },         { vec4,         "int vec4c(int);" },
    { vec3oper,     "int vec3oper(int,int,int,char*);" },         { vec3oper,     "int vec3operc(int);" },
	{ vec3,         "int vec3(int,int,int,char*);"     },         { vec3,         "int vec3c(int);" },
    { cycle,        "int cycle(int,int,int,char*);"},             { cycle,        "int cyclec(int);" },
    { scalaroper,   "int scalaroper(int,int,int,char*);"},        { scalaroper,   "int scalaroperc(int);" },
    { scalar,       "int scalar(int,int,int,char*);"   },         { scalar,       "int scalarc(int);" },
    { quatoper,     "int quatoper(int,int,int,char*);"},          { quatoper,     "int quatoperc(int);" },
    { quat,         "int quat(int,int,int,char*);"   },           { quat,         "int quatc(int);" },
    { convertor,    "int convertor(int,int,int,char*);"},         { convertor,    "int convertorc(int);" },
	{ sequence,     "int sequence(int,int,char*);"     },         { sequence,     "int sequencec();" },
	{ seqAdd,       "bool seqadd(int,int);" },
	{ plugNodes,    "bool plugnodes(int,int,int,int);" },
	{ unplugInput,  "bool unpluginput(int,int);" },
	{ getNodeByName,"int getnode(char*);" },
	{ deleteNode,   "bool delnode(int);" },
	{ confNode,     "bool confnode(int,int,int);" },
	{ dataMat4,     "int datamat4(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);" },
	{ dataVec4,     "int datavec4(float,float,float,float);" },
	{ dataVec3,     "int datavec3(float,float,float);" },
	{ dataScalar,   "int datascalar(float);" },
	{ loadW,        "bool load(char*);" },
	{ appendW,      "bool append(char*);" },
	{ saveW,        "bool save(char*);" },
	{ saveSelW,     "bool savesel(char*);" },
	{ runScript,    "bool run(char*);" },
	{ help,         "void help();" },{ help2,    "void help2();" },{ help3,    "void help3();" },
    { nullptr,         nullptr }
};

const char defs[]="typedef int bool;";

void platformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, nullptr, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&parser, &returnType, &Identifier, nullptr);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &platformLibraryI3T);
    IncludeRegister(pc, "I3T.h", nullptr, platformLibraryI3T, defs);//ADD_CUSTOM


    //mat4 transform
    VariableDefinePlatformVar(pc, nullptr, "free",          &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.free,          false);
    VariableDefinePlatformVar(pc, nullptr, "uniscale",      &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.uniscale,      false);
    
    //mat4 transform intersect mat4oper
    VariableDefinePlatformVar(pc, nullptr, "scale",         &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.scale,         false);
    VariableDefinePlatformVar(pc, nullptr, "rotatex",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatex,       false);
    VariableDefinePlatformVar(pc, nullptr, "rotatey",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatey,       false);
    VariableDefinePlatformVar(pc, nullptr, "rotatez",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatez,       false);
    VariableDefinePlatformVar(pc, nullptr, "translate",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.translate,     false);
    VariableDefinePlatformVar(pc, nullptr, "axisangle",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.axisangle,     false);
    VariableDefinePlatformVar(pc, nullptr, "ortho",         &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.ortho,         false);
    VariableDefinePlatformVar(pc, nullptr, "perspective",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.perspective,   false);
    VariableDefinePlatformVar(pc, nullptr, "frustrum",      &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.frustrum,      false);
    VariableDefinePlatformVar(pc, nullptr, "lookat",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.lookAt,        false);

    //mat4oper
    VariableDefinePlatformVar(pc, nullptr, "determinant",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.determinant,   false);
    VariableDefinePlatformVar(pc, nullptr, "trackball",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.trackball,     false);
    VariableDefinePlatformVar(pc, nullptr, "transpose",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.transpose,     false);
    VariableDefinePlatformVar(pc, nullptr, "matmulvec",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.matmulvec,     false);
    VariableDefinePlatformVar(pc, nullptr, "vecmulmat",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.vecmulmat,     false);
    VariableDefinePlatformVar(pc, nullptr, "floatmulmat",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.floatmulmat,   false);

    //vecoper
    VariableDefinePlatformVar(pc, nullptr, "cross",         &pc->IntType, (union AnyValue*)&scriptingData.vecOperators.cross,       false);
    VariableDefinePlatformVar(pc, nullptr, "dot",           &pc->IntType, (union AnyValue*)&scriptingData.vecOperators.dot,         false);
    VariableDefinePlatformVar(pc, nullptr, "vecmulfloat",   &pc->IntType, (union AnyValue*)&scriptingData.vecOperators.vecmulfloat, false);
    VariableDefinePlatformVar(pc, nullptr, "perspdiv",      &pc->IntType, (union AnyValue*)&scriptingData.vecOperators.perspdiv,    false);

    //arithmetic oper
    VariableDefinePlatformVar(pc, nullptr, "add",           &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.add,  false);
    VariableDefinePlatformVar(pc, nullptr, "sub",           &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.sub,  false);
    VariableDefinePlatformVar(pc, nullptr, "div",           &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.div,  false);
    VariableDefinePlatformVar(pc, nullptr, "mul",           &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.mul,  false);
    VariableDefinePlatformVar(pc, nullptr, "show",          &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.show, false);
    VariableDefinePlatformVar(pc, nullptr, "mix",           &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.mix,  false);
    VariableDefinePlatformVar(pc, nullptr, "inverse",       &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.inverse,false);
    VariableDefinePlatformVar(pc, nullptr, "norm",          &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.norm, false);
    VariableDefinePlatformVar(pc, nullptr, "length",        &pc->IntType, (union AnyValue*)&scriptingData.arithmeticOperators.length,false);

    //float oper
    VariableDefinePlatformVar(pc, nullptr, "clamp",         &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.clamp,     false);
    VariableDefinePlatformVar(pc, nullptr, "cycle",         &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.cycle,     false);
    VariableDefinePlatformVar(pc, nullptr, "pow",           &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.pow,       false);
    VariableDefinePlatformVar(pc, nullptr, "sincos",        &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.sincos,    false);
    VariableDefinePlatformVar(pc, nullptr, "asinacos",      &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.asinacos,  false);
    VariableDefinePlatformVar(pc, nullptr, "signum",        &pc->IntType, (union AnyValue*)&scriptingData.floatOperators.signum,    false);

    //convertors
    VariableDefinePlatformVar(pc, nullptr, "mat_tr",        &pc->IntType, (union AnyValue*)&scriptingData.convertors.mat_tr,        false);
    VariableDefinePlatformVar(pc, nullptr, "tr_mat",        &pc->IntType, (union AnyValue*)&scriptingData.convertors.tr_mat,        false);
    VariableDefinePlatformVar(pc, nullptr, "mat_vecs4",     &pc->IntType, (union AnyValue*)&scriptingData.convertors.mat_vecs4,     false);
    VariableDefinePlatformVar(pc, nullptr, "mat_quat",      &pc->IntType, (union AnyValue*)&scriptingData.convertors.mat_quat,      false);
    VariableDefinePlatformVar(pc, nullptr, "mat_scalars",   &pc->IntType, (union AnyValue*)&scriptingData.convertors.mat_scalars,   false);
    VariableDefinePlatformVar(pc, nullptr, "vecs4_mat",     &pc->IntType, (union AnyValue*)&scriptingData.convertors.vecs4_mat,     false);
    VariableDefinePlatformVar(pc, nullptr, "vec4_vec3",     &pc->IntType, (union AnyValue*)&scriptingData.convertors.vec4_vec3,     false);
    VariableDefinePlatformVar(pc, nullptr, "vec4_scalars",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.vec4_scalars,  false);
    VariableDefinePlatformVar(pc, nullptr, "vecs3_mat",     &pc->IntType, (union AnyValue*)&scriptingData.convertors.vecs3_mat,     false);
    VariableDefinePlatformVar(pc, nullptr, "vec3_vec4",     &pc->IntType, (union AnyValue*)&scriptingData.convertors.vec3_vec4,     false);
    VariableDefinePlatformVar(pc, nullptr, "vec3_scalars",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.vec3_scalars,  false);
    VariableDefinePlatformVar(pc, nullptr, "quat_mat",      &pc->IntType, (union AnyValue*)&scriptingData.convertors.quat_mat,      false);
    VariableDefinePlatformVar(pc, nullptr, "quat_scalars",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.quat_scalars,  false);
    VariableDefinePlatformVar(pc, nullptr, "scalars_mat",   &pc->IntType, (union AnyValue*)&scriptingData.convertors.scalars_mat,   false);
    VariableDefinePlatformVar(pc, nullptr, "scalars_vec3",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.scalars_vec3,  false);
    VariableDefinePlatformVar(pc, nullptr, "scalars_vec4",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.scalars_vec4,  false);
    VariableDefinePlatformVar(pc, nullptr, "scalars_quat",  &pc->IntType, (union AnyValue*)&scriptingData.convertors.scalars_quat,  false);
    
    //quat oper
    VariableDefinePlatformVar(pc, nullptr, "scalarvec3_quat",   &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.scalarvec3_quat,false);
    VariableDefinePlatformVar(pc, nullptr, "angleaxis_quat",    &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.angleaxis_quat, false);
    VariableDefinePlatformVar(pc, nullptr, "vec3vec3_quat",     &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.vec3vec3_quat,  false);
    VariableDefinePlatformVar(pc, nullptr, "quat_scalarvec3",   &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.quat_scalarvec3,false);
    VariableDefinePlatformVar(pc, nullptr, "quat_angleaxis",    &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.quat_angleaxis, false);
    VariableDefinePlatformVar(pc, nullptr, "scalarmulquat",     &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.scalarmulquat,  false);
    VariableDefinePlatformVar(pc, nullptr, "quat_euler",        &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.quat_euler,     false);
    VariableDefinePlatformVar(pc, nullptr, "euler_quat",        &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.euler_quat,     false);
    VariableDefinePlatformVar(pc, nullptr, "slerp",             &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.slerp,          false);
    VariableDefinePlatformVar(pc, nullptr, "longslerp",         &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.longslerp,      false);
    VariableDefinePlatformVar(pc, nullptr, "lerp",              &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.lerp,           false);
    VariableDefinePlatformVar(pc, nullptr, "conjugate",         &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.conjugate,      false);
    VariableDefinePlatformVar(pc, nullptr, "qvq",               &pc->IntType, (union AnyValue*)&scriptingData.quatOperators.qvq,            false);
    //node lod
    VariableDefinePlatformVar(pc, nullptr, "full",          &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.full,            false);
    VariableDefinePlatformVar(pc, nullptr, "setvalues",     &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.setvalues,       false);
    VariableDefinePlatformVar(pc, nullptr, "label",         &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.label,           false);
}
