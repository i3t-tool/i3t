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
#include <type_traits>

#include "DiwneObject.h"

#include "Canvas.h"
#include "DiwneStyle.h"
#include "Input/NodeEditorInputAdapter.h"
#include "diwne_iterators.h"

// TODO: REMOVE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
	                                                    on window size ( \see updateViewportRects() ) */

	float minWorkAreaZoom = 0.25;          /**< minimal value of zoom */
	float maxWorkAreaZoom = 4;             /**< maximal value of zoom */
	float workAreaInitialZoom = 1;         /**< initial value of zoom */
	float zoomWheelReverseSenzitivity = 8; /**< Higher number -> smaller change, can not be 0 */
	float selectionRounding = 0;           /**< rounding od selection */
	float mouseDragThreshold = 2.0f;       // 6.0 is ImGui default

	ImVec2 initPopupPosition = ImVec2(0, 0); /**< where to show popup when not set later */

	ImVec4 selectionRectFullColor = ImVec4(0.0, 0.0, 1.0, 0.1);
	ImVec4 selectionRectTouchColor = ImVec4(0.0, 1.0, 0.0, 0.1);
	float selectionRectBorderAlpha = 0.8f;

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

	// Some new flags that are yet to be truly finalized, subject to be moved to a different place
	bool selectNodeOnDrag = false;
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
	SettingsDiwne* mp_settingsDiwne;

	std::vector<std::shared_ptr<Node>> m_nodes;
	std::vector<std::shared_ptr<Link>> m_links;

	/// not draw popup two times \todo maybe unused when every object is drawn just one time
	bool m_popupDrawn{false};
	/// not draw tooltip two times \todo maybe unused when every object is drawn just one time
	bool m_tooltipDrawn{false};
	/// only one object can be focused
	/// TODO: A flag indicating that an object has been focused that frame?
	///  Review this flag, its hard to track, why not keep track of the "focused" object instead
	///  Also still yet to exactly estabilish what "focused" really means
	bool m_objectFocused{false}; // TODO: Remove?

	InteractionState interactionState;

	bool m_takeSnap{false}; // TODO: Rename or at least add documentation,
	                        //  this feature shouldn't be specific to our undo/redo system if it were to remain here

	DIWNE_DEBUG_VARS()

protected:
	std::unique_ptr<Canvas> m_canvas = std::make_unique<Canvas>(*this);
	std::unique_ptr<NodeEditorInputAdapter> m_input = std::make_unique<NodeEditorInputAdapter>(*this);
	std::unique_ptr<DiwneStyle> m_style = std::make_unique<DiwneStyle>();

	ImDrawListSplitter m_channelSplitter;

	// TODO: The dragged pin could be technically stored in the Context actionData, but I do have some concerns about
	//  copying that info etc.
	//  I am not sure if these concerns are valid, the mechanism for this is there and is nicer than using NodeEditor
	//  member variables to store data
	//	std::shared_ptr<DIWNE::Pin> mp_lastActivePin; ///< Used for storing which pin is being dragged //TODO: Maybe
	// rename?
	std::shared_ptr<Node> mp_lastActiveNode; ///< Last node that had a logical update

	bool m_nodesSelectionChanged{false};

	ImVec2 m_popupPosition;

	ImDrawListSplitter m_splitter; ///< Every nodes should be draw to its own channel

public:
	NodeEditor(SettingsDiwne* settingsDiwne);
	virtual ~NodeEditor();

	// Lifecycle
	// =============================================================================================================
	void draw(DrawMode drawMode = DrawMode_Interactive) override;

	void initializeDiwne(DrawInfo& context) override;

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;

	void afterDraw(DrawInfo& context) override;

	void updateLayout(DrawInfo& context) override;

	// Interaction
	// =============================================================================================================

	bool isPressedDiwne() override;
	bool isJustPressedDiwne() override;
	bool allowHover() const override;

	void processInteractions(DrawInfo& context) override;

	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	void onReleased(bool justReleased, DrawInfo& context) override;

protected:
	bool isDraggedDiwne() override;

