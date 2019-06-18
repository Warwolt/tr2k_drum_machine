/*
****************************************************************************************************
* Brief	: Header to define new/delete operators as they aren't provided by avr-g++ by default
*         Taken from http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453
****************************************************************************************************
*/

#ifndef NEW_DELETE_OPERATIONS_H
#define NEW_DELETE_OPERATIONS_H

#include <stdlib.h>

void* operator new(size_t objsize);
void* operator new(size_t objsize, void* ptr);
void* operator new(size_t, unsigned char*);
void operator delete(void* obj);

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

extern "C" void __cxa_pure_virtual();

#endif /* NEW_DELETE_OPERATIONS_H */
