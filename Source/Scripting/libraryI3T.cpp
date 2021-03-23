#include "Dependencies/picoc/picoc.h"
#include "Core/API.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "libraryI3T.h"
#include "Scripting/Scripting.h"
#include <stdio.h>

WorkspaceLayout workspaceLayout;

WorkspaceLayout*getWorkspaceLayout(){return &workspaceLayout;}
void clearWorkspaceLayout(){
    workspaceLayout.mat4Nodes.clear();
    workspaceLayout.normVec4Nodes.clear();
    workspaceLayout.nodePlugs.clear();
}

void mat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int dataindex=Param[1]->Val->Integer;
    int x=Param[2]->Val->Integer;
    int y=Param[3]->Val->Integer;

    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < workspaceLayout.nodeData.size()) { mat = workspaceLayout.nodeData[dataindex]; }
    int map=workspaceLayout.mat4Types.free;
    if (type > -1 && type < 7) { map = type; }

    NodeMat4 nm = NodeMat4(map,mat, x, y);
    workspaceLayout.mat4Nodes.push_back(nm);
    ReturnValue->Val->Integer = (int)workspaceLayout.mat4Nodes.size() - 1;
}
void normVec4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int dataindex = Param[0]->Val->Integer;
    int x = Param[1]->Val->Integer;
    int y = Param[2]->Val->Integer;

    glm::vec4 vec = glm::vec4(1.0f);
    if (dataindex > -1 && dataindex < workspaceLayout.nodeData.size()) { vec = workspaceLayout.nodeData[dataindex][0]; }

    NodeNormVec4 nm = NodeNormVec4(vec, x, y);
    workspaceLayout.normVec4Nodes.push_back(nm);
    ReturnValue->Val->Integer = (int)workspaceLayout.normVec4Nodes.size() - 1;
}
void plugNodes(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int indexa=Param[0]->Val->Integer;
    int indexb=Param[1]->Val->Integer;
    int outputindex= Param[2]->Val->Integer;
    int inputindex= Param[3]->Val->Integer;
    if(indexa>=workspaceLayout.mat4Nodes.size()||indexa < 0){ReturnValue->Val->Integer=FALSE;return;}
    if(indexb>=workspaceLayout.mat4Nodes.size()||indexb < 0){ReturnValue->Val->Integer=FALSE;return;}

    workspaceLayout.nodePlugs.push_back(NodePlug(indexa,indexb,outputindex,inputindex));
    ReturnValue->Val->Integer=TRUE;
}
void dataMat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m;
    for(int i=0;i<4;i++){for(int j=0;j<4;j++){m[i][j]=(float)Param[j+4*i]->Val->FP; }}
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void dataVec4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m=glm::mat4(1.0f);
    m[0][0]=(float)Param[0]->Val->FP;m[1][1]= (float)Param[1]->Val->FP;m[2][2]=(float)Param[2]->Val->FP; m[3][3] = (float)Param[3]->Val->FP;
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void dataVec3(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)Param[0]->Val->FP; m[1][1] = (float)Param[1]->Val->FP; m[2][2] = (float)Param[2]->Val->FP;
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void dataScalar(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)Param[0]->Val->FP;
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void loadW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename= Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    WorkspaceWindow* ww = (WorkspaceWindow*)I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != NULL) {ww->m_workspaceCoreNodes.clear(); status=LoadWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer=status;
}
void saveW(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    std::string filename = Config::getAbsolutePath((char*)Param[0]->Val->Pointer);
    WorkspaceWindow * ww = (WorkspaceWindow*)I3T::getWindowPtr<WorkspaceWindow>();
    bool status=false;
    if (ww != NULL) {status=SaveWorkspace(filename.c_str(), &ww->m_workspaceCoreNodes); }
    ReturnValue->Val->Integer = status;
}
void runScript(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
{
	{ mat4,         "int mat4(int,int,int,int);" },
	{ normVec4,     "int normvec4(int,int,int);" },
	{ plugNodes,    "bool plugnodes(int,int,int,int);" },
	{ dataMat4,     "int datamat4(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);" },
	{ dataVec4,     "int datavec4(float,float,float,float);" },
	{ dataVec3,     "int datavec3(float,float,float);" },
	{ dataScalar,   "int datascalar(float);" },
	{ loadW,        "bool load(char*);" },
	{ saveW,        "bool save(char*);" },
	{ runScript,    "bool run(char*);" },
    { NULL,         NULL }
};

const char defs[]="typedef int bool;typedef void node;";

void PlatformLibraryInitI3T(Picoc *pc)
{
    /* picoc problem */
	//TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, NULL);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", NULL, PlatformLibrary1, defs);//ADD_CUSTOM

    VariableDefinePlatformVar(pc, NULL, "free",     &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.free,        FALSE);
    VariableDefinePlatformVar(pc, NULL, "scale",    &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.scale,       FALSE);
    VariableDefinePlatformVar(pc, NULL, "uniscale", &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.uniscale,    FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatex",  &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.rotatex,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatey",  &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.rotatey,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatez",  &pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.rotatez,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "translate",&pc->IntType, (union AnyValue *)&workspaceLayout.mat4Types.translate,   FALSE);
}

