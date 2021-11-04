#include "Manager.h"

#include "Stateful.h"

void StateManager::takeSnapshot()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	m_mementos.push_back(m_originator->getState());

	m_currentStateIdx = m_mementos.size() - 1;
}

void StateManager::undo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canUndo()) return;

	auto memento = m_mementos[m_currentStateIdx];

	m_originator->setState(memento);
	--m_currentStateIdx;
}

void StateManager::redo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canRedo()) return;

	++m_currentStateIdx;  // move cursor to the next state
	auto memento = m_mementos[m_currentStateIdx];
	m_originator->setState(memento);
}

bool StateManager::canUndo() const
{
	return !m_mementos.empty() && m_currentStateIdx >= 0;
}

bool StateManager::canRedo() const
{
	return !m_mementos.empty() && m_currentStateIdx != m_mementos.size() - 1;
}

std::optional<Memento> StateManager::getCurrentState() const
{
	if (m_currentStateIdx == -1) return std::nullopt;
	return m_mementos[m_currentStateIdx];
}

bool StateManager::hasNewestState() const
{
	return m_currentStateIdx == m_mementos.size() - 1;
}
