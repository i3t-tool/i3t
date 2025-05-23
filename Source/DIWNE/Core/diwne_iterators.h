/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <iterator>
#include <stack>
#include <type_traits>
#include <vector>

#include "Elements/Containers/INodeContainer.h"
#include "Elements/Node.h"

// Node iterators
// ============================================================================================================
// Iterator implementations for Node range "containers".

// TODO: Add iterator conversion constructors (regular -> recursive)
// TODO: Are const iterators actually needed?

// The inherit iterator traits macros just bring into scope the iterator typedefs from the parent class.
// This is needed because when inheriting from a templated class, members aren't brought into scope by default.
// See https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
#define DIWNE_INHERIT_ITERATOR_TRAITS_ALL(T)                                                                           \
	using iterator_category = T::iterator_category;                                                                    \
	using difference_type = T::difference_type;                                                                        \
	using value_type = T::value_type;                                                                                  \
	using pointer = T::pointer;                                                                                        \
	using reference = T::reference;

#define DIWNE_INHERIT_ITERATOR_TRAITS_BASIC(T)                                                                         \
	using difference_type = T::difference_type;                                                                        \
	using value_type = T::value_type;                                                                                  \
	using pointer = T::pointer;                                                                                        \
	using reference = T::reference;

namespace DIWNE
{
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
	virtual ~ForwardIteratorBase() = default;

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

	/// Return the owning pointer to the current element. Creates a new shared pointer object.
	std::shared_ptr<Value> sharedPtr() const { return self()->sharedPtr(); }

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

/**
 * A simple forward node iterator for DIWNE node containers that is used as a base implementation for other iterators.
 * Node iterators are essentially a pair of a vector of node pointers and an index pointing to one of them.
 *
 * Node iterators are mainly to be used for read only iteration in order to avoid costly construction of filtered
 * containers.
 *
 * Currently all the iterators wrap the default owning NodeList (std::vector<std::shared_ptr<Node>>).
 * Eg. a list of shared pointers to nodes.
 * A reference is kept to this list and dereferencing an iterator return a reference or raw pointer to a single node.
 * An owning shared pointer can be retrieved from the reference or raw pointer by calling the sharedPtr() method.
 * Or by just calling shared_from_this() directly as all DiwneObjects are std::enable_shared_from_this objects.
 *
 * The iterator itself can be iterated over like a container using its begin() and end() methods.
 * This is to avoid having an intermediary container for each iterator type and to simplify range based for loops.
 * CRTP and inheritance is used mostly to just reduce code duplication and generally a derived iterator shouldn't be
 * casted to its parent type.
 *
 * The iterators attempt to comply with C++ legacy (pre C++20) iterator named requirements, but aren't necessarily
 * guaranteed to work well with the rest of the standard library.
 * @tparam Iterator The CRTP derived iterator
 * @note Both regular dynamic and CRTP static inheritance is used in the iterators. For performance reasons iterators
 * often rely on static (compile time) inheritance to avoid virtual call overhead. For clarity virtual calls are used.
 */
template <typename Iterator, typename NodeType, bool IsConst>
class ForwardNodeIterator : public ForwardIteratorBase<Iterator, NodeType, IsConst>
{
protected:
	using Super = ForwardIteratorBase<Iterator, NodeType, IsConst>;

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)

	//	using Container =
	//	    std::conditional<IsConst, const NodeList*,
	// NodeList*>::type;

	// Non-const iterators are not mutable to allow usage in const methods
	using Container = const NodeList*;
	using shared_pointer = std::shared_ptr<typename Super::value_type>;

protected:
	Container _nodes{nullptr}; ///< Reference to the node list
	std::size_t _idx{0};       ///< Current node index // TODO: Should probably be an int

public:
	ForwardNodeIterator() {} // Default constructible to adhere to STD requirements
	ForwardNodeIterator(Container nodes, std::size_t idx) : _nodes(nodes), _idx(idx)
	{
		assert(_nodes != nullptr && "Can't create a DIWNE iterator on a null node list!");
		assert(_idx >= 0 && _idx <= _nodes->size() &&
		       "DIWNE iterator index should be within the <0, node count> range!");
	}

