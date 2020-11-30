#ifndef NIGHT_FS_H
#define NIGHT_FS_H

#include <libk/types.h>

#define FS_FILE 0x1
#define FS_DIR 0x2
#define FS_LINK 0x3
#define FS_SYS 0x4

typedef struct {
  char name[256];
  struct fs_node* files;
  int count;
} dir;

typedef void (*open_fn)(struct fs_node*);
typedef void (*close_fn)(struct fs_node*);
typedef size_t (*read_fn)(struct fs_node*, int, int, u8*);
typedef int (*write_fn)(struct fs_node*, int, int, u8*);
typedef dir* (*read_dir_fn)(struct fs_node*);
typedef struct fs_node* (*find_dir_fn)(struct fs_node*, char*);


typedef struct {
  char fname[256];
  u16 flags;
  int size;
  int uid;
  int gid;
  int mask;
  open_fn open;
  close_fn close;
  read_fn read;
  write_fn write;
  read_dir_fn read_dir;
  find_dir_fn find_dir;
  void* link;
} fs_node;

// Small Helper function to convert void* to fs_node*
fs_node* get_link(void* ptr) {
  return (fs_node*)ptr;
}

extern fs_node* root;

// The Generic Functions for messing with a node
size_t read(fs_node* nd, size_t length, size_t offset, u8* buffer);
size_t write(fs_node* nd, size_t length, size_t offset, u8* buffer);
void open(fs_node* nd);
void close(fs_node* nd);
dir* read_dir(fs_node* nd);
fs_node* find_dir(fs_node* nd, char* name);

#endif