#include "diwne_include.h"

namespace DIWNE
{


Diwne::Diwne(SettingsDiwne const & settingsDiwne, void *customData)
    : m_workAreaDiwne( settingsDiwne.workAreaDiwne.Min
                      ,settingsDiwne.workAreaDiwne.Max)
    , m_workAreaZoomDiwne(settingsDiwne.workAreaInitialZoomDiwne)
    , m_zoomWheelSenzitivity(settingsDiwne.zoomWheelSenzitivity)
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

        idl->_FringeScale = 1/m_workAreaZoomDiwne;

        ImGui::SetWindowFontScale(m_workAreaZoomDiwne);

            ImGui::Text("\n\n");

            ImVec2 windowPadding = ImGui::GetStyle().WindowPadding + ImGui::GetStyle().WindowPadding; /* \todo JH somehow get width of border (included manipulable part)*/ windowPadding.x/=m_workAreaZoomDiwne; windowPadding.y/=m_workAreaZoomDiwne;
            //ImGui::PushClipRect(m_workAreaScreen.Min, transformFromImGuiToDiwne(m_workAreaScreen.Max)- windowPadding, false);
            //ImGui::PushClipRect(ImVec2(200, 200), ImVec2(800, 800), false);

//            if (m_workAreaZoomDiwne < 1)
//            {
//                ImGuiWindow *window = windowDiwne;
//                while(window)
//                {
//                    for(auto& rect : window->DrawList->_ClipRectStack )
//                    {
//                        rect = transformFromImGuiToDiwne(rect);
//                    }
//                    window->ClipRect = window->DrawList->_ClipRectStack.back();
//                    window = window->ParentWindow;
//                }
//            }

            for(auto& rect : parent_window->DrawList->_ClipRectStack )
            {
                ImGui::Text(fmt::format("ParentClipRect: {} _ {} _ {} _ {} ",rect.x, rect.y, rect.z, rect.w).c_str());
            }
            for(auto& rect : idl->_ClipRectStack )
            {
                ImGui::Text(fmt::format("ClipRect: {} _ {} _ {} _ {} ",rect.x, rect.y, rect.z, rect.w).c_str());
            }

            ImGui::Text(fmt::format("WindowPadding: {}_{} ",ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y).c_str());
            ImGui::Text(fmt::format("ParentWindowClipRect: {} _ {} _ {} _ {} ",parent_window->ClipRect.Min.x, parent_window->ClipRect.Min.y, parent_window->ClipRect.Max.x, parent_window->ClipRect.Max.y).c_str());

            ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ",ImGui::GetCurrentWindow()->ClipRect.Min.x, ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x, ImGui::GetCurrentWindow()->ClipRect.Max.y).c_str());



        putInvisibleButtonUnder("BackgroundDiwne", m_workAreaScreen.GetSize());
        if (!m_inner_interaction_happen)
        {
            if (ImGui::IsItemActive())
            {
               translateWorkAreaDiwne(ImVec2(-ImGui::GetIO().MouseDelta.x, -ImGui::GetIO().MouseDelta.y));
            }

            float mouseWheel = ImGui::GetIO().MouseWheel;
            if (ImGui::IsItemHovered() && mouseWheel != 0)
            {
                m_workAreaZoomDiwne += mouseWheel/m_zoomWheelSenzitivity;
            }

            popupDiwneItem("BackgroundPopup", &expandPopupBackgroundContent, *this );
        }
        m_inner_interaction_happen = false;

        /* debug */
        ImRect workAreaScreen = getWorkAreaScreen();
        ImRect workAreaDiwne = getWorkAreaDiwne();
        ImGui::SetCursorScreenPos(workAreaScreen.Min + ImVec2(0,200));
        ImGui::Text(fmt::format("WADiwne: {}-{}  -  {}-{}\nWAScreen: {}-{}  -  {}-{}", workAreaDiwne.Min.x, workAreaDiwne.Min.y, workAreaDiwne.Max.x, workAreaDiwne.Max.y,
                            workAreaScreen.Min.x, workAreaScreen.Min.y, workAreaScreen.Max.x, workAreaScreen.Max.y).c_str());
        ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());

        setPopupPosition(ImGui::GetIO().MouseClickedPos[1]); /* must be before transformation mouse to Diwne because popup is new independent window */
