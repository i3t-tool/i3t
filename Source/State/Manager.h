#pragma once

#include <optional>

#include "Core/Defs.h"

#include "Memento.h"

class IStateful;

/**
 *
 */
class StateManager : public Singleton<StateManager>
{
public:
	void setOriginator(IStateful* originator) { m_originator = originator; }
	void takeSnapshot();
	void undo();
	void redo();

	[[nodiscard]] std::optional<Memento> getCurrentState() const;

private:
	[[nodiscard]] bool hasNewestState() const;

	int m_currentStateIdx = -1;
	IStateful* m_originator;
	std::vector<Memento> m_mementos;
};
