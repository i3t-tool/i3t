#include "Dependencies/picoc/picoc.h"
#include "Core/API.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixInversion.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixMulMatrix.h"
#include "GUI/Elements/Nodes/WorkspaceDeterminant.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranspose.h"
#include "GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
#include "GUI/Elements/Nodes/WorkspaceVectorFree.h"
#include "GUI/Elements/Nodes/WorkspaceFloatFree.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"

#include "libraryI3T.h"
#include "Scripting/Scripting.h"
#include <stdio.h>

ScriptingData scriptingData;

ScriptingData*getScriptingData(){return &scriptingData;}
void clearScriptingData(){
    scriptingData.nodeData.clear();
}
void mat4oper(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int x=0,y=0;
    char*l="-";
    if(NumArgs==4){
        x=Param[1]->Val->Integer;
        y=Param[2]->Val->Integer;
        if(Param[3]->Val->Pointer!=nullptr){l = (char*)Param[3]->Val->Pointer;}
    }
    
    if (type<0||type>6) {ReturnValue->Val->Integer = -1; return;}

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
        return;
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
    else {
        return;
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));

    if(NumArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }
    ReturnValue->Val->Integer = (int)workspace->size()-1;
}
void mat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int dataindex=Param[1]->Val->Integer;
    int x=0,y=0;
    char*l="-";
    if(NumArgs==5){
        x=Param[2]->Val->Integer;
        y=Param[3]->Val->Integer;
        if(Param[4]->Val->Pointer!=nullptr){l = (char*)Param[4]->Val->Pointer;}
    }
    

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }
    if(type<0||type>6){ ReturnValue->Val->Integer=-1;return;}

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	if(type== scriptingData.mat4Types.free){
        return;
	}
	else if (type == scriptingData.mat4Types.scale) {
		workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, l));
		ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec3)mat[0]);
	}
	else if (type == scriptingData.mat4Types.translate) {
		workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, l));
		ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec3)mat[0]);
	}

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(NumArgs==2){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }
    
    ReturnValue->Val->Integer = (int)workspace->size()-1;
}
void vec4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int dataindex=Param[0]->Val->Integer;
    int x=0,y=0;
    char*l="-";
    if(NumArgs==4){
        x=Param[1]->Val->Integer;
        y=Param[2]->Val->Integer;
        if(Param[3]->Val->Pointer!=nullptr){l = (char*)Param[3]->Val->Pointer;}
    }
    

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

	workspace->push_back(std::make_unique<WorkspaceVectorFree>((ImTextureID)0, l));
	ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue((glm::vec4)mat[0]);

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(NumArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label,"#%02u %s",workspace->back()->getNodebase()->getId(),workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }
    
    ReturnValue->Val->Integer = (int)workspace->size()-1;
}
void vec4oper(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type = Param[0]->Val->Integer;
    int x=0,y=0;
    char*l="-";
    if(NumArgs==4){
        x = Param[1]->Val->Integer;
        y = Param[2]->Val->Integer;
        if(Param[3]->Val->Pointer!=nullptr){l = (char*)Param[3]->Val->Pointer;}
    }
    if(type<0||type>0){ ReturnValue->Val->Integer=-1;return;}

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if(type == scriptingData.vec4Operators.norm){
        workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, l));
    }

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if(NumArgs==1){
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100]={0};
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
    }
    ReturnValue->Val->Integer = (int)workspace->size()-1;
}
void scalar(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int dataindex = Param[0]->Val->Integer;
    int x = 0, y = 0;
    char* l = "-";
    if (NumArgs == 4) {
        x = Param[1]->Val->Integer;
        y = Param[2]->Val->Integer;
        if (Param[3]->Val->Pointer != nullptr) { l = (char*)Param[3]->Val->Pointer; }
    }


    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    workspace->push_back(std::make_unique<WorkspaceFloatFree>((ImTextureID)0, l));
    ValueSetResult result = (workspace->back().get())->getNodebase().get()->setValue(mat[0][0]);

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (NumArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    ReturnValue->Val->Integer = (int)workspace->size() - 1;
}
void sequence(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int x = 0, y = 0;
    char* l = "-";
    if (NumArgs == 3) {
        x = Param[0]->Val->Integer;
        y = Param[1]->Val->Integer;
        if (Param[2]->Val->Pointer != nullptr) { l = (char*)Param[2]->Val->Pointer; }
    }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    workspace->push_back(std::make_unique<WorkspaceSequence>((ImTextureID)0, l));

    ne::SetNodePosition(workspace->back()->getId(), ImVec2((float)x, (float)y));
    if (NumArgs == 1) {
        ne::CenterNodeOnScreen(workspace->back()->getId());
        char label[100] = { 0 };
        sprintf(label, "#%02u %s", workspace->back()->getNodebase()->getId(), workspace->back()->getNodebase()->getOperation()->keyWord.c_str());
        workspace->back()->getHeaderLabel() = label;
    }

    ReturnValue->Val->Integer = (int)workspace->size() - 1;
}
void plugNodes(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int indexa=Param[0]->Val->Integer;
    int indexb=Param[1]->Val->Integer;
    int outputindex=Param[2]->Val->Integer;
    int inputindex= Param[3]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace = &(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(indexa<0||indexa>=workspace->size()){ReturnValue->Val->Integer=false;return;}
    if(indexb<0||indexb>=workspace->size()){ReturnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->getNodebase();
	Ptr<Core::NodeBase> pcb= (workspace->at(indexb).get())->getNodebase();

    if(outputindex>=pca->getOutputPins().size()){ReturnValue->Val->Integer=false;return;}
    if(inputindex>= pcb->getInputPins().size()){ ReturnValue->Val->Integer=false;return;}

	ENodePlugResult p = Core::GraphManager::plug(pca,pcb, outputindex, inputindex);
    ReturnValue->Val->Integer =(int)p==0;
}
void unplugInput(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int indexa=Param[0]->Val->Integer;
    int inputindex= Param[1]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(indexa<0||indexa>=workspace->size()){ReturnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->getNodebase();
    if(inputindex>= pca->getInputPins().size()){ ReturnValue->Val->Integer=false;return;}
    Core::GraphManager::unplugInput(pca, inputindex);

    ReturnValue->Val->Integer=true;
}
void getNodeByName(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    char* label=(char*)Param[0]->Val->Pointer;
    int occurances=0;
    ReturnValue->Val->Integer=-1;
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
            ReturnValue->Val->Integer= i;
            if(occurances>1){std::cout<<"Id: "<<i<<", Name: "<<label<<std::endl;}
        }
    }

}
void deleteNode(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int index=Param[0]->Val->Integer;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(index<0||index>=workspace->size()){ReturnValue->Val->Integer=false;std::cout<<"Node does not exist."<<std::endl;return;}

    WorkspaceNodeWithCoreData* cc= (workspace->at(index).get());
    Ptr<WorkspaceWindow> ww = I3T::getWindowPtr<WorkspaceWindow>();
    ww->NodeDelete(cc->getId());

    ReturnValue->Val->Integer=true;
}
void confNode(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int index=Param[0]->Val->Integer;
    int precision=Param[1]->Val->Integer;
    int lod=Param[2]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);

    if(index<0||index>=workspace->size()){ReturnValue->Val->Integer=false;return;}
    //if(precision<0||precision>4){precision==0;}
    if(lod<0||lod>3){lod=0;}

    WorkspaceNodeWithCoreData* cc= (workspace->at(index).get());
    cc->setNumberOfVisibleDecimal(precision);
    cc->setLevelOfDetail((WorkspaceLevelOfDetail)lod);

    ReturnValue->Val->Integer=true;
}
void dataMat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    glm::mat4 m;
    for(int i=0;i<4;i++){for(int j=0;j<4;j++){m[i][j]=(float)Param[j+4*i]->Val->FP; }}
    scriptingData.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataVec4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    glm::mat4 m=glm::mat4(1.0f);
    m[0][0]=(float)Param[0]->Val->FP;m[0][1]= (float)Param[1]->Val->FP;m[0][2]=(float)Param[2]->Val->FP; m[0][3] = (float)Param[3]->Val->FP;
    scriptingData.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataVec3(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)Param[0]->Val->FP; m[0][1] = (float)Param[1]->Val->FP; m[0][2] = (float)Param[2]->Val->FP;
    scriptingData.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void dataScalar(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)Param[0]->Val->FP;
    scriptingData.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)scriptingData.nodeData.size() - 1;
}
void loadW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename= Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != nullptr) {ww->m_workspaceCoreNodes.clear(); status=LoadWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer=status;
}
void appendW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename= Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != nullptr) {status=LoadWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer=status;
}
void saveW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename = Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != nullptr) {status=SaveWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer = status;
}
void runScript(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    ReturnValue->Val->Integer=PicocRunFile((char*)Param[0]->Val->Pointer)==0;
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
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
	{ runScript,    "bool run(char*);" },
    { nullptr,         nullptr }
};

