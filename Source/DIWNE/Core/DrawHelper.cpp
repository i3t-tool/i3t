#include "DrawHelper.h"

#include "NodeEditor.h"

namespace DIWNE
{
float DrawHelper::zoom() const
{
	return e->getWorkAreaZoom();
}

void DrawHelper::AddRectFilledDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding,
                                    ImDrawFlags rounding_corners, bool ignoreZoom) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */
	float zoom = e->getWorkAreaZoom();
	idl->AddRectFilled(e->diwne2screen(p_min), e->diwne2screen(p_max), ImGui::ColorConvertFloat4ToU32(col),
	                   rounding * (ignoreZoom ? 1.0f : zoom), rounding_corners);
}

void DrawHelper::AddLine(const ImVec2& p1, const ImVec2& p2, ImVec4 col, float thickness, bool ignoreZoom) const
{
	float zoom = e->getWorkAreaZoom();
	ImGui::GetWindowDrawList()->AddLine(e->diwne2screen(p1), e->diwne2screen(p2), ImGui::ColorConvertFloat4ToU32(col),
	                                    thickness * (ignoreZoom ? 1.0f : zoom));
}

void DrawHelper::AddRectDiwne(const ImVec2& p_min, const ImVec2& p_max, ImVec4 col, float rounding,
                              ImDrawFlags rounding_corners, float thickness, bool ignoreZoom) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();
	float zoom = e->getWorkAreaZoom();
	idl->AddRect(e->diwne2screen(p_min), e->diwne2screen(p_max), ImGui::ColorConvertFloat4ToU32(col),
	             rounding * (ignoreZoom ? 1.0f : zoom), rounding_corners, thickness * (ignoreZoom ? 1.0f : zoom));
}

void DrawHelper::AddBezierCurveDiwne(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImVec4 col,
                                     float thickness, int num_segments /*=0*/) const
{
	float zoom = e->getWorkAreaZoom();
	ImDrawList* idl = ImGui::GetWindowDrawList(); /* \todo maybe use other channel with correct
	                                                 Clip rect for drawing of manual shapes, but
	                                                 be careful with order of drew elements */

	idl->AddBezierCubic(e->diwne2screen(p1), e->diwne2screen(p2), e->diwne2screen(p3), e->diwne2screen(p4),
	                    ImGui::ColorConvertFloat4ToU32(col), thickness * zoom, num_segments);
}

void DrawHelper::DrawIconCircle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                ImVec2 bottomRight, bool filled, float thickness /*=1*/) const
{
	ImVec2 center = ImVec2((topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2);
	float radius = std::min(bottomRight.x - topLeft.x, bottomRight.y - topLeft.y) / 2;

	idl->AddCircleFilled(center, radius, shapeColor);
	if (!filled)
	{
		idl->AddCircleFilled(center, radius - thickness, innerColor);
	}
}

void DrawHelper::DrawIconRectangle(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                   ImVec2 bottomRight, bool filled, ImVec2 thickness /*=ImVec2(1, 1)*/,
                                   float rounding /*= 0*/) const
{
	idl->AddRectFilled(topLeft, bottomRight, shapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thickness, bottomRight - thickness, innerColor, rounding);
	}
}
void DrawHelper::DrawIconPause(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                               ImVec2 bottomRight, bool filled, ImVec2 thickness, float rounding) const
{
	const float width = bottomRight.x - topLeft.x;
	// const float columnWidth = width / 3.0f;
	const float columnWidth = width * 6.0f / 15.0f;
	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth;
	bottomRight1.y = bottomRight.y;

	ImVec2 topLeft2;
	topLeft2.x = bottomRight.x - columnWidth;
	topLeft2.y = topLeft.y;

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor, rounding);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor, rounding);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + thickness, bottomRight1 - thickness, innerColor, rounding);
		idl->AddRectFilled(topLeft2 + thickness, bottomRight - thickness, innerColor, rounding);
	}
}

