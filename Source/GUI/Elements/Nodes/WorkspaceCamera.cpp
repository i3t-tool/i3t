#include "WorkspaceCamera.h"

WorkspaceCamera::WorkspaceCamera()
    :   WorkspaceNodeWithCoreData(Core::GraphManager::createCamera())
    ,   m_projection(std::make_shared<WorkspaceSequence>( m_nodebase->as<Core::Camera>()->getProj() ) )
    ,   m_view (std::make_shared<WorkspaceSequence>( m_nodebase->as<Core::Camera>()->getView() ) )
{}

bool WorkspaceCamera::isCamera()
{
    return true;
}

//void WorkspaceCamera::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
//{
//    builder.Begin(m_id);
//	drawHeader(builder);
//	drawInputs(builder, newLinkPin);
//    drawData(builder, 0);
//	drawOutputs(builder, newLinkPin);
//	builder.End();
//
//		ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(3,builder.HeaderMax.y-builder.HeaderMin.y+1);
//   	m_dataRect = ImRect(dataLeftTop, dataLeftTop);
//
//    ne::SetNodePosition(m_projection->getId(), m_dataRect.Min);
//  Theme& t = I3T::getTheme();
//  if(m_projection->isTransformation()){
//    t.transformationColorTheme();
//  }else
//  {
//    t.operatorColorTheme();
//  }
//	m_projection->drawNode(builder);
//
//	/*if(!m_nodebase->as<Core::Camera>()->getProj()->getMatrices().empty()){
//		const glm::mat4& coreData = m_nodebase->as<Core::Camera>()->getProj()->getMatrices()[0]->getData().getMat4();
//		I3T::getTheme();
//	}*/
//	t.returnFloatColorToDefault();
//
//
//	if(m_projection->getInnerWorkspaceNodes().empty()){
//		m_dataRect.Add(ImGui::GetItemRectMax());
//	}else
//	{
//		m_dataRect.Add(ImGui::GetItemRectMax() + ImVec2(1.5f * I3T::getSize(ESizeVec2::Nodes_IconSize).x, 0));
//	}
//
//	ne::SetNodePosition(m_view->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
//
//  if(m_view->isTransformation()){
//    t.transformationColorTheme();
//  }else
//  {
//    t.operatorColorTheme();
//  }
//	m_view->drawNode(builder);
//  t.returnFloatColorToDefault();
//
//
//	if(m_view->getInnerWorkspaceNodes().empty()){
//		m_dataRect.Add(ImGui::GetItemRectMax());
//	}else
//	{
//		m_dataRect.Add(ImGui::GetItemRectMax() + ImVec2(1.5f * I3T::getSize(ESizeVec2::Nodes_IconSize).x, 0));
//	}
//}

bool WorkspaceCamera::middleContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
	inner_interaction_happen |= m_projection->drawNodeDiwne(diwne);
	inner_interaction_happen |= m_view->drawNodeDiwne(diwne);
	return inner_interaction_happen;
}

int WorkspaceCamera::maxLenghtOfData(int index)
{
    Debug::Assert(false, "Calling WorkspaceCamera::maxLenghtOfData() make no sense because every included Sequention has its own independent data");
    return -1; /* should be unused */
}




