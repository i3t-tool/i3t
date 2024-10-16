/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <functional>

#include "DiwneObject.h"

// DIWNE Refactoring todos:
// TODO: (DR) Seperate Icon drawing into its own class
// TODO: (DR) Refactor and revise the whole bypass/input/action trigger system thats going on here

namespace DIWNE
{
/** \brief Storage of all values
 */
struct SettingsDiwne
{
	DIWNE::ID const editorId = 0; /**< as well as all other DiwneObject, Diwne has to have id too */
	std::string const editorlabel = "diwneBackground"; /**< as well as all other DiwneObject, Diwne has to
	                                                      have identification label too */
	ImRect const workAreaDiwne = ImRect(0, 0, 0, 0); /**< workarea in Diwne coordinates (so what part of infinite space
	                                                    of node editor is on window) only initial value - mostly based
	                                                    on window size ( \see updateWorkAreaRectangles() ) */

	float minWorkAreaZoom = 0.25;          /**< minimal value of zoom */
	float maxWorkAreaZoom = 4;             /**< maximal value of zoom */
	float workAreaInitialZoom = 1;         /**< initial value of zoom */
	float zoomWheelReverseSenzitivity = 8; /**< Higher number -> smaller change, can not be 0 */
	float selectionRounding = 0;           /**< rounding od selection */

	ImVec2 initPopupPosition = ImVec2(0, 0); /**< where to show popup when not set later */

	ImVec4 selectionRectFullColor = ImVec4(0.0, 0.0, 1.0, 0.35);
	ImVec4 selectionRectTouchColor = ImVec4(0.0, 1.0, 0.0, 0.35);

	ImVec4 itemSelectedBorderColor = ImVec4(1.0, 0.9, 0.4, 0.6);
	float itemSelectedBorderThicknessDiwne = 2.5;

	ImVec4 objectFocusBorderColor = ImVec4(0.0, 0.0, 0.0, 0.20);
	float objectFocusBorderThicknessDiwne = 1.5;
	ImVec4 objectFocusForInteractionBorderColor = ImVec4(0.0, 0.0, 0.0, 0.20);
	float objectFocusForInteractionBorderThicknessDiwne = 1.5;

	float middleAlign = 0.5; /**< value < 0 , 1 > where is horizontal position of
	                            middle of nodes */

	ImVec4 pinHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float pinHoveredBorderThicknessDiwne = 2;
	ImVec4 nodeHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float nodeHoveredBorderThicknessDiwne = 2;
	ImVec4 backgroundHoveredBorderColor = ImVec4(0.35, 0.35, 0.0, 1.0);
	float backgroundHoveredBorderThicknessDiwne = 2;

	float linkInteractionWidthDiwne = 10;
	float linkThicknessDiwne = 10;
	float linkThicknessSelectedBorderDiwne = 4;
	ImVec4 linkColor = ImVec4(0.6, 0.3, 0.35, 0.6);
	ImVec4 linkColorSelected = ImVec4(0.6, 0.3, 0.35, 0.8);
	ImVec2 linkStartControlOffsetDiwne = ImVec2(10, 0);
	ImVec2 linkEndControlOffsetDiwne = ImVec2(-10, 0);
	float linkAlphaHovered = 1;
	float linkAlpha = 0.2;
	float linkAlphaSelectedHovered = 1;
	float linkAlphaSelected = 0.5;
	ImVec4 fontColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); ///< Color of the text in the node
};
// \todo   void setMiddleAlign(float v) {assert(v>=0 && v<=1); m_middleAlign =
// v;}; /* from 0==left to 1==right */
//         void setLinkControlpointsOffsetDiwne(const ImVec2 controlStart, const
//         ImVec2 controlEnd) { m_startControlOffsetDiwne = controlStart;
//         m_endControlOffsetDiwne = controlEnd; };

/**
 * \brief pin icon shape type
 * used in WorkspaceCorePin::content(), file WorkspaceElementWitCoreData.cpp::379
 * drawn by Diwne::DrawIcon, method Diwne::DrawIconXXXX, file Diwne.cpp
 */
