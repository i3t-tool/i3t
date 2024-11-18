#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

using RegisterFn = std::function<void(sol::state&)>;

class Registry
{
public:
	static void registerAll(sol::state& L)
	{
		for (const auto& fn : m_functions)
		{
			fn(L);
		}
	}

	static void record(RegisterFn fn)
	{
		m_functions.push_back(fn);
	}

private:
	static inline std::vector<RegisterFn> m_functions;
};

/// Macro for registering Lua functions. It exposes a variable `L` of type `sol::state&` which represents the Lua state.
#define LUA_REGISTRATION                                                                                               \
	static void register__(sol::state& L);                                                                             \
	namespace                                                                                                          \
	{                                                                                                                  \
	struct Registrator                                                                                                 \
	{                                                                                                                  \
		Registrator() noexcept                                                                                         \
		{                                                                                                              \
			Registry::record(register__);                                                                              \
		}                                                                                                              \
	};                                                                                                                 \
	static const Registrator registrator__;                                                                            \
	}                                                                                                                  \
	static void register__(sol::state& L)
