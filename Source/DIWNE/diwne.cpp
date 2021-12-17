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
{
    //s_linkInteractionWidth = settingsDiwne.linkInteractionWidth;
}

Diwne::~Diwne()
{}

bool Diwne::bypassDiwneHoveredAction() {return ImGui::IsItemHovered();}
bool Diwne::bypassDiwneRaisePopupAction() {return ImGui::IsMouseReleased(1);}
bool Diwne::bypassDiwneHoldAction() {return ImGui::IsMouseClicked(0) && bypassDiwneHoveredAction();}
bool Diwne::bypassDiwneUnholdAction() {return !ImGui::IsMouseDown(0);}

bool Diwne::bypassDiwneSetPopupPositionAction() {return ImGui::IsMouseReleased(1);}
ImVec2 Diwne::bypassDiwneGetPopupNewPositionAction() {return bypassGetMousePos();}

bool Diwne::processDiwneHold()
{
    if (!m_inner_interaction_happen && !m_previousFrame_inner_interaction_happen && bypassDiwneHoldAction())
    {
        setDiwneAction(DiwneAction::HoldWorkarea);
        return true;
    }
    return false;
}

bool Diwne::processDiwneUnhold()
{
    if (bypassDiwneUnholdAction())
    {
        setDiwneAction(DiwneAction::None);
        return true;
    }
    return false;
}

bool Diwne::processDiwneDrag()
{
    if(!m_inner_interaction_happen && !m_previousFrame_inner_interaction_happen
        && m_diwneAction == DiwneAction::HoldWorkarea && bypassIsMouseDragging0())
    {
        translateWorkAreaDiwneZoomed(bypassGetMouseDelta()*-1);
        return true;
    }
    return false;
}

bool Diwne::processDiwneZoom()
{
    float mouseWheel = bypassGetMouseWheel();
    if(!m_inner_interaction_happen && !m_previousFrame_inner_interaction_happen
        && mouseWheel != 0 && bypassDiwneHoveredAction())
    {
        setWorkAreaZoomDiwne(m_workAreaZoomDiwne + mouseWheel/m_zoomWheelSenzitivity);
        return true;
    }
    return false;
}

/* this function use template usable in other Elements too -> revise it */
bool Diwne::processDiwnePopupDiwne()
{
    if(!m_inner_interaction_happen && !m_previousFrame_inner_interaction_happen
        && m_previousFrameDiwneAction == DiwneAction::None && m_diwneAction == DiwneAction::None
         && bypassDiwneRaisePopupAction())
    {
        ImGui::OpenPopup("BackgroundPopup");
    }
    return popupDiwneItem("BackgroundPopup", &expandPopupBackgroundContent, *this );

}

bool Diwne::processDiwneSetPopupPosition()
{
    if(m_diwneAction == DiwneAction::None && bypassDiwneSetPopupPositionAction())
    {
        setPopupPosition(bypassDiwneGetPopupNewPositionAction());
        return true;
    }
    return false;
}

bool Diwne::processDiwne()
{
    if (m_diwneAction == DiwneAction::NewLink)
    {
        m_helperLink.drawLinkDiwne(*this);
    }

    m_diwneAction == DiwneAction::HoldWorkarea ? processDiwneUnhold() : processDiwneHold();
    processDiwneDrag();
    processDiwneZoom();

    processDiwneSetPopupPosition();
    processDiwnePopupDiwne();

		return false;
}

void Diwne::BeginDiwne(const char* imgui_id)
{
    ImGui::BeginChild(imgui_id, ImVec2(0,0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        updateWorkAreaRectangles();
        m_inner_interaction_happen = false;
        m_nodesSelectionChanged = false;

        /* zoom */
        ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1/m_workAreaZoomDiwne;
        ImGui::SetWindowFontScale(m_workAreaZoomDiwne);
        m_StoreItemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::GetStyle().ItemSpacing *= m_workAreaZoomDiwne;

#ifdef DIWNE_DEBUG
        ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255,0,0), 0, ImDrawCornerFlags_None, 10);

        ImGui::Text(fmt::format("WindowPadding: {}_{} ",ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y).c_str());
        ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ",ImGui::GetCurrentWindow()->ClipRect.Min.x, ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x, ImGui::GetCurrentWindow()->ClipRect.Max.y).c_str());

        ImRect workAreaScreen = getWorkAreaScreen();
        ImRect workAreaDiwne = getWorkAreaDiwne();
        ImGui::Text(fmt::format("WADiwne: {}-{}  -  {}-{}\nWAScreen: {}-{}  -  {}-{}", workAreaDiwne.Min.x, workAreaDiwne.Min.y, workAreaDiwne.Max.x, workAreaDiwne.Max.y,
                            workAreaScreen.Min.x, workAreaScreen.Min.y, workAreaScreen.Max.x, workAreaScreen.Max.y).c_str());
        ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());

        ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());
        ImGui::Text(fmt::format("Zoom: {}", m_workAreaZoomDiwne).c_str());

        if (m_diwneAction == DIWNE::DiwneAction::None) ImGui::Text("DiwneAction: None");
        if (m_diwneAction == DIWNE::DiwneAction::NewLink) ImGui::Text("DiwneAction: NewLink");
        if (m_diwneAction == DIWNE::DiwneAction::HoldNode) ImGui::Text("DiwneAction: HoldNode");
        if (m_diwneAction == DIWNE::DiwneAction::DragNode) ImGui::Text("DiwneAction: DragNode");
        if (m_diwneAction == DIWNE::DiwneAction::HoldWorkarea) ImGui::Text("DiwneAction: HoldWorkarea");

        m_inner_interaction_happen ? ImGui::Text("inner_interaction happen") : ImGui::Text("inner_interaction not happen");
        m_previousFrame_inner_interaction_happen ? ImGui::Text("prev inner_interaction happen") : ImGui::Text("prev inner_interaction not happen");

