#pragma once
//----------------
/**
 * \file WorkspaceScreen.h
 * \author Petr Felkel, MartinHerich
 * \brief screen box in the workspace
 * Based on WorkspaceScreen
 *
 */
//---------------
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceScreen : public WorkspaceNodeWithCoreDataWithPins
{
public:
  WorkspaceScreen();
	~WorkspaceScreen();

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		// visitor.visit(std::static_pointer_cast<WorkspaceSequence>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

  //bool drawDataFull(DIWNE::Diwne& diwne, int index);
	int maxLenghtOfData();  //todo
	bool middleContent(DIWNE::Diwne& diwne);  // the most important function
	void drawMenuLevelOfDetail();  //todo

private:

	// variables of the workspace box
  GLuint m_textureID = 0;           // rendered texture name (COLOR_ATTACHMENT0 in m_fbo)
  ImVec2 m_textureSize = {100,100};  // initial render texture size - should be large enough or changed during zoom
	Camera *m_camera;                 // local camera for model rendering in the box

  float val = 0;   // temporary variable for testing

	// can be removed if no direct rendering to the texture m_textureID
	GLuint m_fbo = 0;              // FBO for texture rendering - needed obly for additional rendering to the m_textureID
	RenderTexture *renderTexture;  // needed when creating camera only, can be a local variable

	void init();
};
