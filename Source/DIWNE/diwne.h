#ifndef DIWNE_H
#define DIWNE_H

#include "diwne_include.h"

/* ================================================== */
/* ===== I M P O R T A N T  (TRICKY)  N O T E S ===== */
/* ================================================== */
/* * 1) Node on top has to be drawn two times (first and last) - first for catch interaction, last for draw it on top of other elements


*/



/* ImDrawList functions works in screen coordinates */
namespace DIWNE
{

typedef std::function<void(...)> popupContent_function_pointer;

/* ===================== */
/* ===== p o p u p ===== */
/* ===================== */
static bool s_popupContentDrawn = false; /* only one popup content per frame (avoid double-content on DragFloat popup) */

template<typename T, std::enable_if<std::is_base_of<DiwneObject, T>::value, bool>::type = true >
static void expandPopupContent(T& object) /* for popupContent() functions of member of class */
{
    object.popupContent();
}

template <typename... Args>
static bool popupDiwne(std::string const popupID, ImVec2 const & popupPos, void (*popupContent)(Args...), Args&&... args) {
    bool interaction_happen = false;

    if(ImGui::IsPopupOpen(popupID.c_str()))
    {
        ImGui::SetNextWindowPos(popupPos);
        if (ImGui::BeginPopup(popupID.c_str()))
        {
            interaction_happen = true;

            popupContent(std::forward<Args>(args)...);

            ImGui::EndPopup();
        }
    }
    return interaction_happen || s_popupContentDrawn;
}

struct SettingsDiwne
{
    DIWNE::ID const editorId = 0;
    std::string const editorlabel = "diwneBackground";
    ImRect const workAreaDiwne = ImRect(0,0,0,0); /* only initial value - mostly based on window size ( see updateWorkAreaRectangles() ) */

    float minWorkAreaZoom = 0.25;
    float maxWorkAreaZoom = 4;
    float workAreaInitialZoom = 1;
    float zoomWheelReverseSenzitivity = 8; /* Higher number -> smaller change, can not be 0 */

    ImVec2 initPopupPosition = ImVec2(0,0);

    ImColor selectionRectFullColor = ImColor(0,255,0,100);
    ImColor selectionRectTouchColor = ImColor(0,0,255,100);

    ImColor itemSelectedBorderColor = ImColor(100,100,0,255);
    float itemSelectedBorderThicknessDiwne = 5;

    ImColor nodeHoveredBorderColor = ImColor(100,100,0,255);
    float nodeHoveredBorderThicknessDiwne = 2;
    float middleAlign = 0.5;

    ImColor pinHoveredBorderColor = ImColor(100,100,0,255);
    float pinHoveredBorderThicknessDiwne = 2;

    ImColor backgroundHoveredBorderColor = ImColor(100,100,0,255);
    float backgroundHoveredBorderThicknessDiwne = 2;

    float linkInteractionWidthDiwne = 10;
    float linkThicknessDiwne = 10;
    float linkThicknessSelectedBorderDiwne = 4;
    ImColor linkColor = ImColor(150,75,100,150);
    ImColor linkColorSelected = ImColor(150,75,100,200);
    ImVec2 linkStartControlOffsetDiwne = ImVec2(10,0);
    ImVec2 linkEndControlOffsetDiwne = ImVec2(-10,0);
    float linkAlphaHovered = 1;
    float linkAlpha = 0.5;
    float linkAlphaSelectedHovered = 1;
    float linkAlphaSelected = 0.5;
};
// \todo JH void setMiddleAlign(float v) {assert(v>=0 && v<=1); m_middleAlign = v;}; /* from 0==left to 1==right */
//         void setLinkControlpointsOffsetDiwne(const ImVec2 controlStart, const ImVec2 controlEnd) { m_startControlOffsetDiwne = controlStart; m_endControlOffsetDiwne = controlEnd; };


/* ===================== */
/* ===== D i w n e ===== */
/* ===================== */
class Diwne : public DiwneObject
{
    public:
#ifdef DIWNE_DEBUG
        bool m_diwneDebug_on = true;
#endif // DIWNE_DEBUG
        /** Default constructor */
        Diwne(DIWNE::SettingsDiwne* settingsDiwne);

