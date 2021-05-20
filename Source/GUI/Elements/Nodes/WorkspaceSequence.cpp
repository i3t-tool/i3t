#include "WorkspaceSequence.h"
#include "WorkspaceMatrixTranslation.h"

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "WorkspaceSequence";
}

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, Ptr<Core::Sequence> nodebase, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, nodebase == nullptr ? Core::Builder::createSequence() : nodebase, headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "WorkspaceSequence";

	//pushNode(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));
	//pushNode(std::make_shared<WorkspaceMatrixTranslation>(headerBackground));

	m_dataRect = ImRect(0,0,0,0); /* init value used just in first frame */
}

bool WorkspaceSequence::isSequence()
{
    return true;
}

int WorkspaceSequence::getInnerPosition(ImVec2 point)
{
    ImVec2 sequence_position = ne::GetNodePosition(getId());
    ImVec2 sequence_size = ne::GetNodeSize(getId());

    ImRect rect = ImRect(sequence_position, sequence_position+sequence_size);
    if (!rect.Contains(point))
    {
        return -1;
    }

    rect.Max.x = rect.Min.x; /* squeeze rect at begin -> then in cykle shift rect and check point position */
    int i = 0;
    for (auto const & innerNode : getInnerWorkspaceNodes())
    {
        rect.Max.x = ne::GetNodePosition(innerNode->getId()).x + ne::GetNodeSize(innerNode->getId()).x/2;
        if(rect.Contains(point))
        {
            return i;
        }
        rect.Min.x = rect.Max.x;
        i++;
    }
    return i;
}

int WorkspaceSequence::getInnerPosition(std::vector<ImVec2> points)
{
    int position = -1; /* position of first found point that match */
    for(auto const & point : points)
    {
        position = getInnerPosition(point);
        if (position > -1){return position;}
    }
    return position;
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

void WorkspaceSequence::setPostionOfDummyData(int positionOfDummyData) {m_position_of_dummy_data = positionOfDummyData;}
void WorkspaceSequence::setWidthOfDummy(int width) {m_widthOfDummy = width;}



void WorkspaceSequence::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
{
    builder.Begin(m_id);
	drawHeader(builder);
	drawInputs(builder, newLinkPin);
    drawData(builder,0);
	drawOutputs(builder, newLinkPin);
	builder.End();

        ImVec2 dataLeftTop = ne::GetNodePosition(m_id) + ImVec2(I3T::getSize(ESizeVec2::Nodes_IconSize).x+5,builder.HeaderMax.y-builder.HeaderMin.y+1) /*+ ImVec2(0, builder.HeaderMax.y)*/;  /* \todo JH add shift based on size of header and inputs pins */
        m_dataRect = ImRect(dataLeftTop, dataLeftTop);

        int i = 0;
        for( auto const & transformation : m_workspaceTransformation )
        {
            if(m_position_of_dummy_data == i)
            {
                ne::SetNodePosition(transformation->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
            }else
            {
                ne::SetNodePosition(transformation->getId(), ImVec2(m_dataRect.Max.x, m_dataRect.Min.y));
            }

            Theme& t = I3T::getTheme();
            t.transformationColorTheme();

            transformation->drawNode(builder, nullptr, false);

						ImVec2 tmp = ImGui::GetItemRectSize();
						if(i == 0){
							m_dataRect.Max.y += tmp.y - 4* I3T::getSize(ESizeVec2::Nodes_IconSize).y +1;
						}
						m_dataRect.Max.x += tmp.x;


            i++;
        }
        if (m_position_of_dummy_data == i) /* add dummy after last inner */
        {
            m_dataRect.Add(m_dataRect.Max + ImVec2(m_widthOfDummy, 0));
        }
}

void WorkspaceSequence::drawDataSetValues(util::NodeBuilder& builder)
{
    drawDataFull(builder,0);
}


ImVec2 WorkspaceSequence::getDataSize()
{
    return m_dataRect.Max - m_dataRect.Min + ImVec2(I3T::getSize(ESizeVec2::Nodes_IconSize).x,0);
}

void WorkspaceSequence::drawDataFull(util::NodeBuilder& builder, int index)
{
    ImGui::Dummy(getDataSize());
}

int WorkspaceSequence::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every included Transformation has its own independent data");
    return -1; /* should be unused */
}



