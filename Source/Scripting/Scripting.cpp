#include "picoc.h"

#include "Scripting.h"
#include "platform/libraryI3T.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */


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
  PicocIncludeAllSystemHeaders(&pc);
  PicocParse(&pc, "somefilename", source, (int)strlen(source), TRUE, TRUE, TRUE, TRUE);
  PicocCleanup(&pc);
  return pc.PicocExitValue;
}