#pragma once

//
//  Copyright (C) 2020 Pharap (@Pharap)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

// For std::size_t, std::ptrdiff_t
#include <cstddef>

// For std::move
#include <utility>

// For std::array
#include <array>

// For std::assert
#include <cassert>

// For std::reverse_iterator, std::bidirectional_iterator_tag
#include <iterator>


template<typename Type, std::size_t capacity>
class circular_deque;

template<typename Type, std::size_t capacity>
class circular_deque_iterator;


template<typename Type, std::size_t capacity_value>
class circular_deque
{
public:
	static_assert(capacity_value > 1, "Attempt to instantiate circular_deque with a capacity less than 2");

	friend class circular_deque_iterator<Type, capacity_value>;

public:
	using value_type = Type;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using iterator = circular_deque_iterator<value_type, capacity_value>;
	using const_iterator = circular_deque_iterator<const value_type, capacity_value>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	static constexpr size_type capacity = capacity_value;
	
private:
	static constexpr size_type first_index = 0;
	static constexpr size_type last_index = (capacity - 1);
	static constexpr size_type initial_back_index = (capacity / 2);
	static constexpr size_type initial_front_index = ((capacity / 2) - 1);

private:
	size_type count = 0;
	size_type back_index = initial_back_index;
	size_type front_index = initial_front_index;
	std::array<value_type, capacity_value> array {};

private:
	constexpr size_type previous_back_index() const
	{
		return previous_back_index(this->back_index);
	}

	constexpr size_type next_back_index() const
	{
		return next_back_index(this->back_index);
	}

	constexpr size_type previous_front_index() const
	{
		return previous_front_index(this->front_index);
	}

	constexpr size_type next_front_index() const
	{
		return next_front_index(this->front_index);
	}

	constexpr size_type begin_index() const
	{
		return this->previous_front_index();
	}

	constexpr size_type end_index() const
	{
		return this->back_index;
	}

public:
	constexpr circular_deque() = default;

	 // O(1)
	constexpr bool empty() const
	{
		return (this->size() == 0);
	}
	
	// O(1)
	constexpr bool full() const
	{
		return (this->size() == this->max_size());
	}

	// O(1)
	constexpr size_type size() const
	{
		return this->count;
	}

	// O(1)
	constexpr size_type max_size() const
	{
		return capacity;
	}

	// O(1)
	pointer data()
	{
		return this->array.data();
	}

	// O(1)
	constexpr const_pointer data() const
	{
		return this->array.data();
	}
	
	// O(1)
	reference back()
	{
		assert(!this->empty());
		return this->array[this->previous_back_index()];
	}
	
	// O(1)
	constexpr const_reference back() const
	{
		assert(!this->empty());
		return this->array[this->previous_back_index()];
	}

	// O(1)
	reference front()
	{
		assert(!this->empty());
		return this->array[this->previous_front_index()];
	}

	// O(1)
	constexpr const_reference front() const
	{
		assert(!this->empty());
		return this->array[this->previous_front_index()];
	}

	// O(1)
	iterator begin()
	{
		return iterator(*this, this->begin_index());
	}

	// O(1)
	constexpr const_iterator begin() const
	{
		return const_iterator(*this, this->begin_index());
	}

	// O(1)
	constexpr const_iterator cbegin() const
	{
		return const_iterator(*this, this->begin_index());
	}

	// O(1)
	iterator end()
	{
		return iterator(*this, this->end_index());
	}

	// O(1)
	constexpr const_iterator end() const
	{
		return const_iterator(*this, this->end_index());
	}

	// O(1)
	constexpr const_iterator cend() const
	{
		return const_iterator(*this, this->end_index());
	}

	// O(1)
	reverse_iterator rbegin()
	{
		return reverse_iterator(iterator(*this, this->begin_index()));
	}

