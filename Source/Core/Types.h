#pragma once

#include <variant>

template <typename T, typename E>
class Result
{
public:
	Result(T&& val) : m_state(val) {}
	Result(E&& val) : m_state(val) {}

	bool isOk() const { return m_state.index() == 0; }
	bool isErr() const { return m_state.index() == 1; }

	T&& unwrap() { return std::get<T>(m_state); }
	const T& unwrap() const { return std::get<T>(m_state); }

	E&& err() { return std::get<E>(m_state); }
	const E& err() const { return std::get<E>(m_state); }

private:
	std::variant<T, E> m_state;
};

using Void = std::monostate;

/// Definition of more friendly shared_ptr usage.
template <typename T> using Ptr = std::shared_ptr<T>;

/// Unique pointer shortcut.
template <typename T> using UPtr = std::unique_ptr<T>;

template <typename T> using WPtr = std::weak_ptr<T>;

//

template <typename BaseClass> class ICloneable
{
public:
	virtual Ptr<BaseClass> clone() = 0;
};
