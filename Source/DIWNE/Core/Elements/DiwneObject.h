/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \date 20.3.2022
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * Declaration of abstract object from which all other object inherit
 */
#pragma once

#include <any>
#include <memory>
#include <string>
#include <type_traits>

#include "DIWNE/Core/diwne_actions.h"
#include "DIWNE/Core/diwne_common.h"

namespace DIWNE
{
typedef unsigned int ID;

class NodeEditor;

// TODO JH: some attributes should be private/protected // Agreed

/// Drawing mode flags of a DiwneObject, realized by the DrawMode_ enum containing bit flags.
/// Is just an alias for an int to allow easy bit operations without explicit conversion.
using DrawMode = int;
enum DrawMode_ : int
{
	DrawMode_Interactive = 0,    // Draw and react to input, the default behavior
	DrawMode_JustDraw = 1 << 0,  // Draw only, interactions should be blocked
	DrawMode_ForceDraw = 1 << 1, // Force the drawing of the object to obtain size information off screen

	// TODO: Remove DrawAtPosition, cursor can be set manually before each draw, everything should be drawn at the
	// cursor by default. This unique caveat of Node drawing was moved into NodeEditor drawing code
	//  DrawMode_DrawAtPosition = 1 << 2, // Draw the object at it's last position rather than the ImGui cursor
};

class DrawInfo;

/**
 * The base class for all DIWNE elements containing common functionality.
 */
class DiwneObject : public std::enable_shared_from_this<DiwneObject>
{
public:
	// TODO: Might be better to have a pointer here, imagine cases where nodes are moved between editors / copy / move
	DIWNE::NodeEditor& diwne; /**< Every object have access to Diwne - is used for share information if needed */
	// TODO: Do we need id and label to be separate?
	//  Pros: Easy lookup / comparsion using unique ID
	//  Cons: Somewhat confusing? Both label and id are UNIQUE identifiers
	//   label includes ID at the end which isn't too clear
	//  Perhaps keep this and make clear in docs
	static unsigned long long g_diwneIDCounter;
	DIWNE::ID m_idDiwne;      /**< Used for creating ImGui id/labels */
	std::string m_labelDiwne; /**< Used for identifying object and creating ImGui id/labels */
	// TODO: Do we need to retain this as a member variable? Its just constant prefix + label
	std::string m_popupIDDiwne; /**< Used for identifying what element raise popup */

	/**
	 * Rectangle bounds of the object in diwne coordinates (editor canvas coordinates).
	 * The top left corner of the rectangle (.Min) represents the objects position.
	 * Dimensions are the differences between the .Min and .Max (bottom right corner) coordinates.
	 * <br>
	 * The objects rectangle gets updated every frame by the updateLayout() method.
	 * DiwneObjects are drawn at the position of the ImGui cursor, but the position of the object can be set
	 * and then used to manually set the ImGui cursor position, which is how Nodes are drawn by the NodeEditor.
	 * @see updateLayout()
	 */
	ImRect m_rect;
	bool m_destroy{false};  ///< Indicates the object is to be deleted (and deallocated)
	bool m_deletable{true}; ///< Whether the object can be deleted by the user

	// TODO: (DR) Implement (true for most inner nodes, like in sequences)
	//  In theory drag operations could affect sequences but ONLY when only same sequence ndoes are selected
	bool m_fixed{false}; ///< Whether the object can be moved by user operations @see isFixed()

	//

	// TODO: Not sure if its needed to keep this as a local variable
	//  maybe its enough to pass around the frame context instead
	//	DrawMode m_drawMode; /**< \see enum DrawMode */

	/// Experimental, read-only flag thats updated on each drawDiwne() call.
	/// Essentially just a way to avoid passing this along everywhere as it should be mostly constant
	DrawMode m_drawMode2; // TODO: Rename, experimental (!)

	//

