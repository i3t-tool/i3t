#pragma once

#include <stack>

namespace Core
{
typedef unsigned int ID;

class IdGenerator
{
	friend class Node;
	friend class Pin;

public:
	IdGenerator();
	ID next();
	void markAsUsed(unsigned int id);
	void returnId(unsigned int id);

private:
	std::stack<unsigned int> m_ids;
};
} // namespace Core
