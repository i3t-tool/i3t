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

#include "Core/Input/InputManager.h"
#include "Scripting/Scripting.h"

#include "../Nodes/WorkspaceNodeWithCoreData.h"

//transformations{
#include "../Nodes/WorkspaceAxisAngle.h"
#include "../Nodes/WorkspaceEulerX.h"
#include "../Nodes/WorkspaceEulerY.h"
#include "../Nodes/WorkspaceEulerZ.h"
#include "../Nodes/WorkspaceFrustum.h"
#include "../Nodes/WorkspaceLookAt.h"
#include "../Nodes/WorkspaceMatrixScale.h"
#include "../Nodes/WorkspaceMatrixTranslation.h"
#include "../Nodes/WorkspaceOrtho.h"
#include "../Nodes/WorkspacePerspective.h"
#include "../Nodes/WorkspaceSequence.h"
#include "../Nodes/WorkspaceTransformationFree.h"
//todo quat
//#include "../Nodes/WorkspaceQuatRot.h"
//} tranformationa end

//operators {

//	transformations{
#include "../Nodes/WorkspaceMakeAxisAngle.h"
#include "../Nodes/WorkspaceMakeEulerX.h"
#include "../Nodes/WorkspaceMakeEulerY.h"
#include "../Nodes/WorkspaceMakeEulerZ.h"
#include "../Nodes/WorkspaceMakeFrustum.h"
#include "../Nodes/WorkspaceMakeLookAt.h"
#include "../Nodes/WorkspaceMakeOrtho.h"
#include "../Nodes/WorkspaceMakePerspective.h"
#include "../Nodes/WorkspaceMakeScale.h"
#include "../Nodes/WorkspaceMakeTranslation.h"
//	} tranformation end

//	matrix{
#include "../Nodes/WorkspaceMatrixFree.h"
#include "../Nodes/WorkspaceMatrixInversion.h"
#include "../Nodes/WorkspaceMatrixMulMatrix.h"
#include "../Nodes/WorkspaceMatrixTranspose.h"
#include "../Nodes/WorkspaceDeterminant.h"
#include "../Nodes/WorkspaceMatrixAddMatrix.h"
#include "../Nodes/WorkspaceMatrixMulFloat.h"
#include "../Nodes/WorkspaceMatrixMulVector.h"
#include "../Nodes/WorkspaceVectorMulMatrix.h"
//	} matrix end

//	vec4{
#include "../Nodes/WorkspaceVectorFree.h"
#include "../Nodes/WorkspaceNormalizeVector.h"
#include "../Nodes/WorkspaceVectorDotVector.h"
#include "../Nodes/WorkspaceVectorAddVector.h"
#include "../Nodes/WorkspaceVectorSubVector.h"
#include "../Nodes/WorkspaceVectorMulFloat.h"
#include "../Nodes/WorkspaceVectorPerspectiveDivision.h"
#include "../Nodes/WorkspaceMixVector.h"
//	} vec4 end

//	vec3{
#include "../Nodes/WorkspaceVector3Free.h"
#include "../Nodes/WorkspaceNormalizeVector3.h"
#include "../Nodes/WorkspaceVector3CrossVector3.h"
#include "../Nodes/WorkspaceVector3DotVector3.h"
#include "../Nodes/WorkspaceVector3AddVector3.h"
#include "../Nodes/WorkspaceVector3SubVector3.h"
#include "../Nodes/WorkspaceVector3MulFloat.h"
#include "../Nodes/WorkspaceVector3Length.h"
#include "../Nodes/WorkspaceShowVector3.h"
#include "../Nodes/WorkspaceMixVector3.h"
//	} vec4 end

//	float{
#include "../Nodes/WorkspaceFloatFree.h"
#include "../Nodes/WorkspaceClampFloat.h"
#include "../Nodes/WorkspaceFloatMulFloat.h"
#include "../Nodes/WorkspaceFloatDivFloat.h"
#include "../Nodes/WorkspaceFloatAddFloat.h"
#include "../Nodes/WorkspaceFloatPowFloat.h"
#include "../Nodes/WorkspaceMixFloat.h"
#include "../Nodes/WorkspaceSignum.h"
//#include "../Nodes/WorkspaceFloatSinCos.h"
//#include "../Nodes/WorkspaceASinACos.h"
//#include "../Nodes/WorkspaceFloatCycle.h"
//	} float end

//	quat{
#include "../Nodes/WorkspaceQuatVecConjQuat.h"
#include "../Nodes/WorkspaceQuatLength.h"
//TODO Wqiting for free quat
//	} quat end

//  conversion{
#include "../Nodes/WorkspaceFloatsToMatrix.h"
#include "../Nodes/WorkspaceFloatsToVector.h"
#include "../Nodes/WorkspaceFloatsToVector3.h"
#include "../Nodes/WorkspaceQuatToMatrix.h"
#include "../Nodes/WorkspaceTRToMatrix.h"
#include "../Nodes/WorkspaceVector3ToVector.h"
#include "../Nodes/WorkspaceVectors3ToMatrix.h"
#include "../Nodes/WorkspaceVectorsToMatrix.h"
#include "../Nodes/WorkspaceVectorToVector3.h"
#include "../Nodes/WorkspaceQuatToEuler.h"
//  }
//}operators end

#include "Core/Input/InputManager.h"
#include "Scripting/Scripting.h"

#include <math.h>



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


typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow
{
public:
	I3T_WINDOW(WorkspaceWindow)

	Application& m_wholeApplication;
	ne::EditorContext* m_nodeEditorContext; /*! \brief Object for store workspace scene - its wrapper for api functions only */
	ne::Detail::EditorContext* m_ne_usable; /*! \brief On this object you can call inner functions */

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

	ImTextureID HeaderBackground; /* ImTextureID is not id, but void* - so whatever application needs */

	const float ConstTouchTime; /*! \brief \TODO: take values from (move to) Const.h */

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedWorkspaceCoreNodes();

    std::vector<Ptr<WorkspaceSequence>> getSequenceNodes();
	Ptr<WorkspaceSequence> getSequenceOfWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> node);

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

    void NodeDelete(ne::NodeId nodeId);

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
};
