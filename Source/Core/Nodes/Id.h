#pragma once

#include <set>

namespace Core
{
typedef unsigned int ID;

class IdGenerator
{
public:
	IdGenerator();

	bool hasNext();
	ID   next();
	void markAsUsed(ID id);
	void returnId(ID id);

private:
	std::set<ID> m_IDs;
};
} // namespace Core
