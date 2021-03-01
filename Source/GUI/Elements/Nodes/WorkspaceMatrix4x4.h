#include "WorkspaceElements.h"

/* Order of inheritance is important! WorkspaceNodeBaseData has to be first because it is necessary first create
 * NodeBase to read data from it */
class WorkspaceMatrix4x4 : public WorkspaceNodeBaseData, public WorkspaceNode

{
public:
	WorkspaceMatrix4x4(ImTextureID headerBackground);

	void drawWorkspaceNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void drawWorkspaceNodeData(util::NodeBuilder& builder);
};
