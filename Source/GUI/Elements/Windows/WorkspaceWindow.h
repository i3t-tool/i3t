/**
 * \file	  GUI/Elements/Windows/WorkspaceWindow.h
 * \authors Sofie Sasorina, Jaroslav Holecek
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#define WORKSPACE_DEBUG 1

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

#include "Config.h"

#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/IWindow.h"

#include "GUI/Elements/Nodes/WorkspaceSingleInclude.h"


#include <glm/glm.hpp>

#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"
#include "Scripting/Scripting.h"


typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

enum WorkspaceWindowAction
{
    None,
    SelectionRectFull,
    SelectionRectTouch,
    CreateAndPlugTypeConstructor
};

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow, public DIWNE::Diwne
{
public:
	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);
	~WorkspaceWindow() override;

	Application& m_wholeApplication;

//    DIWNE::Diwne m_diwne;
    void popupBackgroundContent();

    bool processDiwne();
    void BeginDiwne(const char* id);
    void EndDiwne();

    bool m_first_frame = true;

    ImRect m_selectionRectangeDiwne;
    WorkspaceWindowAction m_workspaceWindowAction, m_workspaceWindowPreviousFrameAction;

    WorkspaceCoreLink *m_creatingLink;
    WorkspaceCorePin *m_linkCreatingPin;

	ImVec2 m_rightClickPosition = ImVec2(100,100);
//	ImVec2 m_newNodePostion = ImVec2(100,100);

	/**
	 * \brief All WorkspaceNodes
	 *
	 * \todo Needs to be static.
	 * \todo Move this variable somewhere else.
	 */
	static std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceCoreNodes;

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedNodes();

	bool processCreateAndPlugTypeConstructor();

	template <typename T>
    void addTypeConstructorNode()
    {
        addNodeToPosition<T>( m_linkCreatingPin->getLinkConnectionPointDiwne() + ImVec2(-100,0) ); /* \todo JH shift to Theme */
        static_cast<WorkspaceCoreInputPin*>(m_linkCreatingPin)->plug(std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(m_workspaceCoreNodes.back())->getOutputs().at(0).get()); /* \todo JH always 0 with type constructor? */
    }

	/// \todo JH, MH - Needs to be accessed by scene loader, but it may be weird that the loader needs to have reference to the Workspace.
	template <class T>
	static auto inline addNodeToPosition(ImVec2 const position)
	{
		auto node = std::make_shared<T>();

		node->setNodePositionDiwne( position );
		m_workspaceCoreNodes.push_back(node);

		return node;
	}

    template<class T>
    void inline addNodeToPositionOfPopup()
    {
        addNodeToPosition<T>(screen2diwne(getPopupPosition()));
    }

	ImTextureID HeaderBackground; /* ImTextureID is not id, but void* - so whatever application needs */

	const float ConstTouchTime; /*! \brief \TODO: take values from (move to) Const.h */

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedWorkspaceCoreNodes();

//	std::vector<Ptr<WorkspaceSequence>> getSequenceNodes();
//	Ptr<WorkspaceSequence> getSequenceOfWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> node);

//	Ptr<WorkspaceNodeWithCoreData> getWorkspaceCoreNodeByID(ne::NodeId const id);
//	Ptr<WorkspaceNodeWithCoreData> getWorkspaceCoreNodeByPinID(ne::PinId const id);
//
//	Ptr<WorkspaceCorePinProperties> getWorkspacePinPropertiesByID(ne::PinId const id);

	void manipulatorStartCheck3D();
//
//	void checkUserActions();
//
//	void checkQueryElements();
//	void checkQueryElementsCreating();
//	void checkQueryLinkCreate();
//	void checkQueryNodeCreate();
//	void checkQueryElementsDeleting();
//	void checkQueryLinkDelete();
//	void checkQueryNodeDelete();
//
////	void NodeDelete(ne::NodeId nodeId);
//
//	void selectAll();
//	void invertSelection();
//
//	//void checkQueryContextMenus();
//

    bool bypassDiwneSelectionRectangleAction();
    ImVec2 bypassDiwneGetSelectionRectangleStartPosition();
    ImVec2 bypassDiwneGetSelectionRectangleSize();

    bool processSelectionRectangle();

	void shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const & nodesToShift);
	void shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const & nodesToShift);
	void shiftDragedOrHoldNodeToEnd();

//	void showPopUpLabel(std::string label, ImColor color);
//
//	void UpdateTouchAllNodes();

//	void WorkspaceDrawNodes(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void render();

	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
	              float splitter_long_axis_size = -1.0f);

	void ShowLeftPane(float paneWidth);

	void ShowStyleEditor(bool* show = nullptr);

	private:
		void showEditMenu();
};

/* >>>>> STATIC FUNCTIONS <<<<< */
//extern void backgroundPopupContent(DIWNE::Diwne &diwne, std::vector<Ptr<WorkspaceNodeWithCoreData>> &workspaceCoreNodes);
