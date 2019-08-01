/*
****************************************************************************************************
* brief : Minimal implementation of an std::vector equivalent using static allocation.
* note  : This class will grow in features as they become needed in this project. (YAGNI).
****************************************************************************************************
*/

#ifndef R2K_VECTOR_H
#define R2K_VECTOR_H

#include "r2k/ivector.h"

namespace r2k
{
template<class T, size_t CAPACITY>
class vector : public ivector<T>
{
public:
	using value_type = T;
	using iterator = T*;
	using reference = T&;

	vector() = default;

	vector(std::initializer_list<T> list)
	{
		numElements = (CAPACITY < list.size() ? CAPACITY : list.size());
		memcpy(array, list.begin(), numElements);
	}

	size_t size() final
	{
		return numElements;
	}

	size_t capacity() final
	{
		return CAPACITY;
	}

	void push_back(const value_type& val) final
	{
		if(numElements < CAPACITY)
		{
			array[numElements++] = val;
		}
	}

	reference operator[](size_t index) final
	{
		return array[index];
	}

	iterator begin() final
	{
		return array;
	}

	iterator end() final
	{
		return array + numElements;
	}

private:
	T array[CAPACITY];
	size_t numElements = 0;
};
}

#endif /* R2K_VECTOR_H */
