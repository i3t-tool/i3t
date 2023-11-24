/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <string>

#include "spdlog/fmt/fmt.h"

#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Transform.h" /* building transformations nodes*/
#include "WorkspaceElements.h"

#include "State/NodeVisitor.h"

class WorkspaceCorePin;
class WorkspaceLink;
class WorkspaceWindow;

typedef std::vector<Core::Pin>::const_iterator corePinIter;
typedef std::vector<Ptr<WorkspaceCorePin>>::const_iterator workspacePinIter;
typedef std::vector<Ptr<WorkspaceLink>>::const_iterator workspaceLinkIter;

typedef std::function<float()> getter_function_pointer;
typedef std::function<Core::ValueSetResult(float)> setter_function_pointer;

class WorkspaceNodeWithCoreData : public WorkspaceNode, public IVisitable
{
protected:
	int m_numberOfVisibleDecimal; ///< number of decimal places used while display floats in the workspace
	float m_dataItemsWidth;
	float m_headerMinWidth{0}; ///< Can be used to specify the minimum header width of the node.
	bool m_isLabelBeingEdited = false;
	bool m_isFirstDraw = true;
	FloatPopupMode m_floatPopupMode;
	WorkspaceLevelOfDetail m_levelOfDetail;

	/**
	 * \brief reference to Core
	 *
	 * WorkspaceNodeWithCoreData is owner
	 */
	Ptr<Core::Node> const m_nodebase;

public:
	WorkspaceNodeWithCoreData(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase);
	~WorkspaceNodeWithCoreData() override;

	Ptr<Core::Node> const getNodebase() const;

	int getNumberOfVisibleDecimal();
	virtual void setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode()
	{
		return m_floatPopupMode;
	};
	void setFloatPopupMode(FloatPopupMode mode)
	{
		m_floatPopupMode = mode;
	};

	virtual int maxLengthOfData() = 0;

	float getDataItemsWidth();
	virtual float updateDataItemsWidth();
	bool getIsLabelBeingEdited()
	{
		return m_isLabelBeingEdited;
	};

	WorkspaceLevelOfDetail setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail);
	WorkspaceLevelOfDetail getLevelOfDetail();

	Core::DataMap const* setDataMap(const Core::DataMap* mapToSet);
	Core::DataMap const* getDataMap();

	bool drawDataLabel();
	void drawMenuSetEditable();

	/* DIWNE function */
	virtual bool topContent();

	virtual void drawMenuLevelOfDetail() = 0;
	void drawMenuSetPrecision();

	virtual void popupContent();
	bool processObjectDrag();

	bool processSelect() override;
	bool processUnselect() override;

	// TODO: (DR) Figure out what this was about
	// TODO: (DR) Mouse buttons are "hard-coded" in DiwneObject, presumably JH was
	//  trying to hook them up to
	//  the InputManager. But that change was only made here and not in the
	//  DiwneObject superclass causing inconsistent behaviour. So I commented these
	//  overrides out for the time being

	// bool bypassDragAction();
	// bool bypassHoldAction();
	// bool bypassUnholdAction();
	// bool bypassSelectAction();
	// bool bypassUnselectAction();
	// bool bypassTouchAction();
	void drawMenuDuplicate();

private:
	const char* getButtonSymbolFromLOD(WorkspaceLevelOfDetail detail);
};

class WorkspaceCoreOutputPin;
class WorkspaceCoreInputPin;
///*! \class WorkspaceCoreLink
class WorkspaceCoreLink : public DIWNE::Link
{
protected:
	WorkspaceCoreOutputPin* m_startPin;
	WorkspaceCoreInputPin* const m_endPin;

public:
	WorkspaceCoreLink(DIWNE::Diwne& diwne, DIWNE::ID id, WorkspaceCoreInputPin* endPin);

	WorkspaceCoreOutputPin* getStartPin() const
	{
		return m_startPin;
	};
	WorkspaceCoreInputPin* const getEndPin() const
	{
		return m_endPin;
	};
	void setStartPin(WorkspaceCoreOutputPin* startPin)
	{
		m_startPin = startPin;
	};
	void unplug();

	virtual void popupContent();
	virtual bool initialize();

