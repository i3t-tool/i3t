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
	getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1); /* \todo Jh always 1? */

	m_displayOptions.showAxes = false;
	m_displayOptions.showGrid = false;
	m_displayOptions.showCamera = false;
	m_displayOptions.showFrustum = false;

	m_renderOptions.multisample = false;
	m_renderOptions.framebufferAlpha = false;
	m_renderOptions.wboit = false;
	m_renderOptions.clearColor = Config::BACKGROUND_COLOR;

	// Move screen input to the output (right) side
	auto it = std::next(m_leftPins.begin(), 0);
	(*it)->m_isLeft = false;
	(*it)->makeInput();
	(*it)->m_showData = m_showDataOnPins;
	m_rightPins.insert(m_rightPins.begin(), *it);
	m_leftPins.erase(it);

	m_workspaceOutputs.at(Core::I3T_CAMERA_OUT_SCREEN)->setRendered(false); // Hide output screen pin
}

void Screen::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuSetEditable();

	ImGui::Separator();

	drawMenuDuplicate(context);

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
	ImVec2 zoomedTextureSize = diwne.canvas().diwne2screenSize(m_textureSize);
	ImVec2 topLeftCursorPos = ImGui::GetCursorScreenPos();

	if (getNodebase()->getInput(0).isPluggedIn())
	{
		// Get projection and view matrix from screen input
		std::pair<glm::mat4, glm::mat4> screenValue = getNodebase()->data().getScreen();

		Vp::Viewport* viewport = I3T::getViewport();
		m_renderOptions.lightingModel = viewport->getSettings().global().lighting_lightingModel;
		viewport->drawScreen(m_renderTarget, zoomedTextureSize.x, zoomedTextureSize.y, screenValue.second,
		                     screenValue.first, m_renderOptions, m_displayOptions);
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
		float origScale = diwne.canvas().applyZoomScalingToFont(I3T::getFont(EFont::TutorialBold), 1.25f);
		GUI::TextCentered("NO INPUT", rect,
		                  ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_noInput_text)));
		diwne.canvas().stopZoomScalingToFont(I3T::getFont(EFont::TutorialBold), origScale);
		ImGui::PopClipRect();
	}

	if (drawResizeHandles(topLeftCursorPos, zoomedTextureSize))
		context.consumeInput();
}

int Screen::maxLengthOfData()
{
	return Tools::numberOfCharWithDecimalPoint(
	    getOutputs()[Core::I3T_SCREEN_OUT_ASPECT]->getCorePin().data().getFloat(), getNumberOfVisibleDecimal());
}

void Screen::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

bool Screen::drawResizeHandles(ImVec2 topLeftCursorPos, ImVec2 zoomedTextureSize)
{
	bool interaction_happen = false;

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

	// Bottom RIGHT button
	ImVec2 bottomRightPos = topLeftCursorPos + zoomedTextureSize;
	ImVec2 cursorPos = bottomRightPos - zoomedButtonSize;
	ImGui::SetCursorScreenPos(cursorPos);
	interaction_happen |= diwne.canvas().IconButton(
	    fmt::format("screenButton:{}right", getId()), false, DIWNE::IconType::TriangleDownRight,
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgShape), I3T::getColor(EColor::Nodes_Screen_resizeBtn_bgInner),
	    DIWNE::IconType::GrabDownRight, I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgShape),
	    I3T::getColor(EColor::Nodes_Screen_resizeBtn_fgInner), zoomedButtonSize,
	    ImVec4(buttonIconPadding, buttonIconPadding, buttonIconPadding, buttonIconPadding), true);

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);

	if (ImGui::IsItemActive())
	{
		interaction_happen = true;
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
		if (diwne.input().bypassIsMouseDragging0())
		{
			ImVec2 mousePos = ImGui::GetIO().MousePos;
			ImVec2 dragOffset = zoomedButtonSize - ImGui::GetCurrentContext()->ActiveIdClickOffset;
			ImVec2 newTextureScreenSize = (mousePos - topLeftCursorPos + dragOffset) / diwne.getZoom();

			float uiScale = I3T::getUI()->getUiScale();
			m_textureSize.x =
			    std::max(newTextureScreenSize.x, buttonSize.x + ImGui::GetStyle().ItemSpacing.x) / uiScale;
			m_textureSize.y =
			    std::max(newTextureScreenSize.y, buttonSize.y + ImGui::GetStyle().ItemSpacing.y) / uiScale;

			// must be index 1, as there is a hidden output index 0, storing the incoming PV matrix
			getNodebase()->setValue(m_textureSize.x / m_textureSize.y, 1);
		}
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