	// O(1)
	constexpr const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(const_iterator(*this, this->begin_index()));
	}

	// O(1)
	constexpr const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(const_iterator(*this, this->begin_index()));
	}

	// O(1)
	reverse_iterator rend()
	{
		return reverse_iterator(iterator(*this, this->end_index()));
	}

	// O(1)
	constexpr const_reverse_iterator rend() const
	{
		return const_reverse_iterator(const_iterator(*this, this->end_index()));
	}

	// O(1)
	constexpr const_reverse_iterator crend() const
	{
		return const_reverse_iterator(const_iterator(*this, this->end_index()));
	}

	// O(1)
	void push_back(const value_type & value)
	{
		// Ensure the deque isn't full
		assert(!this->full());

		// Copy the value into the underlying array
		this->array[this->back_index] = value;

		// Move the back index forwards
		this->back_index = this->next_back_index();
			
		// Increase the object counter
		++this->count;
	}

	// O(1)
	void push_back(value_type && value)
	{
		// Ensure the deque isn't full
		assert(!this->full());

		// Move the value into the underlying array
		this->array[this->back_index] = std::move(value);

		// Move the back index forwards
		this->back_index = this->next_back_index();
			
		// Increase the object counter
		++this->count;
	}
	
	// O(1)
	void push_front(const value_type & value)
	{
		// Ensure the deque isn't full
		assert(!this->full());

		// Copy the value into the underlying array
		this->array[this->front_index] = value;

		// Move the front index backwards
		this->front_index = this->next_front_index();
			
		// Increase the object counter
		++this->count;
	}
	
	// O(1)
	void push_front(value_type && value)
	{
		// Ensure the deque isn't full
		assert(!this->full());

		// Copy the value into the underlying array
		this->array[this->front_index] = std::move(value);

		// Move the front index backwards
		this->front_index = this->next_front_index();
			
		// Increase the object counter
		++this->count;
	}
	
	// O(1)
	void pop_back()
	{
		// Ensure the deque isn't empty
		assert(!this->empty());
		
		// Destroy the object at the back
		this->array[this->back_index].~value_type();
		
		// Move the back index backwards
		this->back_index = this->previous_back_index();
		
		// Decrease the object counter
		--this->count;
	}
	
	// O(1)
	void pop_front()
	{
		// Ensure the deque isn't empty
		assert(!this->empty());
		
		// Destroy the object at the front
		this->array[this->front_index].~value_type();
		
		// Move the front index forwards
		this->front_index = this->previous_front_index();
		
		// Decrease the object counter
		--this->count;
	}
	
	// O(n)
	void clear()
	{
		// If the list isn't already clear
		if (this->count > 0)
		{
			// If the front index is less than the back index
			if (this->front_index < this->back_index)
			{
				// The array objects can be deleted linearly
				for (size_type index = this->front_index; index < this->back_index; ++index)
					this->array[index].~value_type();
			}
			// Otherwise, if the indices have swapped around
			else
			{
				// Delete the objects at the front first
				for (size_type index = this->front_index; index < capacity; ++index)
					this->array[index].~value_type();
					
				// Then the objects at the back
				for (size_type index = first_index; index < this->back_index; ++index)
					this->array[index].~value_type();
			}
			
			// Reset the object counter to zero
			this->count = 0;
		}

		// Either way, return the indices to their optimal positions
		this->back_index = initial_back_index;
		this->front_index = initial_front_index;
	}

	// O(n)
	// Note:
	// This may be removed if it's not found to be
	// more efficient than using iterator algorithms.
	bool contains(const value_type & value) const
	{
		// If the deque is empty, it can't contain value
		if (this->empty())
			return false;

		// If the front index is less than the back index
		if (this->front_index < this->back_index)
		{
			// A linear search can be done
			for (size_type index = this->front_index; index < this->back_index; ++index)
				if (this->array[index] == value)
					return true;
					
			return false;
		}		
		// Otherwise, if the indices have swapped around
		else
		{
			// Search the front first
			for (size_type index = this->front_index; index < capacity; ++index)
				if (this->array[i] == value)
					return true;
			
			// Then the back
			for (size_type index = first_index; index < this->back_index; ++index)
				if (this->array[i] == value)
					return true;
					
			return false;
		}
	}

private:
	static constexpr size_type previous_back_index(size_type back_index)
	{
		return (back_index > first_index) ? (back_index - 1) : last_index;
	}

	static constexpr size_type next_back_index(size_type back_index)
	{
		return (back_index < last_index) ? (back_index + 1) : first_index;
	}

	static constexpr size_type previous_front_index(size_type front_index)
	{
		return (front_index < last_index) ? (front_index + 1) : first_index;
	}

	static constexpr size_type next_front_index(size_type front_index)
	{
		return (front_index > first_index) ? (front_index - 1) : last_index;
	}
};


template<typename Type, std::size_t capacity_value>
class circular_deque_iterator
{
private:
	friend class circular_deque<Type, capacity_value>;

private:
	using circular_deque = circular_deque<Type, capacity_value>;
	using size_type = typename circular_deque::size_type;

public:
	using difference_type = typename circular_deque::difference_type;
	using value_type = typename circular_deque::size_type;
	using pointer = typename circular_deque::pointer;
	using const_pointer = typename circular_deque::const_pointer;
	using reference = typename circular_deque::reference;
	using const_reference = typename circular_deque::const_reference;
	using iterator_category = std::bidirectional_iterator_tag;

private:
	circular_deque * owner = nullptr;
	size_type index = 0;

	explicit constexpr circular_deque_iterator(circular_deque & owner, size_type index) :
		owner { &owner }, index { index }
	{
	}

public:
	// Must have a default constructor to meet the requirements of forward iterator
	constexpr circular_deque_iterator() = default;

	reference operator *()
	{
		return this->owner->array[this->index];
	}

	constexpr const_reference operator *() const
	{
		return this->owner->array[this->index];
	}

	pointer operator ->()
	{
		return &this->owner->array[this->index];
	}

	constexpr const_pointer operator ->() const
	{
		return &this->owner->array[this->index];
	}

	circular_deque_iterator & operator ++()
	{
		this->index = circular_deque::next_back_index(this->index);
		return *this;
	}

	circular_deque_iterator operator ++(int)
	{
		auto temporary = *this;
		this->operator++();
		return temporary;
	}

	circular_deque_iterator & operator --()
	{
		this->index = circular_deque::previous_back_index(this->index);
		return *this;
	}

	circular_deque_iterator operator --(int)
	{
		auto temporary = *this;
		this->operator--();
		return temporary;
	}

	constexpr bool operator ==(circular_deque_iterator & other) const
	{
		// Two iterators are only equal if they refer to the same index in the same deque
		return (this->index == other.index) && (this->owner == other.owner);
	}

	constexpr bool operator !=(circular_deque_iterator & other) const
	{
		return (this->index != other.index) || (this->owner != other.owner);
	}
};