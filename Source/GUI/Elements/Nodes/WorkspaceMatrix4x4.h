

#include "WorkspaceNode.h"


class WorkspaceMatrix4x4 :
    public WorkspaceNode,
    public WorkspaceNodeFrame,
    public WorkspaceNodeData,
    public WorkspaceNodeInputs,
    public WorkspaceNodeOutputs
{
    public:
        WorkspaceMatrix4x4(int id, std::string label, ImTextureID header_background, std::string state, std::unique_ptr<Core::NodeBase> nodebase)
        :WorkspaceNode(id, state)
        WorkspaceNodeFrame(label, header_background),
        WorkspaceNodeData(nodebase),
        WorkspaceNodeInputs(),
        WorkspaceNodeOutputs() {}


};
