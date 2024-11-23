#pragma once

#include "InputAdapter.h"

namespace DIWNE
{
class NodeEditor;

class NodeEditorInputAdapter : public InputAdapter
{
protected:
	NodeEditor* editor;

public:
	NodeEditorInputAdapter(NodeEditor* editor);

	bool selectionRectangleKeyDown();
	bool selectionRectangleKeyPressed();
	bool selectionRectangleDragging();
	ImVec2 selectionRectangleStartPosition();
	ImVec2 selectionRectangleSize();

	bool panKeyDown();
	bool panKeyPressed();
	bool panDragging();

	bool bypassIsMouseDragging0() override;
	bool bypassIsMouseDragging1() override;
	bool bypassIsMouseDragging2() override;
};
} // namespace DIWNE