	// clang-format off
	// TODO: Add debug asserts ensuring static_casts aren't invalid
	reference dereference() const { return static_cast<reference>(*(*_nodes)[_idx]); }
	pointer ptr() const { return static_cast<pointer>(((*_nodes)[_idx]).get()); }
	shared_pointer sharedPtr() const { return std::static_pointer_cast<value_type>((*_nodes)[_idx]); }

	/// Determines whether this iterator is pointing to the same element as another one (used by the == operator)
	bool equals(const Iterator& b) const { return _idx == b._idx; }
	/// Advance the iterator
	void next() { ++_idx; }
	/// Check whether the iterator is pointing at a valid element (eg. not the end of a container)
	bool valid() const { return this->_nodes != nullptr && (this->_idx < this->_nodes->size()); }
	// clang-format on
};

template <typename Iterator, typename NodeType, bool IsConst>
class BidirectionalNodeIterator : public ForwardNodeIterator<Iterator, NodeType, IsConst>
{
protected:
	using Super = ForwardNodeIterator<Iterator, NodeType, IsConst>;

public:
	using iterator_category = std::bidirectional_iterator_tag;
	DIWNE_INHERIT_ITERATOR_TRAITS_BASIC(Super)
	using shared_pointer = typename Super::shared_pointer;

	BidirectionalNodeIterator() {}
	BidirectionalNodeIterator(Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}

	// clang-format off
	// Bidirectional access operators
	Iterator& operator--() { back(); return *this->self(); }
	Iterator operator--(int) { Iterator tmp = *this->self(); --(*this); return tmp; }

	/// Move the iterator to the previous position
	virtual void back() { --(this->_idx); }
	bool valid() const { return Super::valid() && this->_idx >= 0; }
	// clang-format on
};

template <typename NodeType, bool IsConst>
class NodeIteratorImpl : public BidirectionalNodeIterator<NodeIteratorImpl<NodeType, IsConst>, NodeType, IsConst>
{
protected:
	using Super = BidirectionalNodeIterator<NodeIteratorImpl<NodeType, IsConst>, NodeType, IsConst>;
	using Iterator = NodeIteratorImpl<NodeType, IsConst>;

public:
	using iterator_category = std::random_access_iterator_tag;
	DIWNE_INHERIT_ITERATOR_TRAITS_BASIC(Super)
	using shared_pointer = typename Super::shared_pointer;

	NodeIteratorImpl() {}
	NodeIteratorImpl(typename Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}

	// clang-format off
	// Random access operators
	inline reference operator[](difference_type idx) const {return *(*this->_nodes)[idx];}
	inline Iterator& operator+=(difference_type rhs) {this->_idx += rhs; return *this->self(); }
	inline Iterator& operator-=(difference_type rhs) {this->_idx -= rhs; return *this->self(); }
	inline Iterator operator+(difference_type rhs) const {return Iterator(this->_nodes, this->_idx+rhs);}
	inline Iterator operator-(difference_type rhs) const {return Iterator(this->_nodes, this->_idx-rhs);}
	inline bool operator>(const Iterator& rhs) const {return this->_idx > rhs._idx;}
	inline bool operator<(const Iterator& rhs) const {return this->_idx < rhs._idx;}
	inline bool operator>=(const Iterator& rhs) const {return this->_idx >= rhs._idx;}
	inline bool operator<=(const Iterator& rhs) const {return this->_idx <= rhs._idx;}
	// clang-format on
};

template <typename NodeType = Node>
class NodeIterator : public NodeIteratorImpl<NodeType, false>
{
protected:
	using Super = NodeIteratorImpl<NodeType, false>;

public:
	using iterator_category = std::random_access_iterator_tag;
	DIWNE_INHERIT_ITERATOR_TRAITS_BASIC(Super)
	using shared_pointer = typename Super::shared_pointer;

	NodeIterator() {}
	NodeIterator(typename Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}
};

template <typename NodeType = Node>
class ConstNodeIterator : public NodeIteratorImpl<NodeType, true>
{
protected:
	using Super = NodeIteratorImpl<NodeType, true>;

public:
	using iterator_category = std::random_access_iterator_tag;
	DIWNE_INHERIT_ITERATOR_TRAITS_BASIC(Super)
	using shared_pointer = typename Super::shared_pointer;

	ConstNodeIterator() {}
	ConstNodeIterator(typename Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}
};

