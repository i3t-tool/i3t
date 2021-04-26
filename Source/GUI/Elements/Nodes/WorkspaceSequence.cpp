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

	pushNode(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));
	pushNode(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));

	m_dataRect = ImRect(0,0,0,0); /* init value used just in first frame */
}

bool WorkspaceSequence::isSequence()
{
    return true;
}

int WorkspaceSequence::getInnerPosition(ImVec2 point)
{
    ImVec2 position = ne::GetNodePosition(getId());
    ImVec2 size = ne::GetNodeSize(getId());
    ImRect rect = ImRect(position, position+size);
    if (!rect.Contains(point))
    {
        return -1;
    }
    int i = 0;
    for (auto const & innerNode : getInnerWorkspaceNodes())
    {
        position = ne::GetNodePosition(innerNode->getId());
        size = ne::GetNodeSize(innerNode->getId());
        rect  = ImRect(position, position+size);
        rect.TranslateX(-size.x/2);
        if(rect.Contains(point))
        {
            return i;
        }
        i++;
    }
    return i;
}

void WorkspaceSequence::popNode(Ptr<WorkspaceNodeWithCoreData> node)
{

    auto node_iter = std::find_if(m_workspaceTransformation.begin(),
                                            m_workspaceTransformation.end(),
                                            [node](Ptr<WorkspaceNodeWithCoreData> const &in_node) -> bool { return node->getId() == in_node->getId(); });

    if (node_iter != m_workspaceTransformation.end())
    {
        int index = node_iter-m_workspaceTransformation.begin();
        m_workspaceTransformation.erase(node_iter);
        m_nodebase->as<Core::Sequence>()->popMatrix(index);
    }
}

void WorkspaceSequence::pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index)
{
    if(node->isTransformation() && 0 <= index && index <= m_workspaceTransformation.size())
    {
        m_workspaceTransformation.insert(m_workspaceTransformation.begin()+index, node);
        m_nodebase->as<Core::Sequence>()->addMatrix(node->getNodebase()->as<Core::Transformation>(), index);
    }
}


std::vector<Ptr<WorkspaceNodeWithCoreData>> const& WorkspaceSequence::getInnerWorkspaceNodes() const  { return m_workspaceTransformation; }


void WorkspaceSequence::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
{
    builder.Begin(m_id);
	drawHeader(builder);
	drawInputs(builder, newLinkPin);
    drawData(builder);
	drawOutputs(builder, newLinkPin);
	builder.End();

        ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(40,30);  /* \todo JH add shift based on size of header and inputs pins */
        m_dataRect = ImRect(dataLeftTop, dataLeftTop);

        int i = 0;
        for( auto const & transformation : m_workspaceTransformation )
        {
            ne::SetNodePosition(transformation->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
            transformation->drawNode(builder, nullptr, false);

            m_dataRect.Add(ImGui::GetItemRectMax());
        }
}

void WorkspaceSequence::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder);
}


ImVec2 WorkspaceSequence::getDataSize()
{
    return m_dataRect.Max - m_dataRect.Min;
}

void WorkspaceSequence::drawDataFull(util::NodeBuilder& builder)
{
    ImGui::Dummy(getDataSize());
}

int WorkspaceSequence::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



