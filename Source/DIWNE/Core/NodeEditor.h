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

#include "Elements/DiwneObject.h"

#include "Canvas.h"
#include "DIWNE/Core/Style/StyleBase.h"
#include "DiwneSettings.h"
#include "Elements/Containers/NodeContainer.h"
#include "Input/NodeEditorInputAdapter.h"
#include "diwne_iterators.h"

// DIWNE Refactoring todos:
// TODO: (DR) Refactor and revise the whole bypass/input/action trigger system thats going on here

// TODO: Eventually NodeEditor should inherit from NodeContainer (or equivalent with links)

namespace DIWNE
{
class Node;
class Pin;
class Link;

/**
 * // TODO: Update docs
 * \brief The node editor object
 * In ancestor of this object you will probably store your nodes (links, pins)
 * It store inter-object interactions
 * Every DiwneObject has reference to this object
 */
class NodeEditor : public DiwneObject, public NodeContainer
{
	using Super = DiwneObject;

public:
	SettingsDiwne* mp_settingsDiwne;

	std::vector<std::shared_ptr<Link>> m_links; ///< List of links, nodes are stored in NodeContainer superclass

	InteractionState interactionState; ///< State of multi-frame user operations

	bool m_takeSnap{false}; // TODO: Rename or at least add documentation,
	//  this feature shouldn't be specific to our undo/redo system if it were to remain here

	DIWNE_DEBUG_VARS()

protected:
	std::unique_ptr<Canvas> m_canvas = std::make_unique<Canvas>(*this);
	std::unique_ptr<NodeEditorInputAdapter> m_input = std::make_unique<NodeEditorInputAdapter>(*this);
	std::unique_ptr<StyleBase> m_style = std::make_unique<StyleBase>();

	ImDrawListSplitter m_channelSplitter;

	std::weak_ptr<Node> mp_lastActiveNode; ///< Last node that requested focus (had a logical update)
	bool m_lastActiveNodeChanged{false};

	bool m_nodesSelectionChanged{false};

	ImVec2 m_popupPosition;

	ImDrawListSplitter m_splitter; ///< Every nodes should be draw to its own channel

public:
	/**
	 * Create a node editor instance.
	 * @note NodeEditor must MUST be instantiated as a shared pointer using std::make_shared! It cannot be defined
	 * as a local stack variable or static value variable or even manually using the new operator.
	 * @param settingsDiwne
	 * @return
	 */
	NodeEditor(const char* label, SettingsDiwne* settingsDiwne);
	~NodeEditor() override;

	// Lifecycle
	// =============================================================================================================
	void draw(DrawMode drawMode = DrawMode_Interactive) override;

