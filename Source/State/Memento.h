#pragma once

#include <string>
#include <vector>

using State = std::vector<std::string>;

class Memento
{
public:
	Memento() = default;
	explicit Memento(const State& state);
	const State& getSnapshot() const { return m_state; }

private:
	State m_state;
};
