#include "WorkspaceScreen.h"

//#include "World/HardcodedMeshes.h"
#include "Utils/Format.h"
#include "World/RenderTexture.h"    // FBO

#define TEST

WorkspaceScreen::WorkspaceScreen(DIWNE::Diwne& diwne)
	: WorkspaceNodeWithCoreDataWithPins(diwne, Core::Builder::createNode<ENodeType::Screen>())
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

	getNodebase()->setValue(m_textureSize.x/m_textureSize.y, 1); /* \todo Jh always 1? */
	printf("Screen initialized\n");
}


bool WorkspaceScreen::middleContent()
{
	bool interaction_happen = false, resize_texture = false;
	ImVec2 dragDelta;
	ImVec2 buttonSize = ImVec2(20,20); /* \todo JH MH from setting */

	// \todo
	m_camera->m_perspective = getNodebase()->getData().getMat4();
	m_camera->update();

	ImGui::TextUnformatted(Utils::toString(m_camera->m_perspective).c_str());

	ImGui::Image(reinterpret_cast<ImTextureID>(m_textureID), m_textureSize*diwne.getWorkAreaZoom(), ImVec2(0.0f, 1.0f), ImVec2(1.0f,0.0f)); //vertiocal flip

	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	cursorPos.y -= (buttonSize.y*diwne.getWorkAreaZoom() + ImGui::GetStyle().ItemSpacing.y);
	ImGui::SetCursorScreenPos(cursorPos);

	interaction_happen |= diwne.IconButton(DIWNE::IconType::Rectangle, ImColor(100,50,50,150), ImColor(100,50,50,150), /* \todo JH MH constants to setting */
                                         DIWNE::IconType::Cross, ImColor(100,100,150,150), ImColor(100,100,150,150),
                                         buttonSize*diwne.getWorkAreaZoom(), ImVec4(2,2,2,2)/**diwne.getWorkAreaZoom()*/, true, fmt::format("screenButton:{}left", getId()));
    ImGui::SameLine();

    if (diwne.bypassIsItemActive() && diwne.bypassIsMouseDragging0())
    {
        interaction_happen = true;
        resize_texture = true;

        dragDelta = diwne.bypassGetMouseDragDelta0()/diwne.getWorkAreaZoom();

        ImVec2 nodePos = getNodePositionDiwne(); nodePos.x += dragDelta.x;
        setNodePositionDiwne(nodePos);

        dragDelta.x *= -1; /* for same code (sign) in if(resize_texture) */


    }

	ImGui::Indent(std::max(0.0f, (m_textureSize.x - buttonSize.x)*diwne.getWorkAreaZoom()));

    interaction_happen |= diwne.IconButton(DIWNE::IconType::Rectangle, ImColor(100,50,50,150), ImColor(100,50,50,150),
                                         DIWNE::IconType::Cross, ImColor(100,100,150,150), ImColor(100,100,150,150),
                                         buttonSize*diwne.getWorkAreaZoom(), ImVec4(2,2,2,2)/**diwne.getWorkAreaZoomDiwne()*/, true, fmt::format("screenButton:{}right", getId()));

    if (diwne.bypassIsItemActive() && diwne.bypassIsMouseDragging0())
    {
        interaction_happen = true;
        resize_texture = true;

        dragDelta = diwne.bypassGetMouseDragDelta0()/diwne.getWorkAreaZoom();
    }

    if(resize_texture)
    {
        m_textureSize.x = std::max(2*(buttonSize.x+ImGui::GetStyle().ItemSpacing.x/diwne.getWorkAreaZoom()), m_textureSize.x + dragDelta.x); /* button should fit into middle... */
        m_textureSize.y = std::max(buttonSize.y+ImGui::GetStyle().ItemSpacing.y/diwne.getWorkAreaZoom(), m_textureSize.y + dragDelta.y);

        ImGui::ResetMouseDragDelta(0);

        m_renderTexture->resize(m_textureSize.x*diwne.getWorkAreaZoom(), m_textureSize.y*diwne.getWorkAreaZoom());
        getNodebase()->setValue(m_textureSize.x/m_textureSize.y, 1); /* \todo Jh always 1? */
    }
    
  getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1);  // must be index 1, as there is a hidden output index 0, storing the incoming PV matrix

  return interaction_happen;
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
