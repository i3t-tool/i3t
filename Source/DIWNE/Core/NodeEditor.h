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
#include "DiwneStyle.h"
#include "DrawHelper.h"
#include "Input/NodeEditorInputAdapter.h"

// TODO: REMOVE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
namespace Workspace
{
class CoreNode;
}

// DIWNE Refactoring todos:
// TODO: (DR) Refactor and revise the whole bypass/input/action trigger system thats going on here

namespace DIWNE
{
/** \brief Storage of all values
 */
struct SettingsDiwne
{
	// TODO: Unused? DiwneObject id should cover this? Remove since ids are generated automatically now.
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
	float mouseDragThreshold = 2.0f;       // 6.0 is ImGui default

	ImVec2 initPopupPosition = ImVec2(0, 0); /**< where to show popup when not set later */

	ImVec4 selectionRectFullColor = ImVec4(0.0, 0.0, 1.0, 0.35);
	ImVec4 selectionRectTouchColor = ImVec4(0.0, 1.0, 0.0, 0.35);

	ImVec4 itemSelectedBorderColor = ImVec4(1.0, 0.9, 0.4, 0.6);
	float itemSelectedBorderThicknessDiwne = 2.5;

	ImVec4 objectHoverBorderColor = ImVec4(0.0, 0.0, 0.0, 0.20);
	float objectHoverBorderThicknessDiwne = 1.5;
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
	// TODO: Font color is unused, was deleted, reimplement if needed
	ImVec4 fontColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); ///< Color of the text in the node
};
// \todo   void setMiddleAlign(float v) {assert(v>=0 && v<=1); m_middleAlign =
// v;}; /* from 0==left to 1==right */
//         void setLinkControlpointsOffsetDiwne(const ImVec2 controlStart, const
//         ImVec2 controlEnd) { m_startControlOffsetDiwne = controlStart;
//         m_endControlOffsetDiwne = controlEnd; };

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
	DIWNE::SettingsDiwne* mp_settingsDiwne;
	std::unique_ptr<DIWNE::DrawHelper> m_renderer;

	std::vector<std::shared_ptr<Workspace::CoreNode>> m_workspaceCoreNodes;
	std::vector<std::shared_ptr<Link>> m_links;

	std::shared_ptr<DrawInfo> m_prevContext;

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

	DiwneStyle m_style;

	DIWNE_DEBUG_VARS()

	bool m_takeSnap{false}; // TODO: Rename or at least add documentation,
	                        //  this feature shouldn't be specific to our undo/redo system if it were to remain here

	std::unique_ptr<NodeEditorInputAdapter> m_input = std::make_unique<NodeEditorInputAdapter>(this);

protected:
	// TODO: Review if we could keep DiwneAction info in Context?
	//  Really begs the question where are we okay to use diwne member variables
	//  and where it might be better to use method parameters / returns rather than a
	//  "global" member context.
	DiwneAction m_diwneAction{DiwneAction::None}, m_diwneAction_previousFrame{DiwneAction::None};

	ImDrawListSplitter m_channelSplitter;

	std::shared_ptr<DIWNE::Pin> mp_lastActivePin;
	std::shared_ptr<DIWNE::Node> mp_lastActiveNode;

	std::unique_ptr<DIWNE::Link> m_helperLink; /**< for showing link that is in process of creating */

	bool m_nodesSelectionChanged{false};

	// TODO: Could be moved into Actions::SelectionRectData
	ImRect m_selectionRectangeDiwne{0, 0, 0, 0};

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

	void initializeDiwne(DrawInfo& context) override;

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;

protected:
	void afterDraw(DrawInfo& context) override;

protected:
	void finalizeDiwne(DrawInfo& context) override;

public:
	void addNode(std::shared_ptr<Workspace::CoreNode> node, const ImVec2 position = ImVec2(0, 0),
	             bool shiftToLeftByNodeWidth = false);

	// TODO: Do we really need a template function for this?
	// TOOD: This method doesn't allow to create nodes with arbitrary constructors (no args passthrough)
	template <class T>
	auto inline createNode(const ImVec2 position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		auto node = std::make_shared<T>(*this);
		addNode(node, position, shiftToLeftByNodeWidth);
		return node;
	}

	virtual bool allowProcessZoom();
	virtual bool bypassZoomAction();
	virtual bool processZoom();
	virtual bool processDiwneZoom();

	bool isPressedDiwne() override;
	bool isJustPressedDiwne() override;
	bool allowHover() const override;

	void updateLayout(DrawInfo& context) override;

	bool blockRaisePopup(); /**< sometimes we do not want to raise popup - here specify
	                           it ( now it is when selecting action run ) */

	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	void processInteractionsDiwne(DrawInfo& context) override;

protected:
	bool isDraggedDiwne() override;

public:
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

	// TODO: All this stuff below could be moved into some kind of Canvas class
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


	// TODO: Remove these old action things
	DiwneAction getDiwneActionActive() const;

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

	DIWNE::Link& getHelperLink();

	// Node shifting
	// =============================================================================================================

	void shiftNodesToBegin(std::vector<std::shared_ptr<Workspace::CoreNode>> const& nodesToShift);
	void shiftNodesToEnd(std::vector<std::shared_ptr<Workspace::CoreNode>> const& nodesToShift);
	void shiftInteractingNodeToEnd();

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

	// =============================================================================================================

	void setNodesSelectionChanged(bool value)
	{
		m_nodesSelectionChanged = value;
	};
	bool getNodesSelectionChanged()
	{
		return m_nodesSelectionChanged;
	};

	// TODO: Probably move all input related stuff into some kind of InputAdapter
	//  Similarly another class should handle styling
	virtual float bypassGetZoomDelta();

	virtual bool bypassIsItemActive(); // TODO: Remove this is dumb

	virtual bool bypassDiwneSetPopupPositionAction();
	virtual ImVec2 bypassDiwneGetPopupNewPositionAction();

	// TODO: These methods might not actually be necessary (handled in onDrag / InputAdapter)
	// Selection rectangle
	ImRect getSelectionRectangleDiwne();

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