public:
	// Interaction - Zooming
	// =============================================================================================================
	virtual bool allowProcessZoom(); // TODO: Rename to allowZoom()
	virtual bool isZoomingDiwne();
	virtual bool processZoom();      // TODO: Rename to onZoom()
	virtual bool processDiwneZoom(); // TODO: Rename to processZoomDiwne()

	/**
	 * Called by Canvas::setZoom() when the zoom level changes.
	 */
	virtual void onZoom(){};

	// Subsystems
	// =============================================================================================================
	/**
	 * Get a reference to the canvas component of the node editor.
	 * Canvas contains information about the position of the viewport displaying the editor's infinite 2D plane.
	 * It also holds utility methods used for drawing content within the plane (diwne coordinates).
	 */
	Canvas& canvas() const
	{
		return *m_canvas;
	}
	// TODO: Add a way to change input adapter
	/**
	 * Get a reference to the node editors input adapter.
	 * This object contains input bindings and can be set on the fly externally to swap input schemes.
	 */
	NodeEditorInputAdapter& input() const
	{
		return *m_input;
	}
	// TODO: Migrate style settings from legacy DiwneSettings into style, add a way to change styles and some defaults
	/**
	 * Get a reference to the editors style settings, which specify various colors and sizes much like ImStyle.
	 */
	DiwneStyle& style() const
	{
		return *m_style;
	}

	// Object management
	// =============================================================================================================
	/** Clear all nodes from the node editor */
	virtual void clear();

	/// Get a reference to the underlying owning vector of all nodes in the node editor.
	std::vector<std::shared_ptr<Node>>& getNodeList()
	{
		return m_nodes;
	}

	// TODO: I probably need const versions of node ranges <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  Non-mutable Iterators now take a const pointer to the node vector meaning they can be created in const methods
	/**
	 * Get a view of all nodes in the node editor which can be iterated over.
	 * The object returned is a trivial container wrapper around a node iterator.
	 * TODO: Better docs with an example
	 */
	NodeRange<> getNodes()
	{
		return NodeRange(&m_nodes);
	};

	ConstNodeRange<> getNodes() const
	{
		return ConstNodeRange(&m_nodes);
	};

	virtual RecursiveNodeRange<> getAllNodesInnerIncluded() const
	{
		return RecursiveNodeRange<>(&m_nodes);
	}

	virtual FilteredNodeRange<> getSelectedNodes() const
	{
		return FilteredNodeRange<>(
		    [](const Node* node) -> bool {
			    return node->getSelected();
		    },
		    &m_nodes);
	}

	virtual FilteredRecursiveNodeRange<> getSelectedNodesInnerIncluded() const
	{
		return FilteredRecursiveNodeRange<>(
		    [](const Node* node) -> bool {
			    return node->getSelected();
		    },
		    &m_nodes);
	}

	virtual void deselectNodes();

	void addNode(std::shared_ptr<Node> node);
	void addNode(std::shared_ptr<Node> node, const ImVec2 position, bool shiftToLeftByNodeWidth = false);

	// TODO: Do we really need a template function for this?
	// TODO: This method doesn't allow to create nodes with arbitrary constructors (no args passthrough)
	template <class T>
	auto inline createNode(const ImVec2 position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		auto node = std::make_shared<T>(*this);
		addNode(node, position, shiftToLeftByNodeWidth);
		return node;
	}

	/**
	 * Create a new link of type T and add it to the node editor.
	 * @tparam T
	 * @return An owning shared ptr to the new link. Note that the link is already owned by the node editor at this
	 * point in time.
	 */
	template <class T, typename... Args>
	auto inline createLink(Args&&... args)
	{
		auto link = std::make_shared<T>(*this, std::forward<Args>(args)...);
		addLink(link);
		return link;
	}

	void addLink(std::shared_ptr<Link> link);

protected:
	/// Erases objects marked for deletion or removal from the editor.
	void purgeObjects();

public:
	// Node shifting
	// =============================================================================================================

	void shiftNodesToBegin(const std::vector<std::shared_ptr<Node>>& nodesToShift);
	void shiftNodesToEnd(const std::vector<std::shared_ptr<Node>>& nodesToShift);
	void shiftInteractingNodeToEnd();

	// TODO: Perhaps use this utility getter functions in multiple places
	//  might make the code cleaner, avoiding manual static_casts
	//  However we should be using static_pointer_cast/static_cast as we should know which type we want.
	//  We don't check if these methods return null anyway so its equivalent to a static cast (will crash on fail)
	//  The static asserts might help with that though

	// TODO: (DR) Rename this to focused node. Adds docs relating it with DrawInfo logical updates.
	//   Logical updates could maybe be called focusUpdate? As it in a way captures focus?
	template <typename T>
	std::shared_ptr<T> getLastActiveNode()
	{
		static_assert(std::is_base_of<Node, T>::value, "Node must be derived from DIWNE::Node class.");
		return std::dynamic_pointer_cast<T>(mp_lastActiveNode);
	}

	void setLastActiveNode(std::shared_ptr<Node> node)
	{
		mp_lastActiveNode = node;
	}

	// =============================================================================================================

	ImVec2 const& getPopupPosition() const;
	void setPopupPosition(ImVec2 position);

	void setNodesSelectionChanged(bool value);
	bool getNodesSelectionChanged();

	void setZoom(float val)
	{
		m_canvas->setZoom(val);
	}
	float getZoom() const
	{
		return m_canvas->getZoom();
	}
};

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
