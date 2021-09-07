#ifndef PIN_H
#define PIN_H

#include "diwne.h"
#include "Node.h"
#include "Link.h"

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


    protected:

    private:
        DIWNE::ID m_idDiwne;
        ImRect m_pinRectDiwne;
        DIWNE::Link m_linkDiwne;
};

} /* namespace DIWNE */

#endif // PIN_H
