

#include "WorkspaceMatrix4x4.h"

WorkspaceMatrix4x4::drawNode(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
    drawNodeHeader(builder);
    drawInputs(builder, newLinkPin);
    drawData(builder);
    drawOutputs(builder, newLinkPin);

}

