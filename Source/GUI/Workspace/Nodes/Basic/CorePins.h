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

#include "DIWNE/diwne_include.h"

#include "Core/Nodes/NodeData.h"
#include "GUI/Theme/Theme.h"

#include "CoreLink.h"
#include "CoreNode.h"

namespace Workspace
{
/*! \enum PinKind
    \brief kinds (in/out) of Workspace Pin
 */
enum class PinKind
{
	Output, ///< on the box right hand side
	Input   ///< on the box left hand side
};

/* DIWNE - \todo JH \todo MH see in .cpp to remove, but I need something what
 * use instead -> from Type get Shape and Color */
extern std::map<Core::EValueType, EColor> PinColorBackground;

extern std::map<Core::EValueType, DIWNE::IconType> PinShapeBackground;

extern std::map<Core::EValueType, DIWNE::IconType> PinShapeForeground;

extern std::map<Core::EValueType, EColor> PinColorForeground;

/*! \class WorkspaceCorePinProperties
    \brief Information of Pin for graphic
 */
class CorePin : public DIWNE::Pin
{
protected:
	Core::Pin const& m_pin;
	CoreNode& m_node;
	ImRect m_iconRectDiwne;

public:
	// (PF) icon for the cycle box, Triangle elsewhere
	DIWNE::IconType m_iconType = DIWNE::IconType::TriangleRight;


	CorePin(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node);

	//    bool allowInteraction() const override;
	//    bool allowProcessFocused() const override;

	Core::Pin const& getCorePin() const;

	CoreNode& getNode()
	{
		return m_node;
	}
	const CoreNode& getNode() const
	{
		return m_node;
	};

	int getIndex() const;
	PinKind getKind() const;
	Core::EValueType getType() const;
	bool isConnected() const;
	void popupContent() override;

	bool processFocused() override;

	/* DIWNE function */
	bool content() override;
	bool processDrag() override;
	bool processConnectionPrepared() override;

	bool bypassFocusForInteractionAction() override;
};

class CoreOutPin;

class CoreInPin : public CorePin
{
protected:
	CoreLink m_link;
	bool m_connectionChanged; ///< Flag indicating that the pin has been plugged or unplugged this frame
	                          ///< Note: This flag gets reset the moment the pin is drawn, which is kinda dumb,
	                          ///< don't rely on this too much, it is currently used for a rather specific purpose


public:
	CoreInPin(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node);

	/**
	 * \brief Draw pin icon + label and register the connected wire
	 * \return interaction happened
	 */
	bool drawDiwne(DIWNE::DrawMode = DIWNE::DrawMode::Interacting) override;

	CoreLink& getLink()
	{
		return m_link;
	};
	void updateConnectionPointDiwne() override
	{
		m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Min.x, (m_iconRectDiwne.Min.y + m_iconRectDiwne.Max.y) / 2);
	};
	void setConnectedWorkspaceOutput(CoreOutPin* ou);

	/**
	 * \pre Needs to be plugged
	 */
	void unplug(bool logEvent = true);
	bool plug(CoreOutPin* ou, bool logEvent = true);
	bool connectionChanged() const;

	/* DIWNE function */
	/**
	 * \brief Draw icon and label, if defined
	 * \return false - both icon and label do not allow interactions
	 */
	bool content() override;
	bool processInteractions() override;
	virtual bool processCreateAndPlugConstrutorNode();
	bool processUnplug();

	bool bypassUnplugAction();
	bool allowCreateAndPlugConstructorNodeAction();
	bool bypassCreateAndPlugConstructorNodeAction();
};

class CoreOutPin : public CorePin
{
protected:
public:
	CoreOutPin(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node);

	void updateConnectionPointDiwne() override
	{
		m_connectionPointDiwne = ImVec2(m_iconRectDiwne.Max.x, (m_iconRectDiwne.Min.y + m_iconRectDiwne.Max.y) / 2);
	};

	bool content() override;
};

class DataOutPin : public CoreOutPin
{
public:
	DataOutPin(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node);

	bool content() override; ///< draw data, label, and pin

	virtual bool drawData() = 0;
	virtual int maxLengthOfData() = 0;
};

class DataOutPinMatrix : public DataOutPin
{
public:
	DataOutPinMatrix(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){};

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinVector4 : public DataOutPin
{
public:
	DataOutPinVector4(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){};

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinVector3 : public DataOutPin
{
public:
	DataOutPinVector3(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){};

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinFloat : public DataOutPin
{
public:
	DataOutPinFloat(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){};

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinQuat : public DataOutPin
{
public:
	DataOutPinQuat(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){};

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinPulse : public DataOutPin
{
public:
	// std::string m_buttonText;
	DataOutPinPulse(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : DataOutPin(diwne, id, pin, node){
	          // m_buttonText = m_pin.getLabel();
	      };

	bool drawData() override;
	int maxLengthOfData() override;
};

class DataOutPinScreen : public DataOutPin
{
private:
	GLuint renderTexture;

public:
	DataOutPinScreen(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node);

	bool drawData() override;
	int maxLengthOfData() override;
};

/* MatrixMulPin is Output and Input because of different
 * getLinkConnectionPointDiwne() function */
class CoreOutPinMatrixMultiply : public CoreOutPin
{
public:
	CoreOutPinMatrixMultiply(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : CoreOutPin(diwne, id, pin, node){};
};

class CoreInPinMatrixMultiply : public CoreInPin
{
public:
	CoreInPinMatrixMultiply(DIWNE::NodeEditor& diwne, DIWNE::ID const id, Core::Pin const& pin, CoreNode& node)
	    : CoreInPin(diwne, id, pin, node){};
};
} // namespace Workspace