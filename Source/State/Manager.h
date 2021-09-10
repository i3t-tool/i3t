#pragma once

#include <optional>

#include "Memento.h"

class IStateful;

class StateManager
{
public:
	explicit StateManager(IStateful* originator) : m_originator(originator) {}

	void takeSnapshot();
	void undo();
	void redo();

	std::optional<Memento> getCurrentState() const;

private:
	bool hasNewestState() const;

	int m_currentStateIdx = -1;
	IStateful* m_originator;
	std::vector<Memento> m_mementos;
};
