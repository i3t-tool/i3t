#pragma once

#include <memory>
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
	virtual ~DiwneAction() = default;

	void end()
	{
		endActionThisFrame = true;
	}

	/// Called at the end of each frame the action is active for
	virtual void onFrameEnd(){};

	/// Callback called at the end of the frame the action was ended.
	virtual void onEnd(){};
};

struct EditorAction : public DiwneAction
{
	NodeEditor& editor;

	EditorAction(NodeEditor& editor, const std::string& name, const std::string& source)
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
	std::vector<std::shared_ptr<Node>> nodes; // Using shared ptrs here to avoid weak ptr locking

	DragNodeAction(NodeEditor& editor, const std::string& source, std::vector<std::shared_ptr<Node>> nodes)
	    : nodes(std::move(nodes)), EditorAction(editor, dragNode, source)
	{}

	void onFrameEnd() override;
};

}; // namespace Actions
} // namespace DIWNE