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

#include "diwne_actions.h"
#include "diwne_common.h"

namespace DIWNE
{
typedef unsigned int ID;

class NodeEditor;

// TODO JH: some attributes should be private/protected // Agreed

/*! \brief Drawing mode of DiwneObject  */
enum DrawMode
{
	JustDraw,   /*!< Draw object only - block all (node editor's) interactions */
	Interacting /*!< Usual mode - draw and allow interaction too */
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

	ImRect m_rect;
	bool m_destroy{false}; ///< Indicates the object is to be deleted

	std::string m_parentLabel; ///< Sets the parent object of object, relevant in hover hierarchy.

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
	bool m_rendered{true}; ///< Whether the object should be rendered

	bool m_interactive{true}; // TODO: "Force" JustDraw DrawMode (implement change of draw mode) <<<<<<<<<<<<<<<<<<<<<<<
	bool m_drawnThisFrame{false};
	bool m_justHidden{false};

	//

	bool m_selectable{true};
	bool m_isHeld{false}; /**< Is object held. Requirement for dragging. When dragged it is still held. */

	bool m_isDragged{false}; /**< Is object dragged */
	bool m_selected{false};  /**< Is object selected */

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

	// TODO: Active is marked true if this is the FIRST object that had m_inner_interaction_happen be true
	bool m_isActive{false}; /**< Something happen with object */

protected:
	bool m_internalHover; ///< Temporary storage for an internal ImGui::IsItemHovered() check
	                      ///< Can be set in the end() method to determine if object is hovered if applicable

public:
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
	virtual void draw(DrawMode drawMode = DrawMode::Interacting);

	/**
	 * Decide whether the object should be drawn (when outside the window for example)
	 */
	virtual bool allowDrawing();

	/**
	 * Internal draw method.
	 * @param context The drawing context of the current frame.
	 * @return A DrawResult object reporting what interaction has occurred during the draw.
	 */
	virtual void drawDiwne(DrawInfo& context, DrawMode drawMode = DrawMode::Interacting);

	// TODO: Docs
	DrawInfo drawDiwneEx(DrawInfo& context, DrawMode drawMode = DrawMode::Interacting);

	// Frame lifecycle/Content methods
	// =============================================================================================================

	// TODO: Wrapper thought: Not every method seemingly needs a wrapper, maybe we could bunch up the "diwne" code into
	//  a few internal methods with more descriptive names rather than to have a "Diwne" suffix method everywhere.

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
	 * This method is called before the afterDraw() method which can use the calculated size values for final drawing.
	 */
	virtual void updateLayout(DrawInfo& context) = 0;

	/**
	 * Called last during drawing.
	 * At this point the m_rect of the object should be calculated in the updateLayout() method and this method should
	 * be able to work with it. Because of that the drawing code within shouldn't affect the objects size anymore.
	 */
	virtual void afterDraw(DrawInfo& context);

	/**
	 * The final method to be called, gets called every frame and doesn't do any drawing.
	 */
	virtual void finalize(DrawInfo& context);

	// Interaction
	// =============================================================================================================

	virtual bool allowInteraction() const; ///< Decide whether the object can interact (not including content elements)
	virtual void processInteractions(DrawInfo& context){};

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

	// TODO: Rename to just getRect()
	//  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	inline virtual ImRect getRectDiwne() const
	{
		return m_rect;
	}
	inline virtual ImVec2 getPosition() const
	{
		return m_rect.Min;
	}
	virtual void setPosition(const ImVec2& position);
	virtual void translate(const ImVec2& vec);

	bool isRendered() const;
	void setRendered(bool mRendered);

	virtual bool setSelected(bool const selected); ///< \return New state of selection
	// TODO: Rename isSelected
	// TODO: Go over m_selected usages and switch to using getter
	virtual bool getSelected() const; ///< Whether the object is selected or not
	virtual void setSelectable(bool const selectable);
	virtual bool getSelectable();

	// INTERACTION
	// =============================================================================================================
	// DiwneObject has a few built-in interactions that derived classes can react to.
	// These are basic actions like the mouse hovering over the object or the object being pressed on.
	//
	// The interaction code consists of four methods.
	// 1. The user method - A callback of sorts that gets triggered when a certain interaction occurs.
	// 2. The trigger method - A method defining a condition that triggers the interaction.
	// 3. The toggle method - A method determining whether the user method should be called or not.
	// 4. Internal implementation method - The internal implementation using the methods above.
	//
	// All except the user method are implemented by default in the DiwneObject.

	// Interaction user methods
	// =============================================================================================================
	// TODO: Rename to onXXX

	virtual void onHover(DrawInfo& context);

