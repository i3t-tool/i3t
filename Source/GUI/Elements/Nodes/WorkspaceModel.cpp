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
	: WorkspaceNodeWithCoreDataWithPins(Core::Builder::createNode<ENodeType::Model>())
{
	init();
	//setDataItemsWidth();
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

bool WorkspaceModel::middleContent(DIWNE::Diwne& diwne)
{
    ImGui::Text("Speciální text v model");
   return false;
}

int WorkspaceModel::maxLenghtOfData()
{
    Debug::Assert(false, "Calling WorkspaceModel::maxLenghtOfData() make no sense because Model has no float data to show");
    return -1; /* should be unused */
}

void WorkspaceModel::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}
