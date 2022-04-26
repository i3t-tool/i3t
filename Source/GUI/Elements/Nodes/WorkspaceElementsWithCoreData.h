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
	WorkspaceNodeWithCoreData(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase);
	~WorkspaceNodeWithCoreData() override;

	Ptr<Core::NodeBase> const getNodebase() const;

	int getNumberOfVisibleDecimal();
	virtual void setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode() {return m_floatPopupMode;};
	void setFloatPopupMode(FloatPopupMode mode){m_floatPopupMode = mode;};

	float inactiveMarcToLeft(float step=0.1){return m_inactiveMark-=step;};
	float inactiveMarcToRight(float step=0.1){return m_inactiveMark+=step;};

	virtual int maxLenghtOfData() = 0;

	float getDataItemsWidth();
	float setDataItemsWidth();

	WorkspaceLevelOfDetail setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail);
	WorkspaceLevelOfDetail getLevelOfDetail();

	Core::Transform::DataMap const* setDataMap(const Core::Transform::DataMap* mapToSet);
	Core::Transform::DataMap const* getDataMap();

	bool drawDataLabel();

	/* DIWNE function */
    virtual bool topContent();

	virtual void drawMenuLevelOfDetail()=0;
	void drawMenuSetPrecision();

	virtual void popupContent();

	bool bypassDragAction();
    bool bypassHoldAction();
    bool bypassUnholdAction();
    bool bypassSelectAction();
    bool bypassUnselectAction();
    bool bypassTouchAction();


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
	WorkspaceCoreLink(DIWNE::Diwne& diwne, DIWNE::ID id, WorkspaceCoreInputPin *endPin);

    WorkspaceCoreOutputPin* getStartPin() const {return m_startPin;};
    WorkspaceCoreInputPin* const getEndPin() const {return m_endPin;};
    void setStartPin(WorkspaceCoreOutputPin *startPin) {m_startPin = startPin;};
    void unplug();

    virtual void popupContent();
    virtual bool initialize();

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
	ImRect m_iconRectDiwne;



public:
    WorkspaceCorePin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

//    bool allowInteraction() const override;
//    bool allowProcessFocused() const override;

	Core::Pin const& getCorePin() const;

	WorkspaceCorePin& getParentPin() const;

	WorkspaceNodeWithCoreData& getNode() const;


	int				 getIndex() const;
	PinKind		 getKind() const;
	EValueType getType() const;
	bool			 isConnected() const;

	/* DIWNE function */
	virtual bool content();
	virtual bool processDrag();
	virtual bool processConnectionPrepared();

	virtual bool bypassFocusForInteractionAction();

};


class WorkspaceCoreInputPin : public WorkspaceCorePin
{
    protected:
        WorkspaceCoreLink m_link;
    public:
        WorkspaceCoreInputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);
        WorkspaceCoreLink& getLink() {return m_link;};
        void updateConnectionPointDiwne() {m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Min.x, (m_iconRectDiwne.Min.y+m_iconRectDiwne.Max.y)/2);};
        void setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou);

        void unplug();
        void plug(WorkspaceCoreOutputPin* ou);
        bool m_connection_changed;

        bool drawDiwne(DIWNE::DrawMode=DIWNE::DrawMode::Interacting);

        /* DIWNE function */
        virtual bool content();
        virtual bool processInteractions();
        virtual bool processCreateAndPlugConstrutorNode();
        bool processUnplug();

        bool bypassUnplugAction();
        bool allowCreateAndPlugConstrutorNodeAction();
        bool bypassCreateAndPlugConstrutorNodeAction();


};

class WorkspaceCoreOutputPin : public WorkspaceCorePin
{
protected:
    public:
        WorkspaceCoreOutputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);
        void updateConnectionPointDiwne() {m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Max.x, (m_iconRectDiwne.Min.y+m_iconRectDiwne.Max.y)/2);};
};

