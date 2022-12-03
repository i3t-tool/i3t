#include "WorkspaceScreen.h"

#include "Utils/Format.h"
#include "Viewport/Viewport.h"
#include "World/RenderTexture.h" // FBO

#define TEST

WorkspaceScreen::WorkspaceScreen(DIWNE::Diwne& diwne)
    : WorkspaceNodeWithCoreDataWithPins(diwne, Core::Builder::createNode<ENodeType::Screen>())
{
	init();
}

WorkspaceScreen::~WorkspaceScreen()
{
	// Empty
}

void WorkspaceScreen::init()
{
	m_framebuffer = std::make_unique<Vp::Framebuffer>(m_textureSize.x, m_textureSize.y, false, false);
	getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1); /* \todo Jh always 1? */
}

//  The screen has two triangles for resize:
//  ----------
//  |        |
//  |        |
//  |\      /|
//  | \    / |
//  ----------
bool WorkspaceScreen::middleContent()
{
	bool interaction_happen = false;
	bool resize_texture = false; /// the screen size has changed -> update FBO
	                             /// size too to avoid blurring
	ImVec2 dragDelta;            /// zoom-scaled mouse move
	ImVec2 buttonSize = I3T::getSize(ESizeVec2::Nodes_Screen_resizeButtonSize);
	float buttonIconPadding = 0.f; /// not used 2*diwne.getWorkAreaZoom();

	// Get projection and view matrix from screen input
	std::pair<glm::mat4, glm::mat4> screenValue = getNodebase()->getData().getScreen();

	// Prepare FBO
	m_framebuffer->start(m_textureSize.x * diwne.getWorkAreaZoom(), m_textureSize.y * diwne.getWorkAreaZoom());
	glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Draw scene if screen input is plugged in
	if (getNodebase()->getInputPins()[0].isPluggedIn())
	{
		App::get().viewport()->draw(screenValue.second, screenValue.first, {true, false, true, false, false});
	}

	m_framebuffer->end();

// Problem: ImGui uses int values for DC.CursorPos. Triangle positions not in
// int coordinates are rounded one pixel out of the texture rectangle solution
// for zoom > 1: use also floored positions problem remains for zoom <= 1,
// probably caused by constants used for path thickness

// #define IM_FLOOR(_VAL) ((float) (int) (_VAL))    // this macro is used in
//  ImGui.cpp:8155 for DC.CursorPos
#define FLOOR_VEC2(_VAL) (ImVec2((float)(int)((_VAL).x), (float)(int)((_VAL).y))) // version of IM_FLOOR for Vec2
	float zoom = diwne.getWorkAreaZoom();
	ImVec2 zoomedTextureSize = FLOOR_VEC2(m_textureSize * diwne.getWorkAreaZoom()); // floored position - same as in ImGui
	ImVec2 zoomedButtonSize = FLOOR_VEC2(buttonSize * diwne.getWorkAreaZoom());

	ImVec2 topLeftCursorPos = FLOOR_VEC2(ImGui::GetCursorScreenPos());
	// ---------- Texture Image ------------
	// ImGui::Image(reinterpret_cast<ImTextureID>(m_textureID),
	// m_textureSize*diwne.getWorkAreaZoom(), ImVec2(0.0f, 1.0f),
	// ImVec2(1.0f,0.0f)); //vertical flip
	ImGui::Image((void*)(intptr_t)m_framebuffer->getColorTexture(), zoomedTextureSize, ImVec2(0.0f, 1.0f),
	             ImVec2(1.0f, 0.0f)); // vertical flip

	// ---------- Bottom-left button ------------
	// ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	// cursorPos.y -= (buttonSize.y*diwne.getWorkAreaZoom() +
	// ImGui::GetStyle().ItemSpacing.y);

	ImVec2 cursorPos = topLeftCursorPos;
	cursorPos.y += zoomedTextureSize.y - zoomedButtonSize.y;

	ImGui::SetCursorScreenPos(cursorPos);

	interaction_happen |= diwne.IconButton(
	    DIWNE::IconType::TriangleDownLeft, I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgShape),
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgInner), DIWNE::IconType::TriangleDownLeft,
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgShape), I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgInner),
	    zoomedButtonSize, ImVec4(buttonIconPadding, buttonIconPadding, buttonIconPadding, buttonIconPadding), true,
	    fmt::format("screenButton:{}left", getId()));
	// mouse cursor 5 "ResizeNESW"
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(5);

	ImGui::SameLine();

	if (diwne.bypassIsItemActive() && diwne.bypassIsMouseDragging0())
	{
		interaction_happen = true;
		resize_texture = true;

		dragDelta = diwne.bypassGetMouseDragDelta0() / diwne.getWorkAreaZoom();

		ImVec2 nodePos = getNodePositionDiwne();
		nodePos.x += dragDelta.x;
		setNodePositionDiwne(nodePos);

		dragDelta.x *= -1; /* (the drag direction sign) - in order to have the same
		                      code in   if(resize_texture) */
	}

	// ---------- Bottom-right button ------------
	// ImGui::Indent(std::max(0.0f, (m_textureSize.x -
	// buttonSize.x)*diwne.getWorkAreaZoom()));

	cursorPos = topLeftCursorPos + zoomedTextureSize - zoomedButtonSize;
	ImGui::SetCursorScreenPos(cursorPos);

	interaction_happen |= diwne.IconButton(

	    DIWNE::IconType::TriangleDownRight, I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgShape),
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgInner), DIWNE::IconType::TriangleDownRight,
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgShape), I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgInner),
	    zoomedButtonSize, ImVec4(buttonIconPadding, buttonIconPadding, buttonIconPadding, buttonIconPadding), true,
	    fmt::format("screenButton:{}right", getId()));
	// mouse cursor  6 "ResizeNWSE"
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(6);

	if (diwne.bypassIsItemActive() && diwne.bypassIsMouseDragging0())
	{
		interaction_happen = true;
		resize_texture = true;

		dragDelta = diwne.bypassGetMouseDragDelta0() / diwne.getWorkAreaZoom();
	}

	// ask for texture resize, if the viewport size changed
	// todo: check if we should use the floor too
	if (resize_texture)
	{
		m_textureSize.x = std::max(2 * (buttonSize.x + ImGui::GetStyle().ItemSpacing.x / diwne.getWorkAreaZoom()),
		                           m_textureSize.x + dragDelta.x); /* button should fit into middle... */
		m_textureSize.y = std::max(buttonSize.y + ImGui::GetStyle().ItemSpacing.y / diwne.getWorkAreaZoom(),
		                           m_textureSize.y + dragDelta.y);

		ImGui::ResetMouseDragDelta(0);
	}

	getNodebase()->setValue(m_textureSize.x / m_textureSize.y,
	                        1); // must be index 1, as there is a hidden output
	                            // index 0, storing the incoming PV matrix

	return interaction_happen;
}

int WorkspaceScreen::maxLenghtOfData() // todo
{
	return numberOfCharWithDecimalPoint(getOutputs()[1]->getCorePin().data().getFloat(),
	                                    getNumberOfVisibleDecimal()); /* \todo JH \todo MH not 1 but some
	                                                                     Core::Pin_Code */
}

void WorkspaceScreen::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
	                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}
