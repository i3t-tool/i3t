#include "../interpreter.h"
#include<stdio.h>

int cosi=666;

void PrintAdd(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)  // gps_dist(lat1, lon1, lat2, lon2)
{
    int a, b;
    a = Param[0]->Val->Integer;
    b = Param[1]->Val->Integer;
    printf("%d+%d=%d\n",a,b,a+b);
    //ReturnValue->Val->Typ;
    ReturnValue->Val->Integer = 888;
} 
void createGameObject(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)  // gps_dist(lat1, lon1, lat2, lon2)
{
	ReturnValue->Val->Pointer=(void*)115;
	printf("create\n");
}
void parentGameObject(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)  // gps_dist(lat1, lon1, lat2, lon2)
{
	void *a, *b;
    a = Param[0]->Val->Pointer;
    b = Param[1]->Val->Pointer;
    printf("parent\n");
}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary1[] =
{
	{ PrintAdd,     	"int printadd(int,int);" },
	{ createGameObject, "GameObject *createGameObject();" },
	{ parentGameObject, "void parentGameObject(GameObject*,GameObject*);" },
    { NULL,         NULL }
};
/*struct mat4{
	float f[16];
};*/
const char defs[]="typedef void GameObject;";

void PlatformLibraryInitI3T(Picoc *pc)
{
	//TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "mat4"), sizeof(struct mat4));
	//TypeParse(&Parser, &ReturnType, &Identifier, NULL);
	//printf("AAAAAAAA");
    //LibraryAdd(&GlobalTable, "platform library", &PlatformLibrary1);
    IncludeRegister(pc, "I3T.h", NULL, &PlatformLibrary1[0], defs);//ADD_CUSTOM
    
    VariableDefinePlatformVar(pc, NULL, "cosi", &pc->IntType, (union AnyValue *)&cosi, TRUE);
}

