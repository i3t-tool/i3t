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

#include "Core/Nodes/Screen.h"

#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"
#include "GUI/Workspace/Tools.h"
#include "Utils/Format.h"
#include "Viewport/Viewport.h"

using namespace Workspace;

Screen::Screen(DIWNE::NodeEditor& diwne) : CoreNodeWithPins(diwne, Core::Builder::createScreen())
{
	m_displayOptions.showAxes = false;
	m_displayOptions.showGrids = false;
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

	// Setup tooltips
	m_workspaceOutputs.at(Core::I3T_SCREEN_OUT_WIDTH)->setTooltipEnabled(true);
	m_workspaceOutputs.at(Core::I3T_SCREEN_OUT_WIDTH)->setTooltip(_tbd("Screen width"));

	m_workspaceOutputs.at(Core::I3T_SCREEN_OUT_HEIGHT)->setTooltipEnabled(true);
	m_workspaceOutputs.at(Core::I3T_SCREEN_OUT_HEIGHT)->setTooltip(_tbd("Screen height"));

	m_viewportScreen = I3T::getViewport()->createScreen(getNodebase()->getId());

	updateCoreData();
}

Screen::~Screen()
{
	// TODO: Shouldn't this be in onDestroy?
	I3T::getViewport()->removeEntity(m_viewportScreen);
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
	ImVec2 zoomedScreenSize = diwne.canvas().diwne2screenSize(m_textureSize);
	ImVec2 zoomedTextureSize = zoomedScreenSize;
	ImVec2 topLeftCursorPos = ImGui::GetCursorScreenPos();

	Core::Screen* coreScreen = getNodebase()->asRaw<Core::Screen>();

	bool invalid = false;
	bool noInput = !coreScreen->getInput(Core::I3T_SCREEN_IN_DISPLAY).isPluggedIn();

	if (!noInput)
	{
		// Get projection and view matrix from screen input
		// std::pair<glm::mat4, glm::mat4> screenValue = getNodebase()->data().getScreen();

		Vp::Viewport* viewport = I3T::getViewport();
		m_renderOptions.lightingModel = viewport->getSettings().global().lighting_lightingModel;

		ImVec2 textureSize = m_textureSize;
		ImVec2 screenTextureOffset = ImVec2(0, 0);

		// Draw the screen
		// Figure out what effect the viewport transformation has
		if (coreScreen->isViewportExplicit())
		{
			// I elected to not actually set glViewport transformation directly in the 3D Viewport code as that would
			// just complicate things.
			// Instead, the viewport transformation is applied to two points: (0, 0) and (width, height) and based on
			// the transformed points, a "real" width and height is determined, rendered into a framebuffer and then the
			// framebuffer is drawn at an offset based on the x,y offset of the (0, 0) point.
			const glm::mat4& viewportMat = coreScreen->getViewport();
			glm::vec4 hVec = viewportMat * glm::vec4(-1.f, -1.f, -1.f, 1.f);
			ImVec2 bottomLeft = ImVec2(hVec.x, hVec.y);
			hVec = viewportMat * glm::vec4(1.f, 1.f, -1.f, 1.f);
			ImVec2 topRight = ImVec2(hVec.x, hVec.y);
			ImVec2 realSize = topRight - bottomLeft;

			// Check if the viewport transformation is valid
			invalid = !Core::isViewportValid(viewportMat);

			textureSize = realSize;
			screenTextureOffset = bottomLeft * diwne.getZoom();
			zoomedTextureSize = textureSize * diwne.getZoom();
		}

		if (!invalid)
		{
			viewport->drawScreen(m_renderTarget, textureSize.x, textureSize.y, glm::mat4(1.f), coreScreen->getView(),
			                     coreScreen->getProjection(), m_renderOptions, m_displayOptions);
			Ptr<Vp::Framebuffer> framebuffer = m_renderTarget->getOutputFramebuffer().lock();

			if (framebuffer)
			{
				ImVec2 screenMin = ImGui::GetCursorScreenPos();
				ImVec2 screenMax = screenMin + zoomedScreenSize;
				if (coreScreen->isViewportExplicit())
				{
					ImDrawList& drawList = *ImGui::GetWindowDrawList();
					ImRect rect(screenMin, screenMax);

					// Draw background
					drawList.AddRectFilled(
					    rect.Min, rect.Max,
					    ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_background)));
				}

				// TODO: [OGL-VULKAN]
				// Texture min must be flipped as OGL origin is in bottom left, not top left like ImGui
				screenTextureOffset =
				    ImVec2(screenTextureOffset.x, zoomedScreenSize.y - screenTextureOffset.y - zoomedTextureSize.y);

				ImVec2 textureMin = screenMin + screenTextureOffset;
				ImVec2 textureMax = textureMin + zoomedTextureSize;

				ImGui::Dummy(zoomedScreenSize);

				ImGuiWindow* window = ImGui::GetCurrentWindow();
				ImGuiContext* g = ImGui::GetCurrentContext();
				ImRect clipRect = ImRect(screenMin, {ceil(screenMax.x), ceil(screenMax.y)});
				clipRect.ClipWithFull(ImRect(window->DrawList->GetClipRectMin(), window->DrawList->GetClipRectMax()));
				window->DrawList->PushClipRect(clipRect.Min, clipRect.Max);
				window->DrawList->AddImage((void*) (intptr_t) framebuffer->getColorTexture(), textureMin, textureMax,
				                           ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), IM_COL32_WHITE);
				window->DrawList->PopClipRect();
			}
			else
			{
				ImGui::Text("Failed to draw the screen!");
			}
		}
	}

	if (noInput || invalid)
	{
		// No screen input connected
		ImDrawList& drawList = *ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImRect rect(pos, pos + zoomedScreenSize);

		// Draw background
		drawList.AddRectFilled(rect.Min, rect.Max,
		                       ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_background)));

		// Draw no input text
		ImGui::PushClipRect(rect.Min, rect.Max, true);
		float origScale = diwne.canvas().applyZoomScalingToFont(I3T::getFont(EFont::TutorialBold), 1.25f);

		const char* noInputText = _tbd("NO INPUT");
		const char* invalidText = _tbd("  INVALID\nVIEWPORT");

		GUI::TextCentered(invalid ? invalidText : noInputText, rect,
		                  ImGui::ColorConvertFloat4ToU32(I3T::getColor(EColor::Nodes_Screen_noInput_text)));
		diwne.canvas().stopZoomScalingToFont(I3T::getFont(EFont::TutorialBold), origScale);
		ImGui::PopClipRect();
	}

	if (drawResizeHandle(topLeftCursorPos, zoomedScreenSize))
		context.consumeInput();
}

