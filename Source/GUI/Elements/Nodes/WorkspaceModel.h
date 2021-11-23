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
  WorkspaceModel();
	~WorkspaceModel();

  //bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData();  //todo
	bool middleContent(DIWNE::Diwne& diwne);  // the most important function
	void drawMenuLevelOfDetail();  //todo 

private:
	GameObject *m_currentModelGameObject = nullptr;  // model associated with this box
	
  GLuint m_textureID = 0;        // rendered m_textureID (COLOR_ATTACHMENT0 in m_fbo)
  ImVec2 m_textureSize = {1,1};
	Camera *m_camera;              // local camera for model rendering in the box

  float val = 0;   // temporary variable for testing

	// can be removed if no direct rendering to the texture m_textureID
	GLuint m_fbo = 0;              // FBO for texture rendering - needed obly for additional rendering to the m_textureID
	RenderTexture *renderTexture;  // needed when creating camera only, can be a local variable

	int m_currentModelIdx = 0;     //todo  what is it for?

	void init();
};
