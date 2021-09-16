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

	if (m_mementos.empty()) return;

	/*
	auto memento = m_mementos.back();
	m_mementos.pop_back();
	 */
	auto memento = m_mementos[m_currentStateIdx];

	m_originator->setState(memento);
	--m_currentStateIdx;
}

void StateManager::redo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (m_mementos.empty()) return;

	auto memento = m_mementos[m_currentStateIdx];
	m_originator->setState(memento);
	++m_currentStateIdx;
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