	// TODO: Workspace nodes use a LOD system, it might be a good idea to generalize that system into the DIWNE library
	//  itself. The LOD system is also, coincidentally, useful as a true LOD system to improve performance (despite the
	//  name meaning something else originally) For higher zooms the need of a LOD object simplification system is
	//  rather big.
	//  As for the m_rendered flag below, it could be replaced with a LOD level like "Hidden" or "DoNotRender".
	//  Although that might be unnecessary as a basic rendered boolean flag is probably useful anyway.

	/// Whether the object should be drawn, this is a general flag that overrides behavior of the allowDrawing() method.
	bool m_rendered{true};

	bool m_interactive{true}; // TODO: "Force" JustDraw DrawMode (implement change of draw mode) <<<<<<<<<<<<<<<<<<<<<<<
	bool m_drawnThisFrame{false};
	bool m_justHidden{false};

	//

protected:
	bool m_selectable{true}; ///< Should not be accessed directly. @see setSelectable()
	bool m_selected{false};  ///< Should not be accessed directly. @see setSelected()

	/// Sets the parent object of object, relevant in node container and hover hierarchy.$
	DiwneObject* m_parentObject{nullptr};

public:
	/**
	 * Is the object pressed down (held)? Requirement for dragging. When dragged it is still pressed.
	 * This flag becomes valid since the processInteractions() lifecycle step.
	 * @see onPressed()
	 */
	bool m_isPressed{false};
	/**
	 * Read only flag indicating the object was pressed this frame (eg. onPressed() with justPressed=true was called)
	 * This flag becomes valid since the processInteractions() lifecycle step.
	 * @see onPressed()
	 */
	bool m_justPressed{false}; ///< Is object just pressed? Eg. was the button pressed down this frame.

	/**
	 * Read only flag indicating the object was released this frame (eg. onReleased() with justReleased true was called)
	 * This flag becomes valid since the processInteractions() lifecycle step.
	 * @see onReleased()
	 */
	bool m_justReleased{false}; ///< Is object just released? Eg. was the button released this frame.

	bool m_isDragged{false}; /**< Is object dragged */
	bool m_draggable{true};  ///< Whether dragging of the object is allowed by default

	// TODO: This flag is oddly named, realistically this means whether we are hovered AND at the same time
	//  hovered in some special area. This area is generally just the header of a node and this tells us when to
	//  drag the node.
	//  Eg we are hovered in some kind of an "active" / "interactable" area
	/**< Is object focus on area that allow interaction with object */
	//	bool m_focusedForInteraction{false};
	// TODO: Reimplement focused for interaction (namely enable dragging by header only)

	/**< Is object focused anywhere (and for example can not be focus other underlying object) */ // TODO: Rename to
	                                                                                              // hover / Rework
	bool m_hovered{false};
	bool m_hoverRoot{false}; ///< Whether hovering this object should prevent other objects from hovering
	bool m_hoverable{true};  ///< Whether hovering is enabled by default

protected:
	bool m_internalHover; ///< Temporary storage for an internal ImGui::IsItemHovered() check
	                      ///< Can be set in the end() method to determine if object is hovered if applicable

public:
	//
	// TODO: Review if these flags are needed (they shouldnt be with the new context impl)
	//  Rework so that DrawInfo context.inputConsumed is used instead
	//  boolean returned from drawing code that is set as a member variable. Means that some part of the drawing code
	//  returned true (eg. a button/slider was clicked) Related to m_isActive
	/**
	 * If some interaction happen with inner elements (DragFloat, Button, ...)
	 * it block interaction with this object
	 */
	bool m_inner_interaction_happen{false};

	// TODO: (DR) Review if this flag is used, it got kinda overtaken by the DrawInfo::logical/focusUpdate thing
	// TODO: Active is marked true if this is the FIRST object that had m_inner_interaction_happen be true
	bool m_isActive{false}; /**< Something happen with object */

	/**
	 * \param diwne is node editor object that this object belongs to
	 * \param id used to identification
	 * \param labelDiwne used to identification
	 */
	DiwneObject(DIWNE::NodeEditor& diwne, std::string const labelDiwne);
	virtual ~DiwneObject() = default;

