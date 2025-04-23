/**
 * \file
 * \brief General iterators for traversing node trees.
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * \details
 * The iterators are based on the old C++11 iterator named requirements and use CRTP static inheritance to reduce code
 * duplication.
 */
#pragma once

#include <iterator>
#include <vector>

#include <glm/mat4x4.hpp>

#include "Node.h"
#include "Pin.h"

#define INHERIT_ITERATOR_TRAITS_ALL(T)                                                                                 \
	using iterator_category = T::iterator_category;                                                                    \
	using difference_type = T::difference_type;                                                                        \
	using value_type = T::value_type;                                                                                  \
	using pointer = T::pointer;                                                                                        \
	using reference = T::reference;

#define INHERIT_ITERATOR_TRAITS_BASIC(T)                                                                               \
	using difference_type = T::difference_type;                                                                        \
	using value_type = T::value_type;                                                                                  \
	using pointer = T::pointer;                                                                                        \
	using reference = T::reference;

template <typename Iterator, typename Value, bool IsConst>
class ForwardIteratorBase
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = typename std::conditional<IsConst, const Value, Value>::type;
	using pointer = typename std::conditional<IsConst, const Value*, Value*>::type;
	using reference = typename std::conditional<IsConst, const Value&, Value&>::type;

protected:
	ForwardIteratorBase() {}

	// clang-format off
	Iterator* self() { return static_cast<Iterator*>(this); } ///< CRTP self pointer (pointer to the derived type)
	const Iterator* self() const { return static_cast<const Iterator*>(this); } ///< CRTP const self pointer

	/// Return a reference to the current element (reference to a value type, not a pointer).
	reference dereference() const { return self()->dereference(); }
	/// Return a raw pointer to the current element.
	pointer ptr() const { return self()->ptr(); }

	/// Determines whether this iterator is pointing to the same element as another one (used by the == operator)
	bool equals(const Iterator& b) const { return self()->equals(b); }
	/// Advance the iterator
	void next() { self()->next(); }
	/// Check whether the iterator is pointing at a valid element (eg. not the end of a container)
	bool valid() const { return self()->valid(); }

public:
	// Basic iterator operators
	reference operator*() { return self()->dereference(); }
	pointer operator->() { return self()->ptr(); }
	Iterator& operator++() { next(); return *self(); }
	Iterator operator++(int) { Iterator tmp = *self(); ++(*this); return tmp; }
	friend bool operator==(const Iterator& a, const Iterator& b) { return a.equals(b); }
	friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }
	// clang-format on
};

namespace Core
{

class PreorderNodeTreeIterator : public ForwardIteratorBase<PreorderNodeTreeIterator, Node, false>
{
protected:
	using Super = ForwardIteratorBase<PreorderNodeTreeIterator, Node, false>;
	using Iterator = PreorderNodeTreeIterator;

	std::stack<Node*, std::vector<Node*>> m_stack;
	Node* m_curr{nullptr}; ///< Current node of the iterated (sub)tree, nullptr indicates invalid/after-end iterator.

	/// Type of connections the iterator considers, corresponds to EValueType enum, -1 for all.
	int m_connectionType{-1};

public:
	INHERIT_ITERATOR_TRAITS_ALL(Super)

	PreorderNodeTreeIterator() {} // Default constructible to adhere to STD requirements
	PreorderNodeTreeIterator(Node* root, int connectionType = -1) : m_curr(root), m_connectionType(connectionType) {}

	// clang-format off
	reference dereference() const { return *m_curr; }
	pointer ptr() const { return m_curr; }

	bool equals(const Iterator& b) const { return m_curr == b.m_curr; }
	bool valid() const { return m_curr != nullptr; }
	// clang-format on

	void next()
	{
		if (m_curr == nullptr)
			return;

		// Fetch children and push them on the stack
		const std::vector<Pin>& children = m_curr->getOutputPins();
		for (auto pinIt = children.rbegin(); pinIt != children.rend(); ++pinIt)
		{
			auto& outPin = *pinIt;
			if (m_connectionType != -1)
			{
				if (m_connectionType != static_cast<int>(outPin.ValueType))
					continue;
			}
			const std::vector<Pin*>& connections = outPin.getOutComponents();
			for (auto conIt = connections.rbegin(); conIt != connections.rend(); ++conIt)
			{
				auto& inPin = *conIt;
				m_stack.emplace(&inPin->Owner);
			}
		}

		if (m_stack.empty()) // no children, end
		{
			m_curr = nullptr;
			return;
		}

		m_curr = m_stack.top();
		m_stack.pop();
	}
};

/**
 * Container representing a tree of nodes connected together using a particular type of connections.
 * The tree is defined by a single root node and consists of all its children. The root node itself can have parents,
 * but those aren't considered part of the tree (hence a sub tree can be represented).
 */
class NodeTree
{
	using TreeIterator = PreorderNodeTreeIterator;

