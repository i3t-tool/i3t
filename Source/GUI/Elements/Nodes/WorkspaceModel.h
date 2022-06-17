#pragma once
//----------------
/**
 * \file WorkspaceModel.h
 * \author Martin Herich, Petr Felkel
 * \brief model box in the workspace
 * The initial version with a lot of comments explaining the basics.
 */
//---------------
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceModel : public WorkspaceNodeWithCoreDataWithPins
{
public:
	WorkspaceModel(DIWNE::Diwne& diwne);
	~WorkspaceModel();

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	//bool drawDataFull(, int index);
	int  maxLenghtOfData();       //todo
	bool middleContent();         // the most important function
	void drawMenuLevelOfDetail(); //todo

	void popupContent();
	void popupContent_axis_showmodel();

	bool m_axisOn, m_showModel;

private:
	// GameObject models:
	// todo - add the resource manager for models and textures
	GameObject* m_sceneModel     = nullptr; // model in the 3D scene (in the hierarchical scene graph)
	GameObject* m_workspaceModel = nullptr; // model in the workspace (as a preview in the box)

	// variables of the workspace box
	GLuint  m_textureID   = 0;         // rendered texture name (COLOR_ATTACHMENT0 in m_fbo)
	ImVec2  m_textureSize = {84, 100}; // initial render texture size - should be large enough or changed during zoom
	Camera* m_camera;                  // local camera for model rendering in the box

	float val = 0; // temporary variable for testing

	// can be removed if no direct rendering to the texture m_textureID
	GLuint         m_fbo = 0;     // FBO for texture rendering - needed for additional rendering to the m_textureID only
	RenderTexture* renderTexture; // needed when creating camera only, can be a local variable

	void init();
};