	/**
	 * Generic draw method meant to be used by external code.
	 * Note that some objects are meant to only by drawn using the internal drawDiwne() method instead.
	 * @param drawMode Drawing mode
	 * @see drawDiwne()
	 */
	virtual void draw(DrawMode drawMode = DrawMode_Interactive);

	/**
	 * Decide whether the object should be drawn (when outside the window for example)
	 */
	virtual bool allowDrawing();

	/**
	 * Internal draw method.
	 * @param context The drawing context of the current frame.
	 * @param drawMode The drawing mode of this draw.
	 * @see @ref diwne_lifecycle
	 */
	virtual void drawDiwne(DrawInfo& context, DrawMode drawMode = DrawMode_Interactive);

	// TODO: Docs
	DrawInfo drawDiwneEx(DrawInfo& context, DrawMode drawMode = DrawMode_Interactive);

	// Frame lifecycle/Content methods
	// ============================================================================================================
	/// @page diwne_lifecycle DIWNE Object Lifecycle
	/// Each DIWNE object is drawn using the drawDiwne() method (the draw method). This method is responsible
	/// for drawing the object as well as reacting to any user input as it is equivalent to usual Dear ImGui draw calls.
	/// The draw method is divided into several stages which should be overriden in derived objects.
	///
	/// # Lifecycle stages
	/// There are two methods which are always called: the initialize() and finalize() methods.
	/// Other methods are only called if the object is truly visible on the screen, eg. the allowDrawing() method
	/// returns true.
	///
	/// # DrawInfo context and DrawMode
	/// TODO: Docs
	///

	/**
	 * First method to be called every frame. Does not handle drawing.
	 */
	virtual void initialize(DrawInfo& context);

	/**
	 * First method to be called during object drawing.
	 * Can be used to initialize drawing code.
	 */
	virtual void begin(DrawInfo& context) = 0;

	/**
	 * Called after begin() during drawing. Draws object content.
	 */
	virtual void content(DrawInfo& context) = 0;

	/**
	 * Called after content(). Used to end content drawing.
	 */
	virtual void end(DrawInfo& context) = 0;

	/**
	 * Update objects m_rect and any other object size variables.
	 * In other words this method is responsible for keeping track of the objects size.
	 * This method is called after end() and before the afterDraw() method which can use the calculated size values
	 * for final drawing.
	 */
	virtual void updateLayout(DrawInfo& context) = 0;

	/**
	 * @brief Method for reacting to user input after the object is fully drawn and its dimensions are known.
	 * It is called after end() and updateLayout(),
	 * but before the afterDraw() lifecycle method. Internal interactions are processed right before.
	 */
	virtual void processInteractions(DrawInfo& context){};

	/**
	 * Called last during drawing.
	 * At this point the m_rect of the object should be calculated in the updateLayout() method and this method should
	 * be able to work with it. Because of that the drawing code within shouldn't affect the objects size anymore.
	 * Interactions are processed before this method and thus interaction related member variables can be used within.
	 */
	virtual void afterDraw(DrawInfo& context);

	/**
	 * The final method to be called, gets called every frame and doesn't do any drawing.
	 */
	virtual void finalize(DrawInfo& context);

	// Popups
	// =============================================================================================================
	virtual void popupContent(DrawInfo& context); ///< Content of popup menu raised on this object

protected:
	// Internal implementation methods, can still be overriden if needed
	// =============================================================================================================
	virtual void initializeDiwne(DrawInfo& context);
	virtual void beginDiwne(DrawInfo& context);
	virtual void endDiwne(DrawInfo& context);
	virtual void afterDrawDiwne(DrawInfo& context);
	virtual void finalizeDiwne(DrawInfo& context);
	virtual void processInteractionsDiwne(DrawInfo& context);

public:
	// =============================================================================================================

	// TODO: Add detailed docs to the main lifecycle functions (begin, content, end)

