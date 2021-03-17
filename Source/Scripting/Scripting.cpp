#include "Dependencies/picoc/picoc.h"

#include "Scripting.h"
#include "libraryI3T.h"
#include "Commands/ApplicationCommands.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool SaveWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	FILE*f=fopen(filename,"w");
	if(f==NULL){return false;}
	//FILE*f=stdout;//dont fclose
	fprintf(f,"//saving\n");
	
	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNode*				node		= _workspace->at(i).get();
		WorkspaceNodeWithCoreData*  nodebasedata= dynamic_cast<WorkspaceNodeWithCoreData*>(node);
		Ptr<Core::NodeBase>			nodebase	= nodebasedata->Nodebase;
		ImVec2						pos			= ne::GetNodePosition(node->Id);
		Core::Transform::DataMap	data		= nodebase->getDataMap();
		const Operation*			operation	= nodebase->getOperation();
		const char*					keyword		= operation->keyWord.c_str();

		if(strcmp(keyword,"MatrixToMatrix")==0){
			glm::mat4 m = nodebase->getData().getMat4();
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff);\n",
				i, m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[0][2], m[1][3],
				m[2][0], m[2][1], m[0][2], m[2][3], m[3][0], m[3][1], m[0][2], m[3][3]);
			fprintf(f, "int n%d=mat4(free,d%d,%d,%d);\n", i, i, (int)pos[0], (int)pos[1]);
		}
		else if (strcmp(keyword, "Scale") == 0) {
			glm::vec3 s = nodebase->getData().getVec3();
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(scale,d%d,%d,%d);\n", i, i, (int)pos[0], (int)pos[1]);
		}
		else if (strcmp(keyword, "Translation") == 0) {
			glm::vec3 s = nodebase->getData().getVec3();
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(translate,d%d,%d,%d);\n", i, i, (int)pos[0], (int)pos[1]);
		}
		else if (strcmp(keyword, "NormalizeVector") == 0) {
			glm::vec4 s = nodebase->getData().getVec4();
			fprintf(f,"int d%d=datavec4(%0.3ff,%0.3ff,%0.3ff,%0.3ff);\n", i, s[0], s[1], s[2], s[3]);
			fprintf(f,"int n%d=normvec4(d%d,%d,%d);\n", i,i, (int)pos[0], (int)pos[1]);
		}
		else {
			glm::mat4 m = nodebase->getData().getMat4();
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff);\n",
				i, m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[0][2], m[1][3],
				m[2][0], m[2][1], m[0][2], m[2][3], m[3][0], m[3][1], m[0][2], m[3][3]);
			fprintf(f, "//int n%d=%s(d%d,%d,%d);//unknown type\n", i, keyword,i, (int)pos[0], (int)pos[1]);
		}
	}
	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNode*				node = _workspace->at(i).get();
		WorkspaceNodeWithCoreData*  nodebasedata = dynamic_cast<WorkspaceNodeWithCoreData*>(node);
		Ptr<Core::NodeBase>			nodebase = nodebasedata->Nodebase;
		ImVec2						pos = ne::GetNodePosition(node->Id);
		Core::Transform::DataMap	data = nodebase->getDataMap();
		const Operation*			operation = nodebase->getOperation();
		const char* keyword =		operation->keyWord.c_str();

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
				if (parent.get() == (dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->at(j).get()))->Nodebase.get()) {parentindex = j;}
			}
			if(parentindex>-1&& i > -1 && indexout > -1 && indexin > -1){fprintf(f,"bool p%d_%d=plugnodes(n%d,n%d,%d,%d);\n",i,indexin, parentindex,i,indexout,indexin);}
		}
	}
	fprintf(f,"//saved\n");
	fclose(f);
	return true;
}
bool LoadWorkspace(const char* filename, std::vector<std::unique_ptr<WorkspaceNode>>* _workspace) {
	int p=PicocRunFile(filename);
	WorkspaceLayout*scene=getWorkspaceLayout();
	int startlen=(int)_workspace->size();
	for (int i = 0; i < scene->mat4Nodes.size();i++) {
		NodeMat4 node=scene->mat4Nodes[i];
		if(node.type==scene->mat4Types.free){
			_workspace->push_back(std::make_unique<WorkspaceMatrix4x4>(new WorkspaceMatrix4x4((ImTextureID)0,"load free")));
			ValueSetResult result =dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue(node.data);
			ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)node.x, (float)node.y));
		}
		else if (node.type == scene->mat4Types.scale) {
			_workspace->push_back(std::make_unique<WorkspaceMatrixScale>(new WorkspaceMatrixScale((ImTextureID)0, "load scale")));
			ValueSetResult result = dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue((glm::vec3)node.data[0]);
			ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)node.x, (float)node.y));
		}
		else if (node.type == scene->mat4Types.translate) {
			_workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>(new WorkspaceMatrixTranslation((ImTextureID)0, "load translation")));
			ValueSetResult result = dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue((glm::vec3)node.data[0]);
			ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)node.x, (float)node.y));
		}
	}
	for (int i = 0; i < scene->normVec4Nodes.size(); i++) {
		NodeNormVec4 node=scene->normVec4Nodes[i];
		_workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, "load NormalizeVector"));
		ValueSetResult result = dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue(node.data);
		ne::SetNodePosition(_workspace->back()->Id, ImVec2((float)node.x, (float)node.y));
	}
	for (int i = 0; i < scene->nodePlugs.size(); i++) {
		int indexA=scene->nodePlugs[i].indexA+startlen;
		int indexB=scene->nodePlugs[i].indexB+startlen;
		Ptr<Core::NodeBase> pca= dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->at(indexA).get())->Nodebase;
		Ptr<Core::NodeBase> pcb= dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->at(indexB).get())->Nodebase;


		ENodePlugResult p = Core::GraphManager::plug(pca,pcb, scene->nodePlugs[i].indexPinA, scene->nodePlugs[i].indexPinB);
		dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->at(indexB).get())->WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(getLinkID()));

	}

	clearWorkspaceLayout();
	return true;
}
void functionWhichProcessConsoleCommands(std::string rawCommand){

}
int PicocRunInteractive(){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	PlatformLibraryInitI3T(&pc);
	if(PicocPlatformSetExitPoint(&pc)){PicocCleanup(&pc); return pc.PicocExitValue;}
	PicocIncludeAllSystemHeaders(&pc);
	PicocParseInteractive(&pc);
	PicocCleanup(&pc);
	return pc.PicocExitValue;
}
int PicocRunFile(const char* filename){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	PlatformLibraryInitI3T(&pc);
	if (PicocPlatformSetExitPoint(&pc)) { PicocCleanup(&pc); return pc.PicocExitValue; }
	PicocIncludeAllSystemHeaders(&pc);
	PicocPlatformScanFile(&pc, filename);
	PicocCleanup(&pc); printf("ASAS %d\n", pc.PicocExitValue);
	return pc.PicocExitValue;
	// for (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++){PicocPlatformScanFile(&pc,
	// argv[ParamCount]);}//run multiple files PicocCallMain(&pc, argc - ParamCount, &argv[ParamCount]);	//call main
}
int PicocRunSource(const char* source){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	PlatformLibraryInitI3T(&pc);
	if (PicocPlatformSetExitPoint(&pc)) { PicocCleanup(&pc); return pc.PicocExitValue; }
	PicocIncludeAllSystemHeaders(&pc);
	PicocParse(&pc, "somefilename", source, (int)strlen(source), TRUE, TRUE, TRUE, TRUE);
	PicocCleanup(&pc);
	return pc.PicocExitValue;
}
void Scripting::runCommand(std::string cmd) {
	if(PicocPlatformSetExitPoint(&m_picoc)){return;}
	PicocParse(&m_picoc, "Run command", cmd.c_str(), (int)cmd.size(), TRUE, TRUE, TRUE, TRUE);
	/*char s[1024]={0};
	int r=(int)fread(s,1,1024,stdout);
	printf(s);*/
}
Scripting::Scripting() {
	PicocInitialise(&m_picoc, PICOC_STACK_SIZE);
	PlatformLibraryInitI3T(&m_picoc);
	PicocIncludeAllSystemHeaders(&m_picoc);
	m_picocExitPoint = false;
}
Scripting::~Scripting() {
	PicocCleanup(&m_picoc);
}