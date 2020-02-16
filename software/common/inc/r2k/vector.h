/*
****************************************************************************************************
* brief : Minimal implementation of an std::vector equivalent using static allocation.
* note  : This class will grow in features as they become needed in this project. (YAGNI).
****************************************************************************************************
*/

#ifndef R2K_VECTOR_H
#define R2K_VECTOR_H

#include "r2k/ivector.h"
#include <string.h>

namespace r2k
{

template<class T, size_t CAPACITY>
class vector : public ivector<T>
{
public:
	using value_type = T;
	using iterator = T*;
	using reference = T&;
	using const_reference = const T&;

	/* Constructors */
	vector() = default;
	vector(std::initializer_list<T> list)
	{
		num_elements = (CAPACITY < list.size() ? CAPACITY : list.size());
		memcpy(array, list.begin(), num_elements);
	}

	/* Iterators */
	iterator begin() final
	{
		return array;
	}

	iterator end() final
	{
		return array + num_elements;
	}

	/* Capacity */
	size_t size() const final
	{
		return num_elements;
	}

	size_t capacity() const final
	{
		return CAPACITY;
	}

	void resize(size_t new_size) final
	{
		if (new_size <= CAPACITY)
		{
			num_elements = new_size;
		};
	}

	bool empty() const final
	{
		return num_elements == 0;
	}

	/* Element access */
	reference operator[](size_t index) final
	{
		return array[index];
	}

	reference back() final
	{
		return array[num_elements-1];
	}

	/* Modifiers */
	void push_back(const_reference val) final
	{
		if (num_elements < CAPACITY)
		{
			array[num_elements++] = val;
		}
	}

	void pop_back() final
	{
		num_elements--;
	}

private:
	T array[CAPACITY];
	size_t num_elements = 0;
};

} /* namespace r2k */

#endif /* R2K_VECTOR_H */
