/*
****************************************************************************************************
* @brief :  Trace printouts, for use in unit testing. Formatting is matching that of gtest prints
*
* __FILE_NAME__ macro based on https://blog.galowicz.de/2016/02/20/short_file_macro/
****************************************************************************************************
*/

#ifndef TEST_PRINT_H
#define TEST_PRINT_H

#include <stdio.h>
using cstr = const char * const;

static constexpr cstr past_last_slash(cstr str, cstr last_slash)
{
    return
        *str == '\0' ? last_slash :
        *str == '/'  ? past_last_slash(str + 1, str + 1) :
                       past_last_slash(str + 1, last_slash);
}

static constexpr cstr past_last_slash(cstr str)
{
    return past_last_slash(str, str);
}

#define __FILE_NAME__ ({constexpr cstr sf__ {past_last_slash(__FILE__)}; sf__;})
#define TRACE_PRINT() printf("[ TRACE    ] %s:%d\n", __FILE_NAME__, __LINE__)
#define INFO(x) "[   INFO   ] " x
#define INFO_PRINT(...) printf("[   INFO   ] %s:%d ", __FILE_NAME__, __LINE__); printf(__VA_ARGS__); printf("\n");
#define ERROR_MSG_STR(x) "\n\tINFO: " << x << "\n"

#endif /* TEST_PRINT_H */
