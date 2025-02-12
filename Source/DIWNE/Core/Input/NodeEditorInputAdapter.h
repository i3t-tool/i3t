#pragma once

#include "InputAdapter.h"

namespace DIWNE
{
class NodeEditor;

class NodeEditorInputAdapter : public InputAdapter
{
protected:
	NodeEditor& editor;

public:
	NodeEditorInputAdapter(NodeEditor& editor);

	bool selectionRectangleKeyDown();
	bool selectionRectangleKeyPressed();
	bool selectionRectangleDragging();
	ImVec2 selectionRectangleStartPosition();
	ImVec2 selectionRectangleSize();

	bool panKeyDown();
	bool panKeyPressed();
	bool panDragging();

	/**
	 * Multi selection allows multiple items to be selected.
	 * Depending on the context is either allows other items to be selected, or it simply toggles the selection state
	 * on individual objects.
	 */
	bool multiSelectionActive();
	/**
	 * Subtractive selection version of the multi selection. Deselects a portion of items.
	 */
	bool multiDeselectionActive();

	float getZoomDelta();

	bool bypassIsMouseDragging0() override;
	bool bypassIsMouseDragging1() override;
	bool bypassIsMouseDragging2() override;
};
} // namespace DIWNE
