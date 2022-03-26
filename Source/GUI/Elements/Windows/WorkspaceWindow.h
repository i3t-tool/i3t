/**
 * \file	  GUI/Elements/Windows/WorkspaceWindow.h
 * \authors Sofie Sasorina, Jaroslav Holecek
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#include "DIWNE/diwne_include.h"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include "Config.h"

#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"

#include "GUI/Elements/IWindow.h"
#include "GUI/Elements/Nodes/WorkspaceSingleInclude.h"

#include "Logger/Logger.h"
#include "Scripting/Scripting.h"
#include "State/StateManager.h"

typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

enum WorkspaceDiwneAction
{
    None
    , CreateAndPlugTypeConstructor
    , NOTunselectAllNodes
};

extern class WorkspaceDiwne* g_workspaceDiwne;

class WorkspaceDiwne : public DIWNE::Diwne
{
	friend void WorkspaceSequence::moveNodeToWorkspace(Ptr<WorkspaceNodeWithCoreData> node);

public:
	WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne);
	~WorkspaceDiwne() override;

    ImDrawListSplitter m_channelSplitter;

    void popupContent();

    bool beforeBegin();
    bool beforeContent();
    bool content();
    bool afterContent();
    bool afterEnd();

    WorkspaceDiwneAction m_workspaceDiwneAction, m_workspaceDiwneActionPreviousFrame;
    void setWorkspaceDiwneAction(WorkspaceDiwneAction wda) {m_workspaceDiwneAction = wda;}
    WorkspaceDiwneAction getWorkspaceDiwneAction() {return m_workspaceDiwneAction;}
    WorkspaceDiwneAction getWorkspaceDiwneAction_previousFrame() {return m_workspaceDiwneActionPreviousFrame;}
    WorkspaceDiwneAction getWorkspaceDiwneActionActive() const {return m_workspaceDiwneAction == WorkspaceDiwneAction::None ? m_workspaceDiwneActionPreviousFrame : m_workspaceDiwneAction; }

	/** * \brief All WorkspaceNodes
        * \note Nodes inside Sequentions are not directly in this vector (they are in Sequence)
	 **/
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceCoreNodes;
	std::vector<Ptr<WorkspaceNodeWithCoreData>> const & getAllNodes() {return m_workspaceCoreNodes;};

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedNodes();

	std::vector<WorkspaceCoreLink*> m_linksToDraw;

	bool processCreateAndPlugTypeConstructor();

	template <typename T>
    void addTypeConstructorNode()
    {
        WorkspaceCoreInputPin *pin = getLastActivePin<WorkspaceCoreInputPin>().get();
        addNodeToPosition<T>( pin->getLinkConnectionPointDiwne() + ImVec2(-100,0) ); /* \todo JH shift to Theme */
        pin->plug(std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(m_workspaceCoreNodes.back())->getOutputs().at(0).get()); /* \todo JH always 0 with type constructor? */
    }

	template <class T>
	auto inline addNodeToPosition(ImVec2 const position=ImVec2(0,0))
	{
		auto node = std::make_shared<T>(*this);

		node->setNodePositionDiwne( position );
		m_workspaceCoreNodes.push_back(node);

		return node;
	}

    template<class T>
    auto inline addNodeToPositionOfPopup()
    {
			auto result = addNodeToPosition<T>(screen2diwne(getPopupPosition()));
			/// TEST /////////////////////////////////////////////////
			StateManager::instance().takeSnapshot();
			//////////////////////////////////////////////////////////

			return result;
    }

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedWorkspaceCoreNodes();

	void manipulatorStartCheck3D();

    void shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const & nodesToShift);
	void shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const & nodesToShift);
	void shiftInteractingNodeToEnd();

	void processDragAllSelectedNodes();

	void selectAll();
	void invertSelection();
	void zoomToAll();
    void zoomToSelected();
    ImRect getOverNodesRectangleDiwne(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodes);
    void zoomToRectangle(ImRect const& rect);

    void deleteSelectedNodes();

    //bool allowInteraction();

    std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedNodesInnerIncluded();
    std::vector<Ptr<WorkspaceNodeWithCoreData>> getAllNodesInnerIncluded();

	bool bypassZoomAction();
	bool bypassDragAction();
	bool bypassHoldAction();
    bool bypassUnholdAction();

    bool bypassSelectionRectangleAction() override;
    ImVec2 bypassDiwneGetSelectionRectangleStartPosition() override;
    ImVec2 bypassDiwneGetSelectionRectangleSize() override;
};

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow
{
public:
	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);
	~WorkspaceWindow() override;

	WorkspaceDiwne& getNodeEditor();

	Application& m_wholeApplication;

	void render();

	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
	              float splitter_long_axis_size = -1.0f);

	void ShowLeftPane(float paneWidth);

	void ShowStyleEditor(bool* show = nullptr);

	private:
		void showEditMenu();
};

/* >>>>> STATIC FUNCTIONS <<<<< */

template <typename T>
auto inline addNodeToNodeEditor(ImVec2 const position=ImVec2(0,0))
{
    return g_workspaceDiwne->addNodeToPosition<T>(position);
}
