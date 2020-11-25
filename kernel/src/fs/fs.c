/* fs/fs.c - Filesystem for the Night Kernel.
*  Copyright (c) 2020 Peeled Fruit Studios and Others. All Rights Reserved.
*  See LICENSE for more information. */

#include <fs/fs.h>

fs_node* root = 0;

size_t read(fs_node* nd, size_t length, size_t offset, u8* buffer) {
  if (nd->read != 0) {
    return nd->read(nd, length, offset, buffer);
  } else {
    return 0;
  }
}

size_t write(fs_node* nd, size_t length, size_t offset, u8* buffer) {
  if (nd->write != 0) {
    return nd->write(nd, length, offset, buffer);
  } else {
    return 0;
  }
}


void open(fs_node* nd) {
  if (nd->open != 0) {
    nd->open(nd);
  } else {
    return;
  }
}

void close(fs_node* nd) {
  if (nd->close != 0) {
    nd->close(nd);
  } else {
    return;
  }
}

dir* read_dir(fs_node* nd) {
  if ((nd->flags & 0xFF) == FS_DIR && nd->read_dir != 0) {
    return nd->read_dir(nd);
  } else {
    return NULL;
  }
}

fs_node* find_dir(fs_node* nd, char* name) {
  if ((nd->flags & 0xFF) == FS_DIR && nd->find_dir != 0) {
    return nd->find_dir(nd, name);
  } else {
    return NULL;
  }
}
