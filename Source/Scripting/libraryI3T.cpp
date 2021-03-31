#include "Dependencies/picoc/picoc.h"
#include "Core/API.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include "libraryI3T.h"
#include "Scripting/Scripting.h"
#include <stdio.h>

ScriptingData scriptingData;

ScriptingData*getScriptingData(){return &scriptingData;}
void clearScriptingData(){
    scriptingData.nodeData.clear();
}

void mat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int dataindex=Param[1]->Val->Integer;
    int x=0,y=0;
    if(NumArgs==4){
        x=Param[2]->Val->Integer;
        y=Param[3]->Val->Integer;
    }
    

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { mat = scriptingData.nodeData[dataindex]; }
    int map=scriptingData.mat4Types.free;
    if (type > -1 && type < 7) { map = type; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    char label[100]={0};

	if(type== scriptingData.mat4Types.free){
		workspace->push_back(std::make_unique<WorkspaceMatrixFree>((ImTextureID)0, "-"));
        sprintf(label,"#%02llu %s",workspace->back()->m_nodebase->getId(),workspace->back()->m_nodebase->getOperation()->keyWord.c_str());
        workspace->back()->m_headerLabel = label;
		ValueSetResult result =(workspace->back().get())->m_nodebase.get()->setValue(mat);
		ne::SetNodePosition(workspace->back()->m_id, ImVec2((float)x, (float)y));
	}
	else if (type == scriptingData.mat4Types.scale) {
		workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, "-"));
        sprintf(label,"#%02llu %s",workspace->back()->m_nodebase->getId(),workspace->back()->m_nodebase->getOperation()->keyWord.c_str());
        workspace->back()->m_headerLabel = label;
		ValueSetResult result = (workspace->back().get())->m_nodebase.get()->setValue((glm::vec3)mat[0]);
		ne::SetNodePosition(workspace->back()->m_id, ImVec2((float)x, (float)y));
	}
	else if (type == scriptingData.mat4Types.translate) {
		workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, ""));
        sprintf(label,"#%02llu %s",workspace->back()->m_nodebase->getId(),workspace->back()->m_nodebase->getOperation()->keyWord.c_str());
        workspace->back()->m_headerLabel= label;
		ValueSetResult result = (workspace->back().get())->m_nodebase.get()->setValue((glm::vec3)mat[0]);
		ne::SetNodePosition(workspace->back()->m_id, ImVec2((float)x, (float)y));
	}
    if(NumArgs==2){ne::CenterNodeOnScreen(workspace->back()->m_id);}
    ReturnValue->Val->Integer = (int)workspace->size() - 1;
}
void normVec4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int dataindex = Param[0]->Val->Integer;
    int x=0,y=0;
    if(NumArgs==3){
        x = Param[1]->Val->Integer;
        y = Param[2]->Val->Integer;
    }
    glm::vec4 vec = glm::vec4(1.0f);
    if (dataindex > -1 && dataindex < scriptingData.nodeData.size()) { vec = scriptingData.nodeData[dataindex][0]; }

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    char label[100]={0};
    ReturnValue->Val->Integer = (int)workspace->size() - 1;

    workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, "-"));
    sprintf(label,"#%02llu %s",workspace->back()->m_nodebase->getId(),workspace->back()->m_nodebase->getOperation()->keyWord.c_str());
    workspace->back()->m_headerLabel = label;
    ValueSetResult result = (workspace->back().get())->m_nodebase.get()->setValue(vec);
    ne::SetNodePosition(workspace->back()->m_id, ImVec2((float)x, (float)y));
    if(NumArgs==1){ne::CenterNodeOnScreen(workspace->back()->m_id);}
    ReturnValue->Val->Integer = (int)workspace->size() - 1;
}
void plugNodes(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int indexa=Param[0]->Val->Integer;
    int indexb=Param[1]->Val->Integer;
    int outputindex=Param[2]->Val->Integer;
    int inputindex= Param[3]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if(indexa<0||indexa>=workspace->size()-1){ReturnValue->Val->Integer=false;return;}
    if(indexb<0||indexb>=workspace->size()-1){ReturnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->m_nodebase;
	Ptr<Core::NodeBase> pcb= (workspace->at(indexb).get())->m_nodebase;

    if(outputindex>=pca->getOutputPins().size()){ReturnValue->Val->Integer=false;return;}
    if(inputindex>= pcb->getInputPins().size()){ ReturnValue->Val->Integer=false;return;}

	ENodePlugResult p = Core::GraphManager::plug(pca,pcb, outputindex, inputindex);
    ReturnValue->Val->Integer =(int)p==0;
}
void unplugInput(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int indexa=Param[0]->Val->Integer;
    int inputindex= Param[1]->Val->Integer;

    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if(indexa<0||indexa>=workspace->size()-1){ReturnValue->Val->Integer=false;return;}

    Ptr<Core::NodeBase> pca= (workspace->at(indexa).get())->m_nodebase;
    if(inputindex>= pca->getInputPins().size()){ ReturnValue->Val->Integer=false;return;}
    Core::GraphManager::unplugInput(pca, inputindex);
}
void getNodeById(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int coreid=Param[0]->Val->Integer;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    for(int i=0;i<workspace->size();i++){
        if (workspace->at(i).get()->m_nodebase->getId() == (long long unsigned)coreid) {
            ReturnValue->Val->Integer=i;
            return;
        }
    }
    ReturnValue->Val->Integer=-1;
}
void deleteNode(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs){
    int index=Param[0]->Val->Integer;
    std::vector<Ptr<WorkspaceNodeWithCoreData>>* workspace=&(I3T::getWindowPtr<WorkspaceWindow>()->m_workspaceCoreNodes);
    if(index<0||index>=workspace->size()){ReturnValue->Val->Identifier=false;return;}
    Core::NodePtr c= (workspace->at(index).get())->m_nodebase;
    Core::GraphManager::unplugAll(c);
    workspace->at(index)=workspace->back();
    workspace->pop_back();
    ReturnValue->Val->Integer=TRUE;
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
    if (ww != NULL) {ww->m_workspaceCoreNodes.clear(); status=LoadWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer=status;
}
void appendW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename= Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != NULL) {status=LoadWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer=status;
}
void saveW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename = Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    auto ww = I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != NULL) {status=SaveWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer = status;
}
void runScript(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    ReturnValue->Val->Integer=PicocRunFile((char*)Param[0]->Val->Pointer)==0;
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
{
	{ mat4,         "int mat4(int,int,int,int);" },         { mat4,         "int mat4c(int,int);" },
	{ normVec4,     "int normvec4(int,int,int);" },         { normVec4,     "int normvec4c(int);" },
	{ plugNodes,    "bool plugnodes(int,int,int,int);" },
	{ unplugInput,  "void unpluginput(int,int);" },
	{ getNodeById,  "int getnode(int);" },
	{ deleteNode,   "bool delnode(int);" },
	{ dataMat4,     "int datamat4(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);" },
	{ dataVec4,     "int datavec4(float,float,float,float);" },
	{ dataVec3,     "int datavec3(float,float,float);" },
	{ dataScalar,   "int datascalar(float);" },
	{ loadW,        "bool load(char*);" },
	{ appendW,      "bool append(char*);" },
	{ saveW,        "bool save(char*);" },
	{ runScript,    "bool run(char*);" },
    { NULL,         NULL }
};

const char defs[]="typedef int bool;";

void PlatformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, NULL);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", NULL, PlatformLibrary1, defs);//ADD_CUSTOM

    VariableDefinePlatformVar(pc, NULL, (char*)"free",     &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.free,        false);
    VariableDefinePlatformVar(pc, NULL, (char*)"scale",    &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.scale,       false);
    VariableDefinePlatformVar(pc, NULL, (char*)"uniscale", &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.uniscale,    false);
    VariableDefinePlatformVar(pc, NULL, (char*)"rotatex",  &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatex,     false);
    VariableDefinePlatformVar(pc, NULL, (char*)"rotatey",  &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatey,     false);
    VariableDefinePlatformVar(pc, NULL, (char*)"rotatez",  &pc->IntType, (union AnyValue *)&scriptingData.mat4Types.rotatez,     false);
    VariableDefinePlatformVar(pc, NULL, (char*)"translate",&pc->IntType, (union AnyValue *)&scriptingData.mat4Types.translate,   false);
}

