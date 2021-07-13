#ifndef PIN_H
#define PIN_H

# include <../Dependencies/imgui_node_editor/external/imgui/imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <../Dependencies/imgui_node_editor/external/imgui/imgui_internal.h>

#include <../Source/DIWNE/diwne.h>
#include <../Source/DIWNE/Node.h>

namespace DIWNE
{

class Pin
{
    public:
        /** Default constructor */
        Pin();
        /** Default destructor */
        virtual ~Pin();

        virtual bool drawPinDiwne();
        virtual bool drawPin();

    protected:

    private:
        ImRect m_pinRect;
};

} /* namespace DIWNE */

#endif // PIN_H
