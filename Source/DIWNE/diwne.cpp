#include "diwne_include.h"

namespace DIWNE
{

/* ===================== */
/* ===== D i w n e ===== */
/* ===================== */

Diwne::Diwne(SettingsDiwne* settingsDiwne)
    :   DiwneObject(*this, settingsDiwne->editorId, settingsDiwne->editorlabel)
    ,   mp_settingsDiwne(settingsDiwne)
    ,   m_workAreaDiwne( settingsDiwne->workAreaDiwne.Min
                        ,settingsDiwne->workAreaDiwne.Max)

    ,   m_workAreaZoom(settingsDiwne->workAreaInitialZoom)

    ,   mp_lastActivePin(nullptr)
    ,   m_helperLink(diwne, 0)

    ,   m_diwneAction(None)
    ,   m_diwneAction_previousFrame(m_diwneAction)
    ,   m_objectFocused(false)

    ,   m_nodesSelectionChanged(false)
    ,   m_selectionRectangeDiwne(ImRect(0,0,0,0))

    ,   m_popupPosition(settingsDiwne->initPopupPosition)
    ,   m_popupDrawn(false)
    ,   m_tooltipDrawn(false)

    ,   m_takeSnap(false)
{
    m_selectable = false;
}

DiwneAction Diwne::getDiwneActionActive() const {return m_diwneAction == DiwneAction::None ? m_diwneAction_previousFrame : m_diwneAction; }

bool Diwne::allowDrawing(){return m_drawing;}

bool Diwne::initializeDiwne()
{
    m_drawing = ImGui::BeginChild(mp_settingsDiwne->editorlabel.c_str(), ImVec2(0,0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    m_diwneAction = DiwneAction::None;
    m_popupDrawn = m_tooltipDrawn = m_objectFocused = m_takeSnap = false;
    return initialize();
}

bool Diwne::beforeBeginDiwne() /* \todo JH redesign to https://en.wikipedia.org/wiki/Call_super */
{
    updateWorkAreaRectangles();
    m_nodesSelectionChanged = false;

    /* zoom */
    ImGui::GetCurrentWindow()->DrawList->_FringeScale = 1/m_workAreaZoom;
//    ImGui::SetWindowFontScale(m_workAreaZoom);
    m_StoreFontScale = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale = m_workAreaZoom;
    ImGui::PushFont(NULL);

    m_StoreItemSpacing = ImGui::GetStyle().ItemSpacing;
    ImGui::GetStyle().ItemSpacing *= m_workAreaZoom;

    return beforeBegin();
}

void Diwne::begin()
{
    ImGui::SetCursorScreenPos(m_workAreaScreen.Min);
    ImGui::PushID(mp_settingsDiwne->editorlabel.c_str());
    ImGui::BeginGroup();

#ifdef DIWNE_DEBUG
ImGui::Checkbox("Show debug texts", &(m_diwneDebug_on));
DIWNE_DEBUG((*this), {
    ImGui::GetWindowDrawList()->AddRect(m_workAreaScreen.Min, m_workAreaScreen.Max, ImColor(255,0,0), 0, ImDrawCornerFlags_None, 10);
    ImGui::Text(fmt::format("\tWADiwne: {}-{}  -  {}-{}\n\tWAScreen: {}-{}  -  {}-{}", m_workAreaDiwne.Min.x, m_workAreaDiwne.Min.y, m_workAreaDiwne.Max.x, m_workAreaDiwne.Max.y,
                        m_workAreaScreen.Min.x, m_workAreaScreen.Min.y, m_workAreaScreen.Max.x, m_workAreaScreen.Max.y).c_str());

    ImGui::Text(fmt::format("MousePos: {}-{}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y).c_str());
    ImGui::Text(fmt::format("PopupPos: {}-{}", getPopupPosition().x, getPopupPosition().y).c_str());
    ImGui::Text(fmt::format("Zoom: {}", m_workAreaZoom).c_str());

    ImGui::Text(fmt::format("WindowPadding: {}_{} ",ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y).c_str());
    ImGui::Text(fmt::format("WindowClipRect: {} _ {} _ {} _ {} ",ImGui::GetCurrentWindow()->ClipRect.Min.x, ImGui::GetCurrentWindow()->ClipRect.Min.y, ImGui::GetCurrentWindow()->ClipRect.Max.x, ImGui::GetCurrentWindow()->ClipRect.Max.y).c_str());

    switch (m_diwneAction_previousFrame)
    {
        case DiwneAction::None: ImGui::Text("PrevDiwneAction: None"); break;
        case DiwneAction::NewLink: ImGui::Text("PrevDiwneAction: NewLink"); break;
        case DiwneAction::HoldNode: ImGui::Text("PrevDiwneAction: HoldNode"); break;
        case DiwneAction::DragNode: ImGui::Text("PrevDiwneAction: DragNode"); break;
        case DiwneAction::HoldWorkarea: ImGui::Text("PrevDiwneAction: HoldWorkarea"); break;
        case DiwneAction::DragWorkarea: ImGui::Text("PrevDiwneAction: DragWorkarea"); break;
        case DiwneAction::SelectionRectFull: ImGui::Text("PrevDiwneAction: SelectionRectFull"); break;
        case DiwneAction::SelectionRectTouch: ImGui::Text("PrevDiwneAction: SelectionRectTouch"); break;
        case DiwneAction::InteractingContent: ImGui::Text("PrevDiwneAction: InteractingContent"); break;
        case DiwneAction::FocusOnObject: ImGui::Text("PrevDiwneAction: FocusOnObject"); break;
        case DiwneAction::HoldPin: ImGui::Text("PrevDiwneAction: HoldPin"); break;
        case DiwneAction::DragPin: ImGui::Text("PrevDiwneAction: DragPin"); break;
        case DiwneAction::HoldLink: ImGui::Text("PrevDiwneAction: HoldLink"); break;
        case DiwneAction::DragLink: ImGui::Text("PrevDiwneAction: DragLink"); break;
        default: ImGui::Text("PrevDiwneAction: Unknown");
    }

}); /* close of macro */
#endif // DIWNE_DEBUG
}

bool Diwne::afterContentDiwne()
{
    bool interaction_happen = false;
    if (m_diwneAction == DiwneAction::NewLink)
    {
        interaction_happen |= m_helperLink.drawDiwne(JustDraw);
    }
    interaction_happen |= afterContent();
    return interaction_happen;
}

void Diwne::end()
{
    ImGui::SetCursorScreenPos(m_workAreaScreen.Max); /* for capture whole window/workarea to Group */
    ImGui::EndGroup();
    ImGui::PopID();
}

bool Diwne::afterEndDiwne()
{
    ImGui::GetStyle().ItemSpacing = m_StoreItemSpacing;

    ImGui::GetFont()->Scale = m_StoreFontScale;
    ImGui::PopFont();

    return DiwneObject::afterEndDiwne();
}

bool Diwne::processInteractions()
{
    return processDiwneSelectionRectangle();
}

bool Diwne::processInteractionsDiwne()
{
    bool interaction_happen = false;

    interaction_happen |= DiwneObject::processInteractionsDiwne();

    if ( m_drawMode == DrawMode::Interacting && bypassFocusForInteractionAction()) /* for example inner interaction (focus on node) is no problem with this actions */
    {
        interaction_happen |= processDiwneZoom();
    }

    return interaction_happen;
}

bool Diwne::finalizeDiwne()
{
    bool interaction_happen = finalize();
    m_diwneAction_previousFrame = m_diwneAction;
    ImGui::EndChild();
    return interaction_happen;
}

bool Diwne::blockRaisePopup()
{
    return m_diwneAction == DiwneAction::SelectionRectFull || m_diwneAction == DiwneAction::SelectionRectTouch || m_diwneAction_previousFrame == DiwneAction::SelectionRectFull || m_diwneAction_previousFrame == DiwneAction::SelectionRectTouch;
}

/* be careful for same mouse button in this functions */
bool Diwne::allowProcessSelectionRectangle(){return m_focusedForInteraction;}
bool Diwne::bypassSelectionRectangleAction() {return bypassIsMouseDragging1();} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 Diwne::bypassDiwneGetSelectionRectangleStartPosition() {return screen2diwne(bypassMouseClickedPos1());} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 Diwne::bypassDiwneGetSelectionRectangleSize() {return bypassGetMouseDragDelta1()/getWorkAreaZoom();} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */

bool Diwne::processDiwneSelectionRectangle()
{
    if(bypassSelectionRectangleAction() && allowProcessSelectionRectangle())
    {
        ImVec2 startPos = bypassDiwneGetSelectionRectangleStartPosition();
        ImVec2 dragDelta = bypassDiwneGetSelectionRectangleSize();
        ImColor color;

        if(dragDelta.x > 0)
        {
            setDiwneAction(DiwneAction::SelectionRectFull);
            m_selectionRectangeDiwne.Min.x = startPos.x;
            m_selectionRectangeDiwne.Max.x = startPos.x + dragDelta.x;
            color = mp_settingsDiwne->selectionRectFullColor;
        }else
        {
            setDiwneAction(DiwneAction::SelectionRectTouch);
            m_selectionRectangeDiwne.Min.x = startPos.x + dragDelta.x;
            m_selectionRectangeDiwne.Max.x = startPos.x;
            color = mp_settingsDiwne->selectionRectTouchColor;
        }

        if(dragDelta.y > 0)
        {
            m_selectionRectangeDiwne.Min.y = startPos.y;
            m_selectionRectangeDiwne.Max.y = startPos.y + dragDelta.y;
        }else
        {
            m_selectionRectangeDiwne.Min.y = startPos.y + dragDelta.y;
            m_selectionRectangeDiwne.Max.y = startPos.y;
        }

        AddRectFilledDiwne(m_selectionRectangeDiwne.Min, m_selectionRectangeDiwne.Max, color);

        return true;
    }
    return false;
}

bool Diwne::processDrag()
{
    translateWorkAreaDiwneZoomed(bypassGetMouseDelta()*-1);
    return true;
}

bool Diwne::processZoom()
{
    ImVec2 mousePosDiwne = screen2diwne(bypassGetMousePos());
    setWorkAreaZoom( m_workAreaZoom + bypassGetZoomDelta() );
    translateWorkAreaDiwne(mousePosDiwne-screen2diwne(bypassGetMousePos()));
    return true;
}

bool Diwne::processDiwneZoom()
{
    if (bypassZoomAction() && allowProcessZoom())
    {
        return processZoom();
    }
    return false;
}

void Diwne::setWorkAreaZoom(float val/*=1*/)
{
    double old = m_workAreaZoom;
    if (val < mp_settingsDiwne->minWorkAreaZoom){ m_workAreaZoom = mp_settingsDiwne->minWorkAreaZoom; }
    else if (val > mp_settingsDiwne->maxWorkAreaZoom){ m_workAreaZoom = mp_settingsDiwne->maxWorkAreaZoom; }
    else m_workAreaZoom = val;
}

/* \todo JH some policy what point in WorkArea hold position on screen - probably CoursorPosition */
void Diwne::updateWorkAreaRectangles()
{
    ImVec2 windowPos = ImGui::GetWindowPos(); /* \todo JH return negative number while sub-window can not move outside from aplication window */
    ImVec2 windowSize = ImGui::GetWindowSize();

    m_workAreaScreen.Min = windowPos;
    m_workAreaScreen.Max = windowPos + windowSize;

    m_workAreaDiwne.Max = m_workAreaDiwne.Min + windowSize/m_workAreaZoom;
}



ImVec2 Diwne::transformFromImGuiToDiwne(const ImVec2& point) const
{
    return workArea2screen(screen2workArea(point) / m_workAreaZoom); /* basically just zoom */
}

ImVec2 Diwne::transformFromDiwneToImGui(const ImVec2& point) const
{
    return workArea2screen(screen2workArea(point) * m_workAreaZoom); /* basically just zoom */
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

void Diwne::translateWorkAreaDiwneZoomed(ImVec2 const &distance)
{
    translateWorkAreaDiwne(ImVec2(distance.x/m_workAreaZoom, distance.y/m_workAreaZoom));
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
        , col, rounding, rounding_corners, thickness*m_workAreaZoom );
}

void Diwne::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments/*=0*/) const
{
    ImDrawList *idl = ImGui::GetWindowDrawList(); /* \todo JH maybe use other channel with correct Clip rect for drawing of manual shapes, but be careful with order of drew elements */

    idl->AddBezierCurve(diwne2screen(p1)
                        , diwne2screen(p2)
                        , diwne2screen(p3)
                        , diwne2screen(p4)
                        , col, thickness*m_workAreaZoom, num_segments);
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
//    ImGui::SetItemAllowOverlap();
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

    ImGui::Dummy(size); /* \todo JH maybe InvisibleButton */
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
    return diwne2workArea_noZoom(point)*m_workAreaZoom;
}

ImVec2 Diwne::workArea2diwne(const ImVec2 & point) const
{
    return workArea2diwne_noZoom(point/m_workAreaZoom);
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

bool Diwne::bypassIsItemClicked0() {return ImGui::IsItemClicked(0);}
bool Diwne::bypassIsItemClicked1() {return ImGui::IsItemClicked(1);}
bool Diwne::bypassIsItemClicked2() {return ImGui::IsItemClicked(2);}
bool Diwne::bypassIsMouseDown0() {return ImGui::IsMouseDown(0);}
bool Diwne::bypassIsMouseDown1() {return ImGui::IsMouseDown(1);}
bool Diwne::bypassIsMouseDown2() {return ImGui::IsMouseDown(2);}
bool Diwne::bypassIsMouseClicked0(){return ImGui::IsMouseClicked(0);}
bool Diwne::bypassIsMouseClicked1(){return ImGui::IsMouseClicked(1);}
bool Diwne::bypassIsMouseClicked2(){return ImGui::IsMouseClicked(2);}
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
float Diwne::bypassGetZoomDelta() {return bypassGetMouseWheel()/mp_settingsDiwne->zoomWheelReverseSenzitivity;}

bool Diwne::allowProcessZoom(){return true;}
bool Diwne::bypassZoomAction() {return diwne.bypassGetZoomDelta() != 0;}
bool Diwne::bypassDiwneSetPopupPositionAction() {return bypassIsMouseClicked1();}
ImVec2 Diwne::bypassDiwneGetPopupNewPositionAction() {return bypassGetMousePos();}

ImRect Diwne::getSelectionRectangleDiwne(){return m_selectionRectangeDiwne;}

} /* namespace DIWNE */