void Screen::finalize(DIWNE::DrawInfo& context)
{
	CoreNodeWithPins::finalize(context);
	updateCoreData();
	updateTrackedScreen();
}

void Screen::onSelection(bool selected)
{
	CoreNodeWithPins::onSelection(selected);
	auto model = m_viewportScreen.lock();
	if (selected)
	{
		model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.selectionColor;
		model->m_highlight = true;
	}
	else
	{
		model->m_highlight = false;
	}
}

int Screen::maxLengthOfData()
{
	int widthMax = GUI::numberOfCharWithDecimalPoint(
	    getOutputs()[Core::I3T_SCREEN_OUT_WIDTH]->getCorePin().data().getFloat(), getNumberOfVisibleDecimal());
	int heightMax = GUI::numberOfCharWithDecimalPoint(
	    getOutputs()[Core::I3T_SCREEN_OUT_HEIGHT]->getCorePin().data().getFloat(), getNumberOfVisibleDecimal());
	return std::max(widthMax, heightMax);
}

void Screen::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

bool Screen::drawResizeHandle(ImVec2 topLeftCursorPos, ImVec2 zoomedScreenSize)
{
	bool interaction_happen = false;

	ImVec2 buttonSize = I3T::getSize(ESizeVec2::Nodes_Screen_resizeButtonSize);
	buttonSize = ImMax(ImVec2(1, 1), buttonSize);
	float buttonIconPadding = 0.f; /// not used 2*diwne.getZoom();

	ImVec2 zoomedButtonSize = buttonSize * diwne.getZoom();

	// NOTE: Prefer to use AddRectFilled as opposed to AddRect for debugging pixel positions
	//   AddRect offsets coordinates by 0.5f
	// NOTE: When moving point by a size parameter it may be necessary to subtract 1 from the size
	//   or specify coordinates in 0.5 intervals (offset by +-0.5) to reference a specific pixel (that's what AddRect
	//   does)

	// Bottom RIGHT button
	ImVec2 bottomRightPos = topLeftCursorPos + zoomedScreenSize;
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
			    std::max(newTextureScreenSize.x, buttonSize.x + ImGui::GetStyle().ItemSpacing.x / diwne.getZoom()) /
			    uiScale;
			m_textureSize.y =
			    std::max(newTextureScreenSize.y, buttonSize.y + ImGui::GetStyle().ItemSpacing.y / diwne.getZoom()) /
			    uiScale;
		}
	}

	return interaction_happen;
}

void Screen::updateCoreData()
{
	getNodebase()->setValue(m_textureSize.x, Core::I3T_SCREEN_OUT_WIDTH);
	getNodebase()->setValue(m_textureSize.y, Core::I3T_SCREEN_OUT_HEIGHT);
	getNodebase()->updateValues();
}

void Screen::updateTrackedScreen()
{
	auto screenModel = m_viewportScreen.lock();
	screenModel->m_visible = false;

	if (!Core::GraphManager::isTracking())
		return;

	Core::MatrixTracker* tracker = Core::GraphManager::getTracker();
	const Core::MatrixTracker::TrackedTransform* transform = tracker->getInterpolatedTransform();
	if (transform->data.space < Core::TransformSpace::Screen)
		return;

	// Check if we're connected to tracked camera
	auto coreScreen = getNodebase();
	auto* parentDisplayPin = coreScreen->getInput(Core::I3T_SCREEN_IN_DISPLAY).getParentPin();
	if (!parentDisplayPin)
		return;

	if (!tracker->getTrackedCamera() || *tracker->getTrackedCamera()->node.lock() != *parentDisplayPin->getOwner())
		return;

	// Update visuals
	const Core::MatrixTracker::TrackedMatrix* trackedMatrix = tracker->getInterpolatedMatrixObject();
	float mt = trackedMatrix->progress;

	screenModel->m_visible = true;
	screenModel->m_opaque = mt == 1.f;
	screenModel->m_opacity = mt;

	// Transform the screen model so it matches screen dimensions in scene view, take into account the tracker
	// viewport scaling factor
	screenModel->updateModelTransform(m_textureSize.x, m_textureSize.y, tracker->g_trackingViewportScalingFactorXY);
}

ImVec2 Screen::getScreenSize() const
{
	return m_textureSize;
}

void Screen::setScreenSize(ImVec2 size)
{
	m_textureSize = size;
}

void Screen::popupContent(DIWNE::DrawInfo& context)
{
	// I3TGui::MenuItemWithLog(_tbd("Set size"));
	//
	// ImGui::Separator();

	drawMenuSetEditable();

	ImGui::Separator();

	drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}