        /** Default destructor */
        virtual ~Diwne(){};
        DIWNE::DiwneAction getHoldActionType() const final {return DiwneAction::HoldWorkarea;};
        DIWNE::DiwneAction getDragActionType() const final {return DiwneAction::DragWorkarea;};


        virtual bool initializeDiwne();
        virtual bool allowDrawing();
        virtual bool beforeBeginDiwne();
        virtual void begin();
        virtual bool afterContentDiwne();
        virtual void end();
        virtual bool afterEndDiwne();
        virtual bool processInteractionsDiwne();
        virtual bool finalizeDiwne();

        bool blockPopup();
        virtual bool allowInteraction();

        virtual ImRect getRectDiwne() const {return getWorkAreaDiwne();};

        virtual bool processHold();
        virtual bool processUnhold();
        virtual bool processDrag();

        void updateWorkAreaRectangles(); /*! \brief Update position and size of work area on screen and on diwne */

        ImRect getWorkAreaDiwne() const {return m_workAreaDiwne;};
        ImRect getWorkAreaScreen() const {return m_workAreaScreen;};
        float getWorkAreaZoom() const {return m_workAreaZoom;};
        void setWorkAreaZoom(float val=1);

        ImVec2 const & getPopupPosition() const {return m_popupPosition;};
        void setPopupPosition(ImVec2 position) {m_popupPosition = position;};

        void translateWorkAreaDiwneZoomed(ImVec2 const &distance);
        void translateWorkAreaDiwne(ImVec2 const &distance);

        ImVec2 transformFromImGuiToDiwne(const ImVec2& point) const;
        ImVec2 transformFromDiwneToImGui(const ImVec2& point) const;
        ImVec4 transformFromImGuiToDiwne(const ImVec4& point) const;
        ImVec4 transformFromDiwneToImGui(const ImVec4& point) const;

        ImVec2 screen2workArea(const ImVec2 & point) const;
        ImVec2 workArea2screen(const ImVec2 & point) const;
        ImVec2 diwne2workArea(const ImVec2 & point) const;
        ImVec2 workArea2diwne(const ImVec2 & point) const;
        ImVec2 screen2diwne(const ImVec2 & point) const;
        ImVec2 diwne2screen(const ImVec2 & point) const;
        ImVec2 diwne2workArea_noZoom(const ImVec2 & point) const;
        ImVec2 workArea2diwne_noZoom(const ImVec2 & point) const;
        ImVec2 screen2diwne_noZoom(const ImVec2 & point) const;
        ImVec2 diwne2screen_noZoom(const ImVec2 & point) const;