	// TODO: Not sure if we need this, can't individual object decide when they want to react to certain input?
	//  virtual void processFocusedForInteraction(DrawInfo& context);

	// TODO: There should be an ABSOLUTE guarantee that dragStart and dragEnd will eventually be true ONCE.
	virtual void onDrag(DrawInfo& context, bool dragStart, bool dragEnd);
	virtual void onPressed(bool justPressed, DrawInfo& context);
	virtual void onReleased(bool justReleased, DrawInfo& context);

	//	virtual void processSelect(DrawInfo& context); // TODO: Renamed to clicked and merge with unselect
	//	virtual void processUnselect(DrawInfo& context);

	// Interaction toggles
	// =============================================================================================================
	virtual bool allowHover() const;

	//	virtual bool allowProcessFocusedForInteraction();
	virtual bool allowPress(const DrawInfo& context) const;
	//	virtual bool allowProcessUnhold();
	virtual bool allowDrag() const; // TODO: Refactor allowProcessDrag()
	                                //	virtual bool allowProcessSelect();
	                                //	virtual bool allowProcessUnselect();
	                                //	virtual bool allowProcessRaisePopup();
	virtual bool allowPopup() const;

protected:
	// Interaction internal processing methods
	// =============================================================================================================

	virtual void processHoverDiwne(DrawInfo& context);

	//	void processFocusedForInteractionDiwne(DrawInfo& context);
	virtual void processPressAndReleaseDiwne(DrawInfo& context);
	//	void processUnholdDiwne(DrawInfo& context);
	virtual void processDragDiwne(DrawInfo& context);

	virtual void processSelectDiwne(DrawInfo& context);
	//	void processUnselectDiwne(DrawInfo& context);
	virtual void processPopup(DrawInfo& context);
	//	virtual void processRaisePopupDiwne(DrawInfo& context); /**< processing raising popup menu */ //TODO: Remove
	//	virtual void processShowPopupDiwne(DrawInfo& context);  /**< processing showing popup menu */ //TODO: Remove

	// =============================================================================================================

	// Trigger methods (Formerly bypass methods)
	// =============================================================================================================

	// TODO: Rename to something clearer, keywords trigger or input.
	//  isXXXTriggered might work, all these should be protected as well

protected:
	virtual bool isHoveredDiwne(); ///< Is mouse hovering over the object? Prerequisite for further interaction.
	virtual bool isDraggedDiwne(); ///< Is mouse dragging the object?

public:
	// TODO: Rename this to something like "action area" / "trigger area"?
	virtual bool bypassFocusForInteractionAction(); /**< action identified as focusing on
	                                                 * object for interacting with it
	                                                 */

	// TODO: Maybe rename to isDownDiwne() or isKeyDownDiwne()
	//  Begs the question which key, well that depends on the impl
	//  The full verbose name is more like isAnyOfTheAllowedKeysPressed()
	/**
	 * Determine whether a key is pressed down over the object.
	 * While the key is down this method should return true until the key is released.
	 * When this method returns true and other conditions are met (hovered, press allowed), the member pressed flag
	 * is set to true and onPress() method is called. If multiple keys make this method return true it is necessary
	 * to distinguish between them later in the implementation if different functionality for each key is desired.
	 * @see When overriding also modify isJustPressedDiwne()
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

	// TODO: Should probably be moved into some util class to keep the DiwneObject concise
	/** \brief Show a colored text for example for immediate hints
	 * \param label is the text to show
	 * \param color is the color of the tooltip
	 */
	void showTooltipLabel(std::string const& label, ImColor const&& color);
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

	std::unique_ptr<DiwneAction> action;

	template <typename T, typename... Args>
	T* setAction(Args&&... args)
	{
		setAction(std::make_unique<T>(std::forward<Args>(args)...));
		return getAction<T>();
	}

	void setAction(std::unique_ptr<DiwneAction> action);

	/// Clears the current action, either right away or at the end of the frame.
	void clearAction(bool immediately = false);
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
		static_assert(std::is_base_of_v<DIWNE::DiwneAction, T>, "T must be derived from DIWNE::DiwneAction class.");
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
	T* getActiveAction(const std::string& source = "")
	{
		static_assert(std::is_base_of_v<DIWNE::DiwneAction, T>, "T must be derived from DIWNE::DiwneAction class.");
		T* ret = dynamic_cast<T*>(action.get());
		if (ret != nullptr && !source.empty())
		{
			if (ret->source != source)
				return nullptr;
		}
		return ret;
	}

	bool dragging{false};
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

	/// Indicates changes in logic, eg. change of some attribute or a non blocking button press. Requests focus.
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