void DrawHelper::DrawIconTriangleLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                      ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2),
	                       bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(ImVec2(bottomRight.x - thickness, topLeft.y + thickness),
		                       ImVec2(topLeft.x + thickness, (topLeft.y + bottomRight.y) / 2),
		                       bottomRight - ImVec2(thickness, thickness), innerColor);
	}
}

// | I < | --- 1 1 5   vertical bar followed by the arrow
void DrawHelper::DrawIconSkipBack(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                  ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding
	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth; // vertical bar
	bottomRight1.y = bottomRight.y;

	// float Left2x = bottomRight.x - 5.0f * columnWidth; // triangle
	ImVec2 middleLeft2;
	middleLeft2.x = bottomRight.x - 5.0f * columnWidth; // triangle
	middleLeft2.y = (topLeft.y + bottomRight.y) / 2;

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor);
	idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y), middleLeft2, bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddRectFilled(topLeft + ImVec2(thickness, thickness), bottomRight1 - ImVec2(thickness, thickness),
		                   innerColor);
		idl->AddTriangleFilled(ImVec2(bottomRight.x, topLeft.y) + ImVec2(-thickness, 2 * thickness),
		                       middleLeft2 + ImVec2(2.0f * thickness, 0.0f),
		                       bottomRight - ImVec2(thickness, 2 * thickness), innerColor);
	}
}

// | < I | --- 5 1 1    arrow followed by the vertical bar
void DrawHelper::DrawIconSkipBack2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                   ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 middleLeft1 = ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2); // triangle
	ImVec2 topRight1 = ImVec2(topLeft.x + 5.0f * columnWidth, topLeft.y);
	ImVec2 bottomRight1 = ImVec2(topRight1.x, bottomRight.y);

	ImVec2 topLeft2 = ImVec2(bottomRight.x - 1.0f * columnWidth, topLeft.y); // rectangle

	idl->AddTriangleFilled(middleLeft1, topRight1, bottomRight1, shapeColor);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(middleLeft1 + ImVec2(2.0f * thickness, 0.0f),
		                       topRight1 + ImVec2(-thickness, 2 * thickness),
		                       bottomRight1 + ImVec2(-thickness, -2 * thickness), innerColor);
		idl->AddRectFilled(topLeft2 + ImVec2(thickness, thickness), bottomRight - ImVec2(thickness, thickness),
		                   innerColor);
	}
}
void DrawHelper::DrawIconRewind(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                ImVec2 bottomRight, bool filled, float thickness) const
{
	const ImVec2 middleLeft = ImVec2(topLeft.x, (topLeft.y + bottomRight.y) / 2.0f);

	const ImVec2 middlePoint = (topLeft + bottomRight) / 2.0f;
	const ImVec2 topMiddle = ImVec2(middlePoint.x, topLeft.y);
	const ImVec2 bottomMiddle = ImVec2(middlePoint.x, bottomRight.y);

	const ImVec2 topRight = ImVec2(bottomRight.x, topLeft.y);

	idl->AddTriangleFilled(topMiddle, middleLeft, bottomMiddle, shapeColor);
	idl->AddTriangleFilled(topRight, middlePoint, bottomRight, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topMiddle + ImVec2(-thickness, 2 * thickness), middleLeft + ImVec2(2 * thickness, 0.0f),
		                       bottomMiddle + ImVec2(-thickness, -2 * thickness), innerColor);
		idl->AddTriangleFilled(topRight + ImVec2(-thickness, 2 * thickness),
		                       middlePoint + ImVec2(2 * thickness, thickness),
		                       bottomRight + ImVec2(-thickness, -2.0f * thickness), innerColor);
	}
}