        void AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All) const;
        void AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All, float thickness = 1.0f) const;
        void AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0) const;

        // padding - top, right, bottom, left
        bool IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                         DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor,
                         ImVec2 size, ImVec4 padding, bool filledm, std::string const id) const;

        // padding - top, right, bottom, left
        void DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                     DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor,
                     ImVec2 size, ImVec4 padding, bool filled) const;


        void DrawIconCircle(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const;
        void DrawIconRectangle(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const;
        void DrawIconTriangleLeft(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const;
        void DrawIconTriangleRight(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const;
        void DrawIconCross(ImDrawList* idl,
                      ImColor ShapeColor, ImColor InnerColor,
                      ImVec2 topLeft, ImVec2 bottomRight, bool filled ) const;


        DiwneAction getDiwneAction() const {return m_diwneAction;};
        void setDiwneAction(DiwneAction action){m_diwneAction = action;};

        DiwneAction getDiwneActionPreviousFrame() const {return m_diwneAction_previousFrame;};

        DIWNE::Link& getHelperLink(){return m_helperLink;};

        template <typename T>
        std::shared_ptr<T> getLastActivePin()
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            return std::dynamic_pointer_cast<T>(mp_lastActivePin);
        }

        template <typename T>
        void setLastActivePin(std::shared_ptr<T> pin)
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            mp_lastActivePin = pin;
        }

        template <typename T>
        std::shared_ptr<T> getLastActiveNode()
        {
            static_assert(std::is_base_of_v<DIWNE::Node, T>/* || std::is_same<T, std::nullptr_t>::value*/, "Node must be derived from DIWNE::Node class.");
            return std::dynamic_pointer_cast<T>(mp_lastActiveNode);
        }

        template <typename T>
        void setLastActiveNode(std::shared_ptr<T> node)
        {
            static_assert(std::is_base_of_v<DIWNE::Node, T>/* || std::is_same<T, std::nullptr_t>::value*/, "Node must be derived from DIWNE::Node class.");
            mp_lastActiveNode = node;
        }

        void setNodesSelectionChanged(bool value){m_nodesSelectionChanged = value;};
        bool getNodesSelectionChanged(){return m_nodesSelectionChanged;};

        virtual bool bypassIsItemClicked0();
        virtual bool bypassIsItemClicked1();
        virtual bool bypassIsItemClicked2();
        virtual bool bypassIsMouseDown0();
        virtual bool bypassIsMouseDown1();
        virtual bool bypassIsMouseDown2();
        virtual ImVec2 bypassMouseClickedPos0();
        virtual ImVec2 bypassMouseClickedPos1();
        virtual ImVec2 bypassMouseClickedPos2();
        virtual bool bypassIsMouseClicked0();
        virtual bool bypassIsMouseClicked1();
        virtual bool bypassIsMouseClicked2();
        virtual bool bypassIsMouseReleased0();
        virtual bool bypassIsMouseReleased1();
        virtual bool bypassIsMouseReleased2();
        virtual bool bypassIsItemActive();
        virtual bool bypassIsMouseDragging0();
        virtual bool bypassIsMouseDragging1();
        virtual bool bypassIsMouseDragging2();
        virtual ImVec2 bypassGetMouseDragDelta0();
        virtual ImVec2 bypassGetMouseDragDelta1();
        virtual ImVec2 bypassGetMouseDragDelta2();
        virtual ImVec2 bypassGetMouseDelta();
        virtual ImVec2 bypassGetMousePos();
        virtual float bypassGetMouseWheel();
        virtual float bypassGetZoomDelta();

        virtual bool bypassZoomAction();
        virtual bool bypassDiwneSetPopupPositionAction();
        virtual ImVec2 bypassDiwneGetPopupNewPositionAction();
        virtual bool bypassSelectionRectangleAction();
        ImRect getSelectionRectangleDiwne();
        ImVec2 bypassDiwneGetSelectionRectangleStartPosition();
        ImVec2 bypassDiwneGetSelectionRectangleSize();

        virtual bool processZoom();
        virtual bool processDiwneZoom();
        virtual bool processSelectionRectangle();
        virtual bool processFocused();
        virtual bool processFocusedForInteraction();



        DIWNE::SettingsDiwne* mp_settingsDiwne;

        bool m_popupDrawn, m_tooltipDrawn, m_objectFocused;


    protected:
        DiwneAction m_diwneAction, m_diwneAction_previousFrame;

        std::shared_ptr<DIWNE::Pin> mp_lastActivePin;
        std::shared_ptr<DIWNE::Node> mp_lastActiveNode;

        DIWNE::Link m_helperLink; /* for showing link that is in process of creating */

        bool m_nodesSelectionChanged;
        ImRect m_selectionRectangeDiwne;

        ImColor m_selectionRectangeTouchColor, m_selectionRectangeFullColor;

        bool m_drawing, m_interactionAllowed;


    private:
    ImRect m_workAreaScreen;     /*! \brief Rectangle of work area on screen */
    ImRect m_workAreaDiwne;  /*! \brief Rectangle of work area on diwne - .Min is set by user, .Max is computed from m_workAreaScreen */
    float m_workAreaZoom;

    ImVec2 m_popupPosition;



    /* restore information */
    ImVec2 m_StoreItemSpacing;

};


/* >>>> STATIC FUNCTIONS <<<< */
extern bool putInvisibleButtonUnder(std::string const imguiID, ImVec2 const &size);

} /* namespace DIWNE */

#endif // DIWNE_H
