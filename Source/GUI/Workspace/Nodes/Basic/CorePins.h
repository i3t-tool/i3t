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

public:
	// (PF) icon for the cycle box, Triangle elsewhere
	DIWNE::IconType m_iconType = DIWNE::IconType::TriangleRight;
	bool m_showData{true};

	CorePin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode* node, bool isInput);

	void content(DIWNE::DrawInfo& context) override;
	void drawPin(DIWNE::DrawInfo& context);
	void drawLabel(DIWNE::DrawInfo& context);
	void drawDataEx(DIWNE::DrawInfo& context);
	void drawData(DIWNE::DrawInfo& context);
	int maxLengthOfData();

	bool allowDrawing() override;
	bool allowInteraction() const override;

	bool preparePlug(Pin* otherPin, DIWNE::Link* link, bool hovering) override;

	Core::Pin const& getCorePin() const;

	int getCoreIndex() const;
	Core::EValueType getType() const;
	bool isConnected() const;
	void popupContent(DIWNE::DrawInfo& context) override;

	bool allowConnection() const override;

	bool plugLink(DIWNE::Pin* startPin, DIWNE::Link* link, bool logEvent = true) override;
	void onPlug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool isStartPin, bool logEvent = true) override;
	void onUnplug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool wasStartPin, bool logEvent = true) override;

	// TODO: Finish this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * This method ensures the state of this pin is synchronized with the core state.
	 * Currently ONLY deletes links that should no longer exist.
	 * As to create new links it would need to search for a GUI equivalent of a certain Core pin.
	 * @return Whether a change was made.
	 */
	bool ensureLinkSyncWithCore();

	std::shared_ptr<DIWNE::Link> createLink() override;

	void onReleased(bool justReleased, DIWNE::DrawInfo& context) override;

private:
	void createNodeFromPin();
	template <typename T>
	void createNodeFromPinImpl();

	void drawBasicPinData(DIWNE::DrawInfo& context);
	void drawPulsePinData(DIWNE::DrawInfo& context);
};
} // namespace Workspace