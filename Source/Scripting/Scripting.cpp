#include "Dependencies/picoc/picoc.h"

#include "Scripting.h"
#include "libraryI3T.h"
#include "Source/World/Transforms.h"
#include "Commands/ApplicationCommands.h"
#include "Source/GUI/Elements/Windows/WorkspaceWindow.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
bool saveWorkspace(FILE* f, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace, int at);

bool saveWorkspace(const char* filename, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace) {
	FILE*f=fopen(filename,"w");
	if(f==NULL){return false;}
	fprintf(f,"//saving\n");
	bool status=saveWorkspace(f,_workspace,0);
	fprintf(f,"//saved\n");
	fclose(f);
	return status;
}
bool saveWorkspace(FILE* f, std::vector<Ptr<WorkspaceNodeWithCoreData>> * _workspace,int at) {
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
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i+at, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(scale,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1],label.c_str());
		}
		else if (strcmp(keyword, "Translation") == 0) {
			glm::mat4 mat4 = nodebase->getData().getMat4();
			glm::vec3 s = (glm::vec3)mat4[3];
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i+at, s[0], s[1], s[2]);
			fprintf(f, "int n%d=mat4(translate,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1],label.c_str());
		}
		else if (strcmp(keyword, "Free") == 0) {
			glm::mat4 m = nodebase->getData().getMat4();
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff, %0.3ff,%0.3ff,%0.3ff,%0.3ff);\n", i+at,
				m[0][0], m[0][1], m[0][2],m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[1][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3]);
			fprintf(f, "int n%d=mat4(free,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "LookAt") == 0) {
			Core::LookAt* lookat = (Core::LookAt*)(nodebase.get());
			glm::mat3 m = glm::mat3(0.0f);
			m[0]=lookat->getCenter();m[1]=lookat->getEye();m[2]=lookat->getUp();
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,%0.3ff,0.0f, %0.3ff,%0.3ff,%0.3ff,0.0f, %0.3ff,%0.3ff,%0.3ff,0.0f, 0.0f,0.0f,0.0f,0.0f);\n", i+at,
				m[0][0],m[0][1],m[0][2], m[1][0],m[1][1],m[1][2], m[2][0],m[2][1],m[2][2]);
			fprintf(f, "int n%d=mat4(lookat,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "EulerX") == 0) {
			glm::mat4 m = nodebase->getData().getMat4();
			glm::vec3 v = (glm::vec3)(m * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			float a = angle2(v[1], v[2]);
			fprintf(f, "int d%d=datascalar(%0.3ff);\n", i+at, a);
			fprintf(f, "int n%d=mat4(rotatex,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "EulerY") == 0) {
			glm::mat4 m = nodebase->getData().getMat4();
			glm::vec3 v = (glm::vec3)(m * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			float a = angle2(v[2], v[0]);
			fprintf(f, "int d%d=datascalar(%0.3ff);\n", i+at, a);
			fprintf(f, "int n%d=mat4(rotatey,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "EulerZ") == 0) {
			glm::mat4 m = nodebase->getData().getMat4();
			glm::vec3 v = (glm::vec3)(m * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			float a = angle2(v[0], v[1]);
			fprintf(f, "int d%d=datascalar(%0.3ff);\n", i+at, a);
			fprintf(f, "int n%d=mat4(rotatez,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Ortho") == 0) {
			glm::mat4 m = getProjParams(glm::inverse(nodebase->getData().getMat4()), false);
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f);\n", i + at,
				m[0][0], m[0][1], m[1][0], m[1][1], m[1][0], m[2][1], m[3][0], m[3][1]);
			fprintf(f, "int n%d=mat4(ortho,d%d,%d,%d,\"%s\");\n", i + at, i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Perspective") == 0) {
			glm::mat4 m = getProjParams(glm::inverse(nodebase->getData().getMat4()), false);
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f);\n", i + at,
				m[0][0], m[0][1], m[1][0], m[1][1], m[1][0], m[2][1], m[3][0], m[3][1]);
			fprintf(f, "int n%d=mat4(perspective,d%d,%d,%d,\"%s\");\n", i + at, i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Frustum") == 0) {
			glm::mat4 m = getProjParams(glm::inverse(nodebase->getData().getMat4()),false);
			fprintf(f, "int d%d=datamat4(%0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f, %0.3ff,%0.3ff,0.0f,0.0f);\n", i + at,
				m[0][0],m[0][1], m[1][0],m[1][1],  m[1][0],m[2][1], m[3][0],m[3][1]);
			fprintf(f, "int n%d=mat4(frustrum,d%d,%d,%d,\"%s\");\n", i + at, i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "AxisAngle") == 0) {
			glm::mat4 m = nodebase->getData().getMat4();
			//Core::AxisAngleRot*ar=(Core::AxisAngleRot*)nodebase.get();
			//float rr=ar->getRot();
			glm::vec3 v = (glm::vec3)(m * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			float a = angle2(v[1], v[2]);
			fprintf(f, "int d%d=datascalar(%0.3ff);\n", i+at, a);
			fprintf(f, "int n%d=mat4(axisangle,d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//float
		else if (strcmp(keyword, "FloatToFloat") == 0) {
			float v = nodebase->getData().getFloat();
			fprintf(f,"int d%d=datascalar(%0.3ff);\n", i+at, v);
			fprintf(f,"int n%d=scalar(d%d,%d,%d,\"%s\");\n", i+at,i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//float oper
		else if (strcmp(keyword, "ClampFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(clamp,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatMulFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(mul,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatDivFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(div,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatAddFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(add,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatPowFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(clamp,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MixFloat") == 0) {
			fprintf(f, "int n%d=scalaroper(mix,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatSinCos") == 0) {
			fprintf(f, "int n%d=scalaroper(sincos,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatAsinAcos") == 0) {
			fprintf(f, "int n%d=scalaroper(asinacos,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Signum") == 0) {
			fprintf(f, "int n%d=scalaroper(signum,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//convertor
		else if (strcmp(keyword, "MatrixToVectors") == 0) {
			fprintf(f, "int n%d=convertor(mat_vec4,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vectors3ToMatrix") == 0) {
			fprintf(f, "int n%d=convertor(vecs3_mat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorsToMatrix") == 0) {
			fprintf(f, "int n%d=convertor(vecs4_mat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixToFloats") == 0) {
			fprintf(f, "int n%d=convertor(mat_scalars,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatsToMatrix") == 0) {
			fprintf(f, "int n%d=convertor(scalars_mat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixToTR") == 0) {
			fprintf(f, "int n%d=convertor(mat_tr,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "TRToMatrix") == 0) {
			fprintf(f, "int n%d=convertor(tr_mat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixToQuat") == 0) {
			fprintf(f, "int n%d=convertor(mat_quat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatToMatrix") == 0) {
			fprintf(f, "int n%d=convertor(quat_mat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorToFloats") == 0) {
			fprintf(f, "int n%d=convertor(vec4_scalars,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatsToVector") == 0) {
			fprintf(f, "int n%d=convertor(scalars_vec4,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3ToFloats") == 0) {
			fprintf(f, "int n%d=convertor(vec3_scalars,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatsToVector3") == 0) {
			fprintf(f, "int n%d=convertor(scalars_vec3,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorToVector3") == 0) {
			fprintf(f, "int n%d=convertor(vec4_vec3,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3ToVector") == 0) {
			fprintf(f, "int n%d=convertor(vec3_vec4,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatToFloats") == 0) {
			fprintf(f, "int n%d=convertor(quat_scalars,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatsToQuat") == 0) {
			fprintf(f, "int n%d=convertor(scalars_quat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeQuat") == 0) {
			fprintf(f, "int n%d=convertor(norm_quat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec4
		else if (strcmp(keyword, "Vector4ToVector4") == 0) {
			glm::vec4 vec = nodebase->getData().getVec4();
			fprintf(f, "int d%d=datavec4(%0.3ff,%0.3ff,%0.3ff,%0.3ff);\n", i+at, vec[0], vec[1], vec[2],vec[3]);
			fprintf(f, "int n%d=vec3(d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec4oper
		else if (strcmp(keyword, "VectorDotVector") == 0) {
			fprintf(f, "int n%d=vec4oper(dot,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorAddVector") == 0) {
			fprintf(f, "int n%d=vec4oper(add,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorSubVector") == 0) {
			fprintf(f, "int n%d=vec4oper(sub,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorMulFloat") == 0) {
			fprintf(f, "int n%d=vec4oper(vecmulfloat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorPerspectiveDivision") == 0) {
			fprintf(f, "int n%d=vec4oper(perspdiv,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeVector") == 0) {
			fprintf(f, "int n%d=vec4oper(norm,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MixVector") == 0) {
			fprintf(f, "int n%d=vec4oper(mix,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}		
		//quat
		else if (strcmp(keyword, "QuatToQuat") == 0) {
			glm::vec4 vec = nodebase->getData().getVec4();
			fprintf(f, "int d%d=datavec4(%0.3ff,%0.3ff,%0.3ff,%0.3ff);\n", i + at, vec[0], vec[1], vec[2],vec[3]);
			fprintf(f, "int n%d=quat(d%d,%d,%d,\"%s\");\n", i + at, i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//quat oper
		else if (strcmp(keyword, "ConjQuat") == 0) {
			fprintf(f, "int n%d=quatoper(conjugate,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatVecToQuat") == 0) {
			fprintf(f, "int n%d=quatoper(scalarvec3_quat,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "AngleAxisToQuat") == 0) {
			fprintf(f, "int n%d=quatoper(angleaxis_quat,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VecVecToQuat") == 0) {
			fprintf(f, "int n%d=quatoper(vec3vec3_quat,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatToFloatVec") == 0) {
			fprintf(f, "int n%d=quatoper(quat_scalarvec3,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatToAngleAxis") == 0) {
			fprintf(f, "int n%d=quatoper(quat_angleaxis,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatToEuler") == 0) {
			fprintf(f, "int n%d=quatoper(quat_euler,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "EulerToQuat") == 0) {
			fprintf(f, "int n%d=quatoper(euler_quat,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatInverse") == 0) {
			fprintf(f, "int n%d=quatoper(inverse,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatSlerp") == 0) {
			fprintf(f, "int n%d=quatoper(slerp,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatLongWaySlerp") == 0) {
			fprintf(f, "int n%d=quatoper(longslerp,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatLerp") == 0) {
			fprintf(f, "int n%d=quatoper(lerp,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "FloatMulQuat") == 0) {
			fprintf(f, "int n%d=quatoper(scalarmulquat,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatMulQuat") == 0) {
			fprintf(f, "int n%d=quatoper(mul,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatVecConjQuat") == 0) {
			fprintf(f, "int n%d=quatoper(qvq,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "QuatLength") == 0) {
			fprintf(f, "int n%d=quatoper(length,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeQuat") == 0) {
			fprintf(f, "int n%d=quatoper(norm,%d,%d,\"%s\");\n", i + at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec3
		else if (strcmp(keyword, "Vector3ToVector3") == 0) {
			glm::vec3 vec3 = nodebase->getData().getVec3();
			fprintf(f, "int d%d=datavec3(%0.3ff,%0.3ff,%0.3ff);\n", i+at, vec3[0], vec3[1], vec3[2]);
			fprintf(f, "int n%d=vec3(d%d,%d,%d,\"%s\");\n", i+at, i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//vec3oper
		else if (strcmp(keyword, "Vector3CrossVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(cross,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3DotVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(dot,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3AddVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(add,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3SubVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(sub,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3MulFloat") == 0) {
			fprintf(f, "int n%d=vec3oper(vecmulfloat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Vector3Length") == 0) {
			fprintf(f, "int n%d=vec3oper(length,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "ShowVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(show,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "NormalizeVector3") == 0) {
			fprintf(f, "int n%d=vec3oper(norm,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MixVector3") == 0) {
			fprintf(f, "int n%d=vec4oper(mix,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		//mat4oper
		else if (strcmp(keyword, "Inversion") == 0) {
			fprintf(f,"int n%d=mat4oper(inverse,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Transpose") == 0) {
			fprintf(f,"int n%d=mat4oper(transpose,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "Determinant") == 0) {
			fprintf(f,"int n%d=mat4oper(determinant,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(mul,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "VectorMulMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(vecmulmat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulVector") == 0) {
			fprintf(f, "int n%d=mat4oper(matmulvec,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixMulFloat") == 0) {
			fprintf(f, "int n%d=mat4oper(floatmulmat,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixAddMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(add,%d,%d,\"%s\");\n", i+at, (int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MatrixToMatrix") == 0) {
			fprintf(f, "int n%d=mat4oper(free,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}

		else if (strcmp(keyword, "MakePerspective") == 0) {
			fprintf(f, "int n%d=mat4oper(perspective,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeOrtho") == 0) {
			fprintf(f, "int n%d=mat4oper(ortho,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeFrustum") == 0) {
			fprintf(f, "int n%d=mat4oper(frustrum,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeAxisAngle") == 0) {
			fprintf(f, "int n%d=mat4oper(axisangle,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerX") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatex,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerY") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatey,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeEulerZ") == 0) {
			fprintf(f, "int n%d=mat4oper(rotatez,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeScale") == 0) {
			fprintf(f, "int n%d=mat4oper(scale,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeLookAt") == 0) {
			fprintf(f, "int n%d=mat4oper(lookat,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		else if (strcmp(keyword, "MakeTranslation") == 0) {
			fprintf(f, "int n%d=mat4oper(translate,%d,%d,\"%s\");\n", i+at,(int)pos[0], (int)pos[1], label.c_str());
		}
		//sequence
		else if (strcmp(keyword, "Sequence") == 0) {
			
			WorkspaceSequence*seq=(WorkspaceSequence*)nodebasedata;
			std::vector<Ptr<WorkspaceNodeWithCoreData>> ctx=seq->getInnerWorkspaceNodes();
			saveWorkspace(f,&ctx,at+i);
			//at+=ctx.size();
			int seqpos= i + at + (int)ctx.size();
			fprintf(f, "int n%d=sequence(%d,%d,\"%s\");\n", seqpos, (int)pos[0], (int)pos[1], label.c_str());
			for(int j=0;j<ctx.size();j++){
				fprintf(f, "bool sa%d_%d=seqadd(n%d,n%d);\n", seqpos,i+at, seqpos,i+at);
				at++;
			}
			
		}
		//
		else {
			fprintf(f, "//int n%d=%s(d%d,%d,%d);//unknown type\n", i+at, keyword,i+at, (int)pos[0], (int)pos[1]);
		}
	}

	for (int i = 0; i < _workspace->size(); i++) {
		WorkspaceNodeWithCoreData* nodebasedata = _workspace->at(i).get();
		int precision = nodebasedata->getNumberOfVisibleDecimal();
		int lod = (int)nodebasedata->getLevelOfDetail();
		const char* names[4] = { "full","setvalues","label",nullptr };
		if (precision != 2 || lod != (int)WorkspaceLevelOfDetail::Full) {
			fprintf(f, "confnode(n%d,%d,%s);\n", i+at, nodebasedata->getNumberOfVisibleDecimal(), names[(int)nodebasedata->getLevelOfDetail()]);
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
			if(parentindex>-1&& i > -1 && indexout > -1 && indexin > -1){fprintf(f,"bool p%d_%d=plugnodes(n%d,n%d,%d,%d);\n",i+at,indexin, parentindex,i+at,indexout,indexin);}
		}
	}
	return true;
}
bool loadWorkspace(const char* filename) {
	ScriptingData*data=getScriptingData();//printf("a\n");
	int datalen=(int)data->nodeData.size(); //printf("b\n");
	int p=picocRunFile(filename); //printf("c\n");
	while(data->nodeData.size()>datalen){data->nodeData.pop_back(); }//printf("d\n");
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
	if(strcmp(cmd.c_str(),"help")==0){
		std::cout<<
			"int datamat4(float f1, float f2, ..., float f16)\n"
			"int datavec4(float f1, float f2, float f3, float f4)\n"
			"int datavec3(float f1, float f2, float f3)\n"
			"int datascalar(float f)\n"
			"\n"
			"int mat4oper(int type, int x, int y, char* header)\n"
			"int mat4operc(int type, char* header)\n"
			"int mat4(int type, int data, int x, int y, char* header)\n"
			"int mat4c(int type, int data,  char* header)\n"
			"int vec4oper(int type, int x, int y, char* header)\n"
			"int vec4operc(int type, char* header)\n"
			"int vec4(int data, int x, int y, char* header)\n"
			"int vec4c(int data, char* header)\n"
			"int vec3oper(int type, int x, int y, char* header)\n"
			"int vec3operc(int type, char* header)\n"
			"int vec3(int data, int x, int y, char* header)\n"
			"int vec3c(int data, char* header)\n"
			"int scalar(int data, int x, int y, char* header)\n"
			"int scalarc(int data, char* header)\n"
			"int scalar3oper(int type, int x, int y, char* header)\n"
			"int scalar3operc(int type, char* header)\n"
			"\n"
			"Page 1/2. Enter \"help2\" to continue.\n";
	}
	else if(strcmp(cmd.c_str(),"help2")==0){
		std::cout <<
			"int quatoper(int type, int x, int y, char* header)\n"
			"int quatoperc(int type, char* header)\n"
			"int quat(int data, int x, int y, char* header)\n"
			"int quatc(int data, char* header)\n"
			"\n"
			"int convertor(int type, int x, int y, char* header)\n"
			"int convertorc(int type, char* header)\n"
			"int sequence(int x, int y, char* header)\n"
			"int sequencec(char* header)\n"
			"\n"
			"bool seqadd(int sequence, int node);\n"
			"bool plugnodes(int lnode, int rnode, int output, int input)\n"
			"bool unpluginput(int node, int input)\n"
			"\n"
			"int getnode(char* header)\n"
			"bool delnode(int node)\n"
			"bool confnode(int node, int precision, int lod)\n"
			"\n"
			"bool load(char* filename)\n"
			"bool append(char* filename)\n"
			"bool save(char* filename)\n"
			"bool savesel(char* filename)\n"
			"bool run(char* filename)\n"
			"\n"
			"Page 2/2.";
	}
	else{
		PicocParse(&m_picoc, "Run command", cmd.c_str(), (int)cmd.size(), TRUE, TRUE, TRUE, TRUE);
	}
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
