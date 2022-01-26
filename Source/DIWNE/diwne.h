#ifndef DIWNE_H
#define DIWNE_H

#include "diwne_include.h"


/* ImDrawList functions works in screen coordinates */
namespace DIWNE
{

typedef std::function<void(...)> popupContent_function_pointer;

enum IconType
{
    Circle,
    Rectangle,
    TriangleLeft,
    TriangleRight,
    Cross
};

enum DiwneAction
{
    None,
    NewLink,
    HoldNode,
    DragNode,
    HoldPin,
    DragPin,
    HoldLink,
    DragLink,
    HoldWorkarea,
    DragWorkarea,
    SelectionRectFull,
    SelectionRectTouch
};

static float s_linkInteractionWidth;

/* ===================== */
/* ===== p o p u p ===== */
/* ===================== */
template<typename T>
//typename std::enable_if<std::is_base_of<DiwneObject, T>::value>::type>
static void expandPopupContent(T& object) /* for memeber popupContent() functions */
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
            //expandPopupContent(std::forward<Args>(args)...);

            ImGui::EndPopup();
        }
    }
    return interaction_happen;
}


struct SettingsDiwne
{
    DIWNE::ID editorId = 0;
    std::string const editorlabel = "diwneBackground";

    ImRect workAreaDiwne = ImRect(0,0,0,0); /* only initial value */

    float minWorkAreaZoom = 0.25;
    float maxWorkAreaZoom = 4;
    float workAreaInitialZoom = 1;

    float zoomWheelReverseSenzitivity = 8; /* Higher number -> smaller change, can not be 0 */

    float linkInteractionWidth = 10;
    ImVec2 initPopupPosition = ImVec2(0,0);

    ImColor selectionRectFullColor = ImColor(0,255,0,100);
    ImColor selectionRectTouchColor = ImColor(0,0,255,100);
};

/* ===================== */
/* ===== D i w n e ===== */
/* ===================== */
class Diwne : public DiwneObject
{
    public:
        /** Default constructor */
        Diwne(DIWNE::SettingsDiwne const & settingsDiwne);

        /** Default destructor */
        virtual ~Diwne(){};

        virtual bool initializeDiwne();
        virtual bool beforeBeginDiwne();
        virtual void begin();
        virtual bool afterContentDiwne();
        virtual void end();
        virtual bool afterEndDiwne();
        virtual bool finalizeDiwne();

        virtual bool processHold();
        virtual bool processUnhold();
        virtual bool processDrag();

        void updateWorkAreaRectangles(); /*! \brief Update position and size of work area on screen and on diwne */

        ImRect getWorkAreaDiwne() const {return m_workAreaDiwne;};
        ImRect getWorkAreaScreen() const {return m_workAreaScreen;};
        float getWorkAreaZoom() const {return m_workAreaZoom;};
        void setWorkAreaZoom(float val=1);


        //float getWorkAreaZoomDeltaDiwne() const {return m_workAreaZoomDeltaDiwne; };


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

//        bool popupFloatDiwne(DIWNE::Diwne &diwne, std::string const popupIDstring, float& selectedValue, bool& valueSelected);
//        virtual void popupFloatContent(float& selectedValue, bool& valueSelected);

        //virtual void popupContent(){if(ImGui::MenuItem("DiwneBGPopup")){}};

//        template <typename... Args>
//        bool popupDiwneItem(std::string const popupID, void (*popupContent)(Args...), Args&&... args) {
//            bool interaction_happen = false;
//
//            if(ImGui::IsPopupOpen(popupID.c_str()))
//            {
//                ImGui::SetNextWindowPos(getPopupPosition()); /* if popup position will be passed as argument, function can be static */
//                if (ImGui::BeginPopup(popupID.c_str()))
//                {
//                    interaction_happen = true;
//
//                    popupContent(std::forward<Args>(args)...);
//
//                    ImGui::EndPopup();
//                }
//            }
//            return interaction_happen;
//        }

//        template <typename... Args>
//        bool popupDiwneRaise(std::string const popupIDstring, void (*popupContent)(Args...), Args&&... args) {
//            bool interaction_happen = false;
//
//            ImGui::OpenPopup(popupIDstring.c_str());
//            ImGui::SetNextWindowPos(getPopupPosition());
//            if (ImGui::BeginPopup(popupIDstring.c_str()))
//            {
//                interaction_happen = true;
//                /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not from (zoomed) diwne */
//                transformMouseFromDiwneToImGui();
//
//                popupContent(std::forward<Args>(args)...);
//
//                ImGui::EndPopup();
//            }
//            return interaction_happen;
//        }

        DiwneAction getDiwneAction() const {return m_diwneAction;};
        void setDiwneAction(DiwneAction action){m_diwneAction = action;};

        DiwneAction getDiwneActionPreviousFrame() const {return m_diwneAction_previousFrame;};

        DIWNE::Link& getHelperLink(){return m_helperLink;};

        template <typename T>
        T* getLastActivePin()
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            return dynamic_cast<T*>(m_lastActivePin);
        }

        template <typename T>
        void setLastActivePin(T* pin)
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            m_lastActivePin = pin;
        }

        std::shared_ptr<DIWNE::Node> m_draged_hold_node; /* \todo JH make protected */


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

        virtual bool bypassRaisePopupAction();


    protected:
        DiwneAction m_diwneAction, m_diwneAction_previousFrame;

        DIWNE::Pin* m_lastActivePin;
        DIWNE::Link m_helperLink; /* for showing link that is in process of creating */

        bool m_nodesSelectionChanged;
        ImRect m_selectionRectangeDiwne;

        ImColor m_selectionRectangeTouchColor, m_selectionRectangeFullColor;


    private:
    ImRect m_workAreaScreen;     /*! \brief Rectangle of work area on screen */
    ImRect m_workAreaDiwne;  /*! \brief Rectangle of work area on diwne - .Min is set by user, .Max is computed from m_workAreaScreen */
    float m_workAreaZoom/*, m_workAreaZoomDeltaDiwne*/;
    float m_zoomWheelReverseSenzitivity; /* Higher number -> smaller change */
    float m_minWorkAreaZoom, m_maxWorkAreaZoom;

    ImVec2 m_popupPosition;

    /* restore information */
    ImVec2 m_StoreItemSpacing;

};


/* >>>> STATIC FUNCTIONS <<<< */
extern bool putInvisibleButtonUnder(std::string const imguiID, ImVec2 const &size);

} /* namespace DIWNE */

#endif // DIWNE_H
