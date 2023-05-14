#pragma once

#include <cstdint>
#include <set>

namespace Core
{
using ID = std::int64_t;
constexpr ID NIL_ID = 0;

class IdGenerator
{
public:
	static ID next() { return ++m_NextId; }

private:
	static ID m_NextId;
};
} // namespace Core
