#pragma once
#include "Core/Nodes/Transform.h" /* building transformations nodes*/
#include "WorkspaceElements.h"

#include "WorkspaceElements.h"

class WorkspaceCorePinProperties;

extern std::map<Core::Transform::DataMap, std::string> WorkspaceDatamapName;
extern std::vector<Core::Transform::DataMap> WorkspaceDatamapMenuList;

typedef std::vector<Core::Pin>::const_iterator corePinIter;
typedef std::vector<Ptr<WorkspaceCorePinProperties>>::const_iterator corePinPropIter;
typedef std::vector<Ptr<WorkspaceLinkProperties>>::const_iterator linkPropIter;

typedef std::function<float()> getter_function_pointer;
typedef std::function<ValueSetResult(float)> setter_function_pointer;

struct WorkspaceNodeWithCoreDataArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default WorkspaceNode header";
    std::string nodeLabel = "Node";

    Ptr<Core::NodeBase> nodebase = nullptr; /* \todo JH some default type of Core-Node will be nice (at least some of existing one) */
};

struct floatWindow {
	bool showMyPopup;
	std::string id;
	std::string name;
	float value;
	int columns;
	int rows;
};

class WorkspaceNodeWithCoreData : public WorkspaceNode
{
protected:
    int m_numberOfVisibleDecimal=2; /* \todo JH default number from some setting */
    float m_dataItemsWidth = 100; /* \todo JH default number from some setting - just for safe if someone not call setDataItemsWidth() in construktor of child class... */
public:

	Ptr<Core::NodeBase> const m_nodebase; /*! \brief reference to Core
                                                WorkspaceNodeWithCoreData is owner
                                           */
	floatWindow fw;


	std::vector<Ptr<WorkspaceLinkProperties>> m_workspaceLinksProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceInputsProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceOutputsProperties;



	WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args);
    WorkspaceNodeWithCoreData(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase = nullptr, std::string headerLabel = "With Core Data", std::string nodeLabel = "With Core Data");

    int getNumberOfVisibleDecimal();
    int setNumberOfVisibleDecimal(int value);

    virtual int maxLenghtOfData()=0;
    float setDataItemsWidth();
    float getDataItemsWidth();

    void setDataMap(const Core::Transform::DataMap& mapToSet);
    void drawMenuSetDataMap();

    void drawMenuSetPrecision();

	virtual void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	virtual void drawDataSetValues_builder(util::NodeBuilder& builder, std::vector<std::string>const & labels, std::vector<getter_function_pointer>const & getters, std::vector<setter_function_pointer>const & setters);

    virtual void drawData(util::NodeBuilder& builder);
    virtual void drawDataFull(util::NodeBuilder& builder)=0;
	virtual void drawDataSetValues(util::NodeBuilder& builder)=0;
	virtual void drawDataLabel(util::NodeBuilder& builder);


	virtual void drawInputLinks();
	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	bool drawDragFloatWithMap_Inline(float* const value, const int mapValue, std::string label);

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
