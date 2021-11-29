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

        virtual bool pinActiveCheck(DIWNE::Diwne &diwne);
        virtual void pinActiveProcess(DIWNE::Diwne &diwne);

        virtual bool pinConnectLinkCheck(DIWNE::Diwne &diwne);
        virtual void pinConnectLinkProcess(DIWNE::Diwne &diwne)=0;

        ImVec2 getCenterDiwne() const {return m_pinRectDiwne.GetCenter(); };
        virtual ImVec2 getLinkConnectionPointDiwne() const {return getCenterDiwne(); };

    protected:
        DIWNE::ID m_idDiwne;
        ImRect m_pinRectDiwne;

    private:
        bool m_isHeld;

};

} /* namespace DIWNE */

#endif // PIN_H
