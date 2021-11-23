#include "WorkspaceModel.h"

#include "World/HardcodedMeshes.h"
#include "World/RenderTexture.h"    // FBO 

const pgr::MeshData* g_meshes[] = {  //todo - remove
		&unitcubeMesh,
		&three_axisMesh
};

const char* g_meshesNames[] = {  //todo - remove
		"cube",
		"axes"
};


WorkspaceModel::WorkspaceModel()
	: WorkspaceNodeWithCoreDataWithPins(Core::Builder::createNode<ENodeType::Model>())
{
	init();
	//setDataItemsWidth();
	//std::cout<<"Model created"<<std::endl;
	printf("Model created\n");
}
WorkspaceModel::~WorkspaceModel()
{
	// delete model from the World
	App::get().world()->removeModel(m_currentModelGameObject);
	//std::cout<<"Model deleted"<<std::endl;
  printf("Model deleted\n");

}

//bool WorkspaceModel::drawDataFull(DIWNE::Diwne& diwne, int index)
//{
//	ImGui::PushItemWidth(getDataItemsWidth(diwne));
//
//	if (ImGui::Combo("model", &m_currentModelIdx, g_meshesNames, IM_ARRAYSIZE(g_meshesNames)))
//	{
//		auto* mesh = g_meshes[m_currentModelIdx];
//	}
//
//	ImGui::PopItemWidth();
//	return false;
//}

void WorkspaceModel::init()
{

	auto* object = App::get().world()->addModel("CubeGray");
	m_nodebase->setValue(static_cast<void*>(object));  //GameObject object = static_cast<GameObject>(&(m_nodebase->getData().getPointer()));
	m_currentModelGameObject = object;

}

bool WorkspaceModel::middleContent(DIWNE::Diwne& diwne)
{
	ImGui::Text("Nad texturou      ");

	//GLuint texture[2*2];
	//texture[0] = 0;
	//texture[1] = 1;
	//texture[2] = 1;
	//texture[3] = 0;
	//ImVec2 m_textureSize {2,2};


	static float val = 0;
	
	
	ImGui::PushItemWidth(50);
	ImGui::InputFloat("Float:", &val);  //DragFloat("float: ", &val, 0.1f,0.0f, 1.0f);
	ImGui::PopItemWidth();

	// Lazy texture creation
  if(!m_textureID) {
  	int w = 84;
  	int h = 104;
			

  	//RenderTexture::RenderTexture(GLuint* colorAttachment, int w, int h
		renderTexture = new RenderTexture( &m_textureID, w, h);  // create and get the FBO and color Attachment for rendering
  	m_textureSize = {float(w),float(h)};

  	// block of control checks
  	m_fbo = renderTexture->getFbo();
  	GLuint color = renderTexture->getColor();  //todo getColorAttachmentID
  	IM_ASSERT(renderTexture->getColor() == m_textureID);
  	IM_ASSERT(renderTexture->getWidth() == w);
  	IM_ASSERT(renderTexture->getHeight() == h);

 		// Create camera rendering to the user-defined framebuffer 
 		// Camera(float viewingAngle, GameObject* sceneRoot,RenderTexture*renderTarget);
 		m_camera = new Camera(60.0f, m_currentModelGameObject, renderTexture);
	}

#if 1	
	// Trial draw to new fbo - works fine, but I should use camera
	GLint viewport[4];  // 3D view viewport
	glGetIntegerv(GL_VIEWPORT, viewport);
  glBindFramebuffer( GL_FRAMEBUFFER, m_fbo);
  glViewport(0,0,static_cast<GLsizei>(m_textureSize.x),static_cast<GLsizei>(m_textureSize.y));  	
	glClearColor(val, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3] );
#endif
	
  const float angleX = 30.0;  //degree
	const float angleY = 55.0;  //degree
	//const float angleZ = 45.0;  //degree
	m_currentModelGameObject->translate(glm::vec3(0.0f, 0.0f, -4.5));
  m_currentModelGameObject->rotate(glm::vec3(0,1,0),angleY);
  m_currentModelGameObject->rotate(glm::vec3(1,0,0),angleX);
	
		m_camera->update();
	
	m_currentModelGameObject->rotate(glm::vec3(1,0,0),-angleX);
	m_currentModelGameObject->rotate(glm::vec3(0,1,0),-angleY);
	m_currentModelGameObject->translate(glm::vec3(0.0f, 0.0f, 4.5));

	// \todo correct image size in the box
	//float padding = I3T::getSize(ESize::Nodes_FloatInnerPadding);
	//float imageWidth = m_textureSize.x + 2 * padding;

	//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
	//texture = pgr::createTexture(Config::getAbsolutePath("Data/textures/cube.png"));
	//ImGui::Image((ImTextureID)texture, m_textureSize, ImVec2(1.0/3.0,0.0), ImVec2(2.0/3.0,1.0/3.0) );  // single cube side X+
	ImGui::Image(reinterpret_cast<ImTextureID>(m_textureID), m_textureSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f,0.0f)); //vertiocal flip

	//if (ImGui::Combo("model", &m_currentModelIdx, g_meshesNames, IM_ARRAYSIZE(g_meshesNames)))
	//{
	//	auto* mesh = g_meshes[m_currentModelIdx];

	//	auto* object = App::get().world()->addModel("CubeGray");
	//m_nodebase->setValue(static_cast<void*>(object));
	//}
	
	ImGui::Text("Pod texturou");
	
  return false;
}

int WorkspaceModel::maxLenghtOfData()  //todo
{
    Debug::Assert(false, "Calling WorkspaceModel::maxLenghtOfData() make no sense because Model has no float data to show");
    return -1; /* should be unused */
}

void WorkspaceModel::drawMenuLevelOfDetail() //todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}
