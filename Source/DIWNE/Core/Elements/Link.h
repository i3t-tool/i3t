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

#include <algorithm>
#include <limits>

#include "DiwneObject.h"

namespace DIWNE
{
class Pin;

class Link : public DiwneObject
{
	using Super = DiwneObject;

protected:
	Pin* m_startPin{nullptr};
	Pin* m_endPin{nullptr};

	ImVec2 m_startDiwne, m_endDiwne;
	ImVec2 m_controlPointStartDiwne, m_controlPointEndDiwne;
	float m_squaredDistanceMouseFromLink{std::numeric_limits<float>::max()};

	ImVec4 m_color;

public:
	bool m_previewPlugged{false}; ///< Style the link as if it was plugged in

	Link(NodeEditor& diwne, std::string labelDiwne = "DiwneLink");

	/**
	 * Connect the two pins using this link.
	 * On success this method calls onPlug() on both pins.
	 * If the link has different pins plugged in already, the corresponding pins get unplugged.
	 * @param startPin
	 * @param endPin
	 * @param logEvent The boolean flag passed to onPlug() on successful connect
	 * @return True when the link was already registered with one of the pins
	 */
	virtual bool connect(Pin* startPin, Pin* endPin, bool logEvent = true);

	/**
	 * Disconnect all pins. On success this method calls onUnplug() on both pins.
	 * @param logEvent The boolean flag passed to onPlug() on successful connect
	 * @return True on success, false otherwise (if it wasn't connected)
	 */
	virtual bool disconnect(bool logEvent = true);

	/**
	 * Disconnect one of the pins. On success this method calls onUnplug() on the disconnected pin.
	 * @param startOrEndPin Whether to disconnect the start or end pin.
	 * @param logEvent The boolean flag passed to onPlug() on successful connect.
	 * @return True on success, false otherwise (if it wasn't connected)
	 */
	virtual bool disconnectPin(bool startOrEndPin, bool logEvent = true);

	virtual bool disconnectPin(Pin* pin, bool logEvent = true);

	// Lifecycle
	// =============================================================================================================
	void initialize(DrawInfo& context) override;
	void begin(DrawInfo& context) override {} ///< Link isn't an ImGui item, nothing to begin
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override; ///< No need to set m_internalHover as we handle hovering ourself
	void updateLayout(DrawInfo& context) override;

	bool allowDrawing() override;

	void onDestroy(bool logEvent) override;

protected:
	void initializeDiwne(DrawInfo& context) override;
	void setInitialPositionDiwne() override;

public:
	// Interaction
	// =============================================================================================================
	void onHover(DrawInfo& context) override;

protected:
	bool isHoveredDiwne() override;

public:
	Pin* getStartPin() const
	{
		return m_startPin;
	};
	Pin* getEndPin() const
	{
		return m_endPin;
	};
	/// Get the pin on either end in cases when only one pin is plugged in.
	/// Returns nullptr no pins are connected or if both ends are connected.
	Pin* getSinglePin() const;
	Pin* getAnyPin() const; ///< Get any of the two pins or nullptr if neither are connected
	/// Get the other pin that this link connects with the specified pin.
	/// Assumes the link is connected on both ends
	/// Returns nullptr when neither of the two pins are the passed one.
	Pin* getOtherPin(Pin* pin) const;

	void setStartPin(Pin* pin)
	{
		m_startPin = pin;
	};
	void setEndPin(Pin* pin)
	{
		m_endPin = pin;
	};
	ImVec2 getStartPoint() const
	{
		return m_startDiwne;
	};
	ImVec2 getEndPoint() const
	{
		return m_endDiwne;
	};
	ImVec2 getStartControlPoint() const
	{
		return m_controlPointStartDiwne;
	};
	ImVec2 getEndControlPoint() const
	{
		return m_controlPointEndDiwne;
	};
	void setStartPoint(const ImVec2& mStartDiwne);
	void setEndPoint(const ImVec2& mEndDiwne);

	// TODO: (DR) figure out why this method isn't used (maybe related to allowDrawing?), what is the hiding behavior?
	bool isLinkOnWorkArea() const; /*!< in fact just rectangle (from startPoint to
	                            endPoint) check - so could return true while Link
	                            is not visible */

	bool isPlugged() const;       ///< Whether the link is connected on both ends
	bool isOnePinPlugged() const; ///< Whether the link has exactly one pin plugged in at either end

protected:
	void updateSquareDistanceMouseFromLink();
	virtual void updateEndpoints();
	virtual void updateControlPoints();

	/// Get the rect formed by the starting and ending point, as well as any curve control points.
	/// This method is used in updateLayout() to update the object's rect as well as in allowDrawing().
	virtual ImRect getBounds();
};

} /* namespace DIWNE */
