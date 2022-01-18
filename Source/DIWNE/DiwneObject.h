#include "diwne_include.h"

namespace DIWNE
{

/* ================================== */
/* ===== D i w n e  O b j e c t ===== */
/* ================================== */
class DiwneObject : public std::enable_shared_from_this<DiwneObject>
{
public: /* \todo JH some atributes should be private/protected */
    DIWNE::Diwne& diwne;
    DIWNE::ID const m_idDiwne;
    std::string const m_labelDiwne;
    std::string const m_popupIDDiwne;
    bool m_drawing, m_interactionAllowed, m_inner_interaction_happen, m_drawWithInteraction;
    bool m_isHeld, m_isDraged, m_selected, m_hovered;

    DiwneObject(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne);
    virtual ~DiwneObject(){};
    void setSelected(bool const selected) {m_selected = selected;};
    bool getSelected() const {return m_selected;};

    virtual void popupContent();

    virtual bool drawDiwne(bool with_interaction=true);
    virtual bool initialize(); /* set m_drawing inside -> whether draw object or not if this can happen (m_drawing = true is default) */
    virtual bool initializeDiwne();
    virtual bool beforeBegin();
    virtual bool beforeBeginDiwne();
    virtual void begin()=0;
    virtual bool beforeContent();
    virtual bool beforeContentDiwne();
    virtual bool content();
    virtual bool contentDiwne();
    virtual bool afterContent();
    virtual bool afterContentDiwne();
    virtual void end()=0;
    virtual void allowInteraction(); /* set m_allowInteraction for settings of interaction - called before afterEndDiwne() */
    virtual bool afterEnd();
    virtual bool afterEndDiwne();
    virtual bool finalize();
    virtual bool finalizeDiwne();

    virtual void updateSizes();

    virtual bool bypassRaisePopupAction();
    virtual bool bypassHoveredAction();
    virtual bool bypassHoldAction();
    virtual bool bypassUnholdAction();
    virtual bool bypassSelectAction();
    virtual bool bypassUnselectAction();
    virtual bool bypassDragAction();

    virtual bool processRaisePopupDiwne();
    virtual bool processShowPopupDiwne();

    virtual bool processDrag();
    virtual bool processHold();
    virtual bool processUnhold();
    virtual bool processSelect();
    virtual bool processUnselect();
    virtual bool processHovered();

    virtual bool processObjectHovered();
    virtual bool processObjectHold();
    virtual bool processObjectUnhold();
    virtual bool processObjectDrag();
    virtual bool processObjectSelect();
    virtual bool processObjectUnselect();

    inline DIWNE::ID const getIdDiwne() const {return m_idDiwne; };

    void showTooltipLabel(std::string const& label, ImColor const&& color);
};


} /* DIWNE namespace */
