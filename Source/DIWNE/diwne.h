#ifndef DIWNE_H
#define DIWNE_H

//------------------------------------------------------------------------------
# include <../Dependencies/imgui_node_editor/external/imgui/imgui.h>
# define IMGUI_DEFINE_MATH_OPERATORS
# include <../Dependencies/imgui_node_editor/external/imgui/imgui_internal.h>
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_extra_math.h"
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_bezier_math.h"
//# include "../Dependencies/imgui_node_editor/external/imgui/imgui_canvas.h"

//# include <vector>
//# include <string>


//------------------------------------------------------------------------------
namespace DIWNE
{

struct SettingsDiwne
{
    ImRect workAreaDiwne = ImRect(0,0, 1000, 1000);
};

class Diwne
{
    public:
        /** Default constructor */
        Diwne(DIWNE::SettingsDiwne *settingsDiwne);
        /** Default destructor */
        virtual ~Diwne();

        /**
        * Call it between ImGui::Begin() and ImGui::End() to update information for this frame
        */
        void Begin(const char* id);

        /**
        * Call it between ImGui::Begin() and ImGui::End() to process actions in this frame
        */
        void End();

        void updateWorkAreaScreen(); /*! \brief Update position and size of work area on screen */

        ImRect getWorkAreaDiwne();
        ImRect getWorkAreaScreen();

        ImVec2 screen2workArea(const ImVec2 & point);
        ImVec2 workArea2screen(const ImVec2 & point);
        ImVec2 diwne2workArea(const ImVec2 & point);
        ImVec2 workArea2diwne(const ImVec2 & point);
        ImVec2 screen2diwne(const ImVec2 & point);
        ImVec2 diwne2screen(const ImVec2 & point);

    protected:

    private:
    ImRect m_workAreaScreen;     /*! \brief Rectangle of work area on screen */

    ImRect m_workAreaDiwne;  /*! \brief Rectangle of work area on diwne */

};

} /* namespace DIWNE */

#endif // DIWNE_H
