#ifndef NIGHT_MAIN_H
#define NIGHT_MAIN_H

// Drivers
#include <drivers/kb.h>
#include <drivers/scrn.h>
#include <drivers/timer.h>

// CPU Startup Code
#include <cpu/irq.h>
#include <cpu/isr.h>
#include <cpu/tables.h>

// Memory Management
#include <mm/paging.h>
#include <mm/pmm.h>

// Filesystems
#include <fs/fs.h>
#include <fs/tar/tar.h>


// Multiboot
#include <multiboot.h>

#endif