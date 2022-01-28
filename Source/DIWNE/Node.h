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

enum DrawModeNodePosition
{
    OnCoursorPosition,
    OnItsPosition
};

class Node : public DiwneObject
{
    public:
        /** Default constructor */
        Node(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne="DiwneNode");
        /** Default destructor */
        virtual ~Node(){};


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

        void updateSizes();

        virtual bool allowDrawing();
        virtual bool beforeBeginDiwne();
        virtual void begin();
        virtual bool content();
        virtual void end();
        virtual bool afterEndDiwne();

        template< typename T >
        bool drawNodeDiwne(DrawModeNodePosition nodePosMode=DrawModeNodePosition::OnItsPosition, DrawMode drawMode=DrawMode::Interacting)
        {
            m_nodePosMode = nodePosMode;
            m_drawMode = drawMode;

            bool interaction_happen = drawDiwne(drawMode);

            if(drawMode == DrawMode::Interacting && interaction_happen)
            {
               diwne.setLastActiveNode<T>(std::static_pointer_cast<T>(shared_from_this()));
               if (!(diwne.getDiwneAction() == DiwneAction::DragNode || diwne.getDiwneAction() == DiwneAction::HoldNode || diwne.getDiwneAction() == DiwneAction::NewLink || diwne.getDiwneAction() == DiwneAction::FocusOnObject)) diwne.setDiwneAction(DiwneAction::InteractingContent);
            }

            return interaction_happen;
        }

        bool topContentDiwne();
        bool leftContentDiwne();
        bool middleContentDiwne();
        bool rightContentDiwne();
        bool bottomContentDiwne();

        virtual ImRect getRectDiwne() const {return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);};

        virtual bool processFocusedForInteraction();
        virtual bool processSelected();
        virtual bool processUnselected();
        virtual bool processHold();
        virtual bool processUnhold();
        virtual bool processDrag();

        virtual bool topContent();
        virtual bool leftContent();
        virtual bool middleContent();
        virtual bool rightContent();
        virtual bool bottomContent();

        void setNodePositionDiwne(ImVec2 const& position) {m_nodePositionDiwne = position; setNodeRectsPositionDiwne(position);};
        ImVec2 getNodePositionDiwne() const { return m_nodePositionDiwne; };
        void translateNodePositionDiwne(ImVec2 const amount) {m_nodePositionDiwne+=amount; translateNodeRectsDiwne(amount); };

        ImRect getNodeRectDiwne() { return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);};
        ImVec2 getNodeRectSizeDiwne() { return m_bottomRectDiwne.Max - m_topRectDiwne.Min;};

        bool getSelected() const {return m_selected;};
        void setSelected(bool selected) {m_selected = selected;};

        float m_drawAnywhere; /* you have to draw node anywhere for example in first frame after you created it -> for obtain its real size */

    protected:

        ImVec2 m_nodePositionDiwne; /* can be public */

        /* Border rects of node - are computed every frame based on node content and m_nodePositionDiwne */
        ImRect  m_topRectDiwne
              , m_leftRectDiwne
              , m_middleRectDiwne
              , m_rightRectDiwne
              , m_bottomRectDiwne; /*! \brief Rectangle of parts of node in diwne */

        float m_centerDummySpace;
        DrawModeNodePosition m_nodePosMode;
        DrawMode m_drawMode;

    private:
        DIWNE::ID m_idDiwne;

        void setNodeRectsPositionDiwne(ImVec2 const& position);
        void translateNodeRectsDiwne(ImVec2 const& amount);
};

} /* namespace DIWNE */


#endif // NODE_H
