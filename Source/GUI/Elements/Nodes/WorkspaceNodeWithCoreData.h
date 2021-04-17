#pragma once
#include "Core/Nodes/Transform.h" /* building transformations nodes*/
#include "WorkspaceElements.h"

#include "WorkspaceElements.h"

class WorkspaceCorePinProperties;

extern std::map<Core::Transform::DataMap const *, std::string> WorkspaceDatamapName;
extern std::vector<Core::Transform::DataMap const *> WorkspaceDatamapMenuList;

typedef std::vector<Core::Pin>::const_iterator corePinIter;
typedef std::vector<Ptr<WorkspaceCorePinProperties>>::const_iterator corePinPropIter;
typedef std::vector<Ptr<WorkspaceLinkProperties>>::const_iterator linkPropIter;

typedef std::function<float()> getter_function_pointer;
typedef std::function<ValueSetResult(float)> setter_function_pointer;

struct floatWindow {
	bool showMyPopup;
	std::string id;
	std::string name;
	float value;
	int columns;
	int rows;
};

struct WorkspaceNodeWithCoreDataArgs
{
    WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
    std::string headerLabel = "default WorkspaceNode header";
    std::string nodeLabel = "Node";

    Ptr<Core::NodeBase> nodebase = nullptr; /* \todo JH some default type of Core-Node will be nice (at least some of existing one) */
};

class WorkspaceNodeWithCoreData : public WorkspaceNode
{
protected:
    int m_numberOfVisibleDecimal = 2; /* \todo JH default number from some setting */
    float m_dataItemsWidth = 100; /* \todo JH default number from some setting - just for safe if someone not call setDataItemsWidth() in construktor of child class... */

	WorkspaceLevelOfDetail m_levelOfDetail;

    std::vector<Ptr<WorkspaceLinkProperties>> m_workspaceLinksProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceInputsProperties;
	std::vector<Ptr<WorkspaceCorePinProperties>> m_workspaceOutputsProperties;

	Ptr<Core::NodeBase> const m_nodebase; /*! \brief reference to Core
                                                WorkspaceNodeWithCoreData is owner
                                           */

public:
	floatWindow fw; /* \todo create it protected */

	WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args);
    WorkspaceNodeWithCoreData(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase = nullptr, std::string headerLabel = "With Core Data", std::string nodeLabel = "With Core Data");

    Ptr<Core::NodeBase> const getNodebase() const;

    virtual std::vector<Ptr<WorkspaceLinkProperties>> const &getLinksProperties() const;
    virtual std::vector<Ptr<WorkspaceCorePinProperties>> const &getInputsProperties() const;
    virtual std::vector<Ptr<WorkspaceCorePinProperties>> const &getOutputsProperties() const;

    int getNumberOfVisibleDecimal();
    int setNumberOfVisibleDecimal(int value);

    virtual int maxLenghtOfData()=0;

    float getDataItemsWidth();
    float setDataItemsWidth();

    WorkspaceLevelOfDetail setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail);
    WorkspaceLevelOfDetail getLevelOfDetail();

    Core::Transform::DataMap const * setDataMap(const Core::Transform::DataMap* mapToSet);
    Core::Transform::DataMap const * getDataMap();

    bool drawDragFloatWithMap_Inline(float* const value, const int mapValue, std::string label);
	virtual void drawDataSetValues_builder(util::NodeBuilder& builder, std::vector<std::string>const & labels, std::vector<getter_function_pointer>const & getters, std::vector<setter_function_pointer>const & setters, std::vector<unsigned char> datamap_values);

	virtual void drawInputLinks();

	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);
	virtual void drawData(util::NodeBuilder& builder);
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);

    virtual void drawDataFull(util::NodeBuilder& builder)=0;
	virtual void drawDataSetValues(util::NodeBuilder& builder)=0;
	virtual void drawDataLabel(util::NodeBuilder& builder);

    void drawMenuLevelOfDetail();
    void drawMenuSetDataMap();
    void drawMenuSetPrecision();

};

/*! \class WorkspaceCorePinProperties
    \brief Information of Pin for graphic
 */
class WorkspaceCorePinProperties : public WorkspacePinProperties
{
protected:
	Core::Pin const &m_pin;
    WorkspaceNodeWithCoreData &m_node;

public:
	WorkspaceCorePinProperties(ne::PinId const id, std::string label, Core::Pin const &pin, WorkspaceNodeWithCoreData &node);

    Core::Pin const & getCorePin() const;
	ne::PinId const getParentPinId() const;

	WorkspaceNodeWithCoreData & getNode() const;


    int getIndex() const;
    PinKind getKind() const;
    EValueType getType() const;
	bool isConnected() const;
};
