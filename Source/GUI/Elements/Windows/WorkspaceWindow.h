/**
 * \file	GUI/Elements/Windows/WorkspaceWindow.h
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#include "GUI/Elements/IWindow.h"

#include "GUI/NodeEditorUtilities/Builders.h" /* \todo soubor s malym pismenkem na zacatku neexistuje - porad mi to prosim neprepisujte :-D */
#include "GUI/NodeEditorUtilities/Widgets.h"

#include <glm/glm.hpp>

#include "../../../Core/Nodes/GraphManager.h"
#include "../../../Core/Nodes/Node.h"
#include "../../../Core/Nodes/NodeImpl.h"
#include "Config.h"
#include "Core/Application.h"
#include "GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "pgr.h"
#include <memory>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui_node_editor.h>
#include <imgui_node_editor_internal.h>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

// using namespace ax;
// using ax::Widgets::IconType;

namespace ne = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

/* >>> Static function <<< */ //{

/*! \fn static inline ImRect ImGui_GetItemRect()
    \brief Get ImRect of last  ( \todo active/added ?) item
    \return ImRect : New ImRect with position and size of last item
*/
static inline ImRect ImGui_GetItemRect()
{
	return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

/*! \fn static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    \brief Enlarge given ImRect (create new / enlarge given ?)
     \param[in/out?] rect ImRect& for enlarge
     \param[in] x float value added to left and right
     \param[in] y float value added to up an d down
    \return ImRect New
*/
static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
	auto result = rect;
	result.Min.x -= x;
	result.Min.y -= y;
	result.Max.x += x;
	result.Max.y += y;
	return result;
}
//} /* >>> Static functions - end <<< */

typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow
{
public:
	I3T_WINDOW(WorkspaceWindow)

	Application& WholeApplication;
	ne::EditorContext* NodeEditorContext; /*! \brief Object for store workspace scene */

	ImTextureID HeaderBackgroundTexture;

	util::NodeBuilder NodeBuilderContext; /* \todo builder as variable of WorkspceWindow?*/

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

	ImTextureID HeaderBackground; /* ImTextureID is not id, but void* - so whatever application needs */

	const float ConstTouchTime; /*! \brief \TODO: take values from (move to) Const.h */

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedWorkspaceCoreNodes();

	WorkspaceWindow(bool show);
	~WorkspaceWindow();

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

    void checkQueryContextMenus();

    void shiftSelectedNodesToFront();



    void showPopUpLabel(std::string label, ImColor color);

    void UpdateTouchAllNodes();

	void WorkspaceDrawNodes(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void render();

	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
	              float splitter_long_axis_size = -1.0f);

	void ShowLeftPane(float paneWidth);

	void ShowStyleEditor(bool* show = nullptr);
};
