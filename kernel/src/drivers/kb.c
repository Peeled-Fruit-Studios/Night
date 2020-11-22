/* drivers/kb.c - Keyboard Driver for Night Kernel.
*  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information  */


#include <drivers/kb.h>

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r) {
  unsigned char scancode;

  /* Read from the keyboard's data buffer */
  scancode = inb(0x60);

  /* If the top bit of the byte we read from the keyboard is
   *  set, that means that a key has just been released */
  if (scancode & 0x80) {
    /* Will be implemented later hopefully */
  } else {   
    putch(kbdus[scancode]);
  }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install() { irq_install_handler(1, keyboard_handler); }