	// TODO: + Solid processInteraction docs!
	//  We should explain the "duality" of interaction, specifically that ImGui interactions and diwne interactions
	//  are slightly different in the sense that ImGui interactions are handled directly in the drawing methods
	//  whether "DIWNE interactions" moreso means an extra step after constructing/drawing an object that checks for
	//  any other non-imgui interactions like DIWNE object hovering/dragging/selecting etc.
	//  For what its worth all of those things could happen during object construction/draw, rather than in this
	//  separate "processInteractions" step, but often these DIWNE interactions need to know the dimensions of the
	//  drawn object, which we only know at the end of drawing (in the endDiwne() method).
	//  endDiwne method is the one that dispatches processInteractions AFTER the user end() method is called.

	/**
	 * Marks the object for lazy deletion.
	 * When the object is deleted and what deletion even means depends on the particular use case.
	 * For example deletion of nodes and links is handled by the NodeEditor in the next frame.
	 * @param logEvent The boolean flag passed to onDestroy(), can be used to determine where was destroy() called from.
	 */
	virtual void destroy(bool logEvent = true);
	// TODO: Implement this, im still not sure if this should be called on the delete marking or the actual removal
	//  Probably when marking as thats what the old Node::deleteAction() method did.
	virtual void onDestroy(bool logEvent);
	bool isDestroyed()
	{
		return m_destroy;
	}

	// TODO: Rename to just getRect()
	//  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	virtual ImRect getRectDiwne() const
	{
		return m_rect;
	}
	virtual ImVec2 getPosition() const
	{
		return m_rect.Min;
	}
	virtual void setPosition(const ImVec2& position);
	virtual void translate(const ImVec2& vec);

	/**
	 * Set the selection state of the object.
	 * This method can be used to select the object.
	 * If the selection state is changed by the call the onSelection() method is invoked.
	 * @return The new state of selection
	 * @see setSelectable()
	 */
	virtual bool setSelected(bool selected); ///< \return New state of selection
	// TODO: Rename isSelected
	// TODO: Go over m_selected usages and switch to using getter
	virtual bool getSelected() const; ///< Whether the object is selected or not

	/**
	 * Whether the object can be selected. When this is set to true the object cannot be selected anymore.
	 * Although it may remain selected if it was selected beforehand.
	 */
	virtual void setSelectable(bool selectable);
	virtual bool getSelectable();

	bool isRendered() const;
	void setRendered(bool val);

	/**
	 * Fixed objects are ones rendered at a position determined by some other object.
	 * In such a case their position is "read only" as it will get overwritten.
	 * Top level nodes aren't fixed, as the editor adjust the ImGui cursor to their position before drawing.
	 * Objects drawn inside other objects will usually be fixed.
	 */
	bool isFixed() const;
	void setFixed(bool val);

	// Interactions
	// =============================================================================================================
	/// @page DIWNE Interactions
	/// DiwneObject has a few built-in interactions that derived classes can react to.
	/// These are basic actions like the mouse hovering over the object or the object being pressed on with the mouse.
	/// As well as more complicated actions like dragging, selection and raising a popup over the object.
	/// \n\n
	/// The interaction code consists of four methods. XXX being the interaction name.\n
	/// 1. The user method (onXXX) - A callback of sorts that gets triggered when a certain interaction occurs.\n
	/// 2. The trigger method (isXXXDiwne) - A method defining a condition that triggers the interaction.\n
	/// 3. The toggle method (allowXXX) - A method determining whether the type of interaction should be processed.\n
	/// 4. Internal implementation method (processXXXDiwne) - The internal implementation using the methods above.
	///   													  This method calls the user method when the interaction
	/// 													  is allowed and triggered.\n
	/// \n
	/// All except the user method are implemented by default in the DiwneObject.\n\n
	/// DiwneObject interactions and their default behavior:\n
	/// - Hover - The mouse hovering over the object\n
	/// - Press and release - The left mouse button being held down while hovering above the object\n
	/// - Drag - The left mouse button being pressed and dragged awa\n
	/// - SelectOnClick - The left mouse button was released over the object (while not dragging)\n
	/// - Popup - Popup is triggered by releasing right click over the object\n
	/// \n
	/// These interactions are special states used for built in interaction in the node editor.
	/// Custom interactions reacting to different inputs can be implemented in derived classes and resolved in the
	/// DiwneObject::processInteractions() method which is called after resolving the built in interactions above.

