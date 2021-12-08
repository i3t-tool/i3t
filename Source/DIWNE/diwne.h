#ifndef DIWNE_H
#define DIWNE_H


#include "diwne_include.h"


/* ImDrawList functions works in screen coordinates */
namespace DIWNE
{

typedef unsigned int ID;

typedef std::function<void(...)> popupContent_function_pointer;

enum IconType
{
    Circle,
    Rectangle,
    TriangleLeft,
    TriangleRight,
    Cross
};

enum MouseLocation /* \todo maybe unused with current zoom politics */
{
    ImGuiLocation,
    DiwneLocation
};

enum DiwneAction
{
    None,
    NewLink,
    HoldNode,
    DragNode
};

struct SettingsDiwne
{
    ImRect workAreaDiwne = ImRect(0,0,0,0); /* only .Min is really used - .Max is based on size of window */
    float workAreaInitialZoomDiwne = 1;
    float zoomWheelSenzitivity = 8; /* Higher number -> smaller change */
    float minWorkAreaZoom = 0.25;
    float maxWorkAreaZoom = 4;
//    FloatPopupMode floatPopupMode = Radians;
};

class Diwne
{
    public:
        /** Default constructor */
        Diwne(DIWNE::SettingsDiwne const & settingsDiwne, void *customData);
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

        void updateWorkAreaRectangles(); /*! \brief Update position and size of work area on screen and on diwne */

        ImRect getWorkAreaDiwne() const {return m_workAreaDiwne;};
        ImRect getWorkAreaScreen() const {return m_workAreaScreen;};
        float getWorkAreaZoomDiwne() const {return m_workAreaZoomDiwne;};
        void setWorkAreaZoomDiwne(float val=1);
        bool getBackgroudPopupRaise() const {return m_backgroundPopupRaise;};


        float getWorkAreaZoomDeltaDiwne() const {return m_workAreaZoomDeltaDiwne; };
        void setWorkAreaZoomDeltaDiwne(float change=1) { m_workAreaZoomDeltaDiwne = change; };





        ImVec2 getPopupPosition() const {return m_popupPosition;};
        void setPopupPosition(ImVec2 position) {m_popupPosition = position;};

        void translateWorkAreaDiwneZoomed(ImVec2 const &distance);
        void translateWorkAreaDiwne(ImVec2 const &distance);

        void transformMouseFromImGuiToDiwne();
        void transformMouseFromDiwneToImGui();

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

        virtual void popupBackgroundContent(){ImGui::Text("bgPopupDebug");};



        template <typename... Args>
        bool popupDiwneItem(std::string const popupIDstring, void (*popupContent)(Args...), Args&&... args) {
            bool interaction_happen = false;

            ImGui::SetNextWindowPos(getPopupPosition());
            if (ImGui::BeginPopupContextItem(popupIDstring.c_str()))
            {
                interaction_happen = true;
                /* Popup is new window so MousePos and MouseClickedPos is from ImGui, not from (zoomed) diwne */
                //transformMouseFromDiwneToImGui();

                popupContent(std::forward<Args>(args)...);

                ImGui::EndPopup();
            }
            return interaction_happen;
        }

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

        DiwneAction getPreviousFrameDiwneAction() const {return m_previousFrameDiwneAction;};

        DIWNE::Link& getHelperLink(){return m_helperLink;};

        template <typename T>
        T* getLastActivePin()
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            return dynamic_cast<T*>(m_lastActivePin);
        }

        //DIWNE::Pin* getActivePin() const {return m_activePin;};

        template <typename T>
        void setLastActivePin(T* pin)
        {
            static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
            m_lastActivePin = pin;
        }
        //void setActivePin(DIWNE::Pin* pin){m_activePin = pin;};

        void * m_customData;

        void showTooltipLabel(std::string label, ImColor color);

        std::shared_ptr<DIWNE::Node> m_draged_hold_node; /* \todo JH make protected */


        void setNodesSelectionChanged(bool value){m_nodesSelectionChanged = value;};
        bool getNodesSelectionChanged(){return m_nodesSelectionChanged;};

        virtual bool bypassItemClicked0();
        virtual bool bypassItemClicked1();
        virtual bool bypassItemClicked2();
        virtual bool bypassIsMouseDown0();
        virtual bool bypassIsMouseDown1();
        virtual bool bypassIsMouseDown2();
        virtual ImVec2 bypassMouseClickedPos0();
        virtual ImVec2 bypassMouseClickedPos1();
        virtual ImVec2 bypassMouseClickedPos2();
        virtual bool bypassIsMouseReleased0();
        virtual bool bypassIsMouseReleased1();
        virtual bool bypassIsMouseReleased2();
        virtual bool bypassIsItemActive();
        virtual bool bypassIsMouseDragging0();
        virtual bool bypassIsMouseDragging1();
        virtual bool bypassIsMouseDragging2();
        virtual ImVec2 bypassGetMouseDelta();
        virtual ImVec2 bypassGetMousePos();
        virtual float bypassGetMouseWheel();



    protected:
        bool m_inner_interaction_happen = false;
        DiwneAction m_diwneAction = None, m_previousFrameDiwneAction = None;

        DIWNE::Pin* m_lastActivePin = nullptr;
        DIWNE::Link m_helperLink; /* for showing link that is in process of creating */

        bool m_nodesSelectionChanged = false;


    private:
    ImRect m_workAreaScreen;     /*! \brief Rectangle of work area on screen */
    ImRect m_workAreaDiwne;  /*! \brief Rectangle of work area on diwne - .Min is set by user, .Max is computed from m_workAreaScreen */
    float m_workAreaZoomDiwne, m_workAreaZoomDeltaDiwne;
    float m_zoomWheelSenzitivity; /* Higher number -> smaller change */
    float m_minWorkAreaZoom, m_maxWorkAreaZoom;

    bool m_backgroundPopupRaise = false;

    ImVec2 m_popupPosition;
    MouseLocation m_mouseLocation;


    /* restore information */
    ImVec2 m_StoreItemSpacing;

};


/* >>>> STATIC FUNCTIONS <<<< */
extern bool putInvisibleButtonUnder(std::string const imguiID, ImVec2 const &size);

static void expandPopupBackgroundContent(DIWNE::Diwne &this_object)
{
    this_object.popupBackgroundContent();
}


} /* namespace DIWNE */

#endif // DIWNE_H