const char defs[]="typedef int bool;";

void PlatformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, nullptr, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, nullptr);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", nullptr, PlatformLibrary1, defs);//ADD_CUSTOM

    VariableDefinePlatformVar(pc, nullptr, (char*)"free",          &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.free,              false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"scale",         &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.scale,             false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"uniscale",      &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.uniscale,          false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"rotatex",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatex,           false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"rotatey",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatey,           false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"rotatez",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatez,           false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"translate",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.translate,         false);
    
    VariableDefinePlatformVar(pc, nullptr, (char*)"determinant",   &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.determinant,   false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"inverse",       &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.inverse,       false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"matadd",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matadd,        false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"matmul",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matmul,        false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"matrix",        &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.matrix,        false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"trackball",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.trackball,     false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"transpose",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Operators.transpose,     false);

    VariableDefinePlatformVar(pc, nullptr, (char*)"norm",          &pc->IntType, (union AnyValue*)&scriptingData.vec4Operators.norm,        false);

    VariableDefinePlatformVar(pc, nullptr, (char*)"full",          &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.full,             false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"setvalues",     &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.setvalues,        false);
    VariableDefinePlatformVar(pc, nullptr, (char*)"label",         &pc->IntType, (union AnyValue*)&scriptingData.nodeLODs.label,            false);
}