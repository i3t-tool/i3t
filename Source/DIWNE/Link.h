#ifndef DIWNE_LINK_H
#define DIWNE_LINK_H

#include "diwne_include.h"

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
        virtual bool linkContent(DIWNE::Diwne &diwne);

        virtual void updateEndpoints(){};
        virtual void updateControlPointsOffsets(){};
        void updateControlPoints(){ m_controlPointStartDiwne=m_startDiwne+m_startControlOffsetDiwne; m_controlPointEndDiwne=m_endDiwne+m_endControlOffsetDiwne;};

        void updateSquareDistanceMouseFromLink(DIWNE::Diwne &diwne);


        ImVec2 getStartpoint(){return m_startDiwne;};
        ImVec2 getEndpoint(){return m_endDiwne;};
        ImVec2 getStartControlPoint(){return m_controlPointStartDiwne;};
        ImVec2 getEndControlPoint(){return m_controlPointEndDiwne;};

        void setLinkEndpointsDiwne(const ImVec2 start, const ImVec2 end) {m_startDiwne = start; m_endDiwne = end; };
        void setLinkControlpointsOffsetDiwne(const ImVec2 controlStart, const ImVec2 controlEnd) { m_startControlOffsetDiwne = controlStart; m_endControlOffsetDiwne = controlEnd; };

        void setSelected(bool selected) {m_selected = selected;};
        bool getSelected() const {return m_selected;};

        /* in fact just rectangle (from startPoint to endPoint) check - so could return true while Link is not vissible*/
        bool isLinkOnWorkArea(DIWNE::Diwne &diwne);

        bool processLinkPopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual void linkPopupContent();

        virtual bool bypassLinkHoveredAction(DIWNE::Diwne &diwne);
        virtual bool bypassLinkSelectAction(DIWNE::Diwne &diwne);
        virtual bool bypassLinkUnselectAction(DIWNE::Diwne &diwne);
        virtual bool bypassLinkHoldAction(DIWNE::Diwne &diwne);
        virtual bool bypassLinkUnholdAction(DIWNE::Diwne &diwne);
        virtual bool bypassLinkRaisePopupAction(DIWNE::Diwne &diwne);

        virtual bool processLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

        virtual bool processLinkHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processLinkSelected(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual bool processLinkUnselected(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

        bool m_just_pluged;

    protected:
        bool m_selected;
        ImColor m_color, m_selectedColor;

    private:
        DIWNE::ID m_idDiwne;
        ImVec2 m_startDiwne, m_endDiwne;
        ImVec2 m_startControlOffsetDiwne, m_endControlOffsetDiwne;
        ImVec2 m_controlPointStartDiwne, m_controlPointEndDiwne;
        float m_squaredDistanceMouseFromLink;
        float m_thickness;
        std::string const m_popupID;
        bool m_translated;
};

static void expandPopupBackgroundContent(DIWNE::Link &this_object)
{
    this_object.linkPopupContent();
}

} /* namespace DIWNE */


#endif // DIWNE_LINK_H
