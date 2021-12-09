#ifndef PIN_H
#define PIN_H


//#include "diwne_include.h"

namespace DIWNE
{
class Pin
{
    public:
        /** Default constructor */
        Pin(DIWNE::ID id);
        /** Default destructor */
        virtual ~Pin();

        DIWNE::ID const getId() const {return m_idDiwne; };

        bool drawPinDiwne(DIWNE::Diwne &diwne);
        virtual bool pinContent(DIWNE::Diwne &diwne) {return false;};


        ImVec2 getCenterDiwne() const {return m_pinRectDiwne.GetCenter(); };
        virtual ImVec2 getLinkConnectionPointDiwne() const {return getCenterDiwne(); };


        virtual bool bypassPinHoveredAction();
        virtual bool bypassPinHoldAction();
        virtual bool bypassPinUnholdAction();
        virtual bool bypassPinRaisePopupAction();
        virtual bool bypassPinConnectLinkAction(DIWNE::Diwne &diwne);

        virtual bool processPin(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinNewLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinConnectLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinHold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinUnhold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

    protected:
        DIWNE::ID m_idDiwne;
        ImRect m_pinRectDiwne;
        bool m_isHeld;

    private:


};

} /* namespace DIWNE */

#endif // PIN_H
