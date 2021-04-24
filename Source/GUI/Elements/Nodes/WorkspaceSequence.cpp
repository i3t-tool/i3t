#include "WorkspaceSequence.h"
#include "WorkspaceMatrixTranslation.h"

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createSequence()})
{

	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL; /* \todo rewrite as some FLOAT_UNDEFINED_VALUE */
	fw.name = "WorkspaceSequence";

	m_workspaceTransformation.push_back(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));
}

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, Core::Builder::createSequence(), headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL; /* \todo rewrite as some FLOAT_UNDEFINED_VALUE */
	fw.name = "WorkspaceSequence";

	m_workspaceTransformation.push_back(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));
	m_workspaceTransformation.push_back(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));
	m_dataSize = ImVec2(0,0); /* init value used just in first frame */
}

bool WorkspaceSequence::isSequence()
{
    return true;
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const  { return m_workspaceTransformation; }


void WorkspaceSequence::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
    builder.Begin(m_id);
	drawHeader(builder);
	drawInputs(builder, newLinkPin);
	drawData(builder);
	drawOutputs(builder, newLinkPin);
	builder.End();

    ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(30,20);  /* \todo JH add some shift based on header and inputs */
    ImVec2 dataRightBottom = dataLeftTop;

    ImGui::SetCursorPos( dataLeftTop );
    for( auto const & transformation : m_workspaceTransformation )
    {
        ne::SetNodePosition(transformation->getId(), ImVec2(dataRightBottom.x, dataLeftTop.y));
        transformation->drawNode(builder, nullptr);

        dataRightBottom = ImGui::GetItemRectMax();
    }

    m_dataSize = dataRightBottom - dataLeftTop;


//	ne::PushStyleColor(ne::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
//    ne::PushStyleColor(ne::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
//    ne::BeginNode(m_id);
//    ImGui::PushID(m_id.AsPointer());
//    ImGui::BeginVertical("content");
//    ImGui::BeginHorizontal("horizontal");
//    ImGui::Spring(1);
//    ImGui::TextUnformatted("Testovaci");
//    ImGui::Spring(1);
//    ImGui::EndHorizontal();
//    ne::Group(ImVec2(200, 400));
//    ImGui::EndVertical();
//    ImGui::PopID();
//    ne::EndNode();
//    ne::PopStyleColor(2);

}

void WorkspaceSequence::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}

void WorkspaceSequence::drawDataFull(util::NodeBuilder& builder)
{
    ImGui::Dummy(m_dataSize);
}

int WorkspaceSequence::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



