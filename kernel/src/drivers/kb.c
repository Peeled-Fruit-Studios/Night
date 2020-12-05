/* drivers/kb.c - Keyboard Driver for Night Kernel.
*  Copyright 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information  */


#include <drivers/kb.h>

unsigned char kbdus[128] = {
  0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
  '9',  '0', '-', '=', '\b', /* Backspace */
  '\t', /* Tab */
  'q',  'w', 'e', 'r', /* 19 */
  't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n', /* Enter key */
  0, /* 29   - Control */
  'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
  '\'', '`', 0, /* Left shift */
  '\\', 'z', 'x', 'c', 'v',  'b', 'n', /* 49 */
  'm',  ',', '.', '/', 0, /* Right shift */
  '*',  0, /* Alt */
  ' ', /* Space bar */
  0, /* Caps lock */
  0, /* 59 - F1 key ... > */
  0,    0,   0,   0,   0,    0,   0,   0,   0, /* < ... F10 */
  0, /* 69 - Num lock*/
  0, /* Scroll Lock */
  0, /* Home key */
  0, /* Up Arrow */
  0, /* Page Up */
  '-',  0, /* Left Arrow */
  0,    0, /* Right Arrow */
  '+',  0, /* 79 - End key*/
  0, /* Down Arrow */
  0, /* Page Down */
  0, /* Insert Key */
  0, /* Delete Key */
  0,    0,   0,   0, /* F11 Key */
  0, /* F12 Key */
  0, /* All other keys are undefined */
};

/* If you have noticed a lot of volatile keywords in my code, it is
*  because the compiler that I am using (gcc) has a tendancy to mess up *  *  *  variables whith optimization. Volatile prevents that from happining*/

volatile char cur_char; 

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
    cur_char = kbdus[scancode];
  }
}
/* I disabled interrupts because a rare condition happens
*  where an interrupt occurs in the middle of getting the current
*  character. This may seem small, but when testing in qemu, it brought
*  down the whole entire system! */

char get_char() {
  volatile char to_ret; // See cur_char
  asm volatile ("cli");
  to_ret = cur_char;
  asm volatile ("sti");
  return to_ret;
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install() { irq_install_handler(1, keyboard_handler); }
