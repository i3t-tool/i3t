#include "WorkspaceModel.h"

#include "World/HardcodedMeshes.h"

const pgr::MeshData* g_meshes[] = {
		&unitcubeMesh,
		&three_axisMesh
};

const char* g_meshesNames[] = {
		"cube",
		"axes"
};

WorkspaceModel::WorkspaceModel(ImTextureID headerBackground, const WorkspaceModelArgs& args)
	: WorkspaceNodeWithCoreData(headerBackground, { .levelOfDetail = args.levelOfDetail, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
{
	init();
	setDataItemsWidth();
}

WorkspaceModel::WorkspaceModel(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
	: WorkspaceNodeWithCoreData(headerBackground, Core::Builder::createNode<ENodeType::Model>(), headerLabel, nodeLabel)
{
	init();
	setDataItemsWidth();
}

void WorkspaceModel::drawDataFull(util::NodeBuilder& builder, int index)
{
	ImGui::PushItemWidth(m_dataItemsWidth);

	if (ImGui::Combo("model", &m_currentModelIdx, g_meshesNames, IM_ARRAYSIZE(g_meshesNames)))
	{
		auto* mesh = g_meshes[m_currentModelIdx];
	}

	ImGui::PopItemWidth();
}

void WorkspaceModel::drawDataSetValues(util::NodeBuilder& builder)
{
	drawDataFull(builder, 0);
}

int	 WorkspaceModel::maxLenghtOfData() { return 1; }

void WorkspaceModel::init()
{
	fw.showMyPopup = false;
	fw.id = "";
	fw.value = NAN;
	fw.name = "pulse";
	fw.rows = 0;
	fw.columns = 0;

	auto* object = App::get().world()->addModel("CubeGray");
	m_nodebase->setValue(static_cast<void*>(object));
}
