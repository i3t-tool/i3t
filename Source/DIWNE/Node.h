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

        virtual bool initializeDiwne();
        virtual bool beforeBeginDiwne();
        virtual void begin();
        virtual bool content();
        virtual void end();
        virtual bool afterContentDiwne();
        virtual bool afterEndDiwne();

        template< typename T >
        bool drawNodeDiwne(bool drawHere=false, bool with_interaction=true)
        {
            m_drawOnCursorPos = drawHere;
            bool interaction_happen = drawDiwne(with_interaction);

            if(interaction_happen)
            {
               diwne.setLastActiveNode<T>(std::static_pointer_cast<T>(shared_from_this()));
               if (!(diwne.getDiwneAction() == DiwneAction::DragNode || diwne.getDiwneAction() == DiwneAction::HoldNode)) diwne.setDiwneAction(DiwneAction::InteractingNodeContent);
            }

            return interaction_happen;
        }

        /* use for pre-draw node on top (draw it first on botom) -> catch interaction and stole interaction of other elements  */
        template< typename T >
        bool pre_drawNodeDiwne(bool drawHere=false)
        {
            m_drawOnCursorPos = drawHere;
            bool interaction_happen = drawNodeDiwne<T>(drawHere, true);
            m_drawing_without_interaction = true; /* for next (view only) drawing */


#ifdef DIWNE_DEBUG
    diwne.AddRectDiwne(getNodePositionDiwne(), getNodePositionDiwne()+getNodeRectSizeDiwne()
                       , ImColor(0,0,0,100), 0, ImDrawCornerFlags_None, 10);
#endif // DIWNE_DEBUG
            ImGui::SetCursorScreenPos(diwne.diwne2screen(getNodePositionDiwne()));
            ImGui::InvisibleButton("IBBlockingOtherImGuiInteractions", getNodeRectSizeDiwne()*diwne.getWorkAreaZoom());

            return interaction_happen;
        }

//        bool pre_drawNodeDiwne(bool drawHere = false);
//        bool drawNodeDiwne(bool drawHere = false);
        bool topContentDiwne();
        bool leftContentDiwne();
        bool middleContentDiwne();
        bool rightContentDiwne();
        bool bottomContentDiwne();

        virtual bool processHovered();
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

        bool m_nodeInteractionAllowed;
        bool m_drawOnCursorPos;
        bool m_drawing_without_interaction; /* used for draw top node (and interact with it) */

    private:
        DIWNE::ID m_idDiwne;

        void setNodeRectsPositionDiwne(ImVec2 const& position);
        void translateNodeRectsDiwne(ImVec2 const& amount);
};

} /* namespace DIWNE */


#endif // NODE_H