	// Interaction user methods (callbacks)
	// =============================================================================================================

	/**
	 * This method gets called when the object is hovered.
	 * This generally indicates mouse hover, but the behavior can be changed via isHoveredDiwne().
	 * @param context
	 * @see processHoverDiwne()
	 */
	virtual void onHover(DrawInfo& context);

	/**
	 * @brief This method gets called when the object is being dragged.
	 *
	 * Drag begins when the object is pressed and isDraggedDiwne() returns true.\n
	 * A call with dragStart true, should always be followed by a call with dragEnd true.
	 * @param dragStart The drag has just started this frame.
	 * @param dragEnd The drag is ending this frame.
	 * @see processDragDiwne(), processPressAndReleaseDiwne()
	 */
	virtual void onDrag(DrawInfo& context, bool dragStart, bool dragEnd);

	/**
	 * Called when the object is pressed, meaning a key is pressed over it.
	 * Which key or keys trigger this state is determined by is isPressedDiwne() and isJustPressedDiwne().
	 * An object is allowed to be pressed only when allowPress() returns true.
	 *
	 * @note This isn't a general method for reacting to any input. This method signals a specific "pressed" or "held"
	 * state of the object which is only triggered for certain keys specified by isPressedDiwne().
	 * By default this means being "pressed by the left mouse button", but this behavior can vary in subclasses.
	 *
	 * @param justPressed True on the frame of the actual press, false otherwise when the key is held down.
	 * @see isPressedDiwne(), processPressAndReleaseDiwne()
	 */
	virtual void onPressed(bool justPressed, DrawInfo& context);

	/**
	 * Called when the object is not pressed. Every frame either this or the onPressed() method is called.
	 * @param justPressed True on the frame the object is released (becomes no longer pressed)
	 * @see onPressed()
	 */
	virtual void onReleased(bool justReleased, DrawInfo& context);
	virtual void onSelection(bool selected);

	// Interaction toggles
	// =============================================================================================================

	/**
	 * Decide whether the object should react to user input in general.
	 * Dear ImGui components might require special handling.
	 */
	virtual bool allowInteraction() const;

	/**
	 * Determines whether the object can be hovered.
	 * Is always allowed by default.
	 * @see isHoveredDiwne()
	 */
	virtual bool allowHover() const;

	/**
	 * Determines whether the object can be pressed.
	 * By default this requires the object to be hovered, input not being consumed and nothing else being dragged.
	 * @see isPressedDiwne(), allowHover(), DrawInfo::inputConsumed, InteractionState::dragging
	 */
	virtual bool allowPress(const DrawInfo& context) const;

	/**
	 * Determines whether a drag operation can start from this object.
	 * By default the only requirement is the object to be pressed.
	 * @see isDraggedDiwne(), allowPress()
	 */
	virtual bool allowDragStart() const;

	/**
	 * Determines whether a popup can be raised over the object.
	 * Allowed by default.
	 * @see processPopupDiwne()
	 */
	virtual bool allowPopup() const;

	/**
	 * Determines whether the object can be selected by clicking it (eg. pressing and then releasing it).
	 * This is a type of interaction and not related to whether the object is "selectable" or not.
	 * Programmatical selection using DiwneObject::setSelected() is not affected.
	 * @see processSelectDiwne()
	 */
	virtual bool allowSelectOnClick(const DrawInfo& context) const;

protected:
	// Interaction internal processing methods
	// =============================================================================================================

	virtual void processHoverDiwne(DrawInfo& context);

	/**
	 * Processes whether the object is currently pressed (held) and further if it was just pressed or released.
	 * By default, being pressed means that the mouse is pressed down when hovering over it.
	 * The first frame of the press is considered just pressed, same for the last with just released.
	 * <br><br>
	 * What key or keys trigger the pressed state depends on the implementation of isPressedDiwne()
	 * and isJustPressedDiwne().
	 * <br><br>
	 * Regarding clicks or key presses:
	 * The concept of a "click" is often ambiguous. In ImGui it generally means the moment the mouse is just pressed.
	 * However clicks are more traditionally considered to be a rapid sequence of press and release.
	 * In DIWNE the click terminology isn't really used, an equivalent would be just released whilist *not* dragging.
	 * Meaning if you want to react to a click put an if (!context.state.dragging) in the onReleased() callback.
	 */
	virtual void processPressAndReleaseDiwne(DrawInfo& context);

