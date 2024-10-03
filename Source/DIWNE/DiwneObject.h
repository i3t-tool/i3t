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

#include <memory>
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "diwne_common.h"

namespace DIWNE
{
typedef unsigned int ID;

class NodeEditor;

enum DiwneAction : unsigned int;

/*! \brief Drawing mode of DiwneObject  */
enum DrawMode
{
	JustDraw,   /*!< Draw object only - block all (node editor's) interactions */
	Interacting /*!< Usual mode - draw and allow interaction too */
};

class FrameContext;
class DrawResult;

/**
 * The base class for all DIWNE elements containing common functionality.
 */
class DiwneObject : public std::enable_shared_from_this<DiwneObject>
{
public:                               /* \todo some attributes should be private/protected */
	DIWNE::NodeEditor& diwne;         /**< Every object have access to Diwne - is used for
	                                share information if needed */
	DIWNE::ID const m_idDiwne;        /**< Used for creating ImGui id/labels */
	std::string const m_labelDiwne;   /**< Used for identifying object and creating
	                                     ImGui id/labels */
	std::string const m_popupIDDiwne; /**< Used for identifying what element raise popup */

	// TODO: Review if these flags are needed
	/**
	 * If some interaction happen with inner elements (DragFloat, Button, ...)
	 * it block interaction with this object
	 */
	bool m_inner_interaction_happen{false};

	// TODO: Not sure if its needed to keep this as a local variable
	//  maybe its enough to pass around the frame context instead

	//	DrawMode m_drawMode; /**< \see enum DrawMode */

	bool m_interactive{true}; // TODO: "Force" JustDraw DrawMode (implement change of draw mode)
	bool m_selectable{true};
	bool m_isHeld{false};    /**< Is object held. When dragged it is still held. */
	bool m_isDragged{false}; /**< Is object dragged */
	bool m_selected{false};  /**< Is object selected */
	bool m_focusedForInteraction{false};
	/**< Is object focus on area that allow interaction with object */ // TODO: Rename / Rework
	bool m_focused{false};
	/**< Is object focused anywhere (and for example can not be focus other underlying object) */ // TODO: Rename to
	                                                                                              // hover I think
	bool m_isActive{false}; /**< Something happen with object */

	/**
	 * \param diwne is node editor object that this object belongs to
	 * \param id used to identification
	 * \param labelDiwne used to identification
	 */
	DiwneObject(DIWNE::NodeEditor& diwne, DIWNE::ID id, std::string const labelDiwne);
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
	virtual void drawDiwne(FrameContext& context);

	// Lifecycle/Content methods
	// =============================================================================================================

	// TODO: Wrapper thought: Not every method seemingly needs a wrapper, maybe we could bunch up the "diwne" code into
	//  a few internal methods with more descriptive names rather than to have a "Diwne" suffix method everywhere.

	virtual void initialize(FrameContext& context); /**< is called every time in drawDiwne() do any
	                              initialization of your object here */
	// New methodology: Begin and End aren't "opening/closing" the object, its just begin and end drawing
	virtual void begin(FrameContext& context) = 0; /**< begin of object  */
	virtual void content(FrameContext& context);   /**< content of object */
	virtual void end(FrameContext& context) = 0;   /**< closing object */

	//	// TODO: Move this method into the Node implementation, eg. move the responsibility to the content method
	//	virtual void updateSizes();        /**< store object size and position after this frame */

	/**
	 * The final lifecycle method, gets called every frame.
	 */
	virtual void finalize(FrameContext& context);

	virtual bool allowInteraction(); ///< Decide whether the object can interact (not including content elements)
	// TODO: Make this get called by the end() method
	virtual void processInteractions(FrameContext& context);

	virtual void popupContent(FrameContext& context); ///< Content of popup menu raise on this object

	// Internal implementation methods, can still be overriden if needed
protected:
	virtual void initializeDiwne(FrameContext& context);
	virtual void beginDiwne(FrameContext& context);
	virtual void endDiwne(FrameContext& context);
	virtual void processInteractionsDiwne(FrameContext& context);

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

