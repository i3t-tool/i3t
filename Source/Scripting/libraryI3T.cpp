#include "Dependencies/picoc/picoc.h"
#include "Core/API.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

//operators {

//	transformations{
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixRotate.h"
#include "GUI/Elements/Nodes/WorkspaceMakeEulerX.h"
#include "GUI/Elements/Nodes/WorkspaceMakeEulerY.h"
#include "GUI/Elements/Nodes/WorkspaceMakeEulerZ.h"
#include "GUI/Elements/Nodes/WorkspaceMakeFrustum.h"
#include "GUI/Elements/Nodes/WorkspaceMakeLookAt.h"
#include "GUI/Elements/Nodes/WorkspaceMakeOrtho.h"
#include "GUI/Elements/Nodes/WorkspaceMakePerspective.h"
//	} tranformationa end

//	matrix{
#include "GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixInversion.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixMulMatrix.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranspose.h"
#include "GUI/Elements/Nodes/WorkspaceDeterminant.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixAddMatrix.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixMulFloat.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixMulVector.h"
#include "GUI/Elements/Nodes/WorkspaceVectorMulMatrix.h"
//	} matrix end
#include "GUI/Elements/Nodes/WorkspaceSequence.h"

//	vec4{
#include "GUI/Elements/Nodes/WorkspaceVectorFree.h"
#include "GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
//	} vec4 end

//	float{
#include "GUI/Elements/Nodes/WorkspaceFloatFree.h"
//	} float end