// | > I | --- 5 1 1   right arrow followed by the vertical bar
void DrawHelper::DrawIconSkipForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                     ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 middleRight1;
	middleRight1.x = topLeft.x + 5.0f * columnWidth; // triangle
	middleRight1.y = (topLeft.y + bottomRight.y) / 2;

	ImVec2 topLeft2;
	topLeft2.x = bottomRight.x - columnWidth; // vertical bar
	topLeft2.y = topLeft.y;

	idl->AddTriangleFilled(topLeft, middleRight1, ImVec2(topLeft.x, bottomRight.y), shapeColor);
	idl->AddRectFilled(topLeft2, bottomRight, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness),
		                       middleRight1 - ImVec2(2.0f * thickness, 0.0f),
		                       ImVec2(topLeft.x, bottomRight.y) + ImVec2(thickness, -2 * thickness), innerColor);
		idl->AddRectFilled(topLeft2 + ImVec2(thickness, thickness), bottomRight - ImVec2(thickness, thickness),
		                   innerColor);
	}
}

// | I > | --- 1 1 5     vertical bar followed by the right arrow
void DrawHelper::DrawIconSkipForward2(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                      ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	const float columnWidth = (bottomRight.x - topLeft.x) / 7.0f; // width of vertical line & padding

	ImVec2 bottomRight1;
	bottomRight1.x = topLeft.x + columnWidth; // vertical bar
	bottomRight1.y = bottomRight.y;

	ImVec2 topLeft2 = ImVec2(bottomRight.x - 5.0f * columnWidth, topLeft.y); // triangle
	ImVec2 bottomLeft2 = ImVec2(topLeft2.x, bottomRight.y);

	ImVec2 middleRight2 = ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2.0f);

	idl->AddRectFilled(topLeft, bottomRight1, shapeColor);
	idl->AddTriangleFilled(topLeft2, bottomLeft2, middleRight2, shapeColor);

	if (!filled)
	{
		idl->AddRectFilled(topLeft + ImVec2(thickness, thickness), bottomRight1 - ImVec2(thickness, thickness),
		                   innerColor);

		idl->AddTriangleFilled(topLeft2 + ImVec2(thickness, 2 * thickness),
		                       bottomLeft2 + ImVec2(thickness, -2 * thickness),
		                       middleRight2 + ImVec2(-2.0f * thickness, 0.0f), innerColor);
	}
}
void DrawHelper::DrawIconFastForward(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                     ImVec2 bottomRight, bool filled, float thickness) const
{
	const ImVec2 bottomLeft = ImVec2(topLeft.x, bottomRight.y);

	const ImVec2 middlePoint = (topLeft + bottomRight) / 2.0f;
	const ImVec2 topMiddle = ImVec2(middlePoint.x, topLeft.y);
	const ImVec2 bottomMiddle = ImVec2(middlePoint.x, bottomRight.y);

	const ImVec2 middleRight = ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2.0f);

	idl->AddTriangleFilled(topLeft, middlePoint, bottomLeft, shapeColor);
	idl->AddTriangleFilled(topMiddle, middleRight, bottomMiddle, shapeColor);

	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness), middlePoint + ImVec2(-2 * thickness, 0.0f),
		                       bottomLeft + ImVec2(thickness, -2 * thickness), innerColor);
		idl->AddTriangleFilled(topMiddle + ImVec2(thickness, 2 * thickness), middleRight + ImVec2(-2 * thickness, 0.0f),
		                       bottomMiddle + ImVec2(thickness, -2 * thickness), innerColor);
	}
}

void DrawHelper::DrawIconTriangleRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                       ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	idl->AddTriangleFilled(topLeft, ImVec2(bottomRight.x, (topLeft.y + bottomRight.y) / 2),
	                       ImVec2(topLeft.x, bottomRight.y), shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(topLeft + ImVec2(thickness, 2 * thickness), // (PF 2x for better border)
		                       ImVec2(bottomRight.x - 2 * thickness, (topLeft.y + bottomRight.y) / 2),
		                       ImVec2(topLeft.x + thickness, bottomRight.y - 2 * thickness), innerColor);
	}
}

