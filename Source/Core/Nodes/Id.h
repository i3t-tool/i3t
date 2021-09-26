#pragma once

#include <list>

namespace Core
{
typedef unsigned int ID;

class IdGenerator
{
	friend class NodeBase;
	friend class Pin;

public:
	IdGenerator();
	ID next();
	void markAsUsed(unsigned int id);

private:
	void returnId(unsigned int id);

	std::list<unsigned int> m_ids;
};
} // namespace Core