#include "libraryI3T.h"
#include "Scripting/Scripting.h"
#include <stdio.h>

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
    
	if(type==scriptingData.mat4Operators.determinant){
		workspace->push_back(std::make_unique<WorkspaceDeterminant>((ImTextureID)0, l));
	}
    else if(type==scriptingData.mat4Operators.inverse){
		workspace->push_back(std::make_unique<WorkspaceMatrixInversion>((ImTextureID)0, l));
	}
    else if(type==scriptingData.mat4Operators.matmul){
        workspace->push_back(std::make_unique<WorkspaceMatrixMulMatrix>((ImTextureID)0, l));
	}
    else if(type==scriptingData.mat4Operators.matadd){
        workspace->push_back(std::make_unique<WorkspaceMatrixAddMatrix>((ImTextureID)0, l));
	}
    else if (type == scriptingData.mat4Operators.matmulvec) {
        workspace->push_back(std::make_unique<WorkspaceMatrixMulVector>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.vecmulmat) {
        workspace->push_back(std::make_unique<WorkspaceVectorMulMatrix>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.floatmulmat) {
        workspace->push_back(std::make_unique<WorkspaceMatrixMulFloat>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.matrix) {
        workspace->push_back(std::make_unique<WorkspaceMatrixFree>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.trackball) {
        return;
    }
    else if (type == scriptingData.mat4Operators.transpose) {
        workspace->push_back(std::make_unique<WorkspaceMatrixTranspose>((ImTextureID)0, l));
    }

    else if (type == scriptingData.mat4Operators.ortho) {
        workspace->push_back(std::make_unique<WorkspaceMakeOrtho>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.perspective) {
        workspace->push_back(std::make_unique<WorkspaceMakePerspective>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.frustrum) {
        workspace->push_back(std::make_unique<WorkspaceMakeFrustum>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.axisangle) {
        workspace->push_back(std::make_unique<WorkspaceMatrixRotate>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.rotatex) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerX>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.rotatey) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerY>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.rotatez) {
        workspace->push_back(std::make_unique<WorkspaceMakeEulerZ>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.scale) {
        workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.lookAt) {
        workspace->push_back(std::make_unique<WorkspaceMakeLookAt>((ImTextureID)0, l));
    }
    else if (type == scriptingData.mat4Operators.translate) {
        workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, l));
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
    returnValue->Val->Integer = (int)workspace->size()-1;
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
    

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	if(type== scriptingData.mat4Transforms.free){
        returnValue->Val->Integer = -1; return;
	}
	else if (type == scriptingData.mat4Transforms.scale) {
		workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, l));
		ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec3)mat[0]);
	}
	else if (type == scriptingData.mat4Transforms.translate) {
		workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, l));
		ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec3)mat[0]);
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
    
    returnValue->Val->Integer = (int)workspace->size()-1;
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
    

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	workspace->push_back(std::make_unique<WorkspaceVectorFree>((ImTextureID)0, l));
	ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec4)mat[0]);

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(numArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }
    
    returnValue->Val->Integer = (int)workspace->size()-1;
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
    if(type == scriptingData.vec4Operators.norm){
        workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, l));
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
    returnValue->Val->Integer = (int)workspace->size()-1;
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


    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    workspace->push_back(std::make_unique<WorkspaceFloatFree>((ImTextureID)0, l));
    ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue(mat[0][0]);

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = (int)workspace->size() - 1;
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

    workspace->push_back(std::make_unique<WorkspaceSequence>((ImTextureID)0, l));

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (numArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    returnValue->Val->Integer = (int)workspace->size() - 1;
}
void plugNodes(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs) {
    int indexa=param[0]->Val->Integer;
    int indexb=param[1]->Val->Integer;
    int outputindex=param[2]->Val->Integer;
    int inputindex= param[3]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(indexa<0||indexa>=workspace->size()){returnValue->Val->Integer=false;return;}
    if(indexb<0||indexb>=workspace->size()){returnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->getNodebase();
	Ptr<Core::NodeBase> pcb= (workspace->at(indexb).get())->getNodebase();

    if(outputindex>=pca->getOutputPins().size()){returnValue->Val->Integer=false;return;}
    if(inputindex>= pcb->getInputPins().size()){ returnValue->Val->Integer=false;return;}

	ENodePlugResult p = Core::GraphManager::plug(pca,pcb, outputindex, inputindex);
    returnValue->Val->Integer =(int)p==0;
}
void unplugInput(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int indexa=param[0]->Val->Integer;
    int inputindex= param[1]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

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
            returnValue->Val->Integer= i;
            if(occurances>1){std::cout<<"Index: "<<i<<", Name: "<<label<<std::endl;}
        }
    }

}
void deleteNode(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int index=param[0]->Val->Integer;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(index<0||index>=workspace->size()){returnValue->Val->Integer=false;std::cout<<"Node does not exist."<<std::endl;return;}

    WorkspaceNodeWithCoreData* cc= (workspace->at(index).get());
    Ptr<WorkspaceWindow> ww = I3T::getWindowPtr<WorkspaceWindow>();
    ww->NodeDelete(cc->getId());

    returnValue->Val->Integer=true;
}
void confNode(struct ParseState* parser, struct Value* returnValue, struct Value** param, int numArgs){
    int index=param[0]->Val->Integer;
    int precision=param[1]->Val->Integer;
    int lod=param[2]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

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
/* list of all library functions and their prototypes */
struct LibraryFunction platformLibraryI3T[] =
{
	{ mat4oper,     "int mat4oper(int,int,int,char*);" },         { mat4oper,     "int mat4operc(int);" },
	{ mat4,         "int mat4(int,int,int,int,char*);" },         { mat4,         "int mat4c(int,int);" },
    { vec4oper,     "int vec4oper(int,int,int,char*);" },         { vec4oper,     "int vec4operc(int);" },
	{ vec4,         "int vec4(int,int,int,char*);"     },         { vec4,         "int vec4c(int);" },
    { scalar,       "int scalar(int,int,int,char*);"   },         { scalar,       "int scalarc(int);" },
	{ sequence,     "int sequence(int,int,char*);"     },         { sequence,     "int sequencec(char*);" },
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
    VariableDefinePlatformVar(pc, nullptr, "free",          &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.free,         false);
    VariableDefinePlatformVar(pc, nullptr, "uniscale",      &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.uniscale,     false);
    
    //mat4 transform intersect mat4oper
    VariableDefinePlatformVar(pc, nullptr, "scale",         &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.scale,        false);
    VariableDefinePlatformVar(pc, nullptr, "rotatex",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.rotatex,      false);
    VariableDefinePlatformVar(pc, nullptr, "rotatey",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.rotatey,      false);
    VariableDefinePlatformVar(pc, nullptr, "rotatez",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.rotatez,      false);
    VariableDefinePlatformVar(pc, nullptr, "translate",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Transforms.translate,    false);
    VariableDefinePlatformVar(pc, nullptr, "axisangle",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.axisangle,     false);
    VariableDefinePlatformVar(pc, nullptr, "ortho",         &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.ortho,         false);
    VariableDefinePlatformVar(pc, nullptr, "perspective",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.perspective,   false);
    VariableDefinePlatformVar(pc, nullptr, "frustrum",      &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.frustrum,      false);
    VariableDefinePlatformVar(pc, nullptr, "lookat",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.lookAt,        false);

    //mat4oper
    VariableDefinePlatformVar(pc, nullptr, "determinant",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.determinant,   false);
    VariableDefinePlatformVar(pc, nullptr, "inverse",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.inverse,       false);
    VariableDefinePlatformVar(pc, nullptr, "matadd",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matadd,        false);
    VariableDefinePlatformVar(pc, nullptr, "matmul",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matmul,        false);
    VariableDefinePlatformVar(pc, nullptr, "matrix",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matrix,        false);
    VariableDefinePlatformVar(pc, nullptr, "trackball",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.trackball,     false);
    VariableDefinePlatformVar(pc, nullptr, "transpose",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.transpose,     false);


    //vec4oper
    VariableDefinePlatformVar(pc, nullptr, "norm",          &pc->IntType, (union AnyValue*)&scriptingData.vec4Operators.norm,        false);

    //node lod
    VariableDefinePlatformVar(pc, nullptr, "full",          &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.full,             false);
    VariableDefinePlatformVar(pc, nullptr, "setvalues",     &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.setvalues,        false);
    VariableDefinePlatformVar(pc, nullptr, "label",         &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.label,            false);
}