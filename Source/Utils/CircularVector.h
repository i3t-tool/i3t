/**
 * \file
 * \brief
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <iostream>
#include <vector>

template <typename T>
class CircularVector
{
private:
	std::vector<T> buffer; // Internal buffer
	size_t fullCapacity;   // Maximum capacity of the buffer
	size_t actualSize;     // Number of elements currently stored
	size_t head;           // Index of the first element (head of the buffer)
	int current;

public:
	// Constructor
	explicit CircularVector(size_t cap);

	// Get the number of filled elements
	size_t size() const;

	int capacity() const;

	// Check if the buffer is empty
	bool empty() const;

	// Insert an element at the end
	void pushBack(T&& value);

	// Insert an element at a specific index, shifting elements to the right
	void insertAfterCurrent(T&& value);

	T& popCurrent();
	T& revertCurrent();
	const T& getCurrent() const;
	void truncateFromCurrent();

	// Remove an element at a specific index, shifting elements to the left
	void truncate(size_t index);

	int distanceToLast() const;

	int distanceToFirst() const;
	void replaceCurrent(T&& value);

	void clear();

	// Overload the [] operator for element access
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	size_t getCurrentIndex() const;
	size_t getSize();
};

template <typename T>
CircularVector<T>::CircularVector(size_t cap) : fullCapacity(cap), actualSize(0), head(0), current(-1)
{
	buffer.resize(fullCapacity);
}

template <typename T>
size_t CircularVector<T>::size() const
{
	return actualSize;
}

template <typename T>
int CircularVector<T>::capacity() const
{
	return fullCapacity;
}

template <typename T>
bool CircularVector<T>::empty() const
{
	return actualSize == 0;
}

template <typename T>
void CircularVector<T>::pushBack(T&& value)
{
	size_t tailIndex = (head + actualSize) % fullCapacity;
	buffer[tailIndex] = std::move(value);

	if (actualSize < fullCapacity)
	{
		++actualSize;
	}
	else
	{
		head = (head + 1) % fullCapacity;
	}
	current = (current + 1) % fullCapacity;
}

template <typename T>
void CircularVector<T>::insertAfterCurrent(T&& value)
{
	if (distanceToLast() == 0)
	{
		pushBack(std::move(value));
		return;
	}

	current = (current + 1) % fullCapacity;
	if (actualSize == 0)
	{
		head = current;
		buffer[current] = std::move(value);
		actualSize = 1;
		return;
	}

	if (actualSize == fullCapacity)
	{
		head = (head + 1) % fullCapacity;
		--actualSize;
	}

	size_t lastUsed = (head + actualSize - 1) % fullCapacity;


	size_t curr = lastUsed;
	while (true)
	{
		if (distanceToLast() < 0)
		{
			break;
		}
		size_t next = (curr + 1) % fullCapacity;
		buffer[next] = std::move(buffer[curr]);

		if (curr == current)
			break;

		curr = (curr + fullCapacity - 1) % fullCapacity;
	}

	buffer[current] = std::move(value);

	++actualSize;
}

template <typename T>
T& CircularVector<T>::popCurrent()
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}
	current = (current + fullCapacity - 1) % fullCapacity;

	return buffer[current];
}

template <typename T>
T& CircularVector<T>::revertCurrent()
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}
	current = (current + 1) % fullCapacity;

	return buffer[current];
}

template <typename T>
const T& CircularVector<T>::getCurrent() const
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}
	return buffer[current];
}

template <typename T>
void CircularVector<T>::truncateFromCurrent()
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}

	size_t offset = (current + fullCapacity - head) % fullCapacity;

	if (offset >= actualSize)
	{
		throw std::out_of_range("Physical index does not correspond to a valid (occupied) element");
	}

	actualSize = offset + 1;
}

template <typename T>
void CircularVector<T>::truncate(size_t index)
{
	index %= fullCapacity;


	size_t offset = (index + fullCapacity - head) % fullCapacity;

	if (offset >= actualSize)
	{
		throw std::out_of_range("Physical index does not correspond to a valid (occupied) element");
	}

	actualSize = offset;
}

template <typename T>
int CircularVector<T>::distanceToLast() const
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}
	size_t offset = (current + fullCapacity - head) % fullCapacity;
	if (offset >= actualSize)
	{
		throw std::out_of_range("Physical index does not correspond to a valid (occupied) element");
	}

	return (actualSize - 1) - offset;
}

template <typename T>
int CircularVector<T>::distanceToFirst() const
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}

	size_t offset = (current + fullCapacity - head) % fullCapacity;

	if (offset >= actualSize)
		throw std::out_of_range("Physical index does not correspond to a valid (occupied) element");

	return offset;
}

template <typename T>
void CircularVector<T>::replaceCurrent(T&& value)
{
	if (actualSize == 0)
	{
		throw std::out_of_range("Buffer is empty");
	}

	buffer[current] = std::move(value);
}

template <typename T>
void CircularVector<T>::clear()
{
	head = 0;
	actualSize = 0;
	current = -1;
}

template <typename T>
T& CircularVector<T>::operator[](size_t index)
{
	if (index >= actualSize)
	{
		throw std::out_of_range("Index out of bounds");
	}
	return buffer[(head + index) % fullCapacity];
}

template <typename T>
const T& CircularVector<T>::operator[](size_t index) const
{
	if (index >= actualSize)
	{
		throw std::out_of_range("Index out of bounds");
	}
	return buffer[(head + index) % fullCapacity];
}

template <typename T>
size_t CircularVector<T>::getCurrentIndex() const
{
	return current;
}

template <typename T>
size_t CircularVector<T>::getSize()
{
	return actualSize;
}
