#pragma once
//----------------
/**
 * \file WorkspaceScreen.h
 * \author Petr Felkel, Martin Herich
 * \brief screen box in the workspace
 * Based on WorkspaceScreen
 *
 */
//---------------
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceScreen : public WorkspaceNodeWithCoreDataWithPins
{
public:
  WorkspaceScreen(DIWNE::Diwne& diwne);
	~WorkspaceScreen();

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceScreen>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

  //bool drawDataFull(, int index);
	int maxLenghtOfData();  //todo
	bool middleContent();   // the most important function
	void drawMenuLevelOfDetail();  //todo

private:

	// variables of the workspace box
  GLuint m_textureID = 0;            // rendered texture name (COLOR_ATTACHMENT0 in m_fbo)
  ImVec2 m_textureSize = {100,100};  // initial render texture size - should be large enough or changed during zoom
	Camera *m_camera;                  // local camera for model rendering in the box

  float val = 0;                     // temporary variable for testing

	// can be removed if no direct rendering to the texture m_textureID
	GLuint m_fbo = 0;                  // FBO for texture rendering - needed only for additional rendering to the m_textureID
	RenderTexture *m_renderTexture;    // needed when creating camera only, can be a local variable

	void init();
};