/**
 * Node iterator that returns only nodes that pass a specified condition.
 * Unlike the regular NodeIterator, it does not have random access, but it can move forwards and backwards.
 * @tparam Iterator Type returned by begin()/end() (the CRTP derived iterator)
 * @see NodeIterator, FilteredRecursiveNodeIterator
 */
template <typename Iterator, typename NodeType, bool IsConst>
class FilteredNodeIteratorImpl : public BidirectionalNodeIterator<Iterator, NodeType, IsConst>
{
protected:
	using Super = BidirectionalNodeIterator<Iterator, NodeType, IsConst>;
	using Predicate = bool (*)(const Node*);
	Predicate _predicate{nullptr};

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
	using shared_pointer = typename Super::shared_pointer;

	FilteredNodeIteratorImpl() {}
	FilteredNodeIteratorImpl(Predicate predicate, typename Super::Container nodes, std::size_t idx)
	    : _predicate(predicate), Super(nodes, idx)
	{
		if (this->_idx < this->_nodes->size() && !valid())
			next(); // The iterator must ensure that it's initial position is valid
	}
	// clang-format off
	void next() { do { Super::next(); } while (this->_idx < this->_nodes->size() && !_predicate(this->ptr())); }
	void back() { do { Super::back(); } while (this->_idx >= 0 && !_predicate(this->ptr())); }
	bool valid() const { return Super::valid() && _predicate(Super::ptr()); }
	// clang-format on
};

template <typename NodeType = Node>
class FilteredNodeIterator : public FilteredNodeIteratorImpl<FilteredNodeIterator<NodeType>, NodeType, false>
{
protected:
	using Super = FilteredNodeIteratorImpl<FilteredNodeIterator<NodeType>, NodeType, false>;

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
	using shared_pointer = typename Super::shared_pointer;

	FilteredNodeIterator() {}
	FilteredNodeIterator(typename Super::Predicate predicate, typename Super::Container nodes, std::size_t idx)
	    : Super(predicate, nodes, idx)
	{}
};

// TODO: Do I need a ConstFilteredNodeIterator?

// TODO: No Const recursive node iterator variant? But do we even really need it?
/**
 * CRTP templated implementation class of the RecursiveNodeIterator
 * @see RecursiveNodeIterator
 */
template <typename Iterator, typename NodeType>
class RecursiveNodeIteratorImpl : public ForwardNodeIterator<Iterator, NodeType, false>
{
protected:
	using Super = ForwardNodeIterator<Iterator, NodeType, false>;
	using ChildNodeRange = NodeRange<>;
	using ChildNodeIterator = NodeIterator<>;

	/// A stack of child container node iterators, they store their own position at each depth level
	std::stack<ChildNodeIterator, std::vector<ChildNodeIterator>> _stack;

	using Predicate = bool (*)(const Node*);
	/// Dive predicate, an additional condition required to consider diving inside node's children
	Predicate _divePredicate{nullptr};

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
	using shared_pointer = typename Super::shared_pointer;

	RecursiveNodeIteratorImpl() {}
	RecursiveNodeIteratorImpl(typename Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}
	RecursiveNodeIteratorImpl(Predicate divePredicate, typename Super::Container nodes, std::size_t idx)
	    : _divePredicate(divePredicate), Super(nodes, idx)
	{}

	// Recursive access
	reference dereference() const
	{
		if (_stack.empty())
			return Super::dereference();
		else
			return static_cast<reference>(_stack.top().dereference());
	}
	pointer ptr() const
	{
		if (_stack.empty())
			return Super::ptr();
		else
			return static_cast<pointer>(_stack.top().ptr());
	}
	shared_pointer sharedPtr() const
	{
		if (_stack.empty())
			return Super::sharedPtr();
		else
			return std::static_pointer_cast<value_type>(_stack.top().sharedPtr());
	}
	/// Returns the base Node* pointer that isn't casted to the derived NodeType
	Node* basePtr()
	{
		if (_stack.empty())
			return ((*this->_nodes)[this->_idx]).get();
		else
			return _stack.top().ptr();
	}

	bool equals(const Iterator& b) const
	{
		std::size_t size = _stack.size();
		if (size <= 0)
		{
			return Super::equals(b);
		}
		else
		{
			std::size_t bSize = b._stack.size();
			if (size != bSize)
				return false;
			return _stack.top() == b._stack.top();
		}
	}

