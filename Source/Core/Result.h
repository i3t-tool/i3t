/**
 * @author Martin Herich martin.herich@phire.cz
 * Taken from PhireGames/Candeo
 */
#pragma once

#include <expected>
#include <string>

#include "Core/Format.h"
#include "Core/Types.h"

struct Error
{
	Error() = default;
	Error(const char* message) : m_message(message) {}
	Error(std::string message) : m_message(std::move(message)) {}

	const char* str() const
	{
		return m_message.c_str();
	}

private:
	std::string m_message;
};

template <typename V, typename E> using Result = std::expected<V, E>;

template <typename V, typename E> bool operator&&(const Result<V, E>& lhs, bool rhs)
{
	return lhs.has_value() && rhs;
}

template <typename V, typename E> bool operator&&(bool lhs, const Result<V, E>& rhs)
{
	return lhs && rhs.has_value();
}

#define Err(x) std::unexpected(x);

template <> struct fmt::formatter<Error>
{
	template <typename ParseContext> constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template <typename FormatContext> auto format(const Error& error, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "{}", error.str());
	};
};
