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

#include "diwne_include.h"

namespace DIWNE
{

/* ================================== */
/* ===== D i w n e  O b j e c t ===== */
/* ================================== */
/** \brief All objects in DIWNE are inherited from DiwneObject
 *  DiwneObject covers the functionality common to all Objects in the node editor - such
 *  as selecting, dragging, hovering,...
 */
class DiwneObject : public std::enable_shared_from_this<DiwneObject>
{
public:                               /* \todo some attributes should be private/protected */
	DIWNE::Diwne& diwne;              /*!< Every object have access to Diwne - is used for
	                                     share information if needed */
	DIWNE::ID const m_idDiwne;        /*!< Used for creating ImGui id/labels */
	std::string const m_labelDiwne;   /*!< Used for identifying object and creating
	                                     ImGui id/labels */
	std::string const m_popupIDDiwne; /*!< Used for identifying what element raise popup */
	bool m_inner_interaction_happen, m_inner_interaction_happen_previous_draw; /*!< If some interaction happen
	                                                                              with inner elements
	                                                                              (DragFloat, Button, ...) it
	                                                                              block interaction with this
	                                                                              object */
	DrawMode m_drawMode;                                                       /*!< \see enum DrawMode */
	bool m_selectable;
	bool m_isHeld;                /*!< Is object held. When dragged it is still held. */
	bool m_isDragged;             /*!< Is object dragged */
	bool m_selected;              /*!< Is object selected */
	bool m_focusedForInteraction; /*!< Is object focus on area that allow
	                                 interaction with object */
	bool m_focused;               /*!< Is object focused anywhere (and for example can not
	                                 be¨focus other underlying object) */
	bool m_isActive;              /*!< Something happen with object */
	;

	/*! \brief Constructor
	 *
	 * \param diwne is node editor object that this object belongs to
	 * \param id used to identification
	 * \param labelDiwne used to identification
	 */
	DiwneObject(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne);
	virtual ~DiwneObject() = default;

	virtual bool interactionBeginInLastDraw()
	{
		return m_inner_interaction_happen && !m_inner_interaction_happen_previous_draw;
	};
	virtual bool interactionEndInLastDraw()
	{
		return !m_inner_interaction_happen && m_inner_interaction_happen_previous_draw;
	};

	/*!
	 * Setter of selection state
	 * \param selected is the new requested state of the object
	 * \return New state of selection
	 */
	virtual bool setSelected(bool const selected)
	{
		m_selected = m_selectable ? selected : false;
		return m_selected;
	};

	/*! \brief Getter of selection state
	 * \return Current selection state of the object.
	 */
	virtual bool getSelected() const
	{
		return m_selected;
	};

	virtual void setSelectable(bool const selectable)
	{
		m_selectable = selectable;
	};
	virtual bool getSelectable()
	{
		return m_selectable;
	};

	/*! \brief Content of popup menu raise on this object
	 */
	virtual void popupContent();

	/*! \brief Decision maker whether object should be drawn (not outside of
	 * window for example) \return draw object on its position?
	 */
	virtual bool allowDrawing();

	/*! \brief Main function for drawing object and deal with interactions
	 * \param drawMode allows interactions with object
	 * \return true if some interaction with object (or inner of this object)
	 * happen, false otherwise
	 */
	virtual bool drawDiwne(DrawMode drawMode = DrawMode::Interacting);
	virtual bool initialize();         /*!< is called every time in drawDiwne() do any
	                                      initialization of your object here */
	virtual bool initializeDiwne();    /*!< DIWNE wrapper */
	virtual bool beforeBegin();        /*!< is called before begin of object drawing (but
	                                      after allowDrawing() decision)  */
	virtual bool beforeBeginDiwne();   /*!< DIWNE wrapper */
	virtual void begin() = 0;          /*!< begin of object  */
	virtual bool beforeContent();      /*!< is called first inside of object */
	virtual bool beforeContentDiwne(); /*!< DIWNE wrapper */
	virtual bool content();            /*!< content of object */
	virtual bool contentDiwne();       /*!< DIWNE wrapper */
	virtual bool afterContent();       /*!< is called as last inside object */
	virtual bool afterContentDiwne();  /*!< DIWNE wrapper */
	virtual void end() = 0;            /*!< closing object */
	virtual void updateSizes();        /*!< store object size and position after this frame */

