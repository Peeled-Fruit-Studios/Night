#ifndef NIGHT_TIMER_H
#define NIGHT_TIMER_H

#include <cpu/irq.h>

unsigned int getUptime();

void sleep(unsigned int sec);

// A More Precise Version of Sleep() 
void sleepP(int ticks);

void timer_install();

#endif