enum IconType : unsigned int
{
	NoIcon,
	Circle,
	Rectangle,
	TriangleLeft,
	TriangleRight,
	TriangleDownLeft,
	TriangleDownRight,
	GrabDownLeft,
	GrabDownRight,
	Cross,
	Hyphen,
	Stop,         ///< Black Square For Stop (U+23F9)
	Pause,        ///< Double Vertical Bar (U+23F8)
	SkipBack,     ///< |< vertical bar followed by the left arrow
	SkipBack2,    ///< "<|" left arrow followed by the vertical bar
	SkipForward,  ///< ">|" right arrow followed by the vertical bar
	SkipForward2, ///< |> vertical bar followed by the right arrow
	Rewind,       ///< Black Left-Pointing Double Triangle (U+23EA)
	FastForward,  ///< Black Right-Pointing Double Triangle (U+23E9)
	AtFrom,       // todo, now a synonym to the FastForward
	AtTo,         // todo, now a synonym to the Rewind
};

class Node;
class Pin;
class Link;

/**
 * \brief The node editor object
 * In ancestor of this object you will probably store your nodes (links, pins)
 * It store inter-object interactions
 * Every DiwneObject has reference to this object
 */
class NodeEditor : public DiwneObject
{
public:
#if DIWNE_DEBUG_ENABLED
	bool m_diwneDebug = false;
	bool m_diwneDebugExtra1 = false;
	bool m_diwneDebugExtra2 = false;
#endif
	bool m_takeSnap{false}; // TODO: Rename or at least add documentation,
	                        //  this feature shouldn't be specific to our undo/redo system if it were to remain here

	////////////////////////////////////////////

	DIWNE::SettingsDiwne* mp_settingsDiwne;

	/// not draw popup two times \todo maybe unused when every object is drawn just one time
	bool m_popupDrawn{false};
	/// not draw tooltip two times \todo maybe unused when every object is drawn just one time
	bool m_tooltipDrawn{false};
	/// only one object can be focused
	/// TODO: A flag indicating that an object has been focused that frame?
	///  Review this flag, its hard to track, why not keep track of the "focused" object instead
	///  Also still yet to exactly estabilish what "focused" really means
	bool m_objectFocused{false};
	/// for example when holding ctrl nodes not going unselected when sleection rect get out of them
	bool m_allowUnselectingNodes{false};

protected:
	// TODO: Review if we could keep DiwneAction info in Context?
	//  Really begs the question where are we okay to use diwne member variables
	//  and where it might be better to use method parameters / returns rather than a
	//  "global" member context.
	DiwneAction m_diwneAction{DiwneAction::None}, m_diwneAction_previousFrame{DiwneAction::None};

	std::shared_ptr<DIWNE::Pin> mp_lastActivePin;
	std::shared_ptr<DIWNE::Node> mp_lastActiveNode;

	std::unique_ptr<DIWNE::Link> m_helperLink; /**< for showing link that is in process of creating */

	bool m_nodesSelectionChanged{false};

	ImRect m_selectionRectangeDiwne{0, 0, 0, 0};
	ImColor m_selectionRectangeTouchColor, m_selectionRectangeFullColor;

	bool m_drawing /**< store whether ImGui window is ready to draw to it */;

protected:
	ImRect m_workAreaScreen; ///< Rectangle of work area on screen
	ImRect m_workAreaDiwne;  ///< Rectangle of work area on diwne
	                         ///< - .Min is set by user, .Max is computed from m_workAreaScreen */
	float m_workAreaZoom;    ///< Workspace zoom factor

	ImVec2 m_popupPosition;

	ImDrawListSplitter m_splitter; ///< Every nodes should be draw to its own channel

	/* zoom scaling restore information */
	bool m_zoomScalingApplied = false;

	float m_zoomOriginalFontScale; ///< For restore value after this window is done
	ImGuiStyle m_zoomOriginalStyle;

public:
	/** Default constructor */
	NodeEditor(DIWNE::SettingsDiwne* settingsDiwne);

	/** Default destructor */
	virtual ~NodeEditor();

	/** Clear all nodes from the node editor */
	virtual void clear();

	void draw(DrawMode drawMode = DrawMode::Interacting) override;

	FrameContext& getFrameContext() const;
	void resetFrameContext(DrawMode drawMode);

