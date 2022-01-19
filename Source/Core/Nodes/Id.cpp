#include "Id.h"

#include "Node.h"

using namespace Core;

IdGenerator::IdGenerator()
{
	// m_ids.resize(Core::MAX_NODES_COUNT);
	for (unsigned i = MAX_NODES_COUNT; i > 0; i--)
	{
		m_ids.push(i);
	}

	/*
	auto it  = m_ids.begin();
	auto val = 1;
	while (it != m_ids.end())
	{
		*it = val;
		it++; val++;
	}
	 */
}

ID IdGenerator::next()
{
	I3T_ASSERT(!m_ids.empty() && "Stack underflow!");

	auto result = m_ids.top();
	m_ids.pop();

	return result;
}

void IdGenerator::markAsUsed(unsigned int id)
{
	/// \todo MH!!!
	// m_ids.remove(id);
}

void IdGenerator::returnId(unsigned int id)
{
	m_ids.push(id);
	/*
	auto back = m_ids.end();
	back--;
	*back = id;
	 */
}
