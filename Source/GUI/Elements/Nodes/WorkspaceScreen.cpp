#include "WorkspaceScreen.h"


//#include "World/HardcodedMeshes.h"
#include "World/RenderTexture.h"    // FBO

#define TEST


WorkspaceScreen::WorkspaceScreen()
	: WorkspaceNodeWithCoreDataWithPins(Core::Builder::createNode<ENodeType::Screen>())
{
	init();
}

WorkspaceScreen::~WorkspaceScreen()
{
	if(m_renderTexture)
		delete(m_renderTexture);
	if(m_camera)
		delete(m_camera);
}


void WorkspaceScreen::init()
{
	m_renderTexture = new RenderTexture( &m_textureID, static_cast<int>(m_textureSize.x), static_cast<int>(m_textureSize.y));  // create and get the FBO and color Attachment for rendering

	//Camera(float viewingAngle, GameObject* sceneRoot, RenderTexture* renderTarget);
	m_camera = new Camera(60.0f, Application::get().world()->sceneRoot, m_renderTexture);  // version with the object shared with the 3D scene and positioned in the scene graph)

	printf("Screen initialized\n");
}


bool WorkspaceScreen::middleContent(DIWNE::Diwne& diwne)
{

	m_camera->update();

	ImGui::Image(reinterpret_cast<ImTextureID>(m_textureID), m_textureSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f,0.0f)); //vertiocal flip

	/* \todo JH PF lots of bug here - just for first testing/using...; better place; ImageButton / DiwneIcon */
	float buttonWidth = 20;
	ImGui::Indent(m_textureSize.x - buttonWidth);
	ImGui::Button("resize_button", ImVec2(buttonWidth, 20));
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
    {
        ImVec2 dragDelta = ImGui::GetMouseDragDelta();
        m_textureSize.x = std::max(buttonWidth, m_textureSize.x+dragDelta.x); /* button should fit into middle... */
        m_textureSize.y = std::max(0.0f, m_textureSize.y+dragDelta.y);

        ImGui::ResetMouseDragDelta(0);

    		m_renderTexture->resize(m_textureSize.x, m_textureSize.y);
    	
    	  return true;
    }

  return false;
}

int WorkspaceScreen::maxLenghtOfData()  //todo
{
    Debug::Assert(false, "TODO Calling WorkspaceScreen::maxLenghtOfData() makes no sense - really?!?!?!!");
    return -1; /* should be unused */
}

void WorkspaceScreen::drawMenuLevelOfDetail() //todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}
