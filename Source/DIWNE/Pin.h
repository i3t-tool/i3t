#ifndef PIN_H
#define PIN_H

#include "diwne_include.h"

namespace DIWNE
{
class Pin : public DiwneObject
{
    public:
        /** Default constructor */
        Pin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne="DiwnePin");
        /** Default destructor */
        virtual ~Pin(){};

        DIWNE::ID const getId() const {return m_idDiwne; };

        virtual bool content() {ImGui::TextUnformatted(fmt::format("{} content", m_labelDiwne).c_str()); return false;};

        DIWNE::DiwneAction getHoldActionType() const final {return DiwneAction::HoldPin;};
        DIWNE::DiwneAction getDragActionType() const final {return DiwneAction::DragPin;};
        DIWNE::DiwneAction getTouchActionType() const final {return DiwneAction::TouchPin;};

        virtual void begin();
        virtual void end();
        virtual void updateSizes();
        virtual bool processInteractionsAlways();


        virtual const ImVec2& getLinkConnectionPointDiwne() { return m_connectionPointDiwne; };

        virtual bool processDrag();

        virtual bool processPin_Pre_ConnectLinkDiwne();
        virtual bool bypassPinLinkConnectionPreparedAction();
        virtual bool allowProcessPin_Pre_ConnectLink();
        virtual bool processConnectionPrepared();

        virtual void setConnectionPointDiwne(ImVec2 value) {m_connectionPointDiwne = value;};

        virtual ImRect getRectDiwne() const {return m_pinRectDiwne;};

    protected:
        DIWNE::ID m_idDiwne;
        ImRect m_pinRectDiwne;
        bool m_isHeld;
        ImVec2 m_connectionPointDiwne;

        /* intended to use when Pin is drawn (use properties dependent on drawing) - setConnectionPointDiwne is "hard" setting */
        virtual void updateConnectionPointDiwne() {m_connectionPointDiwne = m_pinRectDiwne.GetCenter();}

    private:

        std::string const m_popupID; /* \todo JH MH PF how to initialize const char* with id of Node included? */


};

} /* namespace DIWNE */

#endif // PIN_H
