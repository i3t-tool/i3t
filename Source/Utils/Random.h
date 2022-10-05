#pragma once

#include <cstdlib>
#include <time.h>

namespace
{
struct RandInitializer
{
	RandInitializer() noexcept { srand(time(0)); }
};

static RandInitializer randInitializer;
} // namespace

long randLong()
{
	if (sizeof(int) < sizeof(long))
		return (static_cast<long>(rand()) << (sizeof(int) * 8)) | rand();

	return rand();
}
