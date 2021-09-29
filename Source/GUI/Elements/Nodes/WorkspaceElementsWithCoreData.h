#pragma once

#include "WorkspaceElements.h"
#include "Core/Nodes/Transform.h" /* building transformations nodes*/

#include <string>
#include "Core/Input/InputManager.h"
#include "spdlog/fmt/fmt.h"

class WorkspaceCorePin;
class WorkspaceLink;
class WorkspaceWindow;

extern std::map<Core::Transform::DataMap const*, std::string> WorkspaceDatamapName;
extern std::vector<Core::Transform::DataMap const*> WorkspaceDatamapMenuList;

typedef std::vector<Core::Pin>::const_iterator corePinIter;
typedef std::vector<Ptr<WorkspaceCorePin>>::const_iterator workspacePinIter;
typedef std::vector<Ptr<WorkspaceLink>>::const_iterator workspaceLinkIter;

typedef std::function<float()> getter_function_pointer;
typedef std::function<ValueSetResult(float)> setter_function_pointer;



class WorkspaceNodeWithCoreData : public WorkspaceNode
{
protected:
	int     m_numberOfVisibleDecimal;
	float   m_dataItemsWidth;
	float   m_inactiveMark;
    FloatPopupMode m_floatPopupMode;
    WorkspaceLevelOfDetail m_levelOfDetail;
    bool m_toDelete;

    Ptr<Core::NodeBase> const m_nodebase; /*! \brief reference to Core
                                                WorkspaceNodeWithCoreData is owner
                                           */
public:

//	std::vector<Ptr<WorkspaceLink>>       m_workspaceLinks;
	std::vector<Ptr<WorkspaceCorePin>>    m_workspaceInputs;
	std::vector<Ptr<WorkspaceCorePin>>    m_workspaceOutputs;


//	floatWindow fw; /* \todo create it protected */

//	WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args);
//	WorkspaceNodeWithCoreData(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase = nullptr,
//														std::string headerLabel = "With Core Data", std::string nodeLabel = "With Core Data");

	WorkspaceNodeWithCoreData(Ptr<Core::NodeBase> nodebase);
	~WorkspaceNodeWithCoreData();

	Ptr<Core::NodeBase> const getNodebase() const;

//	std::vector<Ptr<WorkspaceLink>> const&		getLinks() const;
	std::vector<Ptr<WorkspaceCorePin>> const& getInputs() const;
	std::vector<Ptr<WorkspaceCorePin>> const& getOutputs() const;

	virtual bool isCamera();
	virtual bool isCycle();
	virtual bool isSequence();
	virtual bool isTrackball();
	virtual bool isQuatToFloatVec();
	virtual bool isQuatToAngleAxis();
	bool            isTransformation();

	virtual bool inSequence();

	int getNumberOfVisibleDecimal();
	int setNumberOfVisibleDecimal(int value);

	virtual int maxLenghtOfData(int index=0) = 0;

	float getDataItemsWidth(DIWNE::Diwne &diwne);
	float setDataItemsWidth();

	WorkspaceLevelOfDetail setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail);
	WorkspaceLevelOfDetail getLevelOfDetail();

	Core::Transform::DataMap const* setDataMap(const Core::Transform::DataMap* mapToSet);
	Core::Transform::DataMap const* getDataMap();

	bool drawDragFloatWithMap_Inline(DIWNE::Diwne &diwne, bool& valueChanged, float& value, int const mapValue, std::string const label);

    virtual bool drawDataSetValues_builder( DIWNE::Diwne &diwne, std::vector<std::string> const& labels,
																				 std::vector<getter_function_pointer> const& getters,
																				 std::vector<setter_function_pointer> const& setters,
																				 std::vector<unsigned char> const& datamap_values);

//	virtual void drawInputLinks();

	virtual bool dataAreValid() {return true;};

    virtual bool drawData(DIWNE::Diwne &diwne, int index);

    virtual bool drawDataFull(DIWNE::Diwne &diwne, int index=0) = 0;
	virtual bool inline drawDataSetValues(DIWNE::Diwne &diwne){ return drawDataFull(diwne); };
	virtual bool drawDataLabel(DIWNE::Diwne &diwne);

	/* DIWNE function */
    virtual bool topContent(DIWNE::Diwne &diwne);
	virtual bool leftContent(DIWNE::Diwne &diwne);
	virtual bool middleContent(DIWNE::Diwne &diwne);
	virtual bool rightContent(DIWNE::Diwne &diwne);


	void drawMenuLevelOfDetail();
	void drawMenuSetDataMap();
	void drawMenuSetPrecision();

	virtual void nodePopupContent();

	bool getToDelete() const {return m_toDelete;};

};

class WorkspaceCoreOutputPin;
class WorkspaceCoreInputPin;
///*! \class WorkspaceCoreLink
class WorkspaceCoreLink : public DIWNE::Link
{
protected:
    WorkspaceCoreOutputPin *m_startPin;
    WorkspaceCoreInputPin * const m_endPin;
public:
	WorkspaceCoreLink(DIWNE::ID id, WorkspaceCoreInputPin *endPin);

    WorkspaceCoreOutputPin* getStartPin() const {return m_startPin;};
    WorkspaceCoreInputPin* const getEndPin() const {return m_endPin;};
    void setStartPin(WorkspaceCoreOutputPin *startPin) {m_startPin = startPin;};

    void updateEndpoints();
    void updateControlPointsOffsets();
};

/*! \class WorkspaceCorePinProperties
    \brief Information of Pin for graphic
 */
class WorkspaceCorePin : public WorkspacePin
{
protected:
	Core::Pin const&            m_pin;
	WorkspaceNodeWithCoreData&  m_node;

//    ,   m_iconSize(I3T::getSize(ESizeVec2::Nodes_IconSize))
//    ,   m_color(I3T::getColor(EColor::MatrixPin))

public:
    WorkspaceCorePin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

	Core::Pin const& getCorePin() const;
	//ne::PinId const	 getParentPinId() const;

	WorkspaceCorePin& getParentPin() const;

	WorkspaceNodeWithCoreData& getNode() const;

//		ImVec2 const		getIconSize() const { return m_iconSize; };
//	ImColor const		getColor() const { return m_color; };

	int				 getIndex() const;
	PinKind		 getKind() const;
	EValueType getType() const;
	bool			 isConnected() const;

	/* DIWNE function */
	bool pinContent(DIWNE::Diwne &diwne);

	void pinActiveProcess(DIWNE::Diwne &diwne);

	//void pinConnectLinkProcess(DIWNE::Diwne &diwne, WorkspaceCoreInputPin& input, WorkspaceCoreOutputPin& output);
	void pinConnectLinkProcess(DIWNE::Diwne &diwne);

};


class WorkspaceCoreInputPin : public WorkspaceCorePin
{
    protected:
        WorkspaceCoreLink m_link;
    public:
        WorkspaceCoreInputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);
        WorkspaceCoreLink& getLink() {return m_link;};
};

class WorkspaceCoreOutputPin : public WorkspaceCorePin
{
    public:
        WorkspaceCoreOutputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

//        virtual void pinConnectLinkProcess(DIWNE::Diwne &diwne);
 //       void pinActiveProcess(DIWNE::Diwne &diwne);

        ImVec2 getLinkConnectionPoint() const {return ImVec2(m_pinRectDiwne.Max.x, (m_pinRectDiwne.Min.y+m_pinRectDiwne.Max.y)/2); };
};


/* >>>>> STATIC FUNCTIONS <<<<< */
extern void popupFloatContent(FloatPopupMode &popupMode, float& selectedValue, bool& valueSelected);

