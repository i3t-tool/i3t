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

        virtual const ImVec2& getLinkConnectionPointDiwne() { return m_connectionPointDiwne; };


        virtual bool bypassPinHoveredAction(DIWNE::Diwne &diwne);
        virtual bool bypassPinHoldAction(DIWNE::Diwne &diwne);
        virtual bool bypassPinUnholdAction(DIWNE::Diwne &diwne);
        virtual bool bypassPinRaisePopupAction(DIWNE::Diwne &diwne);
        virtual bool bypassPinPreconnectLinkAction(DIWNE::Diwne &diwne);

        virtual bool processPin(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinNewLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinConnectLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinHold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinUnhold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processPinHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

        virtual bool processPinPopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual void pinPopupContent();

        virtual void setConnectionPointDiwne(ImVec2 value) {m_connectionPointDiwne = value;};

        ImRect getPinRectDiwne() const {return m_pinRectDiwne;};

    protected:
        DIWNE::ID m_idDiwne;
        ImRect m_pinRectDiwne;
        bool m_isHeld;
        ImVec2 m_connectionPointDiwne;

        /* intended to use when Pin is drawn (use properties dependent on drawing) - setConnectionPointDiwne is "hard" setting */
        virtual void updateConnectionPointDiwne(DIWNE::Diwne &diwne) {m_connectionPointDiwne = m_pinRectDiwne.GetCenter();}

    private:

        std::string const m_popupID; /* \todo JH MH PF how to initialize const char* with id of Node included? */


};

static void expandPopupBackgroundContent(DIWNE::Pin &this_object)
{
    this_object.pinPopupContent();
}

} /* namespace DIWNE */

#endif // PIN_H