//        transformMouseFromImGuiToDiwne();

        ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());

        ImGui::Text(fmt::format("MousePosActual: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
        ImGui::Text(fmt::format("Zoom: {}", m_workAreaZoomDiwne).c_str());

}

void Diwne::End()
{
    ImDrawList *idl = ImGui::GetWindowDrawList();
//    for (auto &vertex : idl->VtxBuffer)
//    {
//        vertex.pos = transformFromDiwneToImGui(vertex.pos);
//    }

    /* debug - whole child "canvas" */
    idl->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255,0,0), 0, ImDrawCornerFlags_None, 10);

    if (m_diwneAction == DiwneAction::NewLink)
    {
        //m_helperLink.setLinkEndpointsDiwne(getActivePin<>()->getLinkConnectionPoint(), screen2workArea(ImGui::GetMousePos()));
        m_helperLink.drawLinkDiwne(*this);
    }

//    transformMouseFromDiwneToImGui();

    m_previousFrameDiwneAction = m_diwneAction;
    m_backgroundPopupRaise = false;
    m_workAreaZoomChangeDiwne = 0;
    m_nodesSelectionChanged = false;

    ImGui::EndChild();
}



void Diwne::updateWorkAreaRectangles()
{
    ImVec2 windowPos = ImGui::GetWindowPos(); /* \todo JH return negative number while sub-window can not move outside from aplication window */
    ImVec2 windowSize = ImGui::GetWindowSize();

    m_workAreaScreen.Min = windowPos;
    m_workAreaScreen.Max = windowPos + windowSize;

    m_workAreaDiwne.Max = m_workAreaDiwne.Min + windowSize; // /m_workAreaZoomDiwne;
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
        ( diwne2screen_noZoom(p_min)
        , diwne2screen_noZoom(p_max)
        , col, rounding, rounding_corners );
}

void Diwne::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawCornerFlags rounding_corners, float thickness) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList();
    idl->AddRect
        ( diwne2screen_noZoom(p_min)
        , diwne2screen_noZoom(p_max)
        , col, rounding, rounding_corners, thickness );
}

void Diwne::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */

    idl->AddBezierCurve(diwne2screen_noZoom(p1)
                        , diwne2screen_noZoom(p2)
                        , diwne2screen_noZoom(p3)
                        , diwne2screen_noZoom(p4)
                        , col, thickness, num_segments);
}

void Diwne::AddBezierCurveScreen(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */
//    idl->PushClipRect(m_workAreaScreen.Min, m_workAreaScreen.Max, false);

    idl->AddBezierCurve(p1, p2, p3, p4, col, thickness, num_segments);

//    idl->PopClipRect();
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
//    ImVec2 const shifted = diwne2workArea_noZoom(point);
//    return ImVec2(shifted.x*m_workAreaZoomDiwne,
//                  shifted.y*m_workAreaZoomDiwne);


        return diwne2workArea_noZoom(point)*m_workAreaZoomDiwne;

}

