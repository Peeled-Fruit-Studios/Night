
/* drivers/timer.c - PIT Driver for Night Kernel.
 *  Copyright (c) 2020 Peeled Fruit Studios and Others. All Rights Reserved.
 *  See LICENSE for more information */
#include <drivers/timer.h>

/* This will keep track of how many ticks that the system
 *  has been running for */
int timer_ticks = 0;

unsigned int secs;

void timer_handler(struct regs *r) {
  /* Increment our 'tick count' */
  timer_ticks++;

  if (timer_ticks % 18 == 0) { secs++; }
}

/* This will continuously loop until the given time has
 *  been reached */
void sleepP(int ticks) {
  unsigned long eticks;

  eticks = timer_ticks + ticks;
  while (timer_ticks < eticks);
}

unsigned int getUptime() { return secs; }

void sleep(unsigned int sec) {
  unsigned int to_wait = secs + sec;
  while (1) {
    if (to_wait == secs) { return; }
  }
}

void timer_install() {
  /* Installs 'timer_handler' to IRQ0 */
  secs = 0;
  irq_install_handler(0, timer_handler);
}
