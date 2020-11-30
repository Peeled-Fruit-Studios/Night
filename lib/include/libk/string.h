#ifndef LIBK_PRINT_H
#define LIBK_PRINT_H

#include "stdarg.h"
#include "types.h"

void sprintf(char* buf, char* fmt, ...);
size_t strlen(const char* s);

#endif