#pragma once
#include "Core/Nodes/Transform.h" /* building transformations nodes*/

#include "WorkspaceElements.h"

class WorkspaceCorePinProperties;


typedef std::vector<Core::Pin>::const_iterator corePinIter;
typedef std::vector<Ptr<WorkspaceCorePinProperties>>::const_iterator corePinPropIter;
typedef std::vector<Ptr<WorkspaceLinkProperties>>::const_iterator linkPropIter;

struct WorkspaceNodeWithCoreDataArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default WorkspaceNode header";
    std::string nodeLabel = "Node";

    Ptr<Core::NodeBase> nodebase = nullptr; /* \todo JH some default type of Core-Node will be nice (at least some of existing one) */
};

class WorkspaceNodeWithCoreData : public WorkspaceNode
{
public:
	/*see:
	 * https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function*/
	Ptr<Core::NodeBase> const m_nodebase; /*! \brief reference to core
	                                                     WorkspaceNodeWithCoreData is owner of unique pointer
	                                                */

	std::vector<Ptr<WorkspaceLinkProperties>> m_workspaceLinksProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceInputsProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceOutputsProperties;

	WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args);
    WorkspaceNodeWithCoreData(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase = nullptr);

    virtual void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	virtual void drawInputLinks();
	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	bool drawDragFloatWithMap_Inline(float * const value, const int mapValue, std::string label);
};

/*! \class WorkspaceCorePinProperties
    \brief Information of Pin for graphic
 */
/* \todo JH later maybe create more general parent class*/
class WorkspaceCorePinProperties
{
public:
	ne::PinId const m_id; /*! \brief unique (among Pins) identificator */
    Core::Pin const &m_pin;
    WorkspaceNodeWithCoreData &m_node;

	std::string m_name;    /*! \brief Name of Pin */
	int m_iconSize; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */
    float m_alpha;

	WorkspaceCorePinProperties(ne::PinId const id, Core::Pin const &pin, WorkspaceNodeWithCoreData &node, char const * name);

    PinKind getKind();
    EValueType getType();
	bool getIsConnected();
};
