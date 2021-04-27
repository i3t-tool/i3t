#include "Dependencies/picoc/picoc.h"

#include "Scripting.h"
#include "libraryI3T.h"
#include "Commands/ApplicationCommands.h"

//transformations{
#include "Source/GUI/Elements/Nodes/WorkspaceSequence.h"
//} tranformationa end

//operators {

//	transformations{
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixRotate.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeEulerX.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeEulerY.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeEulerZ.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeFrustum.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeLookAt.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakeOrtho.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMakePerspective.h"
//	} tranformationa end

//	matrix{
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixInversion.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixMulMatrix.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixTranspose.h"
#include "Source/GUI/Elements/Nodes/WorkspaceDeterminant.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixAddMatrix.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixMulFloat.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMatrixMulVector.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorMulMatrix.h"
//	} matrix end

//	vec4{
#include "Source/GUI/Elements/Nodes/WorkspaceVectorFree.h"
#include "Source/GUI/Elements/Nodes/WorkspaceNormalizeVector.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorDotVector.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorAddVector.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorSubVector.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorMulFloat.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVectorPerspectiveDivision.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMixVector.h"
//	} vec4 end

//	vec4{
#include "Source/GUI/Elements/Nodes/WorkspaceVector3Free.h"
#include "Source/GUI/Elements/Nodes/WorkspaceNormalizeVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3CrossVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3DotVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3AddVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3SubVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3MulFloat.h"
#include "Source/GUI/Elements/Nodes/WorkspaceVector3Length.h"
#include "Source/GUI/Elements/Nodes/WorkspaceShowVector3.h"
#include "Source/GUI/Elements/Nodes/WorkspaceMixVector3.h"
//	} vec4 end