	void next();
};

/**
 * A complex iterator that iterates over all top level nodes as well as any child nodes that individual nodes contain.
 * Child nodes of a node are specified by the INodeContainer::getNodes() method which container nodes must implement.
 * For simplicity this iterator can only go forward and does not provide random access.
 *
 * @tparam NodeType The node type returned nodes are casted to. It <b>must be ensured</b> that the passed node list
 * only contains nodes of this subtype.
 * Otherwise an incompatible node type gets downcasted and undefined behavior will occur (static cast is used).
 * @see RecursiveNodeRange
 */
template <typename NodeType = Node>
class RecursiveNodeIterator : public RecursiveNodeIteratorImpl<RecursiveNodeIterator<NodeType>, NodeType>
{
protected:
	using Super = RecursiveNodeIteratorImpl<RecursiveNodeIterator, NodeType>;

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
	using shared_pointer = typename Super::shared_pointer;

	RecursiveNodeIterator() {}
	RecursiveNodeIterator(typename Super::Container nodes, std::size_t idx) : Super(nodes, idx) {}
};

/**
 * A wrapper around the RecursiveNodeIterator that also filters its output.
 * Like the regular recursive node iterator it is a forward iterator that cannot go back.
 *
 * @warning This iterator iterates to the first valid position immediately on construction.
 * It will iterate over the whole underlying node list if no node matches its filter!
 *
 * @tparam NodeType The node type returned nodes are casted to. It <b>must be ensured</b> that the passed node list
 * only contains nodes of this subtype or the filtering condition is only passed for this subtype.
 * Otherwise an incompatible node type gets downcasted and undefined behavior will occur (static cast is used).
 * @see RecursiveNodeIterator, FilteredRecursiveNodeRange
 */
template <typename NodeType = Node>
class FilteredRecursiveNodeIterator
    : public RecursiveNodeIteratorImpl<FilteredRecursiveNodeIterator<NodeType>, NodeType>
{
protected:
	using Super = RecursiveNodeIteratorImpl<FilteredRecursiveNodeIterator<NodeType>, NodeType>;
	using Iterator = FilteredRecursiveNodeIterator<NodeType>;
	using Predicate = bool (*)(const Node*);
	Predicate _predicate;

public:
	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
	using shared_pointer = typename Super::shared_pointer;

	FilteredRecursiveNodeIterator() {}
	FilteredRecursiveNodeIterator(Predicate predicate, typename Super::Container nodes, std::size_t idx)
	    : FilteredRecursiveNodeIterator(predicate, nullptr, nodes, idx)
	{}
	FilteredRecursiveNodeIterator(Predicate predicate, Predicate divePredicate, typename Super::Container nodes,
	                              std::size_t idx)
	    : _predicate(predicate), Super(divePredicate, nodes, idx)
	{
		if (this->_idx < this->_nodes->size() && !valid())
			next(); // The iterator must ensure that it's initial position is valid
	}
	// clang-format off
	void next() { do { Super::next(); } while (this->_idx < this->_nodes->size() && !_predicate(this->basePtr())); }
	bool valid() const { return Super::valid() && _predicate(Super::ptr()); }
	// clang-format on
};

// template <typename NodeType>
// class RNodeIterator : public RecursiveNodeIteratorImpl<RNodeIterator<NodeType>, false, NodeType>
//{
//	using Super = RecursiveNodeIteratorImpl<RNodeIterator<NodeType>, false, NodeType>;
//
// public:
//	DIWNE_INHERIT_ITERATOR_TRAITS_ALL(Super)
//
//	RNodeIterator() {}
//	RNodeIterator(NodeList* nodes, std::size_t idx) : Super(nodes, idx) {}
// };

// Node ranges
// ============================================================================================================
// Simple container-like objects that expose begim/end methods that create node iterators

/**
 * Base type for the NodeRange objects, it uses CRTP static polymorphism to avoid code duplication in node range
 * implementations.
 * @tparam Range CRTP derived range type
 * @tparam Iterator The range iterator
 * @tparam IsConst Whether the range is const or not
 */
template <typename Range, typename Iterator, typename NodeType, bool IsConst>
class NodeRangeBase
{
public:
	using Container = typename Iterator::Container;

protected:
	Container _nodes;