	virtual void processDragDiwne(DrawInfo& context);

	/**
	 * Processes whether a popup should be opened over this object. If it is, the popupContent() method is called.
	 * @param context
	 */
	virtual void processPopupDiwne(DrawInfo& context);

	/**
	 * Processes any interactions related to whether the object should be selected.
	 * The default behavior is selection when the object is pressed and then released (clicked).
	 * @note This is not the only method which can select the object.
	 * @param context
	 * @return Whether selection processing should end. Can be used to exit early from derived methods.
	 * @see processPressAndReleaseDiwne()
	 */
	virtual bool processSelectDiwne(DrawInfo& context);

public:
	// Trigger methods (Formerly bypass methods)
	// =============================================================================================================
	// TODO: Rename to something clearer, keywords trigger or input.
	//  isXXXTriggered might work, all these should be protected as well

	virtual bool isHoveredDiwne(); ///< Is mouse hovering over the object? Prerequisite for further interaction.
	virtual bool isDraggedDiwne(); ///< Is mouse dragging the object?

	// TODO: Maybe rename to isDownDiwne() or isKeyDownDiwne()
	//  Begs the question which key, well that depends on the impl
	//  The full verbose name is more like isAnyOfTheAllowedKeysPressed()
	/**
	 * @brief Determine whether a key is pressed down over the object.
	 *
	 * While the key is down this method should return true until the key is released.<br>
	 * When this method returns true and other conditions are met (hovered, press allowed), the object pressed flag
	 * is set to true and onPress() method is called.<br>
	 * If multiple keys make this method return true it is necessary to distinguish between them later
	 * in the implementation if different functionality for each key is desired.
	 * @see When overriding also modify isJustPressedDiwne()
	 * @note Overriding the pressed state behavior will modify the dragging and selection behavior.
	 */
	virtual bool isPressedDiwne();

	/**
	 * Determine whether a key was just pressed. For the press action to be carried out this method must return
	 * true at least once during the key press and release cycle. This method is needed to avoid starting the press and
	 * release cycle when the key was not pressed initially over this object (Dragging pressed mouse over the object).
	 * @see isPressedDiwne()
	 */
	virtual bool isJustPressedDiwne();

	virtual bool bypassPressAction();    // TODO: Remove probably
	virtual bool bypassReleaseAction();  // TODO: Remove probably
	virtual bool bypassSelectAction();   /**< action used for selecting object */
	virtual bool bypassUnselectAction(); /**< action used for unselecting object */
	virtual bool bypassTouchAction();    /**< action used for touching object - not interact with
	                                        it, just move it to front of other objects */

	// Miscellaneous
	// =============================================================================================================
public:
	bool isDragging(DrawInfo& context);
	void stopDrag(DrawInfo& context);

	// =============================================================================================================
	// END OF INTERACTION

public:
	inline DIWNE::ID const getId() const
	{
		return m_idDiwne;
	};


	DiwneObject* getParentObject() const;
	void setParentObject(DiwneObject* parent);

	bool isChildObject() const;

	bool isChildOfObject(DiwneObject* parent)
	{
		for (DiwneObject* o = this->m_parentObject; o; o = o->m_parentObject)
		{
			if (o == parent)
				return true;
		}
		return false;
	}

	bool isAnyParentSelected()
	{
		for (DiwneObject* o = this->m_parentObject; o; o = o->m_parentObject)
		{
			if (o->getSelected())
				return true;
		}
		return false;
	}

