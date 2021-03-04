#include "picoc.h"

#include "Scripting.h"
#include "platform/libraryI3T.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

void SaveWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	printf("saving\n");
	
	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNode* w = _workspace->at(i).get();
		WorkspaceNodeBaseData* nodebasedata = dynamic_cast<WorkspaceNodeBaseData*>(w);
		Ptr<Core::NodeBase> nodebase = nodebasedata->Nodebase;

		printf("%d: ",i);
		char* type = "-";
		Core::Transform::DataMap data = nodebase.get()->getDataMap();
		if (Core::Transform::eq(data, Core::Transform::g_EulerX)) { type = "mat4.rotatex"; }
		else if (Core::Transform::eq(data, Core::Transform::g_EulerY)) { type = "mat4.rotatey"; }
		else if (Core::Transform::eq(data, Core::Transform::g_EulerZ)) { type = "mat4.rotatez"; }
		else if (Core::Transform::eq(data, Core::Transform::g_Free)) { type = "mat4.free"; }
		else if (Core::Transform::eq(data, Core::Transform::g_Scale)) { type = "mat4.scale"; }
		else if (Core::Transform::eq(data, Core::Transform::g_Translate)) { type = "mat4.translate"; }
		else if (Core::Transform::eq(data, Core::Transform::g_UniformScale)) { type = "mat4.uniscale"; }

		printf("type %s ", type);
		
		ImVec2 pos=ne::GetNodePosition(w->Id);
		printf("(%d,%d) ",(int)pos[0],(int)pos[1]);
		std::vector<Core::Pin>inputs = nodebase->getInputPins();
		for(int indexin=0;indexin<inputs.size();indexin++){
			Ptr<Core::NodeBase> parent = Core::GraphManager::getParent(nodebase,indexin);
			int parentindex = -1;
			int indexout = -1;
			if(inputs[indexin].isPluggedIn()){
				const Core::Pin*parentpin=inputs[indexin].getParentPin();
				if(parentpin!=NULL){indexout=parentpin->getIndex(); }
			}
			for (int j = 0; j < _workspace->size(); j++) {
				if (parent.get() == (dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(j).get()))->Nodebase.get()) {parentindex = j;}
			}
			if(parentindex>-1&& i > -1 && indexout > -1 && indexin > -1){printf("plugNodes(%d,%d,%d,%d) ", parentindex,i,indexout,indexin);}
		}
		printf("\n");
	}
	printf("saved\n");
}
void LoadWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	PicocRunFile(filename);
	WorkspaceLayout*ret=getWorkspaceLayout();
	int startlen=(int)_workspace->size();
	for (int i = 0; i < ret->matrix4x4Nodes.size();i++) {
		_workspace->push_back(std::make_unique<WorkspaceMatrix4x4>(ret->matrix4x4Nodes[i].node));
		ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)ret->matrix4x4Nodes[i].x, (float)ret->matrix4x4Nodes[i].y));
	}
	for (int i = 0; i < ret->nodePlugs.size(); i++) {
		int indexA=ret->nodePlugs[i].indexA+startlen;
		int indexB=ret->nodePlugs[i].indexB+startlen;
		Ptr<Core::NodeBase> pca= dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexA).get())->Nodebase;
		Ptr<Core::NodeBase> pcb= dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexB).get())->Nodebase;


		ENodePlugResult p = Core::GraphManager::plug(pca,pcb, ret->nodePlugs[i].indexPinA, ret->nodePlugs[i].indexPinB);
		dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexB).get())->WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(getLinkID()));

	}
	clearWorkspaceLayout();
	
}
int PicocRunInteractive(){
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  if(PicocPlatformSetExitPoint(&pc)){PicocCleanup(&pc); return pc.PicocExitValue;}
  PicocIncludeAllSystemHeaders(&pc);
  PicocParseInteractive(&pc);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
}
int PicocRunFile(const char* filename){
  FILE* f = fopen(filename, "r");
  if (f == NULL){return -1;}
  fclose(f);
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  if (PicocPlatformSetExitPoint(&pc)) { PicocCleanup(&pc); return pc.PicocExitValue; }
  PicocIncludeAllSystemHeaders(&pc);
  PicocPlatformScanFile(&pc, filename);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
  // for (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++){PicocPlatformScanFile(&pc,
  // argv[ParamCount]);}//run multiple files PicocCallMain(&pc, argc - ParamCount, &argv[ParamCount]);	//call main
}
int PicocRunSource(const char* source){
  int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
  Picoc pc;
  PicocInitialise(&pc, StackSize);
  if (PicocPlatformSetExitPoint(&pc)) { PicocCleanup(&pc); return pc.PicocExitValue; }
  PicocIncludeAllSystemHeaders(&pc);
  PicocParse(&pc, "somefilename", source, (int)strlen(source), TRUE, TRUE, TRUE, TRUE);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
}