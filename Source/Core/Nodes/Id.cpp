#include "Id.h"

#include "Node.h"

using namespace Core;

/* node-editor use 0 as invalid id. see: https://github.com/thedmd/imgui-node-editor/issues/47*/

IdGenerator::IdGenerator()
{
	for (int i = 1; i < Core::MAX_NODES_COUNT; ++i)
	{
		m_ids.push_back(i);
	}
}

ID IdGenerator::next()
{
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
	m_ids.push_back(id);
}
