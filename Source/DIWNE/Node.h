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
 *  |Left|Middle|Right|
 *  -------------------
 *  |     Bottom      |
 *  -------------------
 */
class Node
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

        void synchronizeSizeRectangles();

        bool drawNodeDiwne(DIWNE::Diwne &diwne);
        bool drawTopDiwne(DIWNE::Diwne &diwne);
        bool drawLeftDiwne(DIWNE::Diwne &diwne);
        bool drawMiddleDiwne(DIWNE::Diwne &diwne);
        bool drawRightDiwne(DIWNE::Diwne &diwne);
        bool drawBottomDiwne(DIWNE::Diwne &diwne);

        virtual bool topContent(DIWNE::Diwne &diwne);
        virtual bool leftContent(DIWNE::Diwne &diwne);
        virtual bool middleContent(DIWNE::Diwne &diwne);
        virtual bool rightContent(DIWNE::Diwne &diwne);
        virtual bool bottomContent(DIWNE::Diwne &diwne);

        bool nodePopupDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring);
        virtual void nodePopupContent();

        void setNodePositionDiwne(ImVec2 const& position) {m_nodePosition = position; };
        ImVec2 getNodePositionDiwne() const { return m_nodePosition; };
        void translateNodePositionDiwne(ImVec2 const amount) {m_nodePosition+=amount; };

        ImRect getNodeRectDiwne() { synchronizeSizeRectangles(); return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);};
        ImVec2 getNodeRectSizeDiwne() { synchronizeSizeRectangles(); return m_bottomRectDiwne.Max-m_topRectDiwne.Min;};

        bool getSelected() const {return m_selected;};
        void setSelected(bool selected) {m_selected = selected;};

    protected:

        ImVec2 m_nodePosition; /* can be public */

        /* Border rects of node - are computed every frame based on node content and m_nodePosition */
        ImRect  m_topRectDiwne
              , m_leftRectDiwne
              , m_middleRectDiwne
              , m_rightRectDiwne
              , m_bottomRectDiwne; /*! \brief Rectangle of parts of node in diwne */

        bool m_popupPositionSet; /* \todo I need something like NULL ImVec2 if possible... */
        bool m_selected;
        bool m_translated;

    private:
        DIWNE::ID m_idDiwne;

        void setNodeRectsPositionDiwne(ImVec2 const& position);
        void translateNodeRectsDiwne(ImVec2 const& amount);
        //void translateNodeRectsDiwneZoomed(DIWNE::Diwne const &diwne, ImVec2 const& amount);


};

} /* namespace DIWNE */


#endif // NODE_H
