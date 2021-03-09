#pragma once
#include "WorkspaceElements.h"
#include "Core/Nodes/Transform.h" /* building transformations nodes*/

typedef std::vector<Core::Pin>::const_iterator pinIter;
typedef std::vector<std::unique_ptr<WorkspacePinProperties>>::const_iterator pinPropIter;
typedef std::vector<std::unique_ptr<WorkspaceLinkProperties>>::const_iterator linkPropIter;

class WorkspaceNodeWithCoreData : public WorkspaceNode
{
public:
	/*see:
	 * https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function*/
	Ptr<Core::NodeBase> const m_nodebase; /*! \brief reference to core
	                                                     WorkspaceNodeWithCoreData is owner of unique pointer
	                                                */

	std::vector<std::unique_ptr<WorkspaceLinkProperties>> m_workspaceLinksProperties;
	std::vector<std::unique_ptr<WorkspacePinProperties>> m_workspaceInputsProperties;
	std::vector<std::unique_ptr<WorkspacePinProperties>> m_workspaceOutputsProperties;

	WorkspaceNodeWithCoreData(Ptr<Core::NodeBase> nodebase, ImTextureID headerBackground, std::string headerLabel = "default CoreData-node header", WorkspaceViewScale viewScale = WorkspaceViewScale::Full);

    virtual void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	virtual void drawInputLinks();
	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	bool drawDragFloatWithMap_Inline(float * const value, const int mapValue, std::string label);
};
