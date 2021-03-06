/*
****************************************************************************************************
* brief : Interface for an implementation of an std::vector equivalent using static allocation.
* note  : This class will grow in features as they become needed in this project. (YAGNI).
****************************************************************************************************
*/

#ifndef R2K_INTERFACE_VECTOR_H
#define R2K_INTERFACE_VECTOR_H

#include "stlstub/initializer_list.h"
#include <stddef.h>

namespace r2k
{

template<class T>
class ivector
{
public:
	using value_type = T;
	using iterator = T*;
	using reference = T&;
	using const_reference = const T&;

	/* Iterators */
	virtual iterator begin() = 0;
	virtual iterator end() = 0;

	/* Capacity */
	virtual size_t size() const = 0;
	virtual size_t capacity() const = 0;
	virtual void resize(size_t) = 0;
	virtual bool empty() const = 0;

	/* Element access */
	virtual reference operator[](size_t index) = 0;
	virtual reference back() = 0;

	/* Modifiers */
	virtual void push_back(const_reference val) = 0;
	virtual void pop_back() = 0;
};

} /* namespace r2k */

#endif /* R2K_INTERFACE_VECTOR_H */