	void updateEndpoints();
	void updateControlPointsOffsets();
};

/*! \class WorkspaceCorePinProperties
    \brief Information of Pin for graphic
 */
class WorkspaceCorePin : public DIWNE::Pin
{
protected:
	Core::Pin const& m_pin;
	WorkspaceNodeWithCoreData& m_node;
	ImRect m_iconRectDiwne;

public:
	WorkspaceCorePin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node);

	//    bool allowInteraction() const override;
	//    bool allowProcessFocused() const override;

	Core::Pin const& getCorePin() const;

	WorkspaceNodeWithCoreData& getNode()
	{
		return m_node;
	}
	const WorkspaceNodeWithCoreData& getNode() const
	{
		return m_node;
	};

	int getIndex() const;
	PinKind getKind() const;
	Core::EValueType getType() const;
	bool isConnected() const;
	void popupContent();

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
	bool m_connectionChanged; ///< Flag indicating that the pin has been plugged or unplugged this frame
	                          ///< Note: This flag gets reset the moment the pin is drawn, which is kinda dumb,
	                          ///< don't rely on this too much, it is currently used for a rather specific purpose


public:
	WorkspaceCoreInputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                      WorkspaceNodeWithCoreData& node);

	bool drawDiwne(DIWNE::DrawMode = DIWNE::DrawMode::Interacting);

	WorkspaceCoreLink& getLink()
	{
		return m_link;
	};
	void updateConnectionPointDiwne()
	{
		m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Min.x, (m_iconRectDiwne.Min.y + m_iconRectDiwne.Max.y) / 2);
	};
	void setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou);

	void unplug(bool logEvent = true);
	bool plug(WorkspaceCoreOutputPin* ou, bool logEvent = true);
	bool connectionChanged() const;

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
	WorkspaceCoreOutputPin(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                       WorkspaceNodeWithCoreData& node);

	void updateConnectionPointDiwne()
	{
		m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Max.x, (m_iconRectDiwne.Min.y + m_iconRectDiwne.Max.y) / 2);
	};

	bool content() override;
};

class WorkspaceCoreOutputPinWithData : public WorkspaceCoreOutputPin
{
public:
	WorkspaceCoreOutputPinWithData(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                               WorkspaceNodeWithCoreData& node);

	virtual bool content();

	virtual bool drawData() = 0;
	virtual int maxLengthOfData() = 0;
};

