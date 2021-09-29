#ifndef DIWNE_LINK_H
#define DIWNE_LINK_H

#include "diwne_include.h"

static float interactionWidth = 20;

namespace DIWNE
{
class Link
{
    public:
        Link(DIWNE::ID id);

        /** Default destructor */
        virtual ~Link();

        DIWNE::ID const getId() const {return m_idDiwne; };

        bool drawLinkDiwne(DIWNE::Diwne &diwne);
        virtual bool linkContent(DIWNE::Diwne &diwne) {return false;};

        virtual void updateEndpoints(){};
        virtual void updateControlPointsOffsets(){};

        ImVec2 getStartpoint(){return m_startDiwne;};
        ImVec2 getEndpoint(){return m_endDiwne;};

        void setLinkEndpointsDiwne(const ImVec2 start, const ImVec2 end) {m_startDiwne = start; m_endDiwne = end; };
        void setLinkControlpointsOffsetDiwne(const ImVec2 controlStart, const ImVec2 controlEnd) { m_startControlOffsetDiwne = controlStart; m_endControlOffsetDiwne = controlEnd; };

        void setSelected(bool selected) {m_selected = selected;};
        bool getSelected() const {return m_selected;};

        bool linkPopupDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring);
        virtual void linkPopupContent();
    protected:
        bool m_selected;

    private:
        DIWNE::ID m_idDiwne;
        ImVec2 m_startDiwne, m_endDiwne;
        ImVec2 m_startControlOffsetDiwne, m_endControlOffsetDiwne;
        ImColor m_color; /* \todo JH from settings */
        float m_thickness; /* \todo JH from settings */
};

} /* namespace DIWNE */


#endif // DIWNE_LINK_H
