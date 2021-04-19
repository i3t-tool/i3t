#include "WorkspaceSequence.h"

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args)
    : WorkspaceNodeWithCoreData(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createSequence()})
{

	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL; /* \todo rewrite as some FLOAT_UNDEFINED_VALUE */
	fw.name = "WorkspaceSequence";
}

WorkspaceSequence::WorkspaceSequence(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    : WorkspaceNodeWithCoreData(headerBackground, Core::Builder::createSequence(), headerLabel, nodeLabel)
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NULL; /* \todo rewrite as some FLOAT_UNDEFINED_VALUE */
	fw.name = "WorkspaceSequence";
}

void WorkspaceSequence::drawDataFull(util::NodeBuilder& builder)
{
    /*for( auto const & transformation : static_cast<Core::Sequence>(m_nodebase)->getMatrices() )
    {
        transformation->drawData();
    }*/
}

void WorkspaceSequence::drawDataSetValues(util::NodeBuilder& builder)
{
    // \todo JH
	drawDataFull(builder);
}

int WorkspaceSequence::maxLenghtOfData()
{
    //Debug::Assert(false, "Calling WorkspaceSequence::maxLenghtOfData() make no sense because every includet Transformation has its own independent data");
    return -1; /* should be unused */
}



