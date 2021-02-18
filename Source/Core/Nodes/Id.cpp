#include "Id.h"

using namespace Core;

ID IdGenerator::m_id = 0;

ID IdGenerator::next()
{
  return m_id++;
}
