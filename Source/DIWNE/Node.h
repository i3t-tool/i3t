#ifndef NODE_H
#define NODE_H

# include <../Dependencies/imgui_node_editor/external/imgui/imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <../Dependencies/imgui_node_editor/external/imgui/imgui_internal.h>

#include <../Source/DIWNE/diwne.h>
#include <../Source/DIWNE/Pin.h>

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
        Node(DIWNE::Diwne *diwne);
        /** Default destructor */
        virtual ~Node();


        /** Copy constructor
         *  \param other Object to copy from
         */
        Node(const Node& other);

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Node& operator=(const Node& other);


        virtual bool drawNodeDiwne();
        virtual bool drawTopDiwne();
        virtual bool drawLeftDiwne();
        virtual bool drawMiddleDiwne();
        virtual bool drawRightDiwne();
        virtual bool drawBottomDiwne();

        void synchronizeSizeRectangles();

        virtual bool drawTop();
        virtual bool drawLeft();
        virtual bool drawMiddle();
        virtual bool drawRight();
        virtual bool drawBottom();

        ImRect getNodeRect() {return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);};

        DIWNE::Diwne *m_diwne;  /*! \brief diwne to which node belong */

    protected:



        ImRect  m_topRectDiwne
              , m_leftRectDiwne
              , m_middleRectDiwne
              , m_rightRectDiwne
              , m_bottomRectDiwne; /*! \brief Rectangle of parts of node in diwne */

        DIWNE::Pin *m_pin;

    private:


};

} /* namespace DIWNE */


#endif // NODE_H
