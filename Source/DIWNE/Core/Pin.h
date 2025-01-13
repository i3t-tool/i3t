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

#include <vector>

#include "DiwneObject.h"

namespace DIWNE
{
class Link;
class Node;

class Pin : public DiwneObject
{
public:
	bool m_isInput{true};                   ///< Whether this is an input or output pin
	bool m_allowMultipleConnections{false}; ///< Only allow one link to be connected, set to true for input pins
	ImRect m_pinRect;                       ///< Rect of the actual pin that can be dragged

	Node* m_node;               ///< Node this pin belongs to
	std::vector<Link*> m_links; ///< Links connected to this pin

	// TODO: Sort this out <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  might be related to that updateDataWidth issue with sequence matrix inputs
	bool m_connectionChanged; ///< Flag indicating that the pin has been plugged or unplugged this frame
	                          ///< Note: This flag gets reset the moment the pin is drawn, which is kinda dumb,
	                          ///< don't rely on this too much, it is currently used for a rather specific purpose

protected:
	ImVec2 m_connectionPoint{ImVec2(0, 0)}; ///< Point where a link connects to this pin

public:
	Pin(NodeEditor& diwne, Node* node, bool isInput, std::string labelDiwne = "DiwnePin");

	bool isInput() const;

	void initialize(DrawInfo& context) override;
	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override{};
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

	void processInteractions(DrawInfo& context) override;
	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	/**
	 * Called when the mouse is hovering over the pin and when it is actually released over the pin.
	 * The method is responsible for the underlying plug in logic and determining whether the connection is valid.
	 * @param hovering True when the mouse is only hovering and the pin shouldn't be plugged in yet.
	 */
	virtual bool tryPlug(Pin* startPin, Link* link, bool hovering);

	/**
	 * Create and plug in a link between the startNode and this node.
	 * @param logEvent The boolean flag passed to onPlug() on successful connect
	 * @return True on success, false otherwise
	 */
	virtual bool plug(Pin* startPin, bool logEvent = true);

	// TODO: Unplug specific links
	/**
	 * Unplug all links from this pin.
	 * @param logEvent The boolean flag passed to onUnplug() on successful disconnect
	 * @param deleteLinks Whether to also destroy the links after unplugging.
	 * @return True on success, false otherwise
	 */
	virtual bool unplug(bool logEvent = true, bool deleteLinks = true);

	// Callbacks
	/**
	 * Called when a link is plugged into the pin.
	 * @param otherPin The pin on the other side of the link.
	 */
	virtual void onPlug(Pin* otherPin, bool logEvent = true);
	// TODO: Which link was unplugged?
	virtual void onUnplug(bool logEvent = true); ///< Called when the pin is unplugged.

	/**
	 * Method responsible for constructing a new link for this pin.
	 * By default a standard Link object is used,
	 * however this method can be overriden to allow specific pins to use specialized link subclasses.
	 * @return The new Link instance, it is already registered with the current editor and has no connections yet.
	 */
	virtual std::shared_ptr<Link> createLink();

	/**
	 * Plug two pins together using an existing link.
	 * The link will get unplugged from other pins if necessary.
	 * @param startPin
	 * @param link
	 * @param logEvent The boolean flag passed to onUnplug() on successful disconnect
	 * @return true
	 */
	virtual bool plugLink(Pin* startPin, Link* link, bool logEvent);

public:
	bool isPlugged() const;
	bool connectionChanged() const;
	/**
	 * A condition for starting and receiving a link connection.
	 * Can be used to specify an area where the pin can be dragged from or a link dropped at.
	 */
	virtual bool allowConnection() const;

	bool allowPopup() const override;
	bool allowDragStart() const override;

	template <typename T = Node>
	T* getNode()
	{
		static_assert(std::is_base_of_v<DIWNE::Node, T>);
		return static_cast<T*>(m_node);
	}
	Node* getNode();
	Link* getLink(size_t index = 0);

	/**
	 * Adds the link to the pin's list of links
	 * @return true if the link was added, false if it already existed
	 */
	bool registerLink(Link* link);

	/**
	 * Removes the link from the pin's list of links
	 * @return true if removed, false if it didn't exist
	 */
	bool unregisterLink(Link* link);

	virtual void setConnectionPointDiwne(const ImVec2 value);
	virtual const ImVec2& getConnectionPoint();
	void onDestroy(bool logEvent) override;

protected:
	virtual void updateConnectionPoint();
};

} /* namespace DIWNE */