	DIWNE::DiwneAction getHoldActionType() const final
	{
		return DiwneAction::HoldWorkarea;
	};
	DIWNE::DiwneAction getDragActionType() const final
	{
		return DiwneAction::DragWorkarea;
	};
	DIWNE::DiwneAction getTouchActionType() const final
	{
		return DiwneAction::TouchWorkarea;
	};

	bool allowDrawing() override;

	void initializeDiwne(FrameContext& context) override;

	void begin(FrameContext& context) override;
	void content(FrameContext& context) override;
	void end(FrameContext& context) override;

	void finalizeDiwne(FrameContext& context) override;

	bool blockRaisePopup(); /**< sometimes we do not want to raise popup - here specify
	                           it ( now it is when selecting action run ) */

	ImRect getRectDiwne() const override
	{
		return getWorkAreaDiwne();
	};

	//	bool processDrag() override;
	//
	//	bool processInteractions() override;

	void updateWorkAreaRectangles(); /** \brief Update position and size of work
	                                    area on screen and on diwne */

	ImRect getWorkAreaDiwne() const
	{
		return m_workAreaDiwne;
	};
	void setWorkAreaDiwne(ImRect rect)
	{
		m_workAreaDiwne = rect;
	};

	ImRect getWorkAreaScreen() const
	{
		return m_workAreaScreen;
	};

	ImVec2 const& getPopupPosition() const
	{
		return m_popupPosition;
	};
	void setPopupPosition(ImVec2 position)
	{
		m_popupPosition = position;
	};
	void translateWorkAreaDiwneZoomed(ImVec2 const& distance);
	void translateWorkAreaDiwne(ImVec2 const& distance);

	ImVec2 transformFromImGuiToDiwne(const ImVec2& point) const;
	ImVec2 transformFromDiwneToImGui(const ImVec2& point) const;
	ImVec4 transformFromImGuiToDiwne(const ImVec4& point) const;
	ImVec4 transformFromDiwneToImGui(const ImVec4& point) const;

	ImVec2 screen2workArea(const ImVec2& point) const;
	ImVec2 workArea2screen(const ImVec2& point) const;
	ImVec2 diwne2workArea(const ImVec2& point) const;
	ImVec2 workArea2diwne(const ImVec2& point) const;
	ImVec2 screen2diwne(const ImVec2& point) const;
	ImVec2 diwne2screen(const ImVec2& point) const;
	ImVec2 diwne2workArea_noZoom(const ImVec2& point) const;
	ImVec2 workArea2diwne_noZoom(const ImVec2& point) const;
	ImVec2 screen2diwne_noZoom(const ImVec2& point) const;
	ImVec2 diwne2screen_noZoom(const ImVec2& point) const;

	// TODO: Maybe move the drawing methods into a class of some sort
	//  IconDrawing is related to this as well, could be put under some kind of a Drawer/Renderer helper class

	void AddLine(const ImVec2& p1, const ImVec2& p2, ImVec4 col, float thickness = 1.0f, bool ignoreZoom = false) const;