void DrawHelper::DrawIconTriangleDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                           ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 p1 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p2 = bottomRight - ImVec2(0.5f, 0.5f);
	ImVec2 p3 = ImVec2(bottomRight.x - 0.5f, topLeft.y + 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(2 * thickness, -thickness), p2 - ImVec2(thickness, thickness),
		                       p3 + ImVec2(-thickness, 2 * thickness), innerColor);
	}
}

void DrawHelper::DrawIconTriangleDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                          ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	ImVec2 p1 = topLeft + ImVec2(0.5f, 0.5f);
	ImVec2 p2 = ImVec2(topLeft.x + 0.5f, bottomRight.y - 0.5f);
	ImVec2 p3 = bottomRight - ImVec2(0.5f, 0.5f);

	idl->AddTriangleFilled(p1, p2, p3, shapeColor);
	if (!filled)
	{
		idl->AddTriangleFilled(p1 + ImVec2(thickness, 2 * thickness), p2 + ImVec2(thickness, -thickness),
		                       p3 - ImVec2(2 * thickness, thickness), innerColor);
	}
}

void DrawHelper::DrawIconGrabDownLeft(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                      ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	float zoom = e->getWorkAreaZoom();

	topLeft = topLeft + ImVec2(0.5f, 0.5f);
	bottomRight = bottomRight - ImVec2(0.5f, 0.5f);

	int lineCount = 3;
	float padding = 1.5f * zoom;
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(bottomRight.x - 1.2f * pointOffsetLong - (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(topLeft.x + pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)), shapeColor,
		             thickness * zoom);
	}
}

void DrawHelper::DrawIconGrabDownRight(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                       ImVec2 bottomRight, bool filled, float thickness /*= 1*/) const
{
	float zoom = e->getWorkAreaZoom();

	topLeft = topLeft + ImVec2(0.5f, 0.5f);
	bottomRight = bottomRight - ImVec2(0.5f, 0.5f);

	int lineCount = 3;
	float padding = 1.5f * zoom;
	float squaredPadding = sqrt(2) * padding;
	float pointOffsetLong = 2 * squaredPadding;
	float pointOffsetShort = padding;
	float size = abs(bottomRight.y - topLeft.y) - pointOffsetLong - pointOffsetShort;
	float step = size / lineCount;

	for (int i = 0; i < lineCount; i++)
	{
		idl->AddLine(ImVec2(topLeft.x + 1.2f * pointOffsetLong + (i * step), bottomRight.y - pointOffsetShort),
		             ImVec2(bottomRight.x - pointOffsetShort, topLeft.y + 1.2f * pointOffsetLong + (i * step)),
		             shapeColor, thickness * zoom);
	}
}