	//	virtual bool processInteractions(); ///< react to interactions with this object */
	//	virtual bool processInteractionsAlways(); ///< processes that happen even in allowInteraction() return false,
	// but
	//	                                          ///< object still has to be in DrawMode::Interaction
	//	virtual bool processInteractionsDiwne();  /**< DIWNE wrapper */

	//

	virtual ImRect getRectDiwne() const = 0; /**< return rectangle of object */

	// TODO: Reconsider during action rework
	virtual DiwneAction getHoldActionType() const = 0;  /**< return which type of object/action this object is */
	virtual DiwneAction getDragActionType() const = 0;  /**< return which type of object/action this object is */
	virtual DiwneAction getTouchActionType() const = 0; /**< return which type of object/action this object is */

	virtual bool setSelected(bool const selected); ///< \return New state of selection
	// TODO: Rename isSelected
	// TODO: Go over m_selected usages and switch to using getter
	virtual bool getSelected() const; ///< Whether the object is selected or not
	virtual void setSelectable(bool const selectable);
	virtual bool getSelectable();

	// Bypass methods
	// =============================================================================================================

	// TODO: Refactor / Restructure
	//   Following actions have 4 associated methods: bypass, process, processDiwne and allowProcess
	//   These could possibly be wrapped into some sort of an Action class or something just to keep things together
	//   Furthermore bypass should be renamed, allowProcess possibly as well
	//
	//   Method usage:
	//   bypass - method to check if action is triggered by user input (key is pressed, mouse is over etc.)
	//   processDiwne - method call to check if the action should be triggered, called in the processInteraction step
	//   process - a more user customizable? version of the processDiwne, currently implemented in DiwneObject too
	//             not sure if this makes sense might need some changes
	//   allowProcess - tests if the action is currently allowed in this state

	virtual bool bypassRaisePopupAction();          /**< action used for raising popup menu */
	virtual bool bypassFocusAction();               /**< action identified as focusing on object (and prevent
	                                                   underlying object from focusing) */
	virtual bool bypassFocusForInteractionAction(); /**< action identified as focusing on
	                                                 * object for interacting with it
	                                                 */
	virtual bool bypassHoldAction();                /**< action used for holding object (check
	                                                   only if object is not held)*/
	virtual bool bypassUnholdAction();              /**< action used for unholding object
	                                                   (check only if object is held)*/
	virtual bool bypassSelectAction();              /**< action used for selecting object */
	virtual bool bypassUnselectAction();            /**< action used for unselecting object */
	virtual bool bypassDragAction();                /**< action used for dragging object */
	virtual bool bypassTouchAction();               /**< action used for touching object - not interact with
	                                                   it, just move it to front of other objects */

	// Process methods
	// =============================================================================================================

//	virtual void processRaisePopupDiwne(FrameContext& context); /**< processing raising popup menu */
//	virtual void processShowPopupDiwne(FrameContext& context);  /**< processing showing popup menu */
//
//	virtual void processDrag(FrameContext& context);
//	virtual void processHold(FrameContext& context);
//	virtual void processUnhold(FrameContext& context);
//	virtual void processSelect(FrameContext& context);
//	virtual void processUnselect(FrameContext& context);
//	virtual void processFocused(FrameContext& context); // TODO: Rename focused to just "hover", gets triggered on mouseover
//	virtual void processFocusedForInteraction(FrameContext& context);
//
//	void processFocusedDiwne(FrameContext& context); // TODO: Rename focused to just "hover", gets triggered on mouseover
//	void processFocusedForInteractionDiwne(FrameContext& context);
//	void processHoldDiwne(FrameContext& context);
//	void processUnholdDiwne(FrameContext& context);
//	void processDragDiwne(FrameContext& context);
//	void processSelectDiwne(FrameContext& context);
//	void processUnselectDiwne(FrameContext& context);

//	virtual bool allowProcessFocused(); // TODO: Rename focused to just "hover", gets triggered on mouseover
//	virtual bool allowProcessFocusedForInteraction();
//	virtual bool allowProcessHold();
//	virtual bool allowProcessUnhold();
//	virtual bool allowProcessDrag();
//	virtual bool allowProcessSelect();
//	virtual bool allowProcessUnselect();
//	virtual bool allowProcessRaisePopup();

