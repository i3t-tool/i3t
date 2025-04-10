#include "NodeEditorInputAdapter.h"

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
NodeEditorInputAdapter::NodeEditorInputAdapter(NodeEditor& editor) : editor(editor) {}

bool NodeEditorInputAdapter::selectionRectangleKeyDown()
{
	return bypassIsMouseDown0();
}
bool NodeEditorInputAdapter::selectionRectangleKeyPressed()
{
	return bypassIsMouseClicked0();
}
bool NodeEditorInputAdapter::selectionRectangleDragging()
{
	return bypassIsMouseDragging0();
}
ImVec2 NodeEditorInputAdapter::selectionRectangleStartPosition()
{
	return editor.canvas().screen2diwne(editor.input().bypassMouseClickedPos0());
}

bool NodeEditorInputAdapter::panKeyDown()
{
	return bypassIsMouseDown2();
}
bool NodeEditorInputAdapter::panKeyPressed()
{
	return bypassIsMouseClicked2();
}
bool NodeEditorInputAdapter::panDragging()
{
	return bypassIsMouseDragging2();
}

bool NodeEditorInputAdapter::selectAllNodes()
{
	return ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_A, false);
}
bool NodeEditorInputAdapter::invertSelection()
{
	return ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_I, false);
}
bool NodeEditorInputAdapter::deleteSelectedNodes()
{
	return ImGui::IsKeyPressed(ImGuiKey_Delete, false);
}

bool NodeEditorInputAdapter::multiSelectionActive()
{
	return ImGui::IsKeyDown(ImGuiMod_Shift);
}

bool NodeEditorInputAdapter::multiDeselectionActive()
{
	return ImGui::IsKeyDown(ImGuiMod_Ctrl);
}

float NodeEditorInputAdapter::getZoomDelta()
{
	return bypassGetMouseWheel() * editor.mp_settingsDiwne->zoomSpeed;
}

bool NodeEditorInputAdapter::bypassIsMouseDragging0()
{
	return ImGui::IsMouseDragging(0, editor.style().decimal(Style::MOUSE_DRAG_THRESHOLD));
}
bool NodeEditorInputAdapter::bypassIsMouseDragging1()
{
	return ImGui::IsMouseDragging(1, editor.style().decimal(Style::MOUSE_DRAG_THRESHOLD));
}
bool NodeEditorInputAdapter::bypassIsMouseDragging2()
{
	return ImGui::IsMouseDragging(2, editor.style().decimal(Style::MOUSE_DRAG_THRESHOLD));
}
} // namespace DIWNE
