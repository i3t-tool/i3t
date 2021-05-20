//
// Created by Sofie on 16.05.2021.
//

#include "WorkspaceTrackball.h"



WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, WorkspaceTrackballArgs const& args)
		: WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{

	buttonSize = ImVec2(
			m_dataItemsWidth*4.0f/3.0f,
			20.0f);

	textureSize = ImVec2(
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y,
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y
	);

	/*rend = new RenderTexture(&renderTexture, textureSize.x, textureSize.y);
	screen = new GameObject(unitcubeMesh, &World::shader0, World::cGridTexture);
	screen->addComponent(new Renderer());
	cam = new Camera(60.0f, screen, rend);*/
}

WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
		//TODO change create Node when MH make Trackball in Core.
		: WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::Matrix>(), headerLabel, nodeLabel){

	buttonSize = ImVec2(
			m_dataItemsWidth*4.0f/3.0f,
			20.0f);

	textureSize = ImVec2(
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y,
			5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y
	);

	/*rend = new RenderTexture(&renderTexture, textureSize.x, textureSize.y);
	screen = new GameObject(unitcubeMesh, &World::shader0, World::cGridTexture);
	screen->addComponent(new Renderer());
	cam = new Camera(60.0f, screen, rend);*/
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

		//Texture
		//cam->update();

		Application& app = Application::get();

		ImGui::Image(app.getRenderTexture(),textureSize,ImVec2(0.0f,1.0f), ImVec2(1,0));

		//Buttons
		ImGui::BeginVertical("Trackball_Buttons");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

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
			setDataItemsWidth(); /* \todo JH maybe somehow wrap setValue to Core and set Items Width */
		}
	}



}