	// =============================================================================================================

	inline DIWNE::ID const getIdDiwne() const
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

// TODO: Outdated docs

// TODO: Consider idea, make FrameContext flags an integer and count how many times it has been incremented
//  An increment is like turning a flag to true, a zero flag is false
//  => This way we can compare context before and after a draw call to get the latest change itself and react to it
//  Solves the issue of a single global context "obscuring" changes in a single draw method.
//  The context would have a method that would find the difference between two contexts (before/after) by subtraction

// Implementation note:
// Each piece of drawing code gets passed a const FrameContext object and returns a new instance of the DrawResult
// object. The returned DrawResult is used to modify the FrameContext object AFTER the method returns by the DIWNE
// implementation.
// This is all just a fancy way of keeping track of what occurred in a specific piece of drawing code and also keeping
// track of what has occurred throughout a whole frame as the same FrameContext instance is passed along through the
// drawing code and the individual changes are merged into it.
//
// This is currently used to store two crucial pieces of information:
// 1. interacted - Whether the user has interacted with the object IN ANY WAY
// 2. inputConsumed - And whether this interaction should also be exclusive to this object and not propagated further
//                    to any other objects this frame.

/**
 * A structure used to relay information between DIWNE objects during their sequential construction/drawing
 * in a single frame.
 * This is mainly used to retain information about whether an object should react to user input or if a diwne object
 * drawn earlier already "captured" the input and other objects should ignore it.
 * An instance of this object is passed along the drawing code for any nested diwne objects and along the way it
 * collects information about what has already happened that frame.
 */
class FrameContext
{
public:
	/**
	 * Whether any kind of interaction that has a visual or logical impact is occurring.
	 * This can be a simple mouse hover that doesn't capture input in any way.
	 */
	bool interacted{false};

	/// Whether input has been captured by an object previously and should not be reacted to anymore.
	bool inputConsumed{false};

	DrawMode drawMode{DrawMode::Interacting}; ///< The drawing mode for this frame

	FrameContext(DrawMode drawMode) : drawMode(drawMode) {}

	void merge(const FrameContext& other); ///< Updates the context with another context returned by a draw method.
	FrameContext& operator|(const FrameContext& other); ///< A quick way to call the merge() method using an OR operator.
};

///**
// * This structure is returned by individual drawing methods to report what has occurred during the draw.
// * It is then used to update the overarching FrameContext object that is maintained by the DIWNE::NodeEditor instance.
// */
// struct DrawResult
//{
//	/**
//	 * Whether any kind of interaction that has a visual or logical impact is occurring.
//	 * This can be a simple mouse hover that doesn't capture input in any way.
//	 */
//	bool interacted{false};
//
//	/// Whether input has been captured by an object previously and should not be reacted to anymore.
//	bool inputConsumed{false};
//
//	void merge(const DrawResult& update);     ///< Updates the context with another context returned by a draw method.
//	DrawResult& operator|(const DrawResult& update); ///< A quick way to call the merge() method using an OR operator.
//};
//
///**
// * A structure used to relay information between DIWNE objects during their sequential construction/drawing
// * in a single frame.
// * This is mainly used to retain information about whether an object should react to user input or if a diwne object
// * drawn earlier already "captured" the input and other objects should ignore it.
// * An instance of this object is passed along the drawing code for any nested diwne objects and along the way it
// * collects information about what has already happened that frame.
// */
// class FrameContext : public DrawResult
//{
// public:
//	DrawMode drawMode{DrawMode::Interacting}; ///< The drawing mode for this frame
//
//	FrameContext(DrawMode drawMode) : drawMode(drawMode) {}
//
//	/**
//	 * Copies over context variables, but default initializes parent class DrawResult variables.
//	 * This preserves the usually fixed context specific variables but resets the DrawResult variables to be used as a
//	 * newly created DrawResult object.
//	 */
//	FrameContext(const FrameContext& other) : drawMode(other.drawMode) {}
//
//	FrameContext& operator|(const FrameContext& update);
//};


} // namespace DIWNE
