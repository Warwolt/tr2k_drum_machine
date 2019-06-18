/*
****************************************************************************************************
* brief :  Trace printouts, for use in unit testing. Formatting is matching that of gtest prints
****************************************************************************************************
*/

#ifndef TEST_PRINT_H
#define TEST_PRINT_H

#include <stdio.h>
#define TRACE_PRINT() printf("[ TRACE    ] %s:%d\n", __FILE__, __LINE__)
#define INFO(x) "[   INFO   ] " x
#define INFO_PRINT(...) printf("[   INFO   ] %s:%d ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n");
#define ERROR_MSG_STR(x) "\n\tINFO: " << x << "\n"

#endif /* TEST_PRINT_H */
