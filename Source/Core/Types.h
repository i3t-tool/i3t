#pragma once

#include <variant>

using Void = std::monostate;

/// Definition of more friendly shared_ptr usage.
template <typename T> using Ptr = std::shared_ptr<T>;

/// Unique pointer shortcut.
template <typename T> using UPtr = std::unique_ptr<T>;

template <typename T> using WPtr = std::weak_ptr<T>;
