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

	ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(20,15);  /* \todo JH add shift based on size of header and inputs pins */
    m_dataRect = ImRect(dataLeftTop, dataLeftTop);

    ne::SetNodePosition(m_projection->getId(), m_dataRect.Min);
	m_projection->drawNode(builder);
	m_dataRect.Add(ImGui::GetItemRectMax());

	ne::SetNodePosition(m_view->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
	m_view->drawNode(builder);
	m_dataRect.Add(ImGui::GetItemRectMax());
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




