#ifndef NIGHT_FS_H
#define NIGHT_FS_H

#include <libk/types.h>

#define IS_DIRECTORY(c) ((c->flags >> 3) & 1)
#define IS_READONLY(c) ((c->flags >> 3) & 1)

struct fs_node;

typedef size_t (*fs_read_t)  ( size_t, u8*, struct fs_node* );
typedef size_t (*fs_write_t) ( size_t, u8*, struct fs_node* );
typedef int    (*fs_open_t)  ( struct fs_node* );
typedef int    (*fs_close_t) ( struct fs_node* );

struct fs_node {
  char* name;
  u8 flags;
  size_t sz;
  fs_read_t read;
  fs_write_t write;
  fs_open_t open;
  fs_close_t close;
  struct fs_node* contents;
  struct fs_node* nxt;
};

typedef struct fs_node fs_node;

size_t fs_read(fs_node* nd, size_t len, u8* buf);
size_t fs_write(fs_node* nd, size_t len, u8* buf);

fs_node* get_file(char* path);
fs_node* make_file(char* name, u8 flags, size_t sz, fs_node* contents, fs_node* nxt);

void init_fs();

#endif // NIGHT_FS_H