class WorkspaceCoreOutputPinMatrix4x4 : public WorkspaceCoreOutputPinWithData
{
public:
	WorkspaceCoreOutputPinMatrix4x4(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                                WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node){};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector4 : public WorkspaceCoreOutputPinWithData
{
public:
	WorkspaceCoreOutputPinVector4(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                              WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node){};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinVector3 : public WorkspaceCoreOutputPinWithData
{
public:
	WorkspaceCoreOutputPinVector3(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                              WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node){};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinFloat : public WorkspaceCoreOutputPinWithData
{
public:
	WorkspaceCoreOutputPinFloat(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                            WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node){};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinQuaternion : public WorkspaceCoreOutputPinWithData
{
public:
	WorkspaceCoreOutputPinQuaternion(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                                 WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node){};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinPulse : public WorkspaceCoreOutputPinWithData
{
public:
	std::string m_buttonText;
	WorkspaceCoreOutputPinPulse(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                            WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPinWithData(diwne, id, pin, node)
	{
		m_buttonText = m_pin.getLabel();
	};

	bool drawData();
	int maxLengthOfData();
};

class WorkspaceCoreOutputPinScreen : public WorkspaceCoreOutputPinWithData
{
private:
	GLuint renderTexture;

public:
	WorkspaceCoreOutputPinScreen(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                             WorkspaceNodeWithCoreData& node);

	bool drawData();
	int maxLengthOfData();
};

/* MatrixMulPin is Output and Input because of different
 * getLinkConnectionPointDiwne() function */
class WorkspaceCoreOutputPinMatrixMul : public WorkspaceCoreOutputPin
{
public:
	WorkspaceCoreOutputPinMatrixMul(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                                WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreOutputPin(diwne, id, pin, node){};
};

class WorkspaceCoreInputPinMatrixMul : public WorkspaceCoreInputPin
{
public:
	WorkspaceCoreInputPinMatrixMul(DIWNE::Diwne& diwne, DIWNE::ID const id, Core::Pin const& pin,
	                               WorkspaceNodeWithCoreData& node)
	    : WorkspaceCoreInputPin(diwne, id, pin, node){};
};

class WorkspaceNodeWithCoreDataWithPins : public WorkspaceNodeWithCoreData
{
private:
	float m_minRightAlignOfRightPins;

protected:
	std::vector<Ptr<WorkspaceCoreInputPin>> m_workspaceInputs;
	std::vector<Ptr<WorkspaceCoreOutputPin>> m_workspaceOutputs;
	bool m_showDataOnPins;

public:
	std::vector<Ptr<WorkspaceCoreInputPin>> const& getInputs() const
	{
		return m_workspaceInputs;
	};
	std::vector<Ptr<WorkspaceCoreOutputPin>> const& getOutputs() const
	{
		return m_workspaceOutputs;
	};

	virtual std::vector<Ptr<WorkspaceCoreOutputPin>> const getOutputsToShow() const
	{
		return getOutputs();
	};

	WorkspaceNodeWithCoreDataWithPins(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins = true);

	virtual bool leftContent();
	virtual bool rightContent();

	virtual bool finalize();
};

/* >>>>> STATIC FUNCTIONS <<<<< */
/* \todo maybe create from this function class "WithPins" and inherit other
 * class from "WithPins" */
// extern void loadWorkspacePinsFromCorePins(WorkspaceNodeWithCoreData&
// workspaceNode, Core::Node::ConstPinListRef coreInputPins,
// Core::Node::ConstPinListRef coreOutputPins,
// std::vector<Ptr<WorkspaceCorePin>> & workspaceInputPins,
// std::vector<Ptr<WorkspaceCorePin>> & workspaceOutputPins);

extern bool bypassFloatFocusAction();
extern bool bypassFloatRaisePopupAction();

extern bool drawDragFloatWithMap_Inline(DIWNE::Diwne& diwne, int const numberOfVisibleDecimals,
                                        FloatPopupMode& floatPopupMode, std::string const label, float& value,
                                        Core::EValueState const& valueState, bool& valueChanged);
extern void popupFloatContent(FloatPopupMode& popupMode, float& selectedValue, bool& valueSelected);

/**
 * \param valueOfChange is set to value setted by user if so - not touched if no
 * user change heppen \return whether some interaction happen
 */
extern bool drawData4x4(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth,
                        FloatPopupMode& floatPopupMode, const glm::mat4& data,
                        std::array<std::array<Core::EValueState, 4> const, 4> const& dataState, bool& valueChanged,
                        int& rowOfChange, int& columnOfChange, float& valueOfChange);
// extern bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int
// const numberOfVisibleDecimals, float dataWidth, FloatPopupMode&
// floatPopupMode, const glm::mat4& data, const Core::DataMap&
// dataMap, bool& valueChanged, int& rowOfChange, int& columnOfChange, float&
// valueOfChange );
extern int maxLengthOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal);

extern bool drawDataVec4(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
                         float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data,
                         std::array<Core::EValueState, 4> const& dataState, bool& valueChanged,
                         glm::vec4& valueOfChange);
extern int maxLengthOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal);

extern bool drawDataVec3(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
                         float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data,
                         std::array<Core::EValueState, 3> const& dataState, bool& valueChanged,
                         glm::vec3& valueOfChange);
extern int maxLengthOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal);

extern bool drawDataFloat(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
                          float dataWidth, FloatPopupMode& floatPopupMode, const float& data,
                          Core::EValueState const& dataState, bool& valueChanged, float& valueOfChange);
extern int maxLengthOfDataFloat(const float& data, int numberOfVisibleDecimal);

extern bool drawDataQuaternion(DIWNE::Diwne& diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals,
                               float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data,
                               std::array<Core::EValueState, 4> const& dataState, bool& valueChanged,
                               glm::quat& valueOfChange);
extern int maxLengthOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal);

extern void drawMenuLevelOfDetail_builder(Ptr<WorkspaceNodeWithCoreData> node,
                                          std::vector<WorkspaceLevelOfDetail> const& levels_of_detail);
