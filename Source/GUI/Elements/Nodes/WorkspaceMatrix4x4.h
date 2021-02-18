

#include "WorkspaceElements.h"


class WorkspaceMatrix4x4 :
    public WorkspaceNode,
    public WorkspaceNodeBaseData
{
    public:
        WorkspaceMatrix4x4();

        void drawWorkspaceNode(util::NodeBuilder& builder, WorkspacePin* newLinkPin);

        void drawWorkspaceNodeData(util::NodeBuilder& builder);
};
