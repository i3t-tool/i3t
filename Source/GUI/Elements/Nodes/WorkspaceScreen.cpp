////
//// Created by Sofie on 18.05.2021.
////
//
//#include "WorkspaceScreen.h"
//
//WorkspaceScreen::WorkspaceScreen()
//		: WorkspaceFloat(Core::Builder::createNode<ENodeType::Screen>())
//{
//	glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//	rend = new RenderTexture(&renderTexture, 256, 256);
//	cam = new Camera(60.0f, Application::get().world()->sceneRoot ,rend);
//	cam->update();
//
//}
//
//
//bool WorkspaceScreen::drawDataFull(DIWNE::Diwne& diwne, int index)
//{
//
//	if(m_workspaceInputs[0]->getCorePin().isPluggedIn()){
//		glm::mat4 camera = Core::GraphManager::getParent(m_nodebase)->getData(2).getMat4();
//
//		cam->m_perspective = camera;
//		cam->update();
//
//		ImGui::Image((void*)(intptr_t)renderTexture,I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize),ImVec2(0.0f,1.0f), ImVec2(1,0));
//	}
//	return false;
//
//}
