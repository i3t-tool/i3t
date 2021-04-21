#include "Dependencies/picoc/picoc.h"

#include "Scripting.h"
#include "libraryI3T.h"
#include "Commands/ApplicationCommands.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool SaveWorkspace(const char* filename, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace) {
	FILE*f=fopen(filename,"w");
	if(f==NULL){return false;}
	//FILE*f=stdout;//dont fclose
	fprintf(f,"//saving\n");

	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNodeWithCoreData*  nodebasedata= _workspace->at(i).get(); /* \todo JH this is confusing - in WorkspaceNodeWithCoreData are also graphic informations, data are in Ptr<Core::NodeBase> */
		Ptr<Core::NodeBase>			nodebase	= nodebasedata->getNodebase();
		ImVec2						pos			= ne::GetNodePosition(nodebasedata->getId());
		const Core::Transform::DataMap&	data		= nodebase->getDataMapRef();
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
		WorkspaceNodeWithCoreData*  nodebasedata = _workspace->at(i).get();
		Ptr<Core::NodeBase>			nodebase = nodebasedata->getNodebase();
		ImVec2						pos = ne::GetNodePosition(nodebasedata->getId());
		const Core::Transform::DataMap&	data = nodebase->getDataMapRef();
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
				if (parent.get() == (_workspace->at(j).get())->getNodebase().get()) {parentindex = j;}
			}
			if(parentindex>-1&& i > -1 && indexout > -1 && indexin > -1){fprintf(f,"bool p%d_%d=plugnodes(n%d,n%d,%d,%d);\n",i,indexin, parentindex,i,indexout,indexin);}
		}
	}
	fprintf(f,"//saved\n");
	fclose(f);
	return true;
}
bool LoadWorkspace(const char* filename, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace) {
	int p=PicocRunFile(filename);
	WorkspaceLayout*scene=getWorkspaceLayout();
	int startlen=(int)_workspace->size();
	for (int i = 0; i < scene->mat4Nodes.size();i++) {
		NodeMat4 node=scene->mat4Nodes[i];
		if(node.type==scene->mat4Types.free){
			_workspace->push_back(std::make_unique<WorkspaceMatrixFree>((ImTextureID)0, "load free"));
			ValueSetResult result =(_workspace->back().get())->getNodebase().get()->setValue(node.data);
			ne::SetNodePosition(_workspace->back()->getId(), ImVec2((float)node.x, (float)node.y));
		}
		else if (node.type == scene->mat4Types.scale) {
			_workspace->push_back(std::make_unique<WorkspaceMatrixScale>((ImTextureID)0, "load scale"));
			ValueSetResult result = (_workspace->back().get())->getNodebase().get()->setValue((glm::vec3)node.data[0]);
			ne::SetNodePosition(_workspace->back()->getId(), ImVec2((float)node.x, (float)node.y));
		}
		else if (node.type == scene->mat4Types.translate) {
			_workspace->push_back(std::make_unique<WorkspaceMatrixTranslation>((ImTextureID)0, "load translation"));
			ValueSetResult result = (_workspace->back().get())->getNodebase().get()->setValue((glm::vec3)node.data[0]);
			ne::SetNodePosition(_workspace->back()->getId(), ImVec2((float)node.x, (float)node.y));
		}
	}
	for (int i = 0; i < scene->normVec4Nodes.size(); i++) {
		NodeNormVec4 node=scene->normVec4Nodes[i];
		_workspace->push_back(std::make_unique<WorkspaceNormalizeVector>((ImTextureID)0, "load NormalizeVector"));
		ValueSetResult result = (_workspace->back().get())->getNodebase().get()->setValue(node.data);
		ne::SetNodePosition(_workspace->back()->getId(), ImVec2((float)node.x, (float)node.y));
	}
	for (int i = 0; i < scene->nodePlugs.size(); i++) {
		int indexA=scene->nodePlugs[i].indexA+startlen;
		int indexB=scene->nodePlugs[i].indexB+startlen;
		Ptr<Core::NodeBase> pca= (_workspace->at(indexA).get())->getNodebase();
		Ptr<Core::NodeBase> pcb= (_workspace->at(indexB).get())->getNodebase();


		ENodePlugResult p = Core::GraphManager::plug(pca,pcb, scene->nodePlugs[i].indexPinA, scene->nodePlugs[i].indexPinB);
		// (_workspace->at(indexB).get())->WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(getLinkID())); /* JH dont know what this does, but WorkspaceLinkProperties all exist since Node is created - just change values in it  */

	}

	clearWorkspaceLayout();
	return true;
}
void functionWhichProcessConsoleCommands(std::string rawCommand){

}
/* picoc problem */
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
