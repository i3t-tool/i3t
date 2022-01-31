#include "WorkspaceCamera.h"

WorkspaceCamera::WorkspaceCamera(DIWNE::Diwne& diwne)
    :   WorkspaceNodeWithCoreDataWithPins(diwne, Core::GraphManager::createCamera(), false)
    ,   m_projection(std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getProj() ) )
    ,   m_view (std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getView() ) )
{
    (m_view->getInputs().at(0).get())->plug(m_projection->getOutputs().at(0).get());
}


bool WorkspaceCamera::isCamera()
{
    return true;
}

void WorkspaceCamera::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceCamera::middleContent()
{
    bool inner_interaction_happen = false;
	inner_interaction_happen |= m_projection->drawNodeDiwne<WorkspaceSequence>(DIWNE::DrawModeNodePosition::OnCoursorPosition, m_drawMode); ImGui::SameLine();
	inner_interaction_happen |= m_view->drawNodeDiwne<WorkspaceSequence>(DIWNE::DrawModeNodePosition::OnCoursorPosition, m_drawMode);
	return inner_interaction_happen;
}

//bool WorkspaceCamera::leftContent(){return false;};
//bool WorkspaceCamera::rightContent(){return WorkspaceNodeWithCoreDataWithPins::rightContent();}; /* draw camera pin on opposite side */

int WorkspaceCamera::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceCamera::maxLenghtOfData() make no sense because every included Sequention has its own independent data");
    return -1; /* should be unused */
}




