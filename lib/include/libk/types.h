#ifndef LIBK_TYPES_H
#define LIBK_TYPES_H

typedef unsigned long size_t;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#define NULL ((void*)0)

// Makeshift Implentation of the bool type
#define bool int
#define true 1
#define false 0

#endif