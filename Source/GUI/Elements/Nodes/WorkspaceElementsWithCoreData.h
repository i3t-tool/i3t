#pragma once

#include <string>

#include "spdlog/fmt/fmt.h"

#include "WorkspaceElements.h"
#include "Core/Nodes/Transform.h" /* building transformations nodes*/
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"

#include "State/NodeVisitor.h"

#include <World/Components/Camera.h>
#include <World/Components/Renderer.h>
#include <World/HardcodedMeshes.h>
#include <World/RenderTexture.h>

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


class WorkspaceNodeWithCoreData : public WorkspaceNode, public IVisitable
{
protected:
	int     m_numberOfVisibleDecimal;
	float   m_dataItemsWidth;
	float   m_inactiveMark;
	FloatPopupMode m_floatPopupMode;
	WorkspaceLevelOfDetail m_levelOfDetail;

	/**
	 * \brief reference to Core
	 *
	 * WorkspaceNodeWithCoreData is owner
	 */
	Ptr<Core::NodeBase> const m_nodebase;

public:
	WorkspaceNodeWithCoreData(Ptr<Core::NodeBase> nodebase);
	~WorkspaceNodeWithCoreData() override;

	Ptr<Core::NodeBase> const getNodebase() const;

	virtual bool isCamera();
	virtual bool isCycle();
	virtual bool isSequence();
	virtual bool isTrackball();
	virtual bool isQuatToFloatVec();
	virtual bool isQuatToAngleAxis();
	bool         isTransformation();

	int getNumberOfVisibleDecimal();
	int setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode() {return m_floatPopupMode;};
	void setFloatPopupMode(FloatPopupMode mode){m_floatPopupMode = mode;};

	virtual int maxLenghtOfData() = 0;

	float getDataItemsWidth(DIWNE::Diwne &diwne);
	float setDataItemsWidth();

	WorkspaceLevelOfDetail setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail);
	WorkspaceLevelOfDetail getLevelOfDetail();

	Core::Transform::DataMap const* setDataMap(const Core::Transform::DataMap* mapToSet);
	Core::Transform::DataMap const* getDataMap();

	bool drawDataLabel(DIWNE::Diwne &diwne);

	/* DIWNE function */
    virtual bool topContent(DIWNE::Diwne &diwne);


	virtual void drawMenuLevelOfDetail()=0;
	void drawMenuSetPrecision();

	virtual void nodePopupContent();


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
    void unplug();

    virtual void linkPopupContent();

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
	ImVec2 m_connectionPoint;
	ImRect m_iconRect;



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

	virtual bool processPinIcon(WorkspaceWindow &workspace);

	/* DIWNE function */
	virtual bool pinContent(DIWNE::Diwne &diwne);

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
        ImVec2 getLinkConnectionPointDiwne() const {return ImVec2(m_iconRect.Min.x, (m_iconRect.Min.y+m_iconRect.Max.y)/2); };
        void setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou);
        bool processPinIcon(WorkspaceWindow& workspace);

        void unplug();
        void plug(WorkspaceCoreOutputPin* ou);


        /* DIWNE function */
        virtual bool pinContent(DIWNE::Diwne &diwne);
};

class WorkspaceCoreOutputPin : public WorkspaceCorePin
{
protected:
    public:
        WorkspaceCoreOutputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);
        ImVec2 getLinkConnectionPointDiwne() const {return ImVec2(m_iconRect.Max.x, (m_iconRect.Min.y+m_iconRect.Max.y)/2); };
};

class WorkspaceCoreOutputPinWithData : public WorkspaceCoreOutputPin
{
    public:
        WorkspaceCoreOutputPinWithData(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

        bool pinContent(DIWNE::Diwne &diwne);
        virtual int maxLengthOfData() = 0;

};

class WorkspaceCoreOutputPinMatrix4x4 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinMatrix4x4(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector4 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinVector4(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector3 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinVector3(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinFloat : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinFloat(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinQuaternion : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinQuaternion(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinPulse : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinPulse(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(id, pin, node) {};

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinScreen : public WorkspaceCoreOutputPinWithData
{
    private:
        GLuint renderTexture;
        RenderTexture* rend;
        Camera* cam;
    public:
        WorkspaceCoreOutputPinScreen(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

        bool pinContent(DIWNE::Diwne &diwne);
        int maxLengthOfData();
};

/* MatrixMulPin is Output and Input because of different  getLinkConnectionPointDiwne()  function */
class WorkspaceCoreOutputPinMatrixMul : public WorkspaceCoreOutputPin
{
    public:
        WorkspaceCoreOutputPinMatrixMul(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPin(id, pin, node) {};
};

class WorkspaceCoreInputPinMatrixMul : public WorkspaceCoreInputPin
{
    public:
        WorkspaceCoreInputPinMatrixMul(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreInputPin(id, pin, node) {};
};

class WorkspaceNodeWithCoreDataWithPins : public WorkspaceNodeWithCoreData
{
    protected:
        std::vector<Ptr<WorkspaceCoreInputPin>>    m_workspaceInputs;
        std::vector<Ptr<WorkspaceCoreOutputPin>>    m_workspaceOutputs;
        bool m_showDataOnPins;

	public:
        std::vector<Ptr<WorkspaceCoreInputPin>> const& getInputs() const {return m_workspaceInputs;};
        std::vector<Ptr<WorkspaceCoreOutputPin>> const& getOutputs() const {return m_workspaceOutputs;};

        WorkspaceNodeWithCoreDataWithPins(Ptr<Core::NodeBase> nodebase, bool showDataOnPins=true);

    virtual bool leftContent(DIWNE::Diwne &diwne);
    virtual bool rightContent(DIWNE::Diwne &diwne);

};

/* >>>>> STATIC FUNCTIONS <<<<< */
/* \todo maybe create from this function class "WithPins" and inherit other class from "WithPins" */
//extern void loadWorkspacePinsFromCorePins(WorkspaceNodeWithCoreData& workspaceNode, Core::Node::ConstPinListRef coreInputPins, Core::Node::ConstPinListRef coreOutputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceInputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceOutputPins);


extern bool drawDragFloatWithMap_Inline(DIWNE::Diwne &diwne, int const numberOfVisibleDecimals, FloatPopupMode& floatPopupMode, std::string const label, float& value, int const mapValue, bool& valueChanged);
extern void popupFloatContent(FloatPopupMode &popupMode, float& selectedValue, bool& valueSelected);

extern bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange );
extern int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal);

extern bool drawDataVec4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::vec4& valueOfChange);
extern int maxLenghtOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal);

extern bool drawDataVec3(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::vec3& valueOfChange);
extern int maxLenghtOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal);

extern bool drawDataFloat(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const float& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, float& valueOfChange);
extern int maxLenghtOfDataFloat(const float& data, int numberOfVisibleDecimal);

extern bool drawDataQuaternion(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::quat& valueOfChange);
extern int maxLenghtOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal);

extern void drawMenuLevelOfDetail_builder(Ptr<WorkspaceNodeWithCoreData> node, std::vector<WorkspaceLevelOfDetail> const & levels_of_detail);





