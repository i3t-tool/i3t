#include "Id.h"

#include "Node.h"

using namespace Core;

IdGenerator::IdGenerator()
{
	m_ids.resize(Core::MAX_NODES_COUNT);

	auto it  = m_ids.begin();
	auto val = 1;
	while (it != m_ids.end())
	{
		*it = val;
		it++; val++;
	}
}

ID IdGenerator::next()
{
	I3T_ASSERT(!m_ids.empty() && "List is empty!");
	auto result = m_ids.front();
	m_ids.pop_front();

	return result;
}

void IdGenerator::markAsUsed(unsigned int id)
{
	m_ids.remove(id);
}

void IdGenerator::returnId(unsigned int id)
{
	auto back = m_ids.end();
	back--;
	*back = id;
}