#endif // DIWNE_DEBUG

        putInvisibleButtonUnder("BackgroundDiwne", m_workAreaScreen.GetSize());
        processDiwne();

}

void Diwne::EndDiwne()
{

    ImGui::GetStyle().ItemSpacing = m_StoreItemSpacing;
    m_previousFrameDiwneAction = m_diwneAction;
    m_previousFrame_inner_interaction_happen = m_inner_interaction_happen;

    ImGui::EndChild();
}

void Diwne::setWorkAreaZoomDiwne(float val/*=1*/)
{
    double old = m_workAreaZoomDiwne;
    if (val < m_minWorkAreaZoom){ m_workAreaZoomDiwne = m_minWorkAreaZoom; }
    else if (val > m_maxWorkAreaZoom){ m_workAreaZoomDiwne = m_maxWorkAreaZoom; }
    else m_workAreaZoomDiwne = val;
//    if (old!=m_workAreaZoomDiwne)
//    {
//        setWorkAreaZoomDeltaDiwne((double)m_workAreaZoomDiwne/old);
//        s_linkInteractionWidth = (float)((double)s_linkInteractionWidth/(double)getWorkAreaZoomDeltaDiwne());
//    } /* \todo JH dangerous division of floats... */
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


void Diwne::AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding/*=0.0f*/, ImDrawCornerFlags rounding_corners/*=ImDrawCornerFlags_All*/) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */
    idl->AddRectFilled
        ( diwne2screen(p_min)
        , diwne2screen(p_max)
        , col, rounding, rounding_corners );
}

void Diwne::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding/*=0.0f*/, ImDrawCornerFlags rounding_corners/*=ImDrawCornerFlags_All*/, float thickness/*=1.0f*/) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList();
    idl->AddRect
        ( diwne2screen(p_min)
        , diwne2screen(p_max)
        , col, rounding, rounding_corners, thickness );
}

void Diwne::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments/*=0*/) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */

    idl->AddBezierCurve(diwne2screen(p1)
                        , diwne2screen(p2)
                        , diwne2screen(p3)
                        , diwne2screen(p4)
                        , col, thickness*m_workAreaZoomDiwne, num_segments);
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

bool Diwne::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                         DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor,
                         ImVec2 size, ImVec4 padding, bool filled, std::string const id) const
{
    ImVec2 initPos = ImGui::GetCursorScreenPos();

    DrawIcon(bgIconType, bgShapeColor, bgInnerColor,
             fgIconType, fgShapeColor, fgInnerColor,
             size, padding, filled);

    ImGui::SetCursorScreenPos(initPos);
    bool result = ImGui::InvisibleButton(id.c_str(), size);
    ImGui::SetItemAllowOverlap();
    return result;


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

bool Diwne::bypassItemClicked0() {return ImGui::IsItemClicked(0);}
bool Diwne::bypassItemClicked1() {return ImGui::IsItemClicked(1);}
bool Diwne::bypassItemClicked2() {return ImGui::IsItemClicked(2);}
bool Diwne::bypassIsMouseDown0() {return ImGui::IsMouseDown(0);}
bool Diwne::bypassIsMouseDown1() {return ImGui::IsMouseDown(1);}
bool Diwne::bypassIsMouseDown2() {return ImGui::IsMouseDown(2);}
bool Diwne::bypassIsMouseReleased0() {return ImGui::IsMouseReleased(0);}
bool Diwne::bypassIsMouseReleased1() {return ImGui::IsMouseReleased(1);}
bool Diwne::bypassIsMouseReleased2() {return ImGui::IsMouseReleased(2);}
ImVec2 Diwne::bypassMouseClickedPos0() {return ImGui::GetIO().MouseClickedPos[0];}
ImVec2 Diwne::bypassMouseClickedPos1() {return ImGui::GetIO().MouseClickedPos[1];}
ImVec2 Diwne::bypassMouseClickedPos2() {return ImGui::GetIO().MouseClickedPos[2];}
bool Diwne::bypassIsItemActive() {return ImGui::IsItemActive();}
bool Diwne::bypassIsMouseDragging0() {return ImGui::IsMouseDragging(0);}
bool Diwne::bypassIsMouseDragging1() {return ImGui::IsMouseDragging(1);}
bool Diwne::bypassIsMouseDragging2() {return ImGui::IsMouseDragging(2);}
ImVec2 Diwne::bypassGetMouseDragDelta0() {return ImGui::GetMouseDragDelta(0);}
ImVec2 Diwne::bypassGetMouseDragDelta1() {return ImGui::GetMouseDragDelta(1);}
ImVec2 Diwne::bypassGetMouseDragDelta2() {return ImGui::GetMouseDragDelta(2);}
ImVec2 Diwne::bypassGetMouseDelta() {return ImGui::GetIO().MouseDelta;}
ImVec2 Diwne::bypassGetMousePos() {return ImGui::GetIO().MousePos;}
float Diwne::bypassGetMouseWheel() {return ImGui::GetIO().MouseWheel;}


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
