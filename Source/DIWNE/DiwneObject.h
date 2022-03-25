/*!
 * \file DIWNE/DiwneObject.h
 * \author Jaroslav Holeček, holecek.jaroslav@email.cz
 * \date 20.3.2022
 *
 * Declaration of abstract object from which all other object inherit
 */

#include "diwne_include.h"

namespace DIWNE
{

/* ================================== */
/* ===== D i w n e  O b j e c t ===== */
/* ================================== */
/** \brief All objects in DIWNE are inherited from DiwneObject
 *  DiwneObject cover funkcionality common for all Objects in node editor - such as selecting, dragging, hovering
 */
class DiwneObject : public std::enable_shared_from_this<DiwneObject>
{
public: /* \todo JH some atributes should be private/protected */
    DIWNE::Diwne& diwne; /*!< Every object have access to Diwne - is used for share information if needed */
    DIWNE::ID const m_idDiwne; /*!< Used for creating ImGui id/labels */
    std::string const m_labelDiwne; /*!< Used for identifying object and creating ImGui id/labels */
    std::string const m_popupIDDiwne; /*!< Used for identifying what element raise popup */
    bool m_inner_interaction_happen; /*!< If some interaction happen with inner elements (DragFloat, Button, ...) it block interaction with this object */
    DrawMode m_drawMode; /*!< \see enum DrawMode */
    bool m_isHeld /*!< Is object held. When dragged it is still held. */, m_isDraged /*!< Is object draged */, m_selected /*!< Is object selected */, m_focusedForInteraction /*!< Is object focus on area that allow interaction with object */, m_focused /*!< Is object focused anywhere (and for example can not be focus other underlying object) */, m_isActive /*!< Something happen with object */;

    /*! \brief Constructor
     *
     * \param diwne is node editor object that this object belongs to
     * \param id used to identification
     * \param labelDiwne used to identification
     */

    DiwneObject(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne);
    virtual ~DiwneObject(){};

    /*! \brief Setter of selection state
     *
     * \param selected is new state of object
     */
    void setSelected(bool const selected) {m_selected = selected;};

    /*! \brief Getter
     * \return actual selection state of object
     */
    bool getSelected() const {return m_selected;};

    /*! \brief Content of popup menu raise on this object
     */
    virtual void popupContent();

    /*! \brief Decision maker whether object should be drawn (not outside of window for example)
     * \return draw object on its position?
     */
    virtual bool allowDrawing();

    /*! \brief Main function for drawing object and deal with interactions
     * \param drawMode allow interactions with object
     * \return true if some interaction with object (or inner of this object) happen, false otherwise
     */
    virtual bool drawDiwne(DrawMode drawMode=DrawMode::Interacting);
    virtual bool initialize(); /*!< is called every time in drawDiwne() do any initialization of your object here */
    virtual bool initializeDiwne(); /*!< DIWNE wrapper */
    virtual bool beforeBegin(); /*!< is called before begin of object drawing (but after allowDrawing() decision)  */
    virtual bool beforeBeginDiwne(); /*!< DIWNE wrapper */
    virtual void begin()=0; /*!< begin of object  */
    virtual bool beforeContent(); /*!< is called first inside of object */
    virtual bool beforeContentDiwne(); /*!< DIWNE wrapper */
    virtual bool content(); /*!< content of object */
    virtual bool contentDiwne(); /*!< DIWNE wrapper */
    virtual bool afterContent(); /*!< is called as last inside object */
    virtual bool afterContentDiwne(); /*!< DIWNE wrapper */
    virtual void end()=0; /*!< closing object */
    virtual void updateSizes(); /*!< store object size and position after this frame */

    /*! \brief Decision maker whether object can interact (not include content elements)
     * \return can this object interact?
     */
    virtual bool allowInteraction();
    virtual bool afterEnd(); /*!< is called after closing object, sizes are updated already */
    virtual bool afterEndDiwne(); /*!< DIWNE wrapper */
    virtual bool processInteractions(); /*!< react to interactions with this object */
    virtual bool processInteractionsAlways(); /*!< processes that happen even in allowInteraction() return false, but object still has to be in DrawMode::Interaction */
    virtual bool processInteractionsDiwne(); /*!< DIWNE wrapper */
    virtual bool finalize(); /*!< is called every time in drawDiwne() do any finalization of your object here */
    virtual bool finalizeDiwne(); /*!< DIWNE wrapper */

    virtual ImRect getRectDiwne() const = 0; /*!< return rectangle of object */
    virtual DIWNE::DiwneAction getHoldActionType() const = 0; /*!< return which type of object/action this object is */
    virtual DIWNE::DiwneAction getDragActionType() const = 0; /*!< return which type of object/action this object is */
    virtual DIWNE::DiwneAction getTouchActionType() const = 0; /*!< return which type of object/action this object is */

    virtual bool bypassRaisePopupAction(); /*!< action used for raising popup menu */
    virtual bool bypassFocusAction(); /*!< action identified as focusing on object (and prevent underlying object from focusing) */
    virtual bool bypassFocusForInteractionAction(); /*!< action identified as focusing on object for interacting with it */
    virtual bool bypassHoldAction(); /*!< action used for holding object (check only if object is not held)*/
    virtual bool bypassUnholdAction(); /*!< action used for unholding object (check only if object is held)*/
    virtual bool bypassSelectAction(); /*!< action used for selecting object */
    virtual bool bypassUnselectAction(); /*!< action used for unselecting object */
    virtual bool bypassDragAction(); /*!< action used for dragging object */
    virtual bool bypassTouchAction(); /*!< action used for touching object - not interact with it, just move it to front of other objects */

    virtual bool processRaisePopupDiwne(); /*!< processing raising popup menu */
    virtual bool processShowPopupDiwne(); /*!< processing showing popup menu */

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

    inline DIWNE::ID const getIdDiwne() const {return m_idDiwne; };

    /*! \brief Show colored text for example for imediate hints
     * \param label is text to show
     * \param color is color of tooltip
     */
    void showTooltipLabel(std::string const& label, ImColor const&& color);
};


} /* DIWNE namespace */
