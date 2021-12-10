#ifndef NODE_H
#define NODE_H


#include "diwne_include.h"

namespace DIWNE
{
    class Pin;

/** Parts of Node:
 *  -------------------
 *  |      Top        |
 *  -------------------
 *  |Left|Middle|Right|    -> |L|M|R| == Center
 *  -------------------
 *  |     Bottom      |
 *  -------------------
 */
class Node : public std::enable_shared_from_this<Node>
{
    public:
        /** Default constructor */
        Node(DIWNE::ID id);
        /** Default destructor */
        virtual ~Node();


//        /** Copy constructor
//         *  \param other Object to copy from
//         */
//        Node(const Node& other);

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Node& operator=(const Node& other);

        DIWNE::ID const getId() const {return m_idDiwne; };

        void updateSizeRectangles(DIWNE::Diwne &diwne);

        bool drawNodeDiwne(DIWNE::Diwne &diwne, bool drawHere = false);
        bool drawTopDiwne(DIWNE::Diwne &diwne);
        bool drawLeftDiwne(DIWNE::Diwne &diwne);
        bool drawMiddleDiwne(DIWNE::Diwne &diwne);
        bool drawRightDiwne(DIWNE::Diwne &diwne);
        bool drawBottomDiwne(DIWNE::Diwne &diwne);

        virtual bool processNodeOutsideOfWorkspace(DIWNE::Diwne &diwne) {return false;};
        virtual bool processNodeOnWorkspace(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

        virtual bool bypassNodeHoveredAction();
        virtual bool bypassNodeSelectAction();
        virtual bool bypassNodeUnselectAction();
        virtual bool bypassNodeHoldAction();
        virtual bool bypassNodeUnholdAction();
        virtual bool bypassNodeRaisePopupAction();

        bool processNodeHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        bool processNodeSelected(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        bool processNodeUnselected(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        bool processNodeHold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        bool processNodeUnhold(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        bool processNodeDrag(DIWNE::Diwne &diwne, bool& inner_interaction_happen);

        bool processNodePopupDiwne(DIWNE::Diwne &diwne, bool& inner_interaction_happen);
        virtual void nodePopupContent();

        virtual bool processInNodeBeforeContent(DIWNE::Diwne &diwne) {return false;};
        virtual bool processInNodeAfterContent(DIWNE::Diwne &diwne);
        virtual bool topContent(DIWNE::Diwne &diwne);
        virtual bool leftContent(DIWNE::Diwne &diwne);
        virtual bool middleContent(DIWNE::Diwne &diwne);
        virtual bool rightContent(DIWNE::Diwne &diwne);
        virtual bool bottomContent(DIWNE::Diwne &diwne);



        void setNodePositionDiwne(ImVec2 const& position) {m_nodePositionDiwne = position; setNodeRectsPositionDiwne(position);};
        ImVec2 getNodePositionDiwne() const { return m_nodePositionDiwne; };
        void translateNodePositionDiwne(ImVec2 const amount) {m_nodePositionDiwne+=amount; translateNodeRectsDiwne(amount); };

        ImRect getNodeRectDiwne() { return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);};
        ImVec2 getNodeRectSizeDiwne() { return m_bottomRectDiwne.Max - m_topRectDiwne.Min;};

        bool getSelected() const {return m_selected;};
        void setSelected(bool selected) {m_selected = selected;};

        void setMiddleAlign(float v) {assert(v>=0 && v<=1); m_middleAlign = v;}; /* from 0==left to 1==right */

    protected:

        ImVec2 m_nodePositionDiwne; /* can be public */

        /* Border rects of node - are computed every frame based on node content and m_nodePositionDiwne */
        ImRect  m_topRectDiwne
              , m_leftRectDiwne
              , m_middleRectDiwne
              , m_rightRectDiwne
              , m_bottomRectDiwne; /*! \brief Rectangle of parts of node in diwne */


        float m_centerDummySpace;

        bool m_popupPositionSet; /* \todo I need something like NULL ImVec2 if possible... */
        bool m_selected;
        bool m_translated;
        bool m_nodeInteractionAllowed;

    private:
        DIWNE::ID m_idDiwne;

        void setNodeRectsPositionDiwne(ImVec2 const& position);
        void translateNodeRectsDiwne(ImVec2 const& amount);
        bool m_isHeld;

        std::string const m_popupID; /* \todo JH MH PF how to initialize const char* with id of Node included? */


        float m_middleAlign;

        float m_firstDraw; /* you have to draw node anywhere it is in first frame after you created it -> for obtain its real size */


};

} /* namespace DIWNE */


#endif // NODE_H
