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
class DiwneObject;

namespace Actions
{
// Base actions
// ============================================================================================================

/**
 * Base class for DIWNE Actions.
 * Actions are operations that span and carry state across multiple frames.
 */
struct DiwneAction
{
	std::string name;
	std::weak_ptr<DiwneObject> source;
	bool endActionThisFrame{false};

	/**
	 * @param name String identifier of the action
	 * @param source DiwneObject considered to be it's source, eg. the object that started it and is most likely also
	 * responsible for ending it.
	 */
	DiwneAction(const std::string& name, const std::shared_ptr<DiwneObject>& source) : name(name), source(source) {}
	virtual ~DiwneAction() = default;

	/**
	 * Marks the action to end at the end of this frame (or beginning of the next one).
	 * At the same time onFrameEnd() and onEnd() callbacks will be called in that order.
	 * \warning Ending the action is usually the responsibility of the object that started it. If this object fails to
	 * end it, it will simply be active forever.
	 * There are mechanisms to prevent this, namely, an action will be ended if the source object gets destroyed or
	 * forcibly deallocated (action source is a weak pointer), and any action can be ended by the NodeEditor when ESC
	 * is pressed.\n
	 * An issue can arise when action is started and then the source object is hidden (not rendered anymore due to
	 * allowDrawing() returning false). In this case, the drawing lifecycle methods and processing of interactions is
	 * NOT called, only the initialize() and finalize() methods are called. So it must be ensured that either the source
	 * object will not get hidden until the end of the action or the check for ending the action happens in one of those
	 * two methods.\n
	 * An exception is ending an action in DiwneObject::onDrag(), this method will always get called when the dragging
	 * ends even when it is due to the object becomming hidden, so it is safe to start and end actions there.
	 */
	void end()
	{
		endActionThisFrame = true;
	}

	/// Called at the end of each frame the action is active for, called before onEnd() on the last frame.
	virtual void onFrameEnd() {};
	/// Callback called at the end of the frame the action was ended. Called after onFrameEnd().
	virtual void onEnd() {};
	/// Checks whether passed object is the source of this action (eg. the object that started it
	bool isSource(const DiwneObject* object) const
	{
		if (this->source.expired())
			return false;
		if (this->source.lock().get() != object)
			return false;
		return true;
	}
	/// Checks whether the source of this and another action are the same.
	bool hasSameSource(const DiwneAction& action) const
	{
		if (this->source.expired() || action.source.expired())
			return false;
		if (this->source.lock() != action.source.lock())
			return false;
		return true;
	}
};

/// Helper class for actions that require an explicit reference to the NodeEditor they are active in.
struct EditorAction : public DiwneAction
{
	NodeEditor& editor;

	EditorAction(NodeEditor& editor, const std::string& name, const std::shared_ptr<DiwneObject>& source)
	    : editor(editor), DiwneAction(name, source)
	{}
};

// Custom actions
// ============================================================================================================

// Global static names of all actions
// TODO: How to solve the "duality" of action name strings and action classes? How do I fetch the action type from an
//  an action string name?
inline static const std::string selectionRect = "selectionRect";
inline static const std::string connectPin = "connectPin";
inline static const std::string dragNode = "dragNode";

// The actual action classes
struct ConnectPinAction : public DiwneAction
{
	Pin* startPin{nullptr};
	Link* draggedLink{nullptr};

	ConnectPinAction(const std::shared_ptr<DiwneObject>& source) : DiwneAction(connectPin, source) {}

	void onEnd() override;
};

struct SelectionRectAction : public DiwneAction
{
	bool touch = false;
	ImRect rect{0, 0, 0, 0};

	SelectionRectAction(const std::shared_ptr<DiwneObject>& source) : DiwneAction(selectionRect, source) {}
};

struct DragNodeAction : public EditorAction
{
	std::vector<std::shared_ptr<Node>> nodes; // Using shared ptrs here to avoid weak ptr locking

	DragNodeAction(NodeEditor& editor, const std::shared_ptr<DiwneObject>& source,
	               std::vector<std::shared_ptr<Node>> nodes)
	    : nodes(std::move(nodes)), EditorAction(editor, dragNode, source)
	{}

	void onFrameEnd() override;
};

}; // namespace Actions
} // namespace DIWNE