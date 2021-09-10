#pragma once

#include <string>
#include <vector>

using State = std::vector<std::string>;

class Memento
{
public:
	explicit Memento(const State& state);
	State getSnapshot() const { return m_state; }

private:
	State m_state;
};
