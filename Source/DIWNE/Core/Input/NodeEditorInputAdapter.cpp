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
ImVec2 NodeEditorInputAdapter::selectionRectangleSize()
{
	return editor.input().bypassGetMouseDragDelta0() / editor.canvas().getZoom();
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
	return ImGui::IsMouseDragging(0, editor.mp_settingsDiwne->mouseDragThreshold);
}
bool NodeEditorInputAdapter::bypassIsMouseDragging1()
{
	return ImGui::IsMouseDragging(1, editor.mp_settingsDiwne->mouseDragThreshold);
}
bool NodeEditorInputAdapter::bypassIsMouseDragging2()
{
	return ImGui::IsMouseDragging(2, editor.mp_settingsDiwne->mouseDragThreshold);
}
} // namespace DIWNE
