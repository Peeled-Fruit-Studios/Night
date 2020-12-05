/* drivers/scrn.c - VGA Screen Driver for Night Kernel. 
*  Copyright (c) 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information. */

#include <drivers/scrn.h>


unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

int vga_locked = 0;
int tstop;

/* Scrolls the screen */
void scroll(void) {
  unsigned blank, temp;

  blank = 0x20 | (attrib << 8);

  if (csr_y >= 25) {
    temp = csr_y - 25 + 1;
    memcpy(textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

    memsetw(textmemptr + (25 - temp) * 80, blank, 80);
    csr_y = 25 - 1;
  }
}

void move_csr(void) {
  unsigned temp;

  /* The equation for finding the index in a linear
   *  chunk of memory can be represented by:
   *  Index = [(y * width) + x] */
  temp = csr_y * 80 + csr_x;

  outb(0x3D4, 14);
  outb(0x3D5, temp >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, temp);
}

// Prevents printing to screen
void lock_vga() {
  vga_locked = 1;
}

void unlock_vga() {
  vga_locked = 0;
}

void tab_stop() {
  tstop = textmemptr + (csr_y * 80 + csr_x);
}

/* Clears the screen */
void cls() {
  unsigned blank;
  int i;

  /* Again, we need the 'short' that will be used to
   *  represent a space with color */
  blank = 0x20 | (attrib << 8);

  /* Sets the entire screen to spaces in our current
   *  color */
  for (i = 0; i < 25; i++) memsetw(textmemptr + i * 80, blank, 80);

  /* Update out virtual cursor, and then move the
   *  hardware cursor */
  csr_x = 0;
  csr_y = 0;
  move_csr();
}

/* Puts a single character on the screen */
void putch(unsigned char c) {
  if(vga_locked) {
    return;
  }
  unsigned short *loc;
  unsigned att = attrib << 8;

  /* Handle a backspace, by moving the cursor back one space */
  if (c == 0x08) {
    loc = textmemptr + (csr_y * 80 + csr_x);
    if(loc <= tstop) { return; }
    if (csr_x != 0) csr_x--;
    loc = textmemptr + (csr_y * 80 + csr_x);
    *loc = ' ' | att;
  }
  /* Handles a tab by incrementing the cursor's x, but only
   *  to a point that will make it divisible by 8 */
  else if (c == 0x09) {
    csr_x = (csr_x + 8) & ~(8 - 1);
  }
  /* Handles a 'Carriage Return', which simply brings the
   *  cursor back to the margin */
  else if (c == '\r') {
    csr_x = 0;
  }
  /* We handle our newlines the way DOS and the BIOS do: we
   *  treat it as if a 'CR' was also there, so we bring the
   *  cursor to the margin and we increment the 'y' value */
  else if (c == '\n') {
    csr_x = 0;
    csr_y++;
  }
  /* Any character greater than and including a space, is a
   *  printable character. The equation for finding the index
   *  in a linear chunk of memory can be represented by:
   *  Index = [(y * width) + x] */
  else if (c >= ' ') {
    loc = textmemptr + (csr_y * 80 + csr_x);
    *loc = c | att; /* Character AND attributes: color */
    csr_x++;
  }

  /* If the cursor has reached the edge of the screen's width, we
   *  insert a new line in there */
  if (csr_x >= 80) {
    csr_x = 0;
    csr_y++;
  }

  scroll();
  move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text) {
  int i;

  for (i = 0; i < strlen(text); i++) { putch(text[i]); }
}

/* Sets the forecolor and backcolor that we will use */
void set_color(unsigned char forecolor, unsigned char backcolor) {
  attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void) {
  textmemptr = (unsigned short *)0xB8000;
  cls();
}
