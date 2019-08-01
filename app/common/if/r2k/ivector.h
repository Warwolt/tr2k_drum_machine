/*
****************************************************************************************************
* brief : Interface for an implementation of an std::vector equivalent using static allocation.
* note  : This class will grow in features as they become needed in this project. (YAGNI).
****************************************************************************************************
*/

#ifndef R2K_INTERFACE_VECTOR_H
#define R2K_INTERFACE_VECTOR_H

#include "stlstub/initializer_list.h"

namespace r2k
{
template<class T>
class ivector
{
public:
	using iterator = T*;
	using lvalue = T&;

	virtual size_t size() = 0;
	virtual size_t capacity() = 0;
	virtual lvalue operator[](size_t index) = 0;
	virtual iterator begin() = 0;
	virtual iterator end() = 0;
};
}

#endif /* R2K_INTERFACE_VECTOR_H */