void DrawHelper::DrawIconCross(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                               ImVec2 bottomRight, bool filled, float shapeThickness /*=4*/,
                               float innerThickness /*=2*/) const
{
	float zoom = e->getWorkAreaZoom();
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	innerThickness *= zoom;
	shapeThickness *= zoom;

	ImVec2 pTL = topLeft;
	ImVec2 pBR = bottomRight;
	ImVec2 pTR = ImVec2(bottomRight.x, topLeft.y);
	ImVec2 pBL = ImVec2(topLeft.x, bottomRight.y);

	// (DR): This is junky, but it seems to help making the cross look more symmetrical
	//   The true reason for the asymmetry probably lies in the zoom level value, on my machine it steps in 0.125
	//   intervals, every other zoom level is fine, the ones between can cause asymmetry, might be a good idea to
	//   investigate this further and maybe restrict what values the zoom can have
	// (PF) this should be set by Nodes_Transformation_ValidIcon_padding for isValid Icon.
	//   for cross was (184, 196, 185, 196)
	//   for hyphen is
	pTR = pTR + ImVec2(0.01f, -0.01f);
	pBR = pBR + ImVec2(0.01f, 0.01f);

	//	LOG_INFO("zoom: {}", m_workAreaZoom);
	//	LOG_INFO("pTL: {:10.3f},{:10.3f}  pBR: {:10.3f},{:10.3f}", pTL.x, pTL.y, pBR.x, pBR.y);
	idl->AddLine(pTL, pBR, shapeColor, shapeThickness);
	idl->AddLine(pBL, pTR, shapeColor, shapeThickness);

	// TODO: (DR) Commenting this out for the time being as it isn't used anyway, needs to be rewritten to support
	//   zooming like the code above
	//	float thicknessDiff = (shapeThickness - thicknesInner) / 2;
	//	ImVec2 thicknessDiffVec = ImVec2(thicknesDiff, thicknesDiff);
	//	if (!filled)
	//	{
	//				idl->AddLine(topLeft + thicknessDiffVec, bottomRight - thicknessDiffVec, InnerColor,
	// thicknessInner); 		idl->AddLine(ImVec2(topLeft.x, bottomRight.y) + thicknessDiffVec,
	// ImVec2(bottomRight.x, topLeft.y) - thicknessDiffVec, InnerColor, thicknessInner);
	//	}
}

void DrawHelper::DrawIconHyphen(ImDrawList* idl, ImColor shapeColor, ImColor innerColor, ImVec2 topLeft,
                                ImVec2 bottomRight, bool filled, float thickness) const
{
	bottomRight = bottomRight - ImVec2(1.0f, 1.0f);

	float middleY = (topLeft.y + bottomRight.y) / 2;
	ImVec2 start = ImVec2(topLeft.x, middleY);
	ImVec2 end = ImVec2(bottomRight.x, middleY);


	idl->AddLine(start, end, shapeColor, thickness * zoom());
}

bool DrawHelper::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor, ImVec2 size,
                            ImVec4 padding, bool filled, std::string const id) const
{
	return IconButton(bgIconType, bgShapeColor, bgInnerColor, DIWNE::IconType::NoIcon, IM_COL32_BLACK, IM_COL32_BLACK,
	                  size, padding, filled, id);
}

void DrawHelper::EmptyButton(ImVec2 size, ImColor color, float rounding /*= 0*/)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	const ImVec2 icon_min = ImGui::GetCursorScreenPos();
	const ImVec2 icon_max = icon_min + size;

	idl->AddRectFilled(icon_min, icon_max, color, rounding, ImDrawFlags_RoundCornersAll);
	ImGui::SetCursorScreenPos(icon_min);
}

bool DrawHelper::IconButton(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                            DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size,
                            ImVec4 padding, bool filled, std::string const id) const
{
	ImVec2 initPos = ImGui::GetCursorScreenPos();

	DrawIcon(bgIconType, bgShapeColor, bgInnerColor, fgIconType, fgShapeColor, fgInnerColor, size, padding, filled);

	ImGui::SetCursorScreenPos(initPos);
	bool result = ImGui::InvisibleButton(id.c_str(), size);
	//    ImGui::SetItemAllowOverlap();
	return result;
}

