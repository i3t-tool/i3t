#include "Id.h"

using namespace Core;

/* node-editor use 0 as invalid id. see: https://github.com/thedmd/imgui-node-editor/issues/47*/
ID IdGenerator::m_id = 1;

ID IdGenerator::next()
{
	return m_id++;
}
