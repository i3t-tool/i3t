#include "WorkspaceModel.h"

#include "World/HardcodedMeshes.h"
#include "World/RenderTexture.h"    // FBO

#undef TEST
#define TEST1

const pgr::MeshData* g_meshes[] = {  //todo - remove
		&unitcubeMesh,
		&three_axisMesh
};

const char* g_meshesNames[] = {  //todo - remove
		"cube",
		"axes"
};

const float angleX = 30.0;  //degree
const float angleY = 55.0;  //degree

WorkspaceModel::WorkspaceModel()
	: WorkspaceNodeWithCoreDataWithPins(Core::Builder::createNode<ENodeType::Model>())
{
	init();
	//setDataItemsWidth();
}

WorkspaceModel::~WorkspaceModel()
{
	// delete model from the World
	App::get().world()->removeModel(m_sceneModel);
	
	// delete local workspace model 
	if(m_workspaceModel)
		delete(m_workspaceModel);

	if(renderTexture)
		delete(renderTexture);

	if(m_camera)
		delete(m_camera);
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

	auto* object = App::get().world()->addModel("CubeGray");   // object added into the scene graph
	m_sceneModel = object;

	// second object just for preview in this box in the Workspace
	m_workspaceModel=new GameObject(unitcubeMesh, &World::shader0, World::textures["cube"]);
  m_workspaceModel->addComponent(new Renderer());

	// nice initial transformation
	m_workspaceModel->translate(glm::vec3(0.0f, 0.0f, -4.5));
  m_workspaceModel->rotate(glm::vec3(0,1,0),angleY);
  m_workspaceModel->rotate(glm::vec3(1,0,0),angleX);

  // pass object pointer to the core
	// to remove the warning:
	ValueSetResult result = 
	m_nodebase->setValue(static_cast<void*>(object));  //GameObject object = static_cast<GameObject>(&(m_nodebase->getData().getPointer()));
}


bool WorkspaceModel::middleContent(DIWNE::Diwne& diwne)
{

	
	bool interaction_happen = false;

#ifdef TEST1
	ImGui::Text("Nad texturou      ");

	ImGui::PushItemWidth(50);
	interaction_happen = ImGui::DragFloat("float: ", &val, 0.01f,-1.0f, 1.0f);
	ImGui::PopItemWidth();
#endif	

#if 1
	auto pin = m_nodebase->getInPin(0);
	//const DataStore& data;
	
	//if(pin.isPluggedIn())
		//data = pin.data();
	//else
	//	auto data = nullptr;
	
	//auto data = m_nodebase->getInPin(0).data();
#endif
	
	// Lazy texture creation
  if(!m_textureID) {
		renderTexture = new RenderTexture( &m_textureID, static_cast<int>(m_textureSize.x), static_cast<int>(m_textureSize.y));  // create and get the FBO and color Attachment for rendering
  	
#ifdef TEST
  	// block of control checks
  	m_fbo = renderTexture->getFbo();
  	GLuint color = renderTexture->getColor();  //todo getColorAttachmentID
  	IM_ASSERT(renderTexture->getColor() == m_textureID);
  	IM_ASSERT(renderTexture->getWidth() == w);
  	IM_ASSERT(renderTexture->getHeight() == h);
#endif
  	
 		// Create camera rendering to the user-defined framebuffer 
 		// Camera(float viewingAngle, GameObject* sceneRoot, RenderTexture* renderTarget);
 		// m_camera = new Camera(60.0f, m_sceneModel, renderTexture);  // version with the object shared with the 3D scene and positioned in the scene graph)
  	m_camera = new Camera(60.0f, m_workspaceModel, renderTexture); // version with the additional object just for this box in the workspace
 		// m_camera->m_perspective = glm::perspective(glm::radians(60.0f), float(m_textureSize.x) / float(m_textureSize.y), 0.2f, 1000.0f);
    
	}
	
#ifdef TEST
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
	
	//const float angleZ = 45.0;  //degree
	//m_sceneModel->translate(glm::vec3(0.0f, 0.0f, -4.5));
 // m_sceneModel->rotate(glm::vec3(0,1,0),angleY);
 // m_sceneModel->rotate(glm::vec3(1,0,0),angleX);
	m_sceneModel->translate(glm::vec3(0.0f, 0.0f, -val));
		m_camera->update();
	
	//m_sceneModel->rotate(glm::vec3(1,0,0),-angleX);
	//m_sceneModel->rotate(glm::vec3(0,1,0),-angleY);
	//m_sceneModel->translate(glm::vec3(0.0f, 0.0f, 4.5));

	// \todo correct image size in the box
	//float padding = I3T::getSize(ESize::Nodes_FloatInnerPadding);
	//float imageWidth = m_textureSize.x + 2 * padding;

	//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
	//texture = pgr::createTexture(Config::getAbsolutePath("Data/textures/cube.png"));  // fixed texture may be enough
	//ImGui::Image((ImTextureID)m_textureID, m_textureSize, ImVec2(1.0/3.0,0.0), ImVec2(2.0/3.0,1.0/3.0) );  // single cube side X+
	ImGui::Image(reinterpret_cast<ImTextureID>(m_textureID), m_textureSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f,0.0f)); //vertiocal flip

	//if (ImGui::Combo("model", &m_currentModelIdx, g_meshesNames, IM_ARRAYSIZE(g_meshesNames)))
	//{
	//	auto* mesh = g_meshes[m_currentModelIdx];

	//	auto* object = App::get().world()->addModel("CubeGray");
	//m_nodebase->setValue(static_cast<void*>(object));
	//}
	
	//ImGui::Text("Pod texturou");
	
  return interaction_happen;
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
