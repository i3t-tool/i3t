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

WorkspaceModel::WorkspaceModel()
	: WorkspaceNodeWithCoreData(Core::Builder::createNode<ENodeType::Model>())
{
	init();
	setDataItemsWidth();
}

bool WorkspaceModel::drawDataFull(DIWNE::Diwne& diwne, int index)
{
	ImGui::PushItemWidth(getDataItemsWidth(diwne));

	if (ImGui::Combo("model", &m_currentModelIdx, g_meshesNames, IM_ARRAYSIZE(g_meshesNames)))
	{
		auto* mesh = g_meshes[m_currentModelIdx];
	}

	ImGui::PopItemWidth();
	return false;
}

int	 WorkspaceModel::maxLenghtOfData(int index) { return 1; }

void WorkspaceModel::init()
{
//	fw.showMyPopup = false;
//	fw.id = "";
//	fw.value = NAN;
//	fw.name = "pulse";
//	fw.rows = 0;
//	fw.columns = 0;

	auto* object = App::get().world()->addModel("CubeGray");
	m_nodebase->setValue(static_cast<void*>(object));
}
