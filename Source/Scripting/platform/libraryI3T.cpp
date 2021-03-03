#include "../interpreter.h"
#include "libraryI3T.h"
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include<stdio.h>

WorkspaceLayout workspaceLayout;

const int mat4scale=0,mat4rotatex=1;

WorkspaceLayout*getWorkspaceLayout(){return &workspaceLayout;}
void clearWorkspaceLayout(){
    workspaceLayout.matrix4x4Nodes.clear(); 
    workspaceLayout.nodePlugs.clear(); 
}

void CreateMat4(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int type=Param[0]->Val->Integer;
    int rotation=Param[1]->Val->Integer;
    int x=Param[2]->Val->Integer;
    int y=Param[3]->Val->Integer;

    NodeMatrix4x4 nm= NodeMatrix4x4(new WorkspaceMatrix4x4((ImTextureID)0), x, y);
    workspaceLayout.matrix4x4Nodes.push_back(nm);

    ReturnValue->Val->Integer=(int)workspaceLayout.matrix4x4Nodes.size()-1;
}
void PlugNodes(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
    int indexa=Param[0]->Val->Integer;
    int indexb=Param[1]->Val->Integer;
    int outputindex= Param[2]->Val->Integer;
    int inputindex= Param[3]->Val->Integer;
    if(indexa>=workspaceLayout.matrix4x4Nodes.size()|| indexb >= workspaceLayout.matrix4x4Nodes.size()){ReturnValue->Val->Integer=FALSE;return;}
    if(indexa<0|| indexb <0){ReturnValue->Val->Integer=FALSE;return;}

    //Ptr<Core::NodeBase> pca= dynamic_cast<WorkspaceNodeBaseData*>((WorkspaceNode*)workspaceLayout.matrix4x4Nodes[a].node)->Nodebase;
    //Ptr<Core::NodeBase> pcb= dynamic_cast<WorkspaceNodeBaseData*>((WorkspaceNode*)workspaceLayout.matrix4x4Nodes[b].node)->Nodebase;

    workspaceLayout.nodePlugs.push_back(NodePlug(indexa,indexb,outputindex,inputindex));
    ReturnValue->Val->Integer=TRUE;

}
void createGameObject(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	ReturnValue->Val->Pointer=(void*)115;
	printf("create\n");
}
void parentGameObject(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	void *a, *b;
    a = Param[0]->Val->Pointer;
    b = Param[1]->Val->Pointer;
    printf("parent\n");
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
{
	{ CreateMat4,     	"int createMat4(int,int,int,int);" },
	{ PlugNodes,     	"bool plugNodes(int,int,int,int);" },
	{ createGameObject, "GameObject *createGameObject();" },
	{ parentGameObject, "void parentGameObject(GameObject*,GameObject*);" },
    { NULL,         NULL }
};
/*struct mat4{
	float f[16];
};*/
const char defs[]="typedef void GameObject;typedef int bool;typedef void node;";

void PlatformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, NULL);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", NULL, &PlatformLibrary1[0], defs);//ADD_CUSTOM
    
    VariableDefinePlatformVar(pc, NULL, "mat4scale", &pc->IntType, (union AnyValue *)&mat4scale, FALSE);
    VariableDefinePlatformVar(pc, NULL, "mat4rotatex", &pc->IntType, (union AnyValue *)&mat4rotatex, FALSE);
}

