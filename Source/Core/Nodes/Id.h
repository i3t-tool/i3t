#pragma once

namespace Core
{
typedef unsigned int ID;

class IdGenerator
{
	static ID m_id;

public:
	static ID next();
};
} // namespace Core