	NodeRangeBase(Container nodes) : _nodes(nodes) {}
	// clang-format off
	Range* self() { return static_cast<Range*>(this); } ///< CRTP self pointer (pointer to the derived type)
public:
	/**
	 * Get a node iterator pointing to the beginning of this node range.
	 */
	Iterator begin() { return self()->begin(); }
	/**
	 * Get a node iterator pointing to an element following the last element of this node range.
	 */
	Iterator end() { return self()->end(); }
//	template <bool D = IsConst, typename = std::enable_if_t<D>>
//	Iterator begin() const { return self()->begin(); }
//	template <bool D = IsConst, typename = std::enable_if_t<D>>
//	Iterator end() const { return self()->end(); }
	// clang-format on
	/**
	 * Collect all elements in the range and return them as a std::vector of owning shared pointers.
	 * This operation iterates over the underlying node iterator and collects all it's elements.
	 * @see collectRaw()
	 */
	std::vector<std::shared_ptr<NodeType>> collect()
	{
		static_assert(std::is_same<std::shared_ptr<NodeType>, typename Iterator::shared_pointer>::value);
		std::vector<std::shared_ptr<NodeType>> out;
		for (auto it = begin(); it != end(); ++it)
		{
			out.push_back(it.sharedPtr());
		}
		return out;
	}

	/**
	 * Collect all elements in the range and return them as a std::vector of non-owning raw pointers.
	 * This operation iterates over the underlying node iterator and collects all it's elements.
	 * @see collect()
	 */
	std::vector<NodeType*> collectRaw()
	{
		static_assert(std::is_same<NodeType*, typename Iterator::pointer>::value);
		std::vector<NodeType*> out;
		for (auto it = begin(); it != end(); ++it)
		{
			out.push_back(it.operator->());
		}
		return out;
	}
};

template <typename Range, typename Iterator, typename NodeType, bool IsConst>
class NodeRangeImpl : public NodeRangeBase<Range, Iterator, NodeType, IsConst>
{
	using Super = NodeRangeBase<Range, Iterator, NodeType, IsConst>;

public:
	NodeRangeImpl(typename Super::Container nodes) : Super(nodes) {}
	// clang-format off
	Iterator begin() { return Iterator(this->_nodes, 0); }
	Iterator end() { return Iterator(this->_nodes, this->_nodes->size()); }
	// clang-format on
};

template <typename Range, typename Iterator, typename NodeType, bool IsConst>
class FilteredNodeRangeImpl : public NodeRangeBase<Range, Iterator, NodeType, IsConst>
{
protected:
	using Super = NodeRangeBase<Range, Iterator, NodeType, IsConst>;
	using Predicate = bool (*)(const Node*);
	Predicate _predicate;

public:
	FilteredNodeRangeImpl(Predicate predicate, typename Super::Container nodes) : _predicate(predicate), Super(nodes) {}
	// clang-format off
	Iterator begin() { return Iterator(_predicate, this->_nodes, 0); }
	Iterator end() { return Iterator(_predicate, this->_nodes, this->_nodes->size()); }
	// clang-format on
};

template <typename Range, typename Iterator, typename NodeType, bool IsConst>
class FilteredRecursiveNodeRangeImpl : public FilteredNodeRangeImpl<Range, Iterator, NodeType, IsConst>
{
protected:
	using Super = FilteredNodeRangeImpl<Range, Iterator, NodeType, IsConst>;
	typename Super::Predicate _divePredicate{nullptr};

public:
	FilteredRecursiveNodeRangeImpl(typename Super::Predicate predicate, typename Super::Container nodes)
	    : Super(predicate, nodes)
	{}
	FilteredRecursiveNodeRangeImpl(typename Super::Predicate predicate, typename Super::Predicate divePredicate,
	                               typename Super::Container nodes)
	    : _divePredicate(divePredicate), Super(predicate, nodes)
	{}
	// clang-format off
	Iterator begin() { return Iterator(this->_predicate, _divePredicate, this->_nodes, 0); }
	Iterator end() { return Iterator(this->_predicate, _divePredicate, this->_nodes, this->_nodes->size()); }
	// clang-format on
};

