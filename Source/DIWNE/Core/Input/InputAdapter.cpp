#include "InputAdapter.h"

namespace DIWNE
{
bool InputAdapter::bypassIsItemClicked0()
{
	return ImGui::IsItemClicked(0);
}
bool InputAdapter::bypassIsItemClicked1()
{
	return ImGui::IsItemClicked(1);
}
bool InputAdapter::bypassIsItemClicked2()
{
	return ImGui::IsItemClicked(2);
}
bool InputAdapter::bypassIsMouseDown0()
{
	return ImGui::IsMouseDown(0);
}
bool InputAdapter::bypassIsMouseDown1()
{
	return ImGui::IsMouseDown(1);
}
bool InputAdapter::bypassIsMouseDown2()
{
	return ImGui::IsMouseDown(2);
}
bool InputAdapter::bypassIsMouseClicked0()
{
	return ImGui::IsMouseClicked(0);
}
bool InputAdapter::bypassIsMouseClicked1()
{
	return ImGui::IsMouseClicked(1);
}
bool InputAdapter::bypassIsMouseClicked2()
{
	return ImGui::IsMouseClicked(2);
}
bool InputAdapter::bypassIsMouseReleased0()
{
	return ImGui::IsMouseReleased(0);
}
bool InputAdapter::bypassIsMouseReleased1()
{
	return ImGui::IsMouseReleased(1);
}
bool InputAdapter::bypassIsMouseReleased2()
{
	return ImGui::IsMouseReleased(2);
}
ImVec2 InputAdapter::bypassMouseClickedPos0()
{
	return ImGui::GetIO().MouseClickedPos[0];
}
ImVec2 InputAdapter::bypassMouseClickedPos1()
{
	return ImGui::GetIO().MouseClickedPos[1];
}
ImVec2 InputAdapter::bypassMouseClickedPos2()
{
	return ImGui::GetIO().MouseClickedPos[2];
}
bool InputAdapter::bypassIsMouseDragging0()
{
	return ImGui::IsMouseDragging(0);
}
bool InputAdapter::bypassIsMouseDragging1()
{
	return ImGui::IsMouseDragging(1);
}
bool InputAdapter::bypassIsMouseDragging2()
{
	return ImGui::IsMouseDragging(2);
}
ImVec2 InputAdapter::bypassGetMouseDragDelta0()
{
	return ImGui::GetMouseDragDelta(0);
}
ImVec2 InputAdapter::bypassGetMouseDragDelta1()
{
	return ImGui::GetMouseDragDelta(1);
}
ImVec2 InputAdapter::bypassGetMouseDragDelta2()
{
	return ImGui::GetMouseDragDelta(2);
}
ImVec2 InputAdapter::bypassGetMouseDelta()
{
	return ImGui::GetIO().MouseDelta;
}
ImVec2 InputAdapter::bypassGetMousePos()
{
	return ImGui::GetIO().MousePos;
}
float InputAdapter::bypassGetMouseWheel()
{
	return ImGui::GetIO().MouseWheel;
}
} // namespace DIWNE