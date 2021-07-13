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
#include <imgui_node_editor.h>
#include <imgui_node_editor_internal.h>

#include "DIWNE/diwne.h"
#include "DIWNE/Node.h"

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
#include "GUI/NodeEditorUtilities/Builders.h" /* \todo soubor s malym pismenkem na zacatku neexistuje - porad mi to prosim neprepisujte :-D */
#include "GUI/NodeEditorUtilities/Widgets.h"

#include "GUI/Elements/Nodes/WorkspaceCamera.h"
#include "GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"

namespace ne = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow
{
public:


	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);
	virtual ~WorkspaceWindow();

	Application& m_wholeApplication;
	ne::EditorContext* m_nodeEditorContext; /*! \brief Object for store workspace scene - its wrapper for api functions only */
	ne::Detail::EditorContext* m_ne_usable; /*! \brief On this object you can call inner functions */

    DIWNE::Diwne m_diwne;
    DIWNE::Node n;

	ImTextureID m_headerBackgroundTexture;

	util::NodeBuilder m_nodeBuilderContext;

	/* \todo JH better name for this atributes - for better description what they do... */
	Ptr<WorkspaceCorePinProperties> m_pinPropertiesForNewLink = nullptr;
	Ptr<WorkspaceCorePinProperties> m_pinPropertiesForNewNodeLink = nullptr;
	bool m_createNewNode = false;

	//ImVec2 m_openPopupMenuPosition = ImVec2(100,100); /* \todo JH some better default value - maybe little bit unused, but for certainty */
	ImVec2 m_rightClickPosition = ImVec2(100,100);
	ImVec2 m_newNodePostion = ImVec2(100,100);
	ne::NodeId m_contextNodeId = 0;
	ne::LinkId m_contextLinkId = 0;
	ne::PinId  m_contextPinId = 0;

	// static std::vector<Namespace*> s_Nodes; /*! \brief All Nodes */
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceCoreNodes; /*! \brief All WorkspaceNodes */

	std::vector<Ptr<WorkspaceSequence>> m_all_sequences;
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_draged_nodes;
	Ptr<WorkspaceNodeWithCoreData> m_draged_node;
	ne::Detail::Node *m_draged_node_nodeeditor;

	ImTextureID HeaderBackground; /* ImTextureID is not id, but void* - so whatever application needs */

	const float ConstTouchTime; /*! \brief \TODO: take values from (move to) Const.h */

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedWorkspaceCoreNodes();

	std::vector<Ptr<WorkspaceSequence>> getSequenceNodes();
	Ptr<WorkspaceSequence> getSequenceOfWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> node);

	Ptr<WorkspaceNodeWithCoreData> getWorkspaceCoreNodeByID(ne::NodeId const id);
	Ptr<WorkspaceNodeWithCoreData> getWorkspaceCoreNodeByPinID(ne::PinId const id);

	Ptr<WorkspaceCorePinProperties> getWorkspacePinPropertiesByID(ne::PinId const id);

	void manipulatorStartCheck3D();

	void checkUserActions();

	void checkQueryElements();
	void checkQueryElementsCreating();
	void checkQueryLinkCreate();
	void checkQueryNodeCreate();
	void checkQueryElementsDeleting();
	void checkQueryLinkDelete();
	void checkQueryNodeDelete();

	void NodeDelete(ne::NodeId nodeId);

	void selectAll();
	void invertSelection();

	void checkQueryContextMenus();

	void shiftNodesToFront(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift);
	void shiftNodesToBack(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift);
	void shiftSelectedNodesToFront();
	void shiftTransformationInSequenceNodesToFront();

	void showPopUpLabel(std::string label, ImColor color);

	void UpdateTouchAllNodes();

	void WorkspaceDrawNodes(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void render();

	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
	              float splitter_long_axis_size = -1.0f);

	void ShowLeftPane(float paneWidth);

	void ShowStyleEditor(bool* show = nullptr);

	private:
		void showEditMenu();
};
