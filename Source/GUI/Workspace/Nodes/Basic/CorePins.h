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

#include "CoreNode.h"

namespace Workspace
{

// TODO: Move to some DIWNE::BasicPin impl
enum class PinStyle
{
	Square,
	Socket
};

extern std::map<Core::EValueType, EColor> PinColorBackground;
extern std::map<Core::EValueType, DIWNE::IconType> PinShapeBackground;
extern std::map<Core::EValueType, DIWNE::IconType> PinShapeForeground;
extern std::map<Core::EValueType, EColor> PinColorForeground;

class CorePin : public DIWNE::Pin
{
protected:
	Core::Pin const& m_pin;

public:
	PinStyle m_pinStyle{PinStyle::Square};
	// (PF) icon for the cycle box, Triangle elsewhere
	DIWNE::IconType m_iconType = DIWNE::IconType::NoIcon;
	bool m_showData{true};

	CorePin(DIWNE::NodeEditor& diwne, Core::Pin const& pin, CoreNode* node, bool isInput,
	        const std::string& label = "CorePin");

	void content(DIWNE::DrawInfo& context) override;

	bool allowDrawing() override;

	void drawPin(bool left, float alpha);
	bool drawLabel(DIWNE::DrawInfo& context);  ///< @return Whether a new item was created
	bool drawDataEx(DIWNE::DrawInfo& context); ///< @return Whether a new item was created
	bool drawData(DIWNE::DrawInfo& context);   ///< @return Whether a new item was created
	int maxLengthOfData();

	void popupContent(DIWNE::DrawInfo& context) override;

	bool allowConnection() const override;
	bool isDisabled() const override;

	bool preparePlug(Pin* otherPin, DIWNE::Link* link, bool hovering) override;
	bool canPlug(Pin* other) const override;
	bool plugLink(DIWNE::Pin* startPin, DIWNE::Link* link, bool logEvent = true) override;
	void onPlug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool isStartPin, bool logEvent = true) override;
	void onUnplug(DIWNE::Pin* otherPin, DIWNE::Link* link, bool wasStartPin, bool logEvent = true) override;
	std::shared_ptr<DIWNE::Link> createLink() override;
	void onReleased(bool justReleased, DIWNE::DrawInfo& context) override;

	Core::Pin const& getCorePin() const;
	int getCoreIndex() const;
	Core::EValueType getType() const;
	bool isConnected() const;

	// TODO: Finish this "debug" check
	/**
	 * This method ensures the state of this pin is synchronized with the core state.
	 * Currently ONLY deletes links that should no longer exist.
	 * As to create new links it would need to search for a GUI equivalent of a certain Core pin.
	 * @return Whether a change was made.
	 */
	bool ensureLinkSyncWithCore();

private:
	// TODO: Maybe make this virtual in DIWNE::Pin?
	ImVec2 getPinSize() const;

	void createNodeFromPin();
	template <typename T>
	void createNodeFromPinImpl();

	bool drawBasicPinData(DIWNE::DrawInfo& context); ///< @return Whether a new item was created
	bool drawPulsePinData(DIWNE::DrawInfo& context); ///< @return Whether a new item was created

	void drawSquarePin(const ImVec2& size, bool left, float alpha);
	void drawSocketPin(const ImVec2& size, bool left, float alpha);
};
} // namespace Workspace