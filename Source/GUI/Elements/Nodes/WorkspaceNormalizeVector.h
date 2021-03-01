#include "WorkspaceElements.h"

/* Order of inheritance is important! WorkspaceNodeBaseData has to be first because it is necessary first create
 * NodeBase to read data from it */
class WorkspaceNormalizeVector : public WorkspaceNodeBaseData, public WorkspaceNode

{
public:
	WorkspaceNormalizeVector(ImTextureID headerBackground);

	void drawWorkspaceNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void drawWorkspaceNodeData(util::NodeBuilder& builder);

	/*! \fn void fromArrayToVec4(glm::vec4& vec, float arr[4])
	    \brief convert float array of size 4 to glm::vec4
	    \param[out] vec glm::vec4& result object
	    \param[in] arr float* input values -> must be of size 4*sizeof(float)
	*/
	void fromArrayToVec4(glm::vec4& vec, float arr[4])
	{
		vec.x = arr[0];
		vec.y = arr[1];
		vec.z = arr[2];
		vec.w = arr[3];
	};

	/*! \fn void fromVecToArray4(glm::vec4& vec, float arr[4])
	    \brief convert glm::vec4 to float array of size 4
	    \param[in] vec glm::vec4& input object
	    \param[out] arr float* result values -> should be of size 4*sizeof(float)
	*/
	void fromVecToArray4(glm::vec4& vec, float arr[4])
	{
		arr[0] = vec.x;
		arr[1] = vec.y;
		arr[2] = vec.z;
		arr[3] = vec.w;
	};
};
