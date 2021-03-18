#pragma once
#include "WorkspaceVector4.h"

struct WorkspaceNormalizeVectorArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default NormalizeVector header";
    std::string nodeLabel = "default NormalizeVector label";
    Ptr<Core::NodeBase> nodebase = Builder::createNode<ENodeType::NormalizeVector>();
};


class WorkspaceNormalizeVector : public WorkspaceVector4
{
public:
	WorkspaceNormalizeVector(ImTextureID headerBackground, WorkspaceNormalizeVectorArgs const& args);

	void drawData(util::NodeBuilder& builder);



//	/*! \fn void fromArrayToVec4(glm::vec4& vec, float arr[4])
//	    \brief convert float array of size 4 to glm::vec4
//	    \param[out] vec glm::vec4& result object
//	    \param[in] arr float* input values -> must be of size 4*sizeof(float)
//	*/
//	void fromArrayToVec4(glm::vec4& vec, float arr[4])
//	{
//		vec.x = arr[0];
//		vec.y = arr[1];
//		vec.z = arr[2];
//		vec.w = arr[3];
//	};
//
//	/*! \fn void fromVecToArray4(glm::vec4& vec, float arr[4])
//	    \brief convert glm::vec4 to float array of size 4
//	    \param[in] vec glm::vec4& input object
//	    \param[out] arr float* result values -> should be of size 4*sizeof(float)
//	*/
//	void fromVecToArray4(glm::vec4& vec, float arr[4])
//	{
//		arr[0] = vec.x;
//		arr[1] = vec.y;
//		arr[2] = vec.z;
//		arr[3] = vec.w;
//	};
};
