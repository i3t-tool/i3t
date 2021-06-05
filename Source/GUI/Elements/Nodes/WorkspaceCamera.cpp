#include "WorkspaceCamera.h"

WorkspaceCamera::WorkspaceCamera(ImTextureID headerBackground, WorkspaceCameraArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::GraphManager::createCamera()})
{

	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "WorkspaceCamera";

//    m_projection = std::make_shared<WorkspaceSequence>(headerBackground, {.nodebase=m_nodebase->as<Core::Camera>()->getProj()});
//	m_view = std::make_shared<WorkspaceSequence>(headerBackground, {.nodebase=m_nodebase->as<Core::Camera>()->getView()});

	m_dataRect = ImRect(0,0,0,0); /* init value used just in first frame */
}

WorkspaceCamera::WorkspaceCamera(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, Core::GraphManager::createCamera(), headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "WorkspaceSequence";

	m_projection = std::make_shared<WorkspaceSequence>(headerBackground, m_nodebase->as<Core::Camera>()->getProj());
	m_view = std::make_shared<WorkspaceSequence>(headerBackground, m_nodebase->as<Core::Camera>()->getView());

	m_dataRect = ImRect(0,0,0,0); /* init value used just in first frame */
}

bool WorkspaceCamera::isCamera()
{
    return true;
}

Ptr<WorkspaceSequence> const& WorkspaceCamera::getProjection() const {return m_projection;}
Ptr<WorkspaceSequence> const& WorkspaceCamera::getView() const {return m_view;}

void WorkspaceCamera::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
{
    builder.Begin(m_id);
	drawHeader(builder);
	drawInputs(builder, newLinkPin);
    drawData(builder, 0);
	drawOutputs(builder, newLinkPin);
	builder.End();

		ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(3,builder.HeaderMax.y-builder.HeaderMin.y+1);
   	m_dataRect = ImRect(dataLeftTop, dataLeftTop);

    ne::SetNodePosition(m_projection->getId(), m_dataRect.Min);
  Theme& t = I3T::getTheme();
  if(m_projection->isTransformation()){
    t.transformationColorTheme();
  }else
  {
    t.operatorColorTheme();
  }
	m_projection->drawNode(builder);

	/*if(!m_nodebase->as<Core::Camera>()->getProj()->getMatrices().empty()){
		const glm::mat4& coreData = m_nodebase->as<Core::Camera>()->getProj()->getMatrices()[0]->getData().getMat4();
		I3T::getTheme();
	}*/
	t.returnFloatColorToDefault();


	if(m_projection->getInnerWorkspaceNodes().empty()){
		m_dataRect.Add(ImGui::GetItemRectMax());
	}else
	{
		m_dataRect.Add(ImGui::GetItemRectMax() + ImVec2(1.5f * I3T::getSize(ESizeVec2::Nodes_IconSize).x, 0));
	}

	ne::SetNodePosition(m_view->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));

  if(m_view->isTransformation()){
    t.transformationColorTheme();
  }else
  {
    t.operatorColorTheme();
  }
	m_view->drawNode(builder);
  t.returnFloatColorToDefault();


	if(m_view->getInnerWorkspaceNodes().empty()){
		m_dataRect.Add(ImGui::GetItemRectMax());
	}else
	{
		m_dataRect.Add(ImGui::GetItemRectMax() + ImVec2(1.5f * I3T::getSize(ESizeVec2::Nodes_IconSize).x, 0));
	}
}

void WorkspaceCamera::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder, 0);
}


ImVec2 WorkspaceCamera::getDataSize()
{
    return m_dataRect.Max - m_dataRect.Min;
}

void WorkspaceCamera::drawDataFull(util::NodeBuilder& builder, int index=0)
{
    ImGui::Dummy(getDataSize());
}

int WorkspaceCamera::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceCamera::maxLenghtOfData() make no sense because every included Sequention has its own independent data");
    return -1; /* should be unused */
}