	/**
	 * Draw filled rectangle to window ImDrawlist
	 * @param p_min bottom left corner in diwne coords
	 * @param p_max top right corner in diwne coords
	 * @param col
	 * @param rounding
	 * @param rounding_corners
	 */
	void AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding = 0.0f,
	                        ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, bool ignoreZoom = false) const;

	/**
	 * Draw rectangle to window ImDrawlist
	 * \see AddRectFilledDiwne
	 */
	void AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding = 0.0f,
	                  ImDrawFlags rounding_corners = ImDrawFlags_RoundCornersAll, float thickness = 1.0f,
	                  bool ignoreZoom = false) const;

	/** \brief Draw Bezier (not Bezier really) curve to window ImDrawList
	 *
	 * \param p1 const ImVec2& start point in diwne coords
	 * \param p2 const ImVec2& start control point in diwne coords
	 * \param p3 const ImVec2& end control point in diwne coords
	 * \param p4 const ImVec2& end point in diwne coords
	 * \param col ImVec4
	 * \param thickness float
	 * \param 0 int num_segments with zero ImGui use some optimal? value
	 * \return void
	 *
	 */
	void AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImVec4 col,
	                         float thickness, int num_segments = 0) const;

	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size, ImVec4 padding,
	                bool filled, std::string const id) const;
	/**
	 * \brief Just the shape of the button, no interaction
	 * \param size
	 * \param color
	 * \param rounding
	 */
	void EmptyButton(ImVec2 size, ImColor color, float rounding);

	/**
	 * Add ImGui Button with icon on it \see DrawIcon()
	 * \return true if interaction with button happen, false otherwise
	 */
	bool IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, DIWNE::IconType fgIconType,
	                ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
	                std::string const id) const;

	// padding - top, right, bottom, left

	/**
	 * \brief Draw an Icon combined from two parts (foreground and background)
	 * to the window \a ImDrawList filled with a \a ShapeColor.
	 * When \a filled == true, both shapes have a border.
	 * Then, the border color is the ShapeColor and the shape is filled with the
	 * \a InnerColor
	 *
	 * \param bgIconType background shape (typically a Rectangle)
	 * \param bgShapeColor color of the background shape (or a border color if not filled)
	 * \param bgInnerColor color of the background shape interior if filled == false
	 * \param fgIconType foreground shape (typically a Triangle or Cross)
	 * \param fgShapeColor color of the foreground shape if filled == false
	 * \param fgInnerColor color of foreground shape interior if filled == false
	 * \param size of the icon in screen coordinates
	 * \param padding of the fg shape (left, bottom, right, top)
	 * \param filled means
	 *  - true fill both shapes with their ShapeColor
	 *  - false draw both shapes with a ShapeColor border and fill them with InnerColor
	 * \return void
	 */
	void DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, DIWNE::IconType fgIconType,
	              ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size, ImVec4 padding, bool filled,
	              ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	/**
	 * \brief Draw a circle icon
	 * \param idl ImDrawList* where to draw
	 * \param shapeColor color of the whole shape (border line if not filled)
	 * \param innerColor color of the inner part (middle) of shape
	 * \param topLeft position of the icon in screen coords
	 * \param bottomRight of the icon in screen coords
	 * \param filled false means use the InnerColor in the foreground Shape
	 * \param thickness of the border for zoomFactor = 1.0
	 */
	void DrawIconCircle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 1) const;

	void DrawIconRectangle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                       bool filled, ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	void DrawIconPause(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                   bool filled, ImVec2 thickness = ImVec2(1, 1), float rounding = 0) const;

	void DrawIconTriangleLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconSkipBack(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                      bool filled, float thickness = 1) const;
	void DrawIconSkipBack2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                       bool filled, float thickness = 1) const;
	void DrawIconRewind(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 1) const;

	void DrawIconTriangleRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                           ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconSkipForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                         ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconSkipForward2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;
	void DrawIconFastForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                         ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconCross(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                   bool filled, float shapeThickness = 2, float innerThickness = 1) const;

	void DrawIconHyphen(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft, ImVec2 bottomRight,
	                    bool filled, float thickness = 3.0f) const;

	void DrawIconTriangleDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                              ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconTriangleDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                               ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconGrabDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                          ImVec2 bottomRight, bool filled, float thickness = 1) const;

	void DrawIconGrabDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
	                           ImVec2 bottomRight, bool filled, float thickness = 1) const;

	DiwneAction getDiwneAction() const
	{
		return m_diwneAction;
	};
	void setDiwneAction(DiwneAction action)
	{
		m_diwneAction = action;
	};

	DiwneAction getDiwneActionPreviousFrame() const
	{
		return m_diwneAction_previousFrame;
	};

	DiwneAction getDiwneActionActive() const;

	DIWNE::Link& getHelperLink();

	template <typename T>
	std::shared_ptr<T> getLastActivePin()
	{
		static_assert(std::is_base_of_v<DIWNE::Pin, T>, "Pin must be derived from DIWNE::Pin class.");
		return std::dynamic_pointer_cast<T>(mp_lastActivePin);
	}

	template <typename T>
	void setLastActivePin(std::shared_ptr<T> pin)
	{
		static_assert(std::is_same<T, std::nullptr_t>::value || std::is_base_of_v<DIWNE::Pin, T>,
		              "Pin must be derived from DIWNE::Pin class.");
		mp_lastActivePin = pin;
	}

	template <typename T>
	std::shared_ptr<T> getLastActiveNode()
	{
		static_assert(std::is_base_of_v<DIWNE::Node, T> /* || std::is_same<T, std::nullptr_t>::value*/,
		              "Node must be derived from DIWNE::Node class.");
		return std::dynamic_pointer_cast<T>(mp_lastActiveNode);
	}

	template <typename T>
	void setLastActiveNode(std::shared_ptr<T> node)
	{
		static_assert(
		    // std::is_same<T, std::nullptr_t>::value ||
		    std::is_base_of_v<DIWNE::Node, T>, "Node must be derived from DIWNE::Node class.");
		mp_lastActiveNode = node;
	}

	void setNodesSelectionChanged(bool value)
	{
		m_nodesSelectionChanged = value;
	};
	bool getNodesSelectionChanged()
	{
		return m_nodesSelectionChanged;
	};

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

	virtual bool bypassDiwneSetPopupPositionAction();
	virtual ImVec2 bypassDiwneGetPopupNewPositionAction();

	virtual bool allowProcessZoom();
	virtual bool bypassZoomAction();
	virtual bool processZoom();
	virtual bool processDiwneZoom();
	virtual bool allowProcessFocused();

	virtual bool allowProcessSelectionRectangle();
	virtual bool bypassSelectionRectangleAction();
	ImRect getSelectionRectangleDiwne();
	virtual ImVec2 bypassDiwneGetSelectionRectangleStartPosition();
	virtual ImVec2 bypassDiwneGetSelectionRectangleSize();
	virtual bool processDiwneSelectionRectangle();

	////////////////////////////////////////////
	// ZOOM SCALING
	////////////////////////////////////////////

	float getWorkAreaZoom() const
	{
		return m_workAreaZoom;
	};
	virtual void setWorkAreaZoom(float val = 1);

	/**
	 * Modifies the current ImGuiStyle and Font depending on the current diwne zoom level.
	 * @return Whether zoom scaling was active before making this call
	 */
	bool applyZoomScaling();

	/**
	 * Restores the current ImGuiStyle and Font back to its state before applyZoomScaling() was last called.
	 * @return Whether zoom scaling was active before making this call
	 */
	bool restoreZoomScaling();

	/**
	 * Modifies the current Font depending on the current diwne zoom level.
	 * @return Whether zoom scaling was active before making this call
	 */
	float applyZoomScalingToFont(ImFont* font, ImFont* largeFont = nullptr);

	void restoreZoomScalingToFont(ImFont* font, float originalScale);

	/**
	 * Ensure that zoom scaling is active or not based on a passed parameter
	 * @return Whether zoom scaling was active before making this call, can be later passed to this method again to
	 * restore original state
	 */
	bool ensureZoomScaling(bool active);
};

