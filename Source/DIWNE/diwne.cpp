#include "diwne.h"

namespace DIWNE
{

Diwne::Diwne(SettingsDiwne *settingsDiwne)
    : m_workAreaDiwne(settingsDiwne->workAreaDiwne)
{
}

Diwne::~Diwne()
{
    //dtor
}

void Diwne::Begin(const char* imgui_id)
{
    updateWorkAreaScreen();
    /* debug - whole workArea */
    ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255,0,0), 0, ImDrawCornerFlags_None, 10);


    ImGui::PushID(imgui_id);
}

void Diwne::updateWorkAreaScreen()
{
    ImVec2 windowPos = ImGui::GetWindowPos();
    m_workAreaScreen.Min = ImGui::GetWindowContentRegionMin() + windowPos;
    m_workAreaScreen.Max = ImGui::GetWindowContentRegionMax() + windowPos;
}

void Diwne::End()
{
    ImGui::PopID();
}

ImRect Diwne::getWorkAreaDiwne()
{
    return m_workAreaDiwne;
}

ImRect Diwne::getWorkAreaScreen()
{
    return m_workAreaScreen;
}

ImVec2 Diwne::screen2workArea(const ImVec2 & point)
{
    return point - m_workAreaScreen.Min;
}

ImVec2 Diwne::workArea2screen(const ImVec2 & point)
{
    return point + m_workAreaScreen.Min;
}

ImVec2 Diwne::diwne2workArea(const ImVec2 & point)
{
    return point - m_workAreaDiwne.Min;
}

ImVec2 Diwne::workArea2diwne(const ImVec2 & point)
{
    return point + m_workAreaDiwne.Min;
}

ImVec2 Diwne::screen2diwne(const ImVec2 & point)
{
    return screen2workArea(workArea2diwne(point));
}

ImVec2 Diwne::diwne2screen(const ImVec2 & point)
{
    return diwne2workArea(workArea2screen(point));
}


} /* namespace DIWNE */