	/**
	 * Returns a shared pointer to this object.
	 * All DiwneObjects are assumed to be stored somewhere as a shared pointer.
	 * This can be used to retrieve an owning shared pointer from a "weak" raw pointer to a DiwneObject.
	 * @tparam T The pointer will be statically cast to this type. Ensure the cast won't fail.
	 * 			 There are runtime checks in debug mode.
	 */
	template <typename T = DiwneObject>
	std::shared_ptr<T> sharedPtr()
	{
		static_assert(std::is_base_of<DiwneObject, T>::value, "T must be derived from DIWNE::DiwneObject!");
		std::shared_ptr<DiwneObject> sharedPtr = shared_from_this();
#ifndef NDEBUG
		if (std::dynamic_pointer_cast<T>(shared_from_this()) == nullptr)
			assert(!"Invalid static pointer cast!");
#endif
		return std::static_pointer_cast<T>(sharedPtr);
	}

	/**
	 * Returns a raw pointer to itself casted to a type.
	 * Useful as a shorthand for casting to derived object type.
	 * @tparam T The pointer will be statically cast to this type. Ensure the cast won't fail.
	 * 			 There are runtime checks in debug mode.
	 */
	template <typename T = DiwneObject>
	T* as()
	{
		static_assert(std::is_base_of<DiwneObject, T>::value, "T must be derived from DIWNE::DiwneObject!");
#ifndef NDEBUG
		if (dynamic_cast<T*>(this) == nullptr)
			assert(!"Invalid static pointer cast!");
#endif
		return static_cast<T*>(this);
	}

	// TODO: Should probably be moved into some util class to keep the DiwneObject concise
	/** \brief Show a colored text for example for immediate hints
	 * \param label is the text to show
	 * \param color is the color of the tooltip
	 */
	void showTooltipLabel(const std::string& label, const ImColor&& color);

private:
	void debugDrawing(DrawInfo& context, int debug_logicalUpdate); ///< Debug drawing helper
};

/*
 * A persistent state storage holding information about multi-frame interactions.
 * Each NodeEditor holds an instance of this state and sets a reference to it in the non-persistent DrawInfo context.
 * By persistence we mean persistence across multiple frames.
 */
class InteractionState
{
public:
	// TODO: Stuff below this is more persistent than above, and cannot really be "versioned" with the short counter.
	//  Meaning finding the difference between two DrawInfos becomes a little meaningless
	//  Thus it would be a good argument to wrap the stuff below into a different struct and keep that as a NodeEditor
	//  member or keep a pointer to it in the context and just pass that along
	//  (would help with copying performance as well)(although I don't particularly consider that to be an issue anyway)

	// TODO: Write docs <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Active action idea, we have a descriptor for the current "action", it has a source
	// The basic handling would be, if I am not the source of the action, ignore it / restrict functionality
	// If I am the source, then I know this action is mind and I am later responsible for ending it (ex. drag n drop)

	//	std::string action;
	//	std::string actionSource;
	//	std::any actionData;
	//	bool clearActionThisFrame{false};

	std::unique_ptr<Actions::DiwneAction> action;

	// TODO: Maybe rename to createAction <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	template <typename T, typename... Args>
	T* startAction(Args&&... args)
	{
		Actions::DiwneAction* a = startAction(std::make_unique<T>(std::forward<Args>(args)...));
		if (a)
			return static_cast<T*>(a);
		return nullptr;
	}

	Actions::DiwneAction* startAction(std::unique_ptr<Actions::DiwneAction> action);

	/// Clears the current action, either right away or at the end of the frame.
	void endAction(bool immediately = false);

	// TODO: (DR) Resolve the dual access to actions via <T> and std::string name
	//  Querying via type is cleaner as we can create actions that way
	//  But it then might require a dynamic cast or something
	bool isActionActive(const std::string& name);
	/// Checks whether an action is active and its source is the passed source diwne label
	bool isActionActive(const std::string& name, const std::string& source);
	bool anyActionActive();

	/**
	 * Returns the current action casted to a specific type.
	 * This method ASSUMES that we already know the action type ahead by calling isActionActive()!
	 */
	template <typename T>
	T* getAction()
	{
		static_assert(std::is_base_of<DIWNE::Actions::DiwneAction, T>::value,
		              "T must be derived from DIWNE::DiwneAction class.");
#ifndef NDEBUG
		if (action && dynamic_cast<T*>(action.get()) == nullptr)
			assert(!"Invalid static cast!");
#endif
		return static_cast<T*>(action.get());
	}

