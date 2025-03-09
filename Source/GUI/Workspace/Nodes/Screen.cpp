/**
 * \file
 * \brief
 * \authors Jaroslav Holeček, Petr Felkel, Martin Herich, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Screen.h"

#include "GUI/Toolkit.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"
#include "GUI/Workspace/Tools.h"
#include "Utils/Format.h"
#include "Viewport/Viewport.h"

using namespace Workspace;

Screen::Screen(DIWNE::NodeEditor& diwne)
    : CoreNodeWithPins(diwne, Core::Builder::createOperator<Core::EOperatorType::Screen>())
{
	init();
}

Screen::~Screen()
{
	// Empty
}

void Screen::init()
{
	getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1); /* \todo Jh always 1? */

	m_displayOptions.showAxes = false;
	m_displayOptions.showGrid = false;
	m_displayOptions.showCamera = false;
	m_displayOptions.showFrustum = false;

	m_renderOptions.multisample = false;
	m_renderOptions.framebufferAlpha = false;
	m_renderOptions.wboit = false;
	m_renderOptions.clearColor = Config::BACKGROUND_COLOR;
}

void Screen::popupContent(DIWNE::DrawInfo& context)
{
	CoreNode::drawMenuSetEditable();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

//  The screen has one (formerly two) triangles for resize:
//  (DR): Reworked to only have one in the bottom right corner as per usual UI conventions
//  ----------
//  |        |
//  |        |
//  |       /|
//  |      / |
//  ----------
void Screen::centerContent(DIWNE::DrawInfo& context)
{
	int width = m_textureSize.x * diwne.getZoom();
	int height = m_textureSize.y * diwne.getZoom();

	ImVec2 zoomedTextureSize = m_textureSize * diwne.getZoom();
	ImVec2 topLeftCursorPos = ImGui::GetCursorScreenPos();

	if (getNodebase()->getInput(0).isPluggedIn())
	{
		// Get projection and view matrix from screen input
		std::pair<glm::mat4, glm::mat4> screenValue = getNodebase()->data().getScreen();

		Vp::Viewport* viewport = I3T::getViewport();
		m_renderOptions.lightingModel = viewport->getSettings().global().lighting_lightingModel;
		viewport->drawScreen(m_renderTarget, width, height, screenValue.second, screenValue.first, m_renderOptions,
		                     m_displayOptions);
		Ptr<Vp::Framebuffer> framebuffer = m_renderTarget->getOutputFramebuffer().lock();

		if (framebuffer)
		{
			ImGui::Image((void*) (intptr_t) framebuffer->getColorTexture(), zoomedTextureSize, ImVec2(0.0f, 1.0f),
			             ImVec2(1.0f, 0.0f)); // vertical flip
		}
		else
		{
			ImGui::Text("Failed to draw the screen!");
		}
	}
	else
	{
		// No screen input connected
		ImDrawList& drawList = *ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImRect rect(pos, pos + zoomedTextureSize);

		// Draw background
		drawList.AddRectFilled(rect.Min, rect.Max,
		                       ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_noInput_background)));

		// Draw no input text
		ImGui::PushClipRect(rect.Min, rect.Max, true);
		float origScale = diwne.canvas().applyZoomScalingToFont(I3T::getFont(EFont::LargeBold));
		GUI::TextCentered("NO INPUT", rect,
		                  ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_noInput_text)));
		diwne.canvas().stopZoomScalingToFont(I3T::getFont(EFont::Header), origScale);
		ImGui::PopClipRect();
	}

	if (drawResizeHandles(topLeftCursorPos, zoomedTextureSize))
	{
		context.update(true, true, false);
	}
}

int Screen::maxLengthOfData() // todo
{
	return Tools::numberOfCharWithDecimalPoint(getOutputs()[1]->getCorePin().data().getFloat(),
	                                           getNumberOfVisibleDecimal()); /* \todo JH \todo MH not 1 but
	                                                                            some Core::Pin_Code */
}

void Screen::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

