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

#include <type_traits>
#include <vector>

#include "DiwneObject.h"

namespace DIWNE
{
class Link;
class Node;

class Pin : public DiwneObject
{
public:
	bool m_isInput{false};                 ///< Whether this is an input or output pin, @see isInput()
	bool m_allowMultipleConnections{true}; ///< Only allow one link to be connected, set to false for input pins
	ImRect m_pinRect;                      ///< Rect of the actual pin that can be dragged

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

	// Lifecycle
	// =============================================================================================================
	void initialize(DrawInfo& context) override;
	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override {};
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	// Interaction
	// =============================================================================================================
	void processInteractions(DrawInfo& context) override;
	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	bool allowPopup() const override;
	bool allowDragStart() const override;

	// Link management
	// =============================================================================================================

	/**
	 * Called when the mouse is dragging a new link and is hovering over this pin as well as when it is released.
	 * When the hovering argument is false, the mouse was released and the link should be connected.
	 * The method is responsible for the underlying plug in logic and determining whether the connection is valid.
	 * @param otherPin The other pin, usually the starting pin of the link, but it can be the end pin as well.
	 * @param link The link that's being prepared for plugging
	 * @param hovering True when the mouse is only hovering over the pin and it shouldn't be plugged in yet.
	 * @return
	 */
	virtual bool preparePlug(Pin* otherPin, Link* link, bool hovering);

	/**
	 * Create a new link between this pin and the other pin.
	 * The other pin is considered the start of the link, except when it is an input pin.
	 * Both pins cannot be input pins.
	 * @param otherPin The other pin on the other side of the new link.
	 * @param logEvent The boolean flag passed to onPlug() on successful connect
	 * @return True on success, false otherwise
	 */
	virtual bool plug(Pin* otherPin, bool logEvent = true);

	// Just a trick to avoid accidental use of plug instead of plugLink
	template <typename T>
	bool plug(Pin* otherPin, T logEvent) = delete;

	// TODO: Wrong assertion, the link can have anything on either end!
	/**
	 * Connect this and some other pin together using an existing link.
	 * The other pin is considered the start of the link, except when it is an input pin.
	 * Both pins cannot be input pins.
	 * The link will get unplugged from other pins if necessary.
	 * @param otherPin The other pin on the other side of the link.
	 * @param link
	 * @param logEvent The boolean flag passed to onUnplug() on successful disconnect
	 * @return true
	 */
	virtual bool plugLink(Pin* otherPin, Link* link, bool logEvent = true);

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
	 * Called after the pin has been connected to another pin using a link.
	 * @param otherPin The pin on the other side of the link. Cannot be null.
	 * @param link The link of the new connection with both ends plugged.
	 * @param isStartPin Whether this pin is the start of the connection (otherwise it's the end).
	 * @param logEvent The flag passed to the origin of the action.
	 */
	virtual void onPlug(Pin* otherPin, Link* link, bool isStartPin, bool logEvent = true);

	/**
	 * Called after the link connecting this pin to another is disconnected.
	 * @param otherPin The pin on the other side of the link. Can be null.
	 * @param link The link of the disconnected connection.
	 * @param wasStartPin Whether this pin was the start of the connection (otherwise it was the end).
	 * @param logEvent The flag passed to the origin of the action.
	 */
	virtual void onUnplug(Pin* otherPin, Link* link, bool wasStartPin, bool logEvent = true);

	/**
	 * Method responsible for constructing a new link for this pin.
	 * By default a standard Link object is used,
	 * however this method can be overriden to allow specific pins to use specialized link subclasses.
	 * @return The new Link instance, it is already registered with the current editor and has no connections yet.
	 */
	virtual std::shared_ptr<Link> createLink();

	Link* getLink(size_t index = 0);

	bool isPlugged() const;
	bool connectionChanged() const;

	/**
	 * A condition for starting and receiving a link connection.
	 * Can be used to specify an area where the pin can be dragged from or a link dropped at.
	 */
	virtual bool allowConnection() const;

	virtual void setConnectionPointDiwne(const ImVec2 value);
	virtual const ImVec2& getConnectionPoint();

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

protected:
	virtual void updateConnectionPoint();

public:
	// Getters
	// =============================================================================================================
	/**
	 * Whether this is an input or output pin.
	 * Input pins are restricted to only accept at most one connection.
	 * They also force links to always end in them, meaning a link can't begin in an input pin.
	 * Output pins have no such restrictions. They can be on either end of a link and have any amount of connections,
	 * when m_allowMultipleConnections is enabled. For input pins that flag is forced to false.
	 * @return
	 */
	bool isInput() const;

	template <typename T = Node>
	T* getNode()
	{
		static_assert(std::is_base_of<DIWNE::Node, T>::value);
		return static_cast<T*>(m_node);
	}
	Node* getNode();
};

} /* namespace DIWNE */