//	float{
#include "Source/GUI/Elements/Nodes/WorkspaceFloatFree.h"
//	} float end
//}operators end
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool saveWorkspace(const char* filename, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace) {
	FILE*f=fopen(filename,"w");
	if(f==NULL){return false;}
	fprintf(f,"//saving\n");

	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNodeWithCoreData*  nodebasedata= _workspace->at(i).get(); /* \todo JH this is confusing - in WorkspaceNodeWithCoreData are also graphic informations, data are in Ptr<Core::NodeBase> */
		Ptr<Core::NodeBase>			nodebase	= nodebasedata->getNodebase(); //printf("a\n");
		ImVec2						pos			= ne::GetNodePosition(nodebasedata->getId()); //printf("b\n");
		const Core::Transform::DataMap&	data	= nodebase->getDataMapRef(); //printf("c\n");
		const Operation*			operation	= nodebase->getOperation(); //printf("d\n");
		const char*					keyword		= operation->keyWord.c_str(); //printf("e\n");
		std::string					label		= nodebasedata->getHeaderLabel(); //printf("f\n");

		//mat4 transform
		if (strcmp(keyword, "Scale") == 0) {
			glm::mat4 mat4 = nodebase->getData().getMat4();
			glm::vec3 s = glm::vec3(mat4[0][0], mat4[1][1], mat4[2][2]);
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(scale,d%d,%d,%d,\"%s\");\n", i, i, (int)pos[0], (int)pos[1],label.c_str());
		}
		else if (strcmp(keyword, "Translation") == 0) {
			glm::mat4 mat4 = nodebase->getData().getMat4();
			glm::vec3 s = (glm::vec3)mat4[3];
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(translate,d%d,%d,%d,\"%s\");\n", i, i, (int)pos[0], (int)pos[1],label.c_str());
		}
		//float
		else if (strcmp(keyword, "FloatToFloat") == 0) {
			float v = nodebase->getData().getFloat();
			fprintf(f,"int d%d=datascalar(%0.3ff);\n", i, v);
			fprintf(f,"int n%d=scalar(d%d,%d,%d,\"%s\");\n", i,i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec4
		else if (strcmp(keyword, "Vector4ToVector4") == 0) {
			glm::vec4 vec4 = nodebase->getData().getVec4();
			fprintf(f, "int d%d=datavec4(%0.3ff,%0.3ff,%0.3ff,%0.3ff);\n", i, vec4[0], vec4[1], vec4[2], vec4[3]);
			fprintf(f, "int n%d=vec4(d%d,%d,%d,\"%s\");\n", i, i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec4oper
		else if (strcmp(keyword, "VectorDotVector") == 0) {
			fprintf(f, "int n%d=vec4oper(dot,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorAddVector") == 0) {
			fprintf(f, "int n%d=vec4oper(add,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorSubVector") == 0) {
			fprintf(f, "int n%d=vec4oper(sub,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorMulFloat") == 0) {
			fprintf(f, "int n%d=vec4oper(vecmulfloat,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorPerspectiveDivision") == 0) {
			fprintf(f, "int n%d=vec4oper(perspdiv,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeVector") == 0) {
			fprintf(f, "int n%d=vec4oper(norm,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MixVector") == 0) {
			fprintf(f, "int n%d=vec4oper(mix,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec3
		else if (strcmp(keyword, "Vector3ToVector3") == 0) {
			glm::vec3 vec3 = nodebase->getData().getVec3();
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i, vec3[0], vec3[1], vec3[2]);
			fprintf(f, "int n%d=vec3(d%d,%d,%d,\"%s\");\n", i, i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec3oper
		else if (strcmp(keyword, "Vector3CrossVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(cross,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3DotVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(dot,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3AddVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(add,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3SubVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(sub,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3MulFloat") == 0) {
			fprintf(f, "int n%d=vec3oper(vecmulfloat,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3Length") == 0) {
			fprintf(f, "int n%d=vec3oper(length,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "ShowVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(show,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(norm,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MixVector3") == 0) {
			fprintf(f, "int n%d=vec4oper(mix,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//mat4oper
		else if (strcmp(keyword, "Inversion") == 0) {
			fprintf(f,"int n%d=mat4oper(inverse,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Transpose") == 0) {
			fprintf(f,"int n%d=mat4oper(transpose,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Determinant") == 0) {
			fprintf(f,"int n%d=mat4oper(determinant,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(mul,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorMulMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(vecmulmat,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulVector") == 0) {
			fprintf(f, "int n%d=mat4oper(matmulvec,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulFloat") == 0) {
			fprintf(f, "int n%d=mat4oper(floatmulmat,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixAddMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(add,%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixToMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(matrix,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}

		else if (strcmp(keyword, "MakePerspective") == 0) {
			fprintf(f, "int n%d=mat4oper(perspective,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeOrtho") == 0) {
			fprintf(f, "int n%d=mat4oper(ortho,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeFrustum") == 0) {
			fprintf(f, "int n%d=mat4oper(frustrum,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeAxisAngle") == 0) {
			fprintf(f, "int n%d=mat4oper(axisangle,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerX") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatex,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerY") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatey,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerZ") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatez,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeScale") == 0) {
			fprintf(f, "int n%d=mat4oper(scale,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeLookAt") == 0) {
			fprintf(f, "int n%d=mat4oper(lookat,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeTranslation") == 0) {
			fprintf(f, "int n%d=mat4oper(translate,%d,%d,\"%s\");\n", i,(int)pos[0], (int)pos[1], label.c_str());
		}
		//sequence
		else if (strcmp(keyword, "Sequence") == 0) {
			fprintf(f, "int n%d=sequence(%d,%d,\"%s\");\n", i, (int)pos[0], (int)pos[1], label.c_str());
		}
		//
		else {
			//printf("g\n");
			glm::mat4 m = glm::mat4(1.0f);//nodebase->getData().getMat4(); printf("h\n");
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff);\n",
				i, m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[0][2], m[1][3],
				m[2][0], m[2][1], m[0][2], m[2][3], m[3][0], m[3][1], m[0][2], m[3][3]);
			fprintf(f, "//int n%d=%s(d%d,%d,%d);//unknown type\n", i, keyword,i, (int)pos[0], (int)pos[1]);
		}
	}

	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNodeWithCoreData* nodebasedata = _workspace->at(i).get();
		int precision							= nodebasedata->getNumberOfVisibleDecimal();
		int lod									= (int)nodebasedata->getLevelOfDetail();
		const char* names[4]={"full","setvalues","label",nullptr};
		if(precision!=2||lod!=(int)WorkspaceLevelOfDetail::Full){
			fprintf(f,"confnode(n%d,%d,%s);\n",i, nodebasedata->getNumberOfVisibleDecimal(),names[(int)nodebasedata->getLevelOfDetail()]);
		}
	}

	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNodeWithCoreData*  nodebasedata = _workspace->at(i).get(); //printf("i\n");
		Ptr<Core::NodeBase>			nodebase = nodebasedata->getNodebase();
		ImVec2						pos = ne::GetNodePosition(nodebasedata->getId());
		const Core::Transform::DataMap&	data = nodebase->getDataMapRef();
		const Operation*			operation = nodebase->getOperation();
		const char* keyword =		operation->keyWord.c_str(); //printf("k\n");

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
	fprintf(f,"//saved");
	fclose(f);
	return true;
}
bool loadWorkspace(const char* filename) {
	ScriptingData*data=getScriptingData();
	int datalen=(int)data->nodeData.size();
	int p=picocRunFile(filename);
	while(data->nodeData.size()>datalen){data->nodeData.pop_back(); }
	return true;
}
int picocRunInteractive(){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	if(PicocPlatformSetExitPoint(&pc)){PicocCleanup(&pc); return pc.PicocExitValue;}
	platformLibraryInitI3T(&pc);
	PicocIncludeAllSystemHeaders(&pc);
	PicocParseInteractive(&pc);
	PicocCleanup(&pc);
	return pc.PicocExitValue;
}
int picocRunFile(const char* filename){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	if (PicocPlatformSetExitPoint(&pc)) {PicocCleanup(&pc); return pc.PicocExitValue;}
	platformLibraryInitI3T(&pc);
	PicocIncludeAllSystemHeaders(&pc);
	PicocPlatformScanFile(&pc, filename);
	PicocCleanup(&pc);
	return pc.PicocExitValue;
	// for (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++){PicocPlatformScanFile(&pc,
	// argv[ParamCount]);}//run multiple files PicocCallMain(&pc, argc - ParamCount, &argv[ParamCount]);	//call main
}
int picocRunSource(const char* source){
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);
	if (PicocPlatformSetExitPoint(&pc)){PicocCleanup(&pc); return pc.PicocExitValue;}
	platformLibraryInitI3T(&pc);
	PicocIncludeAllSystemHeaders(&pc);
	PicocParse(&pc, "somefilename", source, (int)strlen(source), TRUE, TRUE, TRUE, TRUE);
	PicocCleanup(&pc);
	return pc.PicocExitValue;
}
void Scripting::runCommand(std::string cmd) {
	if(!m_init){return;}
	if(PicocPlatformSetExitPoint(&m_picoc)){return;}
	PicocParse(&m_picoc, "Run command", cmd.c_str(), (int)cmd.size(), TRUE, TRUE, TRUE, TRUE);
}
Scripting::Scripting() {
	PicocInitialise(&m_picoc, PICOC_STACK_SIZE);
	if(PicocPlatformSetExitPoint(&m_picoc)){PicocCleanup(&m_picoc); return;}
	platformLibraryInitI3T(&m_picoc);
	PicocIncludeAllSystemHeaders(&m_picoc);
	m_init=true;
}
Scripting::~Scripting() {
	PicocCleanup(&m_picoc);
}
