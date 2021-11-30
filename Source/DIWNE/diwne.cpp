#include "diwne_include.h"

// #define DIWNE_DEBUG

namespace DIWNE
{


Diwne::Diwne(SettingsDiwne const & settingsDiwne, void *customData)
    : m_workAreaDiwne( settingsDiwne.workAreaDiwne.Min
                      ,settingsDiwne.workAreaDiwne.Max)
    , m_workAreaZoomDiwne(settingsDiwne.workAreaInitialZoomDiwne)
    , m_zoomWheelSenzitivity(settingsDiwne.zoomWheelSenzitivity)
    , m_minWorkAreaZoom(settingsDiwne.minWorkAreaZoom)
    , m_maxWorkAreaZoom(settingsDiwne.maxWorkAreaZoom)
    , m_mouseLocation(ImGuiLocation)
    , m_helperLink(0)
    , m_customData(customData)
{}

Diwne::~Diwne()
{}

void Diwne::Begin(const char* imgui_id)
{
    ImGui::BeginChild(imgui_id, ImVec2(0,0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGuiWindow *windowDiwne = ImGui::GetCurrentWindow();
        ImDrawList *idl = windowDiwne->DrawList;

        ImGuiWindow *parent_window = ImGui::GetCurrentWindow()->ParentWindow;
        updateWorkAreaRectangles();

            /* \todo window draw list as variable (constant) in diwne */
        AddRectDiwne(m_workAreaDiwne.Min, m_workAreaDiwne.Max, ImColor(0,255,0), 0, ImDrawCornerFlags_None, 20);

        /* zoom */
        idl->_FringeScale = 1/m_workAreaZoomDiwne;
        ImGui::SetWindowFontScale(m_workAreaZoomDiwne);
        m_StoreItemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::GetStyle().ItemSpacing *= m_workAreaZoomDiwne;

            ImGui::Text("\n\n");

            ImVec2 windowPadding = ImGui::GetStyle().WindowPadding + ImGui::GetStyle().WindowPadding; /* \todo JH somehow get width of border (included manipulable part)*/ windowPadding.x/=m_workAreaZoomDiwne; windowPadding.y/=m_workAreaZoomDiwne;
            //ImGui::PushClipRect(m_workAreaScreen.Min, transformFromImGuiToDiwne(m_workAreaScreen.Max)- windowPadding, false);
            //ImGui::PushClipRect(ImVec2(200, 200), ImVec2(800, 800), false);

#ifdef DIWNE_DEBUG
            ImGui::Text(fmt::format("WindowPadding: {}_{} ",ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y).c_str());
            ImGui::Text(fmt::format("ParentWindowClipRect: {} _ {} _ {} _ {} ",parent_window->ClipRect.Min.x, parent_window->ClipRect.Min.y, parent_window->ClipRect.Max.x, parent_window->ClipRect.Max.y).c_str());
            ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ",ImGui::GetCurrentWindow()->ClipRect.Min.x, ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x, ImGui::GetCurrentWindow()->ClipRect.Max.y).c_str());
#endif // DIWNE_DEBUG


        putInvisibleButtonUnder("BackgroundDiwne", m_workAreaScreen.GetSize());
        if (!m_inner_interaction_happen)
        {
            if (ImGui::IsItemActive())
            {
               translateWorkAreaDiwneZoomed(ImGui::GetIO().MouseDelta*-1);
            }

            float mouseWheel = ImGui::GetIO().MouseWheel;
            if (ImGui::IsItemHovered() && mouseWheel != 0)
            {
                setWorkAreaZoomDiwne(m_workAreaZoomDiwne + mouseWheel/m_zoomWheelSenzitivity);
            }

            popupDiwneItem("BackgroundPopup", &expandPopupBackgroundContent, *this );
        }
        m_inner_interaction_happen = false;

#ifdef DIWNE_DEBUG
        ImRect workAreaScreen = getWorkAreaScreen();
        ImRect workAreaDiwne = getWorkAreaDiwne();
        ImGui::SetCursorScreenPos(workAreaScreen.Min + ImVec2(0,200));
        ImGui::Text(fmt::format("WADiwne: {}-{}  -  {}-{}\nWAScreen: {}-{}  -  {}-{}", workAreaDiwne.Min.x, workAreaDiwne.Min.y, workAreaDiwne.Max.x, workAreaDiwne.Max.y,
                            workAreaScreen.Min.x, workAreaScreen.Min.y, workAreaScreen.Max.x, workAreaScreen.Max.y).c_str());
        ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
#endif // DIWNE_DEBUG

        setPopupPosition(ImGui::GetIO().MouseClickedPos[1]); /* must be before transformation mouse to Diwne because popup is new independent window */
//        transformMouseFromImGuiToDiwne();

#ifdef DIWNE_DEBUG
        ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());
        ImGui::Text(fmt::format("MousePosActual: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
        ImGui::Text(fmt::format("Zoom: {}", m_workAreaZoomDiwne).c_str());
#endif // DIWNE_DEBUG

}

void Diwne::End()
{
#ifdef DIWNE_DEBUG
    /* debug - whole child "canvas" */
    ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255,0,0), 0, ImDrawCornerFlags_None, 10);
#endif // DIWNE_DEBUG

    if (m_diwneAction == DiwneAction::NewLink)
    {
        //m_helperLink.setLinkEndpointsDiwne(getActivePin<>()->getLinkConnectionPoint(), screen2workArea(ImGui::GetMousePos()));
        m_helperLink.drawLinkDiwne(*this);
    }

//    transformMouseFromDiwneToImGui();
    ImGui::GetStyle().ItemSpacing = m_StoreItemSpacing;
    m_previousFrameDiwneAction = m_diwneAction;
    m_backgroundPopupRaise = false;
    m_workAreaZoomChangeDiwne = 0;
    m_nodesSelectionChanged = false;


    ImGui::EndChild();
}

void Diwne::setWorkAreaZoomDiwne(float val/*=1*/)
{
    if (val < m_minWorkAreaZoom) m_workAreaZoomDiwne = m_minWorkAreaZoom;
    else if (val > m_maxWorkAreaZoom) m_workAreaZoomDiwne = m_maxWorkAreaZoom;
    else m_workAreaZoomDiwne = val;
}




/* \todo JH some policy what point in WorkArea hold position on screen - probably CoursorPosition */
void Diwne::updateWorkAreaRectangles()
{
    ImVec2 windowPos = ImGui::GetWindowPos(); /* \todo JH return negative number while sub-window can not move outside from aplication window */
    ImVec2 windowSize = ImGui::GetWindowSize();

    m_workAreaScreen.Min = windowPos;
    m_workAreaScreen.Max = windowPos + windowSize;

    m_workAreaDiwne.Max = m_workAreaDiwne.Min + windowSize/m_workAreaZoomDiwne;
}



ImVec2 Diwne::transformFromImGuiToDiwne(const ImVec2& point) const
{
    return workArea2screen(screen2workArea(point) / m_workAreaZoomDiwne); /* basically just zoom */
}

ImVec2 Diwne::transformFromDiwneToImGui(const ImVec2& point) const
{
    return workArea2screen(screen2workArea(point) * m_workAreaZoomDiwne); /* basically just zoom */
}

ImVec4 Diwne::transformFromImGuiToDiwne(const ImVec4& rect) const
{
    ImVec2 const topleft = transformFromImGuiToDiwne(ImVec2(rect.x, rect.y));
    ImVec2 const bottomright = transformFromImGuiToDiwne(ImVec2(rect.z, rect.w));
    return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

ImVec4 Diwne::transformFromDiwneToImGui(const ImVec4& rect) const
{
    ImVec2 const topleft = transformFromDiwneToImGui(ImVec2(rect.x, rect.y));
    ImVec2 const bottomright = transformFromDiwneToImGui(ImVec2(rect.z, rect.w));
    return ImVec4(topleft.x, topleft.y, bottomright.x, bottomright.y);
}

void Diwne::transformMouseFromImGuiToDiwne()
{
    if (m_mouseLocation == ImGuiLocation)
    {
        m_mouseLocation = DiwneLocation;

        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = transformFromImGuiToDiwne(io.MousePos);
        for (int i=0;i<5;i++) io.MouseClickedPos[i] = transformFromImGuiToDiwne(io.MouseClickedPos[i]);
    }
}

void Diwne::transformMouseFromDiwneToImGui()
{
    if (m_mouseLocation == DiwneLocation)
    {
        m_mouseLocation = ImGuiLocation;

        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = transformFromDiwneToImGui(io.MousePos);
        for (int i=0;i<5;i++) io.MouseClickedPos[i] = transformFromDiwneToImGui(io.MouseClickedPos[i]);
    }

}

void Diwne::translateWorkAreaDiwneZoomed(ImVec2 const &distance)
{
    translateWorkAreaDiwne(ImVec2(distance.x/m_workAreaZoomDiwne, distance.y/m_workAreaZoomDiwne));
}

void Diwne::translateWorkAreaDiwne(ImVec2 const &distance)
{
    m_workAreaDiwne.Translate(distance);
}


void Diwne::AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawCornerFlags rounding_corners) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */
    idl->AddRectFilled
        ( diwne2screen(p_min)
        , diwne2screen(p_max)
        , col, rounding, rounding_corners );
}

void Diwne::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawCornerFlags rounding_corners, float thickness) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList();
    idl->AddRect
        ( diwne2screen(p_min)
        , diwne2screen(p_max)
        , col, rounding, rounding_corners, thickness );
}

