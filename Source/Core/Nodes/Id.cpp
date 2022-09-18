#include "Id.h"

#include "Node.h"

using namespace Core;

IdGenerator::IdGenerator()
{
	for (unsigned i = 0; i < MAX_NODES_COUNT; ++i)
	{
		m_IDs.insert(i);
	}
}

bool IdGenerator::hasNext() { return !m_IDs.empty(); }

ID IdGenerator::next()
{
	I3T_ASSERT(hasNext() && "hasNext() == false");

	auto it = m_IDs.begin();
	auto result = *it;
	m_IDs.erase(it);

	return result;
}

void IdGenerator::markAsUsed(ID id) { m_IDs.erase(id); }

void IdGenerator::returnId(ID id) { m_IDs.insert(id); }
