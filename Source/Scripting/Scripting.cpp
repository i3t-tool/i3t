#include "picoc.h"

#include "Scripting.h"
#include "platform/libraryI3T.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

void SaveWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	FILE*f=stdout;
	fprintf(f,"saving\n");
	
	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNode* w = _workspace->at(i).get();
		WorkspaceNodeBaseData* nodebasedata = dynamic_cast<WorkspaceNodeBaseData*>(w);
		Ptr<Core::NodeBase> nodebase = nodebasedata->Nodebase;
		ImVec2 pos = ne::GetNodePosition(w->Id);
		Core::Transform::DataMap data = nodebase.get()->getDataMap();
		const Operation* operation = nodebase.get()->getOperation();
		//operation->
		const char*keyword=operation->keyWord.c_str();

		char* function = "-";
		
		if(strcmp(keyword,"MatrixToMatrix")==0){
			function="mat4";
			char* subtype = "-";
			if (Core::Transform::eq(data, Core::Transform::g_EulerX)) {
				subtype = "rotatex"; 
				float r=nodebase->getData().getFloat();
				fprintf(f,"int d%d=datamat4rot(%f);\n",i,r);
			}
			else if (Core::Transform::eq(data, Core::Transform::g_EulerY)) { subtype = "rotatey"; }
			else if (Core::Transform::eq(data, Core::Transform::g_EulerZ)) { subtype = "rotatez"; }
			else if (Core::Transform::eq(data, Core::Transform::g_Free)) {
				subtype = "free"; 
				glm::mat4 m=nodebase->getData().getMat4();
				fprintf(f, "int d%d=datamat4free(%0.3f,%0.3f,%0.3f,%0.3f, %0.3f,%0.3f,%0.3f,%0.3f, %0.3f,%0.3f,%0.3f,%0.3f, %0.3f,%0.3f,%0.3f,%0.3f);\n",
					i,	m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[0][2], m[1][3],
						m[2][0], m[2][1], m[0][2], m[2][3], m[3][0], m[3][1], m[0][2], m[3][3] );
			}
			else if (Core::Transform::eq(data, Core::Transform::g_Scale)) {
				subtype = "scale";
				glm::vec4 s = nodebase->getData().getVec4();
				fprintf(f, "int d%d=datamat4scale(%f,%f,%f,%f);\n", i, s[0], s[1], s[2], s[3]);
			}
			else if (Core::Transform::eq(data, Core::Transform::g_Translate)) { subtype = "translate"; }
			else if (Core::Transform::eq(data, Core::Transform::g_UniformScale)) { subtype = "uniscale"; }

			fprintf(f,"int n%d=%s(%s,d%d,%d,%d);\n",i, function, subtype,i, (int)pos[0], (int)pos[1]);
		}
		else if (strcmp(keyword, "NormalizeVector") == 0) {
			function = "normvector4";
			glm::vec4 s = nodebase->getData().getVec4();
			fprintf(f, "int d%d=datavec4(%f,%f,%f,%f);\n", i, s[0], s[1], s[2], s[3]);
			fprintf(f,"int n%d=%s(d%d,%d,%d);\n", i,function,i, (int)pos[0], (int)pos[1]);
		}
		else {
			function = "undefined";
			fprintf(f,"int n%d=%s(float*?,%d,%d);\n",i, function, (int)pos[0], (int)pos[1]);
		}

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
			if(parentindex>-1&& i > -1 && indexout > -1 && indexin > -1){fprintf(f,"plugnodes(n%d,n%d,%d,%d);\n", parentindex,i,indexout,indexin);}
		}
	}
	fprintf(f,"saved\n");
}
void LoadWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	PicocRunFile(filename);
	WorkspaceLayout*ret=getWorkspaceLayout();
	int startlen=(int)_workspace->size();
	for (int i = 0; i < ret->matrix4x4Nodes.size();i++) {
		_workspace->push_back(std::make_unique<WorkspaceMatrix4x4>(ret->matrix4x4Nodes[i].node));
		dynamic_cast<WorkspaceNodeBaseData*>(_workspace->back().get())->Nodebase.get()->setDataMap(ret->matrix4x4Nodes[i].node->Nodebase->getDataMap());//nn datamap must be set again (resets after pushing to workspace?)
		dynamic_cast<WorkspaceNodeBaseData*>(_workspace->back().get())->Nodebase.get()->setInternalValue(ret->matrix4x4Nodes[i].node->Nodebase->getData().getMat4());
		ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)ret->matrix4x4Nodes[i].x, (float)ret->matrix4x4Nodes[i].y));
		//ret->matrix4x4Nodes[i].node->Nodebase->setInternalValue(m);
	}

	for (int i = 0; i < ret->nodePlugs.size(); i++) {
		int indexA=ret->nodePlugs[i].indexA+startlen;
		int indexB=ret->nodePlugs[i].indexB+startlen;
		Ptr<Core::NodeBase> pca= dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexA).get())->Nodebase;
		Ptr<Core::NodeBase> pcb= dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexB).get())->Nodebase;


		ENodePlugResult p = Core::GraphManager::plug(pca,pcb, ret->nodePlugs[i].indexPinA, ret->nodePlugs[i].indexPinB);
		dynamic_cast<WorkspaceNodeBaseData*>(_workspace->at(indexB).get())->WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(getLinkID()));

	}
	/*WorkspaceMatrix4x4* nm = new WorkspaceMatrix4x4((ImTextureID)0);
	nm->Nodebase->setDataMap(Core::Transform::g_UniformScale);
	_workspace->push_back(std::make_unique<WorkspaceMatrix4x4>(nm));
	ne::SetNodePosition(_workspace->back()->Id, ImVec2(200.0f, 600.0f));
	Ptr<Core::NodeBase> nn = dynamic_cast<WorkspaceNodeBaseData*>(_workspace->back().get())->Nodebase;
	nn.get()->setDataMap(nm->Nodebase->getDataMap());//nn datamap must be set again (resets after pushing to workspace?)
	Core::Transform::DataMap data = nn.get()->getDataMap();
	char* subtype = "-";
	if (Core::Transform::eq(data, Core::Transform::g_EulerX)) { subtype = "rotatex"; }
	else if (Core::Transform::eq(data, Core::Transform::g_EulerY)) { subtype = "rotatey"; }
	else if (Core::Transform::eq(data, Core::Transform::g_EulerZ)) { subtype = "rotatez"; }
	else if (Core::Transform::eq(data, Core::Transform::g_Free)) { subtype = "free"; }
	else if (Core::Transform::eq(data, Core::Transform::g_Scale)) { subtype = "scale"; }
	else if (Core::Transform::eq(data, Core::Transform::g_Translate)) { subtype = "translate"; }
	else if (Core::Transform::eq(data, Core::Transform::g_UniformScale)) { subtype = "uniscale"; }
	printf("AAAA %s ", subtype);*/

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