class WorkspaceCoreOutputPinWithData : public WorkspaceCoreOutputPin
{
    public:
        WorkspaceCoreOutputPinWithData(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

        virtual bool content();


        virtual bool drawData() = 0;
        virtual int maxLengthOfData() = 0;
};

class WorkspaceCoreOutputPinMatrix4x4 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinMatrix4x4(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector4 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinVector4(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector3 : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinVector3(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinFloat : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinFloat(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinQuaternion : public WorkspaceCoreOutputPinWithData
{
    public:
        WorkspaceCoreOutputPinQuaternion(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinPulse : public WorkspaceCoreOutputPinWithData
{
    public:
        std::string m_buttonText;
        WorkspaceCoreOutputPinPulse(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPinWithData(diwne, id, pin, node) {m_buttonText = m_pin.getLabel();};

        bool drawData();
        int maxLengthOfData();
};

class WorkspaceCoreOutputPinScreen : public WorkspaceCoreOutputPinWithData
{
    private:
        GLuint renderTexture;
        RenderTexture* rend;
        Camera* cam;
    public:
        WorkspaceCoreOutputPinScreen(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

        bool drawData();
        int maxLengthOfData();
};

/* MatrixMulPin is Output and Input because of different getLinkConnectionPointDiwne() function */
class WorkspaceCoreOutputPinMatrixMul : public WorkspaceCoreOutputPin
{
    public:
        WorkspaceCoreOutputPinMatrixMul(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreOutputPin(diwne, id, pin, node) {};
};

class WorkspaceCoreInputPinMatrixMul : public WorkspaceCoreInputPin
{
    public:
        WorkspaceCoreInputPinMatrixMul(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node) : WorkspaceCoreInputPin(diwne, id, pin, node) {};
};

class WorkspaceNodeWithCoreDataWithPins : public WorkspaceNodeWithCoreData
{
    private:
        float m_minRightAlignOfRightPins;
    protected:
        std::vector<Ptr<WorkspaceCoreInputPin>>    m_workspaceInputs;
        std::vector<Ptr<WorkspaceCoreOutputPin>>    m_workspaceOutputs;
        bool m_showDataOnPins;

	public:
        std::vector<Ptr<WorkspaceCoreInputPin>> const& getInputs() const {return m_workspaceInputs;};
        std::vector<Ptr<WorkspaceCoreOutputPin>> const& getOutputs() const {return m_workspaceOutputs;};

        virtual std::vector<Ptr<WorkspaceCoreOutputPin>> const getOutputsToShow() const {return getOutputs();};

        WorkspaceNodeWithCoreDataWithPins(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase, bool showDataOnPins=true);

    virtual bool leftContent();
    virtual bool rightContent();

    virtual bool finalize();

};

/* >>>>> STATIC FUNCTIONS <<<<< */
/* \todo maybe create from this function class "WithPins" and inherit other class from "WithPins" */
//extern void loadWorkspacePinsFromCorePins(WorkspaceNodeWithCoreData& workspaceNode, Core::Node::ConstPinListRef coreInputPins, Core::Node::ConstPinListRef coreOutputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceInputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceOutputPins);

extern bool bypassFloatFocusAction();
extern bool bypassFloatRaisePopupAction();

extern bool drawDragFloatWithMap_Inline(DIWNE::Diwne &diwne, int const numberOfVisibleDecimals, FloatPopupMode& floatPopupMode, std::string const label, float& value, Core::EValueState const& valueState, bool& valueChanged);
extern void popupFloatContent(FloatPopupMode &popupMode, float& selectedValue, bool& valueSelected);

extern bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data, std::array<std::array<Core::EValueState, 4> const, 4> const& dataState, bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange );
//extern bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange );
extern int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal);

extern bool drawDataVec4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data, std::array<Core::EValueState, 4> const& dataState, bool& valueChanged, glm::vec4& valueOfChange);
extern int maxLenghtOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal);

extern bool drawDataVec3(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data, std::array<Core::EValueState, 3> const& dataState, bool& valueChanged, glm::vec3& valueOfChange);
extern int maxLenghtOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal);

extern bool drawDataFloat(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const float& data, Core::EValueState const& dataState, bool& valueChanged, float& valueOfChange);
extern int maxLenghtOfDataFloat(const float& data, int numberOfVisibleDecimal);

extern bool drawDataQuaternion(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data, std::array<Core::EValueState, 4> const& dataState, bool& valueChanged, glm::quat& valueOfChange);
extern int maxLenghtOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal);

extern void drawMenuLevelOfDetail_builder(Ptr<WorkspaceNodeWithCoreData> node, std::vector<WorkspaceLevelOfDetail> const & levels_of_detail);





