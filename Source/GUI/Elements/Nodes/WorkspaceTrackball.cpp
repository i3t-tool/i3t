//
// Created by Sofie on 16.05.2021.
//

#include "WorkspaceTrackball.h"

#include <World/Components/Camera.h>
#include <World/Components/Renderer.h>
#include <World/HardcodedMeshes.h>
#include <World/RenderTexture.h>

WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, WorkspaceTrackballArgs const& args)
		: WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=args.nodebase})
{}

WorkspaceTrackball::WorkspaceTrackball(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
		//TODO change create Node when MH make Trackball in Core.
		: WorkspaceMatrix4x4(headerBackground, Core::Builder::createNode<ENodeType::Matrix>(), headerLabel, nodeLabel){
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
		ImVec2 buttonSize = ImVec2(
				m_dataItemsWidth*4.0f/3.0f,
				20.0f);
		ImVec2 textureSize = ImVec2(
				5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y,
				5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * buttonSize.y
				);

		//Texture

		/*GameObject*g=new GameObject(unitcubeMesh, &World::shader0, World::cubeTexture);
		g->translate(glm::vec3(-0.0f, 0.0f, -5.0f));//translace do zorného pole kamery
		GLuint renderTexture;
		RenderTexture* rend = new RenderTexture(&renderTexture,256,256);
		Camera*c=new Camera(60.0f, g, rend);
		g->addComponent(new Renderer());
		c->update();
		ImGui::Image(rend,ImVec2(50.0f,50.0f),ImVec2(0.0f,0.0f), ImVec2(50.0f, 50.0f));*/


		ImGui::Image(
					(ImTextureID)(intptr_t)pgr::createTexture(
					Config::getAbsolutePath("Data/textures/trackballTest.png"),
					true),
						textureSize,
				ImVec2(0.0f,0.0f),
				textureSize
				);

		//Buttons
		ImGui::BeginVertical("Trackball_Buttons");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX()-12.0f);

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

		ImGui::SetCursorPosX(ImGui::GetCursorPosX()-12.0f);

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