	void initializeDiwne(DrawInfo& context) override;
	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;
	void afterDraw(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	// Interaction
	// =============================================================================================================

	bool isPressedDiwne() override;
	bool isJustPressedDiwne() override;

	void processInteractions(DrawInfo& context) override;

	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	void onReleased(bool justReleased, DrawInfo& context) override;

protected:
	bool isDraggedDiwne() override;

public:
	// Editor interaction
	// =============================================================================================================
	virtual bool allowZoom();
	virtual bool isZoomingDiwne();
	virtual bool processZoom();
	virtual void onZoom(){}; ///< Called by Canvas::setZoom() when the zoom level changes.

	virtual bool processPan();

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

	template <class T, typename... Args>
	void setInputAdapter(Args&&... args);

	// TODO: Migrate style settings from legacy DiwneSettings into style, add a way to change styles and some defaults
	/**
	 * Get a reference to the editors style settings, which specify various colors and sizes much like ImStyle.
	 */
	Style& style() const override
	{
		return styleBase(); // Returning the editors base style, not the style override
	}
	StyleBase& styleBase() const
	{
		return *m_style;
	}
	void setStyle(std::unique_ptr<StyleBase>&& ptr)
	{
		m_style = std::move(ptr);
	}
	void setStyleOverride(StyleOverride* styleOverride) = delete;
	StyleOverride* getStyleOverride() const = delete;

	// Object management
	// =============================================================================================================
	/** Clear all nodes from the node editor */
	virtual void clear();

	/// Erases objects marked for deletion or removal from the editor.
	/// Also iterates through all NodeContainers in the editor and purges their nodes.
	void purgeAllNodes();

protected:
	/// Erases objects marked for deletion or removal from the editor.
	void purgeObjects();

public:
	/// Get a reference to the underlying owning vector of all nodes in the node editor.
	NodeList& getNodeList() override;

	// TODO: I probably need const versions of node ranges <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  Non-mutable Iterators now take a const pointer to the node vector meaning they can be created in const methods
	//  A: I likely don't *need* const version, nice to have but not crucial
	/**
	 * Get a view of all nodes in the node editor which can be iterated over.
	 * The object returned is a trivial container wrapper around a node iterator.
	 * TODO: Better docs with an example
	 * TODO: This can be const.
	 * TODO: There is an idea that iterators could filter out destroyed nodes by default. Consider.
	 */
	NodeRange<> getNodes() const override;
	// TODO: Docs and rename to getAllNodes()
	virtual RecursiveNodeRange<> getAllNodesInnerIncluded() const;
	// TODO: Docs
	virtual FilteredNodeRange<> getSelectedNodes() const;
	// TODO: Docs and rename to getAllSelectedNodes()
	virtual FilteredRecursiveNodeRange<> getSelectedNodesInnerIncluded() const;

	// TODO: Docs
	virtual void deselectAllNodes();

	// TODO: Docs, should override the virtual base method (right now it just hides it)
	void addNode(const std::shared_ptr<Node>& node);

	// TODO: Docs
	void addNode(const std::shared_ptr<Node>& node, const ImVec2 position, bool shiftToLeftByNodeWidth = false);

	// TODO: Do we really need a template function for this?
	// TODO: This method doesn't allow to create nodes with arbitrary constructors (no args passthrough)
	template <class T>
	auto createNode(const ImVec2 position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		auto node = std::make_shared<T>(*this);
		addNode(node, position, shiftToLeftByNodeWidth);
		return node;
	}

	/**
	 * Creates a new node in the node editor of the specified type.
	 * It is assumed that the first argument of the node's constructor is a NodeEditor reference and can be ommited.
	 * @tparam T Node type
	 * @param position Initial position of the node
	 * @param args Node constructor arguments (except the first NodeEditor reference)
	 * @return Shared pointer to the node.
	 */
	template <class T, typename... Args>
	auto inline createNode(const ImVec2 position = ImVec2(0, 0), Args&&... args)
	{
		auto node = std::make_shared<T>(*this, std::forward<Args>(args)...);
		addNode(node, position);
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

	/**
	 * Selects all nodes
	 * @param deselectIfAllAreSelected If true and all nodes were already selected, deselect them all.
	 */
	void selectAllNodes(bool deselectIfAllAreSelected = false);
	void invertSelection();
	void deleteSelectedNodes();

public:
	// Node shifting
	// =============================================================================================================

	void shiftNodesToBegin(const NodeList& nodesToShift);
	void shiftNodesToEnd(const NodeList& nodesToShift);
	void bringLastActiveNodeToFront(); // TODO: (Dr) Eventually replace with bringMarkedNodesToFront
	void bringMarkedNodesToFront();

	// =============================================================================================================


	// TODO: Perhaps use this utility getter functions in multiple places
	//  might make the code cleaner, avoiding manual static_casts
	//  However we should be using static_pointer_cast/static_cast as we should know which type we want.
	//  We don't check if these methods return null anyway so its equivalent to a static cast (will crash on fail)
	//  The static asserts might help with that though

	// TODO: (DR) Rename this to focused node. Adds docs relating it with DrawInfo logical updates.
	//   Logical updates could maybe be called focusUpdate? As it in a way captures focus?
	template <typename T = Node>
	std::shared_ptr<T> getLastActiveNode()
	{
		static_assert(std::is_base_of<Node, T>::value, "Node must be derived from DIWNE::Node class.");
		if (mp_lastActiveNode.expired())
			return {};
		return std::dynamic_pointer_cast<T>(mp_lastActiveNode.lock());
	}

	void setLastActiveNode(std::shared_ptr<Node> node)
	{
		mp_lastActiveNode = node;
		m_lastActiveNodeChanged = true;
	}

	/// Get position of the last popup in screen coordinates.
	const ImVec2& getPopupPosition() const;
	/// Set position of the last popup in screen coordinates, used for placement of new nodes.
	void setPopupPosition(ImVec2 position);

	void setNodesSelectionChanged(bool value);
	bool getNodesSelectionChanged() const;

	void setZoom(float val)
	{
		m_canvas->setZoom(val);
	}
	float getZoom() const
	{
		return m_canvas->getZoom();
	}
	/// Set the node editor's dpi scale. Updates dpi scale of its style as well.
	void setDpiScale(float dpiScale)
	{
		mp_settingsDiwne->dpiScale = dpiScale;
		m_style->setDpiScale(dpiScale);
	}
	float getDpiScale() const
	{
		return mp_settingsDiwne->dpiScale;
	}
};

template <typename T, std::enable_if<std::is_base_of<DiwneObject, T>::value, bool>::type = true>
static void expandPopupContent(T& object) /**< \brief used for popupContent() functions that
                                             are of member of class */
{
	object.popupContent();
}

template <class T, typename... Args>
void NodeEditor::setInputAdapter(Args&&... args)
{
	m_input = std::make_unique<T>(*this, std::forward<Args>(args)...);
}

/**
 * Shows an unscaled ImGui popup at the current mouse position with the provided content.
 * The provided popup ID popup must be previously opened outside of this method.
 * This can be used to open custom popups that aren't associated with a particular DiwneObject.
 * Otherwise the preferred method of opening popups for DiwneObjects is overridng the popupContent() method.
 *
 * @param popupID is identification of popup that have been raised (so is open
 * and should be drawn)
 * @param popupContent is function with content of popup - it can take any
 * (number of) arguments
 * @param args arguments that will be passed to
 * popupContent function
 * @return true if popup is drawn, false otherwise
 * @note This method resets the ImGui style stack, any pushed styles will get reset after this call.
 */
template <typename... Args>
static bool popupDiwne(NodeEditor& diwne, std::string popupID, void (*popupContent)(Args...), Args&&... args)
{
	bool interacted = false;

	if (ImGui::IsPopupOpen(popupID.c_str()))
	{
		ImGuiStyle styleBackup = ImGui::GetStyle();
		const bool zoomScalingWasActive = diwne.canvas().ensureZoomScaling(false);

		if (ImGui::BeginPopup(popupID.c_str()))
		{
			interacted = true;

			popupContent(std::forward<Args>(args)...);
			ImGui::EndPopup();
		}

		diwne.canvas().ensureZoomScaling(zoomScalingWasActive);
		ImGui::GetCurrentContext()->Style = styleBackup;
	}
	return interacted;
}

} /* namespace DIWNE */
