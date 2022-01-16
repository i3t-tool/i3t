#ifndef DIWNE_LINK_H
#define DIWNE_LINK_H

#include "diwne_include.h"

namespace DIWNE
{
class Link : public DiwneObject
{
    public:
        Link(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne="DiwneLink");

        /** Default destructor */
        virtual ~Link(){};

        virtual bool initialize();
        virtual bool initializeDiwne();
        virtual void begin(){};
        virtual void end(){};
        virtual bool content();

        virtual void updateEndpoints(){};
        void updateControlPoints();

        void updateSquareDistanceMouseFromLink();


        ImVec2 getStartpoint(){return m_startDiwne;};
        ImVec2 getEndpoint(){return m_endDiwne;};
        ImVec2 getStartControlPoint(){return m_controlPointStartDiwne;};
        ImVec2 getEndControlPoint(){return m_controlPointEndDiwne;};

        void setLinkEndpointsDiwne(const ImVec2 start, const ImVec2 end) {m_startDiwne = start; m_endDiwne = end; };

        /* in fact just rectangle (from startPoint to endPoint) check - so could return true while Link is not visible */
        bool isLinkOnWorkArea();

        virtual bool bypassHoveredAction();

        bool m_just_pluged;

    protected:

    private:
        ImVec2 m_startDiwne, m_endDiwne;
        ImVec2 m_controlPointStartDiwne, m_controlPointEndDiwne;
        float m_squaredDistanceMouseFromLink;

};

} /* namespace DIWNE */


#endif // DIWNE_LINK_H