typedef std::function<void(...)> popupContent_function_pointer; /**< \brief you can pass any arguments to you
                                                                   function with popup menu content */

template <typename T, std::enable_if<std::is_base_of<DiwneObject, T>::value, bool>::type = true>
static void expandPopupContent(T& object) /**< \brief used for popupContent() functions that
                                             are of member of class */
{
	object.popupContent();
}

/**
 * \brief Function showing popup
 * Opens ImGui popup at a given position, fills in your content and closes it
 *
 * \param popupID is identification of popup that have been raised (so is open
 * and should be drawn) \param popupPos position of popup in screen coords
 * \param popupContent is function with content of popup - it can take any
 * (number of) arguments \param args arguments that will be passed to
 * popupContent function \return true if popup is drawn, false otherwise
 *
 */
template <typename... Args>
static bool popupDiwne(std::string const popupID, ImVec2 const& popupPos, void (*popupContent)(Args...), Args&&... args)
{
	bool interaction_happen = false;

	if (ImGui::IsPopupOpen(popupID.c_str()))
	{
		ImGui::SetNextWindowPos(popupPos);
		if (ImGui::BeginPopup(popupID.c_str()))
		{
			// float oldSize = ImGui::GetFontSize();
			/* \todo create somewhere pre loaded font of desired size and push/pop
			 * then here */
			interaction_happen = true;

			popupContent(std::forward<Args>(args)...);

			ImGui::EndPopup();
		}
	}
	return interaction_happen;
}

} /* namespace DIWNE */