ImVec2 Diwne::workArea2diwne(const ImVec2 & point) const
{
//    ImVec2 const zoomed = ImVec2(point.x/m_workAreaZoomDiwne, point.y/m_workAreaZoomDiwne);
//    return workArea2diwne_noZoom(zoomed);


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

void Diwne::showPopUpLabel(std::string label, ImColor color)
{
    ImGui::BeginTooltip();

    ImGui::TextColored(color, label.c_str());

    ImGui::EndTooltip();


//	const char* label_c = label.c_str();
//	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
//	ImVec2 labelSize = ImGui::CalcTextSize(label_c);
//
//	ImVec2 padding = ImGui::GetStyle().FramePadding;
//	ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
//
//	ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));
//
//	ImVec2 rectMin = ImGui::GetCursorScreenPos() - padding;
//	ImVec2 rectMax = ImGui::GetCursorScreenPos() + labelSize + padding;
//
//	ImDrawList* drawList = ImGui::GetWindowDrawList();
//	drawList->AddRectFilled(rectMin, rectMax, color, labelSize.y * 0.15f); /* \todo JH remove constant here */
//	ImGui::TextUnformatted(label_c);
}


//
///* \todo JH make one popupDiwne function that take other (content) function as argument - content function is variadic  */
//bool Diwne::popupFloatDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring, float& selectedValue, bool& valueSelected )
//{
//    bool interaction_happen = false;
//    valueSelected = false;
//
//    ImGui::SetNextWindowPos(diwne.getPopupPosition());
//    if (ImGui::BeginPopupContextItem(popupIDstring.c_str()))
//	{
//	    interaction_happen = true;
//	    /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not from (zoomed) diwne */
//        diwne.transformMouseFromDiwneToImGui();
//
//        popupFloatContent(selectedValue, valueSelected);
//
//		ImGui::EndPopup();
//	}
//	return interaction_happen;
//
//}
//
//void Diwne::popupFloatContent(float& selectedValue, bool& valueSelected)
//{
//    ImGui::Text("Set value...                ");
//    ImGui::Separator();
//
//    if (ImGui::RadioButton("Radians", m_floatPopupMode == FloatPopupMode::Radians)){m_floatPopupMode = FloatPopupMode::Radians;} ImGui::SameLine();
//    if (ImGui::RadioButton("Degrees", m_floatPopupMode == FloatPopupMode::Degree)){m_floatPopupMode = FloatPopupMode::Degree;}
//
//    if (m_floatPopupMode == FloatPopupMode::Radians)
//    {
//        ImGui::Columns(2, "floatPopupColumnsRadians", false); // 2-ways, no border
//
//        if (ImGui::Selectable("-PI/6"))
//        {
//            selectedValue		= -M_PI / 6;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-PI/4"))
//        {
//            selectedValue		= -M_PI / 4;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-PI/3"))
//        {
//            selectedValue		= -M_PI / 3;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-PI/2"))
//        {
//            selectedValue		= -M_PI / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-PI"))
//        {
//            selectedValue		= -M_PI;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-3PI/2"))
//        {
//            selectedValue		= -3 * M_PI / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-1"))
//        {
//            selectedValue		= -1.0f;
//            valueSelected = true;
//        }
//        ImGui::NextColumn();
//
//        if (ImGui::Selectable("PI/6"))
//        {
//            selectedValue		= M_PI / 6;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("PI/4"))
//        {
//            selectedValue		= M_PI / 4;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("PI/3"))
//        {
//            selectedValue		= M_PI / 3;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("PI/2"))
//        {
//            selectedValue		= M_PI / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("PI"))
//        {
//            selectedValue		= M_PI;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("3PI/2"))
//        {
//            selectedValue		= -3 * M_PI / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("1"))
//        {
//            selectedValue		= 1.0f;
//            valueSelected = true;
//        }
//
//        ImGui::Columns(1);
//
//        if (ImGui::Selectable("0"))
//        {
//            selectedValue		= 0.0f;
//            valueSelected = true;
//        }
//    }
//    else if (m_floatPopupMode == FloatPopupMode::Degree)
//    {
//        ImGui::Columns(2, "floatPopupColumnsDegrees", false);
//        if (ImGui::Selectable("-1/2"))
//        {
//            selectedValue		= -1.0f / 2.0f;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-sqrt(2)/2"))
//        {
//            selectedValue		= -sqrt(2) / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-sqrt(3)/2"))
//        {
//            selectedValue		= -sqrt(3) / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-2"))
//        {
//            selectedValue		= -2.0f;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("-1"))
//        {
//            selectedValue		= -1.0f;
//            valueSelected = true;
//        }
//
//        ImGui::NextColumn();
//
//        if (ImGui::Selectable("1/2"))
//        {
//            selectedValue = 1.0f / 2.0f;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("sqrt(3)/2"))
//        {
//            selectedValue		= sqrt(3) / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("sqrt(2)/2"))
//        {
//            selectedValue		= sqrt(2) / 2;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("2"))
//        {
//            selectedValue		= 2.0f;
//            valueSelected = true;
//        }
//        if (ImGui::Selectable("1"))
//        {
//            selectedValue		= 1.0f;
//            valueSelected = true;
//        }
//        ImGui::Columns(1);
//        if (ImGui::Selectable("0"))
//        {
//            selectedValue		= 0.0f;
//            valueSelected = true;
//        }
//
//    }
//}

/* >>>> STATIC FUNCTIONS <<<< */
bool putInvisibleButtonUnder(std::string const imguiID, ImVec2 const &size)
{
        bool clicked = ImGui::InvisibleButton(imguiID.c_str(), size );
        ImGui::SetItemAllowOverlap(); /* Maybe some investigation if something like button is draw above */
        ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());

        /* debug */
        if (clicked) ImGui::Text("Background clicked");

        return clicked;
}

} /* namespace DIWNE */
