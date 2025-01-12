#pragma once

#include <string>

namespace DIWNE {

class Pin;
class Link;

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

	/**
	 * Callback called at the end of the frame the action was ended.
	 */
	virtual void onEnd(){};
};

namespace Actions
{
// Global static names of all actions
inline static const std::string selectionRect = "selectionRect";
inline static const std::string connectPin = "connectPin";

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

	SelectionRectAction(const std::string& source) : DiwneAction(selectionRect, source) {}
};

}; // namespace Actions
} // namespace DIWNE