	/**
	 * Can be used to check if an action is active and return it if so.
	 * @return Returns the specified action IF if it currently active. Otherwise null.
	 */
	template <typename T>
	T* getActiveAction(const std::string& name = "", const std::string& source = "")
	{
		static_assert(std::is_base_of<DIWNE::Actions::DiwneAction, T>::value,
		              "T must be derived from DIWNE::DiwneAction class.");
		T* ret = dynamic_cast<T*>(action.get()); // TODO: Avoid this dyn cast, see TODO above in action access duality
		if (ret != nullptr)
		{
			if (!name.empty() && ret->name != name)
				return nullptr;
			if (!source.empty() && ret->source != source)
				return nullptr;
		}
		return ret;
	}

	bool dragging{false};
	bool dragStart{false};
	bool dragEnd{false};
	std::string dragSource; // TODO: If the above was changed, then this can be the source of the action
	                        // TODO: Question then arises what if there are multiple active actions? Do we make those
	                        // two above arrays? vectors?

	// TODO: Docs to explain hover root and object parent references
	/**
	 * Used to specify which object can be handed hover when a hovered child object isn't a hover root.
	 */
	std::string hoverTarget;

	/**
	 * Some operations in the context are queued to be performed at the end of the frame / beginning of the next one.
	 * This method performs those actions.
	 */
	void nextFrame();
};

/**
 * A structure used to relay information between DIWNE objects during their sequential construction/drawing
 * in a single frame.
 * This is mainly used to retain information about whether an object should react to user input or if a diwne object
 * drawn earlier already "captured" the input and other objects should ignore it.
 * An instance of this object is passed along the drawing code for any nested diwne objects and along the way it
 * collects information about what has already happened that frame.
 * A reference to the persistent interaction state is also passed along to simplify access to it.
 */
class DrawInfo
{
public:
	DrawInfo(InteractionState& state);

	// clang-format off
	// TODO: Make actual fields private and add const getters.

	/// Number of purely visual interactions that are occurring. This can be a simple mouse hover.
	unsigned short visualUpdates{0};
	void visualUpdate();

	/**
	 * Indicates a logical interaction, eg. change of some attribute or a non blocking button press. Requests focus.
	 * This can be used to detect whether an object is being clicked on directly, in which case the number of logical
	 * updates should be 0. An object above another should submit a logical update in its onPressed() method.
	 */
	unsigned short logicalUpdates{0};
	void logicalUpdate(bool isVisualUpdateAsWell = true);

	/// Whether input has been captured by an object previously and should not be reacted to anymore.
	unsigned short inputConsumed{0};
	inline void consumeInput() { inputConsumed++; }

	/// Whether objects should not be hovered anymore // TODO: This is currently unused, but potentially useful
	unsigned short hoverConsumed{0};
	inline void consumeHover() { hoverConsumed++; }

	unsigned short popupOpened{0};
	inline void popup() { popupOpened++; }

	// clang-format on

	InteractionState& state;

	/// Composite update method
	void update(bool visual, bool logical = false, bool blockInput = false);

	/**
	 * Find the difference between two contexts. This relates primarily to the few trivial unsigned short variables.
	 * This is used to find the immediate change of the context between two points in time.
	 * Which is useful to figure out how a certain draw call changed the context.
	 * @param other The other, older, context to compare with.
	 * @return New context representing the only change between this context and the other one.
	 */
	DrawInfo findChange(const DrawInfo& other) const;
};

/**
 * A helper class used to track changes in DrawInfo context between two points in time.
 * Used to determine changes a specific DiwneObject::drawDiwne call made.
 */
class ContextTracker
{
public:
	std::shared_ptr<DrawInfo> m_contextCopy;
	ContextTracker(const DrawInfo& context);
	void begin(const DrawInfo& context);
	DrawInfo end(const DrawInfo& context);
};

} // namespace DIWNE