// TODO: Move or copy the iterator docs over here too
template <typename NodeType>
class NodeRange : public NodeRangeImpl<NodeRange<NodeType>, NodeIterator<NodeType>, NodeType, false>
{
public:
	NodeRange(const NodeList* nodes)
	    : NodeRangeImpl<NodeRange<NodeType>, NodeIterator<NodeType>, NodeType, false>(nodes)
	{}
};

template <typename NodeType = Node>
class ConstNodeRange : public NodeRangeImpl<ConstNodeRange<NodeType>, ConstNodeIterator<NodeType>, NodeType, true>
{
public:
	ConstNodeRange(const NodeList* nodes)
	    : NodeRangeImpl<ConstNodeRange<NodeType>, ConstNodeIterator<NodeType>, NodeType, true>(nodes)
	{}
};

template <typename NodeType = Node>
class FilteredNodeRange
    : public FilteredNodeRangeImpl<FilteredNodeRange<NodeType>, FilteredNodeIterator<NodeType>, NodeType, false>
{
	using Super = FilteredNodeRangeImpl<FilteredNodeRange<NodeType>, FilteredNodeIterator<NodeType>, NodeType, false>;

public:
	FilteredNodeRange(typename Super::Predicate predicate, const NodeList* nodes) : Super(predicate, nodes) {}
};

template <typename NodeType = Node>
class RecursiveNodeRange
    : public NodeRangeImpl<RecursiveNodeRange<NodeType>, RecursiveNodeIterator<NodeType>, NodeType, false>
{
	using Super = NodeRangeImpl<RecursiveNodeRange<NodeType>, RecursiveNodeIterator<NodeType>, NodeType, false>;

public:
	RecursiveNodeRange(const NodeList* nodes) : Super(nodes) {}
};

template <typename NodeType = Node>
class FilteredRecursiveNodeRange
    : public FilteredRecursiveNodeRangeImpl<FilteredRecursiveNodeRange<NodeType>,
                                            FilteredRecursiveNodeIterator<NodeType>, NodeType, false>
{
	using Super = FilteredRecursiveNodeRangeImpl<FilteredRecursiveNodeRange<NodeType>,
	                                             FilteredRecursiveNodeIterator<NodeType>, NodeType, false>;

public:
	FilteredRecursiveNodeRange(typename Super::Predicate predicate, const NodeList* nodes) : Super(predicate, nodes) {}
	FilteredRecursiveNodeRange(typename Super::Predicate predicate, typename Super::Predicate divePredicate,
	                           const NodeList* nodes)
	    : Super(predicate, divePredicate, nodes)
	{}
};

// RecursiveNodeIteratorImpl next() impl must be here for NodeRange to not be incomplete
template <typename Iterator, typename NodeType>
void RecursiveNodeIteratorImpl<Iterator, NodeType>::next()
{
	bool topLevel = _stack.empty(); // Whether iterating over the top level nodes or not
	Node* current;
	if (topLevel)
		current = this->basePtr();
	else
		current = _stack.top().operator->();

	// Test whether this node meets conditions for diving in
	if (_divePredicate == nullptr || _divePredicate(current))
	{
		// TODO: Diwne node flags could be used to mark NodeContainers instead of using dynamic cast.
		if (auto container = dynamic_cast<const INodeContainer*>(current))
		{
			// If the node is a node container we dive deeper and iterate over child nodes
			// Note: If dynamic_cast proves to be a performance issue some sort of a tagging system can be used
			ChildNodeRange range = container->getNodes();
			auto it = range.begin();
			if (it.valid())
			{
				// Pushing the container node iterator on the stack
				_stack.push(it);
				// When the iterator is pushed it should point at its first node so no need to advance
				return;
			}
		}
	}

	// Not a container, just keep iterating over the current depth level
	if (topLevel)
	{
		Super::next();
	}
	else
	{
		ChildNodeIterator* top = &_stack.top();
		top->next();
		// When a child iterator ends, we ascend back up to the next first valid iterator
		// (either another iterator in the stack or the top level)
		while (!top->valid() && !_stack.empty())
		{
			_stack.pop();
			if (_stack.empty())
			{ // We've resurfaced to the top level
				Super::next();
			}
			else
			{ // We iterate the parent child iterator
				top = &_stack.top();
				top->next();
			}
		}
	}
}

} // namespace DIWNE