void DrawHelper::DrawIcon(DIWNE::IconType bgIconType, ImColor bgShapeColor, ImColor bgInnerColor,
                          DIWNE::IconType fgIconType, ImColor fgShapeColor, ImColor fgInnerColor, ImVec2 size,
                          ImVec4 padding, bool filled, ImVec2 thickness /*=ImVec2(1, 1)*/, float rounding /*= 0*/) const
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	// TODO: I commented the below code out, it was causing ImGui item / DiwneObject rect / Visual rect misalignment
	//  Centering pins and labels will be separate task to be done later

	// (PF) move Icon slightly lower to match the position of the text
	// todo make it more robust for larger icons?
	//	float h = ImGui::GetTextLineHeight();
	//	float dh = h > size.y ? (h - size.y) / 2.0f : 0.0f;
	//	const ImVec2 icon_min = ImGui::GetCursorScreenPos() + ImVec2(0, dh);

	const ImVec2 icon_min = ImGui::GetCursorScreenPos();

	// const ImVec2 icon_min = ImGui::GetCursorScreenPos();
	const ImVec2 icon_max = icon_min + size;
	const ImVec2 inner_icon_min = icon_min + ImVec2(padding.x, padding.w);
	const ImVec2 inner_icon_max = icon_max - ImVec2(padding.z, padding.y);
	const ImVec2 inner_icon_min2 = icon_min + ImVec2(1.5f * padding.x, 1.5f * padding.w);
	const ImVec2 inner_icon_max2 = icon_max - ImVec2(1.5f * padding.z, 1.5f * padding.y);

	switch (bgIconType)
	{
	case Circle:
		DrawIconCircle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case AtFrom:
	case AtTo:
	case Rectangle:
	case Pause:
	case SkipBack:
	case SkipBack2:
	case SkipForward:
	case SkipForward2:
		DrawIconRectangle(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled, thickness, rounding);
		break;
	case TriangleLeft:
		DrawIconTriangleLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleRight:
		DrawIconTriangleRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case Cross:
		DrawIconCross(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case Hyphen:
		DrawIconHyphen(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownLeft:
		DrawIconTriangleDownLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownRight:
		DrawIconTriangleDownRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownLeft:
		DrawIconGrabDownLeft(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownRight:
		DrawIconGrabDownRight(idl, bgShapeColor, bgInnerColor, icon_min, icon_max, filled);
		break;
	case NoIcon:
	default:
		break;
	}

	switch (fgIconType)
	{
	case Circle:
		DrawIconCircle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Rectangle:
		DrawIconRectangle(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case TriangleLeft:
		DrawIconTriangleLeft(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case TriangleRight:
		DrawIconTriangleRight(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Cross:
		DrawIconCross(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Hyphen:
		DrawIconHyphen(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		// DrawIconHyphen(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownLeft:
		DrawIconTriangleDownLeft(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case TriangleDownRight:

		DrawIconTriangleDownRight(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownLeft:
		DrawIconGrabDownLeft(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case GrabDownRight:
		DrawIconGrabDownRight(idl, fgShapeColor, fgInnerColor, icon_min, icon_max, filled);
		break;
	case Stop:
		DrawIconRectangle(idl, fgShapeColor, fgInnerColor, inner_icon_min2, inner_icon_max2, filled);
		break;
	case Pause:
		DrawIconPause(idl, fgShapeColor, fgInnerColor, inner_icon_min2, inner_icon_max2, filled);
		break;
	case SkipBack:
		DrawIconSkipBack(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipBack2:
		DrawIconSkipBack2(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipForward:
		DrawIconSkipForward(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case SkipForward2:
		DrawIconSkipForward2(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case Rewind:
	case AtFrom:
		DrawIconRewind(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case FastForward:
	case AtTo:
		DrawIconFastForward(idl, fgShapeColor, fgInnerColor, inner_icon_min, inner_icon_max, filled);
		break;
	case NoIcon:
	default:
		break;
	}

//	ImGui::Dummy(size); // Better to use InvisibleButton
	// We're making the InvisibleButton disabled so that when its pressed / dragged it does not set an ActiveID in ImGui
	// Setting ActiveID is the same thing what a DragFloat does when it drags, it disables interaction with other items
	// until the drag/press operation stops. This is not desirable for a pin as we want other things to hover still.
	// TODO: Maybe it actually is desirable, I'm not sure yet, but at least I know how it all works now
	ImGui::BeginDisabled(true);
	ImGui::InvisibleButton("DiwneIcon", ImRect(icon_min, icon_max).GetSize());
	ImGui::EndDisabled();
}
} // namespace DIWNE