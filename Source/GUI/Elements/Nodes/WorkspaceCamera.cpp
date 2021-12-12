#include "WorkspaceCamera.h"

WorkspaceCamera::WorkspaceCamera()
    :   WorkspaceNodeWithCoreDataWithPins(Core::GraphManager::createCamera(), false)
    ,   m_projection(std::make_shared<WorkspaceSequence>( m_nodebase->as<Core::Camera>()->getProj() ) )
    ,   m_view (std::make_shared<WorkspaceSequence>( m_nodebase->as<Core::Camera>()->getView() ) )
{}

bool WorkspaceCamera::isCamera()
{
    return true;
}

void WorkspaceCamera::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceCamera::middleContent(DIWNE::Diwne &diwne)
{
	bool inner_interaction_happen = false;

	inner_interaction_happen |= m_projection->drawNodeDiwne(diwne, true); ImGui::SameLine();

	inner_interaction_happen |= m_view->drawNodeDiwne(diwne, true);
	return inner_interaction_happen;
}

bool WorkspaceCamera::leftContent(DIWNE::Diwne& diwne) { return false; }

/* draw camera pin on opposite side */
bool WorkspaceCamera::rightContent(DIWNE::Diwne& diwne) { return WorkspaceNodeWithCoreDataWithPins::rightContent(diwne); }

int WorkspaceCamera::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceCamera::maxLenghtOfData() make no sense because every included Sequention has its own independent data");
    return -1; /* should be unused */
}




