#include "../interpreter.h"
#include "libraryI3T.h"
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include<stdio.h>

WorkspaceLayout workspaceLayout;
struct Mat4types{
    const int free=0,scale=1,uniscale=2,rotatex=3,rotatey=4,rotatez=5,translate=6;
}mat4types;

WorkspaceLayout*getWorkspaceLayout(){return &workspaceLayout;}
void clearWorkspaceLayout(){
    workspaceLayout.matrix4x4Nodes.clear(); 
    workspaceLayout.nodePlugs.clear(); 
}

void mat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int dataindex=Param[1]->Val->Integer;
    int x=Param[2]->Val->Integer;
    int y=Param[3]->Val->Integer;

    NodeMatrix4x4 nm = NodeMatrix4x4(new WorkspaceMatrix4x4((ImTextureID)0), x, y);
    glm::mat4 mat = glm::mat4(1.0f);
    if (dataindex > -1 && dataindex < workspaceLayout.nodeData.size()) {mat = workspaceLayout.nodeData[dataindex];}

    if(type==mat4types.free){      
        nm.node->Nodebase->setDataMap(Core::Transform::g_Free);
        ValueSetResult result = nm.node->Nodebase->setValue(mat);
    }
    else if(type==mat4types.scale){     
        nm.node->Nodebase->setDataMap(Core::Transform::g_Scale);
        ValueSetResult result = nm.node->Nodebase->setValue(glm::vec4(mat[0][0], mat[1][1], mat[2][2], mat[3][3]));
    }
    else if(type==mat4types.uniscale){  nm.node->Nodebase->setDataMap(Core::Transform::g_UniformScale);}
    else if(type==mat4types.rotatex){  
        nm.node->Nodebase->setDataMap(Core::Transform::g_EulerX);
        ValueSetResult result = nm.node->Nodebase->setValue(mat[0][0]);
    }
    else if(type==mat4types.rotatey){   nm.node->Nodebase->setDataMap(Core::Transform::g_EulerY);}
    else if(type==mat4types.rotatez){   nm.node->Nodebase->setDataMap(Core::Transform::g_EulerZ);}
    else if(type==mat4types.translate){ nm.node->Nodebase->setDataMap(Core::Transform::g_Translate);}

    workspaceLayout.matrix4x4Nodes.push_back(nm);

    ReturnValue->Val->Integer = (int)workspaceLayout.matrix4x4Nodes.size() - 1;

}
void plugNodes(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int indexa=Param[0]->Val->Integer;
    int indexb=Param[1]->Val->Integer;
    int outputindex= Param[2]->Val->Integer;
    int inputindex= Param[3]->Val->Integer;
    if(indexa>=workspaceLayout.matrix4x4Nodes.size()||indexa < 0){ReturnValue->Val->Integer=FALSE;return;}
    if(indexb>=workspaceLayout.matrix4x4Nodes.size()||indexb < 0){ReturnValue->Val->Integer=FALSE;return;}

    workspaceLayout.nodePlugs.push_back(NodePlug(indexa,indexb,outputindex,inputindex));
    ReturnValue->Val->Integer=TRUE;
}
void datamat4free(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){m[i][j]=(float)Param[j+4*i]->Val->FP; }
    }
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void datamat4scale(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m=glm::mat4(1.0f);
    m[0][0]=(float)Param[0]->Val->FP;(float)m[1][1]=Param[1]->Val->FP;m[2][2]=(float)Param[2]->Val->FP;
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
void datamat4rot(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = (float)Param[0]->Val->FP;
    workspaceLayout.nodeData.push_back(m);
    ReturnValue->Val->Integer = (int)workspaceLayout.nodeData.size() - 1;
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
{
	{ mat4,         "int mat4(int,int,int,int);" },
	{ plugNodes,    "bool plugnodes(int,int,int,int);" },
	{ datamat4free, "int datamat4free(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);" },
	{ datamat4scale,"int datamat4scale(float,float,float);" },
	{ datamat4rot,  "int datamat4rot(float);" },
    { NULL,         NULL }
};

const char defs[]="typedef int bool;typedef void node;";

void PlatformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, NULL);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", NULL, &PlatformLibrary1[0], defs);//ADD_CUSTOM
    
    VariableDefinePlatformVar(pc, NULL, "free",     &pc->IntType, (union AnyValue *)&mat4types.free,        FALSE);
    VariableDefinePlatformVar(pc, NULL, "scale",    &pc->IntType, (union AnyValue *)&mat4types.scale,       FALSE);
    VariableDefinePlatformVar(pc, NULL, "uniscale", &pc->IntType, (union AnyValue *)&mat4types.uniscale,    FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatex",  &pc->IntType, (union AnyValue *)&mat4types.rotatex,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatey",  &pc->IntType, (union AnyValue *)&mat4types.rotatey,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "rotatez",  &pc->IntType, (union AnyValue *)&mat4types.rotatez,     FALSE);
    VariableDefinePlatformVar(pc, NULL, "translate",&pc->IntType, (union AnyValue *)&mat4types.translate,   FALSE);
}