bool Screen::drawResizeHandles(ImVec2 topLeftCursorPos, ImVec2 zoomedTextureSize)
{
	bool interaction_happen = false;

	bool resize_texture = false; /// the screen size has changed -> update FBO
	                             /// size too to avoid blurring
	ImVec2 dragDelta;            /// zoom-scaled mouse move
	ImVec2 buttonSize = I3T::getSize(ESizeVec2::Nodes_Screen_resizeButtonSize);
	buttonSize = ImMax(ImVec2(1, 1), buttonSize);
	float buttonIconPadding = 0.f; /// not used 2*diwne.getZoom();

	// floored position - same as in ImGui
	//	ImVec2 zoomedTextureSize = GUI::floorImVec2(m_textureSize * diwne.getZoom());
	//	ImVec2 zoomedButtonSize = GUI::floorImVec2(buttonSize * diwne.getZoom());
	//	ImVec2 topLeftCursorPos = GUI::floorImVec2(ImGui::GetCursorScreenPos());
	ImVec2 zoomedButtonSize = buttonSize * diwne.getZoom();

	// NOTE: Prefer to use AddRectFilled as opposed to AddRect for debugging pixel positions
	//   AddRect offsets coordinates by 0.5f
	// NOTE: When moving point by a size parameter it may be necessary to subtract 1 from the size
	//   or specify coordinates in 0.5 intervals (offset by +-0.5) to reference a specific pixel (that's what AddRect
	//   does)

	ImVec2 cursorPos = topLeftCursorPos;

	// Bottom LEFT button (commented out)
	//	cursorPos.y += zoomedTextureSize.y - zoomedButtonSize.y;
	//	ImGui::SetCursorScreenPos(cursorPos);
	//
	//	interaction_happen |= diwne.IconButton(
	//	    DIWNE::IconType::TriangleDownLeft, I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgShape),
	//	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgInner), DIWNE::IconType::GrabDownLeft,
	//	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgShape),
	// I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgInner), 	    zoomedButtonSize, ImVec4(buttonIconPadding,
	// buttonIconPadding, buttonIconPadding, buttonIconPadding), true, 	    fmt::format("screenButton:{}left",
	// getId()));
	//	// mouse cursor 5 "ResizeNESW"
	//	if (ImGui::IsItemHovered())
	//		ImGui::SetMouseCursor(5);
	//
	//	ImGui::SameLine();
	//
	//	if (ImGui::IsItemActive() && diwne.bypassIsMouseDragging0())
	//	{
	//		interaction_happen = true;
	//		resize_texture = true;
	//
	//		dragDelta = diwne.input().bypassGetMouseDragDelta0() / diwne.getZoom();
	//
	//		ImVec2 nodePos = getPosition();
	//		nodePos.x += dragDelta.x;
	//		setPosition(nodePos);
	//
	//		dragDelta.x *= -1; /* (the drag direction sign) - in order to have the same
	//		                      code in   if(resize_texture) */
	//	}

	// Bottom RIGHT button
	cursorPos = topLeftCursorPos + zoomedTextureSize - zoomedButtonSize;
	ImGui::SetCursorScreenPos(cursorPos);

	interaction_happen |= diwne.canvas().IconButton(
	    DIWNE::IconType::TriangleDownRight, I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgShape),
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgInner), DIWNE::IconType::GrabDownRight,
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgShape), I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgInner),
	    zoomedButtonSize, ImVec4(buttonIconPadding, buttonIconPadding, buttonIconPadding, buttonIconPadding), true,
	    fmt::format("screenButton:{}right", getId()));
	// mouse cursor  6 "ResizeNWSE"
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(6);

	// TODO: With the new drag anywhere stuff dragging the corner breaks
	//  We will probably restrict drag to the header only so it might be an non issue
	//  The fix is probably just consuming input when the corner is moved

	// TODO: Again, bypass isItemActive? just ImGui::IsItemActive bruh
	if (ImGui::IsItemActive() && diwne.input().bypassIsMouseDragging0())
	{
		interaction_happen = true;
		resize_texture = true;

		dragDelta = diwne.input().bypassGetMouseDragDelta0() / diwne.getZoom();
	}

	// ask for texture resize, if the viewport size changed
	// todo: check if we should use the floor too
	if (resize_texture)
	{
		m_textureSize.x = std::max(2 * (buttonSize.x + ImGui::GetStyle().ItemSpacing.x / diwne.getZoom()),
		                           m_textureSize.x + dragDelta.x); /* button should fit into middle... */
		m_textureSize.y =
		    std::max(buttonSize.y + ImGui::GetStyle().ItemSpacing.y / diwne.getZoom(), m_textureSize.y + dragDelta.y);

		// must be index 1, as there is a hidden output index 0, storing the incoming PV matrix
		getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1);

		ImGui::ResetMouseDragDelta(0);
	}

	return interaction_happen;
}

ImVec2 Screen::getAspect() const
{
	return m_textureSize;
}

void Screen::setAspect(ImVec2 aspect)
{
	m_textureSize = aspect;
	getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1);
}