	/*! \brief Decision maker whether object can interact (not include content
	 * elements) \return can this object interact?
	 */
	virtual bool allowInteraction();
	virtual bool afterEnd();                  /*!< is called after closing object, sizes are updated already */
	virtual bool afterEndDiwne();             /*!< DIWNE wrapper */
	virtual bool processInteractions();       /*!< react to interactions with this object */
	virtual bool processInteractionsAlways(); /*!< processes that happen even in
	                                             allowInteraction() return false, but	           object
	                                             still has to be in DrawMode::Interaction */
	virtual bool processInteractionsDiwne();  /*!< DIWNE wrapper */
	virtual bool finalize();                  /*!< is called every time in drawDiwne() do any
	                                             finalization of your object here */
	virtual bool finalizeDiwne();             /*!< DIWNE wrapper */

	virtual ImRect getRectDiwne() const = 0;                   /*!< return rectangle of object */
	virtual DIWNE::DiwneAction getHoldActionType() const = 0;  /*!< return which type of object/action this object is */
	virtual DIWNE::DiwneAction getDragActionType() const = 0;  /*!< return which type of object/action this object is */
	virtual DIWNE::DiwneAction getTouchActionType() const = 0; /*!< return which type of object/action this object is */

	virtual bool bypassRaisePopupAction();          /*!< action used for raising popup menu */
	virtual bool bypassFocusAction();               /*!< action identified as focusing on object (and prevent
	                                                   underlying object from focusing) */
	virtual bool bypassFocusForInteractionAction(); /*!< action identified as focusing on
	                                                 * object for interacting with it
	                                                 */
	virtual bool bypassHoldAction();                /*!< action used for holding object (check
	                                                   only if object is not held)*/
	virtual bool bypassUnholdAction();              /*!< action used for unholding object
	                                                   (check only if object is held)*/
	virtual bool bypassSelectAction();              /*!< action used for selecting object */
	virtual bool bypassUnselectAction();            /*!< action used for unselecting object */
	virtual bool bypassDragAction();                /*!< action used for dragging object */
	virtual bool bypassTouchAction();               /*!< action used for touching object - not interact with
	                                                   it, just move it to front of other objects */

	virtual bool processRaisePopupDiwne(); /*!< processing raising popup menu */
	virtual bool processShowPopupDiwne();  /*!< processing showing popup menu */

	virtual bool processDrag();
	virtual bool processHold();
	virtual bool processUnhold();
	virtual bool processSelect();
	virtual bool processUnselect();
	virtual bool processFocused();
	virtual bool processFocusedForInteraction();

	virtual bool allowProcessFocused();
	virtual bool allowProcessFocusedForInteraction();
	virtual bool allowProcessHold();
	virtual bool allowProcessUnhold();
	virtual bool allowProcessDrag();
	virtual bool allowProcessSelect();
	virtual bool allowProcessUnselect();
	virtual bool allowProcessRaisePopup();

	virtual bool processObjectFocused();
	virtual bool processObjectFocusedForInteraction();
	virtual bool processObjectHold();
	virtual bool processObjectUnhold();
	virtual bool processObjectDrag();
	virtual bool processObjectSelect();
	virtual bool processObjectUnselect();

	inline DIWNE::ID const getIdDiwne() const
	{
		return m_idDiwne;
	};

	/*! \brief Show a colored text for example for immediate hints
	 * \param label is the text to show
	 * \param color is the color of the tooltip
	 */
	void showTooltipLabel(std::string const& label, ImColor const&& color);
};

} // namespace DIWNE
