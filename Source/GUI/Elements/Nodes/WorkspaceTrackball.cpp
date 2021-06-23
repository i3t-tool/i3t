//
// Created by Sofie on 16.05.2021.
//

#include "WorkspaceTrackball.h"
#include "Core/Input/InputManager.h"


WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, WorkspaceTrackballArgs const& args)
		: WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{

	buttonSize = ImVec2(
			m_dataItemsWidth*4.0f/3.0f,
			I3T::getSize(ESize::Nodes_trackballButtonHeight));

	textureSize = ImVec2(
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y,
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y
	);

	texturePos = ImRect();
	move = false;

	glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLuint renderTexture;
	rend = new RenderTexture(&renderTexture, 256, 256);
	//We dont have mesh forr ball now so using a cube for now
	trackball = new GameObject(unitcubeMesh, &World::shader0, World::textures["color_grid"]);
	trackball->addComponent(new Renderer());
	trackball->translate(glm::vec3(0.0f, 0.0f, -5.0f));
	cam = new Camera(60.0f, trackball,rend);
	cam->update();
}

WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
		//TODO change create Node when MH make Trackball in Core.
		: WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::Matrix>(), headerLabel, nodeLabel){

	buttonSize = ImVec2(
			m_dataItemsWidth*4.0f/3.0f,
			I3T::getSize(ESize::Nodes_trackballButtonHeight));

	textureSize = ImVec2(
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y,
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y
	);

	texturePos = ImRect();
	move = false;

	glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	rend = new RenderTexture(&renderTexture, 256, 256);
	//We dont have mesh forr ball now so using a cube for now
	trackball = new GameObject(unitcubeMesh, &World::shader0, World::textures["color_grid"]);
	trackball->addComponent(new Renderer());
	trackball->translate(glm::vec3(0.0f, 0.0f, -5.0f));
	cam = new Camera(60.0f, trackball,rend);
	cam->update();
}

bool WorkspaceTrackball::isTrackball()
{
	return true;
}

void WorkspaceTrackball::drawDataSetValues(util::NodeBuilder& builder)
{
	drawDataFull(builder,0);
}

void WorkspaceTrackball::drawDataFull(util::NodeBuilder& builder, int index){

	if(index == -1){

		float x = InputManager::m_mouseXDelta / I3T::getSize(ESize::Nodes_TrackBallSensitivity);
		float y = InputManager::m_mouseYDelta / I3T::getSize(ESize::Nodes_TrackBallSensitivity);

		ImVec2 mouse = ImGui::GetMousePos();
		if (ImGui::IsMouseClicked(2) && (mouse.x > texturePos.Min.x && mouse.x < texturePos.Max.x && mouse.y > texturePos.Min.y && mouse.y < texturePos.Max.y))
		{
			move = true;
		}
		if (ImGui::IsMouseReleased(2))
		{
			move = false;
		}

		if(move){
				trackball->rotateAround((glm::vec3)trackball->transformation[0], -y, (glm::vec3)trackball->transformation[3]);
				trackball->rotateAround(glm::vec3(0.0f, 1.0f, 0.0f), -x, (glm::vec3)trackball->transformation[3]);
		}

		//Texture
		cam->update();
		ImGui::Image((void*)(intptr_t)renderTexture,textureSize,ImVec2(0,1), ImVec2(1,0));
		texturePos = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());


		//Buttons
		ImGui::BeginVertical("Trackball_Buttons");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		//SS why it have to be shifted? And why this value?
		ImGui::SetCursorPosX(ImGui::GetCursorPosX()-13.25f);

		if(ImGui::Button("x-", buttonSize)){
			//core call
		}
		ImGui::SameLine();
		if(ImGui::Button("y-", buttonSize)){
			//core call
		}
		ImGui::SameLine();
		if(ImGui::Button("z-", buttonSize)){
			//core call
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX()-13.25f);

		if(ImGui::Button("x+", buttonSize)){
			//core call
		}
		ImGui::SameLine();
		if(ImGui::Button("y+", buttonSize)){
			//core call
		}
		ImGui::SameLine();
		if(ImGui::Button("z+", buttonSize)){
			//core call
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();


	}else{
		//Matrix (in output)
		const glm::mat4& coreData = m_nodebase->getData(index).getMat4();
		const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
		int const idOfNode = this->m_id.Get();

		bool valueChanged = false;
		float localData;

		ImGui::PushItemWidth(m_dataItemsWidth);
		/* Drawing is row-wise */

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));


		for (int rows = 0; rows < 4; rows++)
		{
			for (int columns = 0; columns < 4; columns++)
			{

				localData = coreData[columns][rows]; /* Data are column-wise */
				valueChanged |= drawDragFloatWithMap_Inline(&localData, coreMap[columns * 4 + rows], fmt::format("##{}:r{}c{}", idOfNode, rows, columns));

				if(columns != 3)
				{
					ImGui::SameLine();
				}
			}
		}


		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();

		if (valueChanged)
		{
			//core call
			setDataItemsWidth();
		}
	}



}