	Node* m_root; ///< Root of the (sub)tree. Its first node.

	/// Type of connections the node tree consists of, corresponds to the Core::EValueType enum, -1 for all.
	int m_connectionType{-1};

public:
	NodeTree(Node* root, int connectionType = -1) : m_root(root), m_connectionType(connectionType) {}

	/// Get a node iterator pointing to the root of this node tree.
	TreeIterator begin()
	{
		return TreeIterator(this->m_root, m_connectionType);
	}
	/// Get a node iterator pointing to an element following the last element of this node range.
	TreeIterator end()
	{
		return TreeIterator(nullptr, m_connectionType);
	}

	/**
	 * Collect all nodes in the tree and return them as a std::vector of non-owning raw pointers.
	 * This operation iterates over the underlying tree iterator and collects all it's elements.
	 */
	std::vector<Node*> collect()
	{
		static_assert(std::is_same<Node*, typename TreeIterator::pointer>::value);
		std::vector<Node*> out;
		for (auto it = begin(); it != end(); ++it)
		{
			out.push_back(it.operator->());
		}
		return out;
	}
};

/**
 * Preorder iterator over a subtree of sequences and models connected using matrix multiplication connections.
 * An accumulated transform from the root to each node is provided during iteration (not including the node and root).
 */
class SequenceTreeIterator : public ForwardIteratorBase<SequenceTreeIterator, Node, false>
{
protected:
	using Super = ForwardIteratorBase<SequenceTreeIterator, Node, false>;
	using Iterator = SequenceTreeIterator;

	bool m_root = true;
	std::stack<std::pair<Node*, glm::mat4>, std::vector<std::pair<Node*, glm::mat4>>> m_stack;
	Node* m_curr{nullptr}; ///< Current node of the iterated (sub)tree, nullptr indicates invalid/after-end iterator.
	glm::mat4 m_transform{1.0f};

public:
	INHERIT_ITERATOR_TRAITS_ALL(Super)

	SequenceTreeIterator() {} // Default constructible to adhere to STD requirements
	SequenceTreeIterator(Node* root) : m_curr(root) {}

	// clang-format off
	reference dereference() const { return *m_curr; }
	pointer ptr() const { return m_curr; }

	bool equals(const Iterator& b) const { return m_curr == b.m_curr; }
	bool valid() const { return m_curr != nullptr; }
	// clang-format on

	glm::mat4 transform()
	{
		return m_transform;
	}

	void next()
	{
		if (m_curr == nullptr)
			return;

		auto seq = dynamic_cast<Sequence*>(m_curr);
		if (seq && !m_root)
			m_transform *= seq->data(I3T_SEQ_MAT).getMat4();
		m_root = false;

		// Fetch children and push them on the stack
		const std::vector<Pin>& children = m_curr->getOutputPins();
		for (auto pinIt = children.rbegin(); pinIt != children.rend(); ++pinIt)
		{
			auto& outPin = *pinIt;
			if (static_cast<int>(outPin.ValueType) != static_cast<int>(EValueType::MatrixMul))
				continue;
			const std::vector<Pin*>& connections = outPin.getOutComponents();
			for (auto conIt = connections.rbegin(); conIt != connections.rend(); ++conIt)
			{
				auto& inPin = *conIt;
				m_stack.emplace(&inPin->Owner, m_transform);
			}
		}

		if (m_stack.empty()) // no children, end
		{
			m_curr = nullptr;
			m_transform = glm::mat4(1.0f);
			return;
		}

		auto& entry = m_stack.top();
		m_curr = entry.first;
		m_transform = entry.second;
		m_stack.pop();
	}
};

class SequenceTree
{
	using TreeIterator = SequenceTreeIterator;

	Node* m_root; ///< Root of the (sub)tree. Its first node.

public:
	SequenceTree(Node* root) : m_root(root) {}

	/// Get a node iterator pointing to the root of this node tree.
	TreeIterator begin()
	{
		return TreeIterator(this->m_root);
	}
	/// Get a node iterator pointing to an element following the last element of this node range.
	TreeIterator end()
	{
		return TreeIterator(nullptr);
	}
};

} // namespace Core