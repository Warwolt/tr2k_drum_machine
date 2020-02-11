/*
****************************************************************************************************
* Brief	: implementation of new and delete operators
****************************************************************************************************
*/

#include "newdelete.h"

void* operator new(size_t objsize)
{
    return malloc(objsize);
}

// this is the "placement new" operator
void *operator new(size_t objsize, void *ptr)
{
	return ptr;
}

// this prototype needed by the ETL library
void *operator new(size_t objsize, unsigned char *ptr)
{
	return ptr;
}

void operator delete(void* obj)
{
    free(obj);
}

/* This is an error function that is invoked if a pure function is called */
extern "C" void __cxa_pure_virtual()
{
	while (1);
}