void Diwne::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */

    idl->AddBezierCurve(diwne2screen(p1)
                        , diwne2screen(p2)
                        , diwne2screen(p3)
                        , diwne2screen(p4)
                        , col, thickness, num_segments);
}

void Diwne::DrawIconCircle(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const
{
    ImVec2 center = ImVec2((topLeft.x+bottomRight.x)/2, (topLeft.y+bottomRight.y)/2);
    float radius = std::min(bottomRight.x-topLeft.x, bottomRight.y-topLeft.y)/2;
    float thicknes = 1; /* \todo JH from settings_diwne */

    idl->AddCircleFilled(center, radius, ShapeColor);
    if (!filled)
    {
        idl->AddCircleFilled(center, radius-thicknes, InnerColor);
    }


}

void Diwne::DrawIconRectangle(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const
{
    ImVec2 thicknes = ImVec2(1,1); /* \todo JH from settings_diwne */
    float rounding = 0; /* \todo JH from settings_diwne */

    idl->AddRectFilled(topLeft, bottomRight, ShapeColor, rounding);
    if (!filled)
    {
        idl->AddRectFilled(topLeft+thicknes, bottomRight-thicknes, InnerColor, rounding);
    }

}

void Diwne::DrawIconTriangleLeft(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const
{
    float thicknes = 1; /* \todo JH from settings_diwne */

    idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), ImVec2(topLeft.x, (topLeft.y+bottomRight.y)/2), bottomRight, ShapeColor);
    if (!filled)
    {
        idl->AddTriangleFilled(ImVec2(bottomRight.x-thicknes, topLeft.y+thicknes), ImVec2(topLeft.x+thicknes, (topLeft.y+bottomRight.y)/2), bottomRight-ImVec2(thicknes, thicknes), InnerColor);
    }

}

void Diwne::DrawIconTriangleRight(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const
{
    float thicknes = 1; /* \todo JH from settings_diwne */

    idl->AddTriangleFilled(topLeft, ImVec2(bottomRight.x, (topLeft.y+bottomRight.y)/2), ImVec2(topLeft.x, bottomRight.y), ShapeColor);
    if (!filled)
    {
        idl->AddTriangleFilled(topLeft+ImVec2(thicknes, thicknes), ImVec2(bottomRight.x-thicknes, (topLeft.y+bottomRight.y)/2), ImVec2(topLeft.x+thicknes, bottomRight.y-thicknes), InnerColor);
    }

}

void Diwne::DrawIconCross(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const
{
    float thicknesShape = 4, thicknesInner = 2; /* \todo JH from settings_diwne */
    float thicknesDiff = (thicknesShape-thicknesInner)/2;
    ImVec2 thicknesDiffVec = ImVec2(thicknesDiff, thicknesDiff);

    idl->AddLine(topLeft, bottomRight, ShapeColor, thicknesShape);
    idl->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(bottomRight.x, topLeft.y), ShapeColor, thicknesShape);
    if (!filled)
    {
        idl->AddLine(topLeft+thicknesDiffVec, bottomRight-thicknesDiffVec, InnerColor, thicknesInner);
        idl->AddLine(ImVec2(topLeft.x, bottomRight.y)+thicknesDiffVec, ImVec2(bottomRight.x, topLeft.y)-thicknesDiffVec, InnerColor, thicknesInner);
    }

}

void Diwne::DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                     DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor,
                     ImVec2 size, ImVec4 padding, bool filled) const
{
    ImDrawList* idl = ImGui::GetWindowDrawList();
    ImVec2 icon_min = ImGui::GetCursorScreenPos();
    ImVec2 icon_max = icon_min + size;
    ImVec2 inner_icon_min = icon_min + ImVec2(padding.x, padding.w);
    ImVec2 inner_icon_max = icon_max - ImVec2(padding.z, padding.y);

//    idl->PushClipRect(m_workAreaScreen.Min, m_workAreaScreen.Max, false);

    switch(bgIconType)
    {
    case Circle:
        DrawIconCircle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, false);
        break;
    case Rectangle:
        DrawIconRectangle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, false);
        break;
    case TriangleLeft:
        DrawIconTriangleLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, false);
        break;
    case TriangleRight:
        DrawIconTriangleRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, false);
        break;
    case Cross:
        DrawIconCross(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, false);
        break;
    }

    switch(fgIconType)
    {
    case Circle:
        DrawIconCircle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
        break;
    case Rectangle:
        DrawIconRectangle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
        break;
    case TriangleLeft:
        DrawIconTriangleLeft(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
        break;
    case TriangleRight:
        DrawIconTriangleRight(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
        break;
    case Cross:
        DrawIconCross(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
        break;
    }

//    idl->PopClipRect();

    ImGui::Dummy(size);
}

ImVec2 Diwne::screen2workArea(const ImVec2 & point) const
{
    return point - m_workAreaScreen.Min;
}

ImVec2 Diwne::workArea2screen(const ImVec2 & point) const
{
    return point + m_workAreaScreen.Min;
}

ImVec2 Diwne::diwne2workArea(const ImVec2 & point) const
{
    return diwne2workArea_noZoom(point)*m_workAreaZoomDiwne;
}

ImVec2 Diwne::workArea2diwne(const ImVec2 & point) const
{
    return workArea2diwne_noZoom(point/m_workAreaZoomDiwne);
}

ImVec2 Diwne::screen2diwne(const ImVec2 & point) const
{
    return workArea2diwne(screen2workArea(point));
}

ImVec2 Diwne::diwne2screen(const ImVec2 & point) const
{
    return workArea2screen(diwne2workArea(point));
}

ImVec2 Diwne::diwne2workArea_noZoom(const ImVec2 & point) const
{
    return point - m_workAreaDiwne.Min;
}

ImVec2 Diwne::workArea2diwne_noZoom(const ImVec2 & point) const
{
    return point + m_workAreaDiwne.Min;
}

ImVec2 Diwne::screen2diwne_noZoom(const ImVec2 & point) const
{
    return workArea2diwne_noZoom(screen2workArea(point));
}

ImVec2 Diwne::diwne2screen_noZoom(const ImVec2 & point) const
{
    return workArea2screen(diwne2workArea_noZoom(point));
}

void Diwne::showTooltipLabel(std::string label, ImColor color)
{
    ImGui::BeginTooltip();

    ImGui::TextColored(color, label.c_str());

    ImGui::EndTooltip();
}



/* >>>> STATIC FUNCTIONS <<<< */
bool putInvisibleButtonUnder(std::string const imguiID, ImVec2 const &size)
{
        bool clicked = ImGui::InvisibleButton(imguiID.c_str(), size );
        ImGui::SetItemAllowOverlap(); /* Maybe some investigation if something like button is draw above */
        ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());
        return clicked;
}

} /* namespace DIWNE */
