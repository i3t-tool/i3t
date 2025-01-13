#pragma once

#include <string>
#include <vector>

#include "diwne_imgui.h"

namespace DIWNE
{
class NodeEditor;
class Pin;
class Link;
class Node;

namespace Actions
{
// Base actions
// ============================================================================================================
struct DiwneAction
{
	std::string name;
	std::string source;
	bool endActionThisFrame{false};

	DiwneAction(const std::string& name, const std::string& source) : name(name), source(source) {}

	void end()
	{
		endActionThisFrame = true;
	}

	/// Called on each frame during the action
	virtual void onUpdate(){};

	/// Callback called at the end of the frame the action was ended.
	virtual void onEnd(){};
};

struct EditorAction : public DiwneAction
{
	DIWNE::NodeEditor& editor;

	EditorAction(DIWNE::NodeEditor& editor, const std::string& name, const std::string& source)
	    : editor(editor), DiwneAction(name, source)
	{}
};

// Custom actions
// ============================================================================================================

// Global static names of all actions
inline static const std::string selectionRect = "selectionRect";
inline static const std::string connectPin = "connectPin";
inline static const std::string dragNode = "dragNode";

// The actual action classes
struct ConnectPinAction : public DiwneAction
{
	Pin* startPin{nullptr};
	Link* draggedLink{nullptr};

	ConnectPinAction(const std::string& source) : DiwneAction(connectPin, source) {}

	void onEnd() override;
};

struct SelectionRectAction : public DiwneAction
{
	bool touch = false;
	ImRect rect{0, 0, 0, 0};

	SelectionRectAction(const std::string& source) : DiwneAction(selectionRect, source) {}
};

struct DragNodeAction : public EditorAction
{
	std::vector<DIWNE::Node*> nodes;

	DragNodeAction(DIWNE::NodeEditor& editor, const std::string& source, std::vector<DIWNE::Node*> nodes)
	    : nodes(std::move(nodes)), EditorAction(editor, dragNode, source)
	{}

	void onUpdate() override;
};

}; // namespace Actions
} // namespace DIWNE