#ifndef NIGHT_FS_H
#define NIGHT_FS_H

#include <libk/types.h>

#define IS_DIRECTORY(c) ((c->flags >> 3) & 1)
#define IS_READONLY(c) ((c->flags >> 3) & 1)

struct fs_node {
  char* name;
  u8 flags;
  size_t sz;
  size_t (*read)  ( size_t, u8*, struct fs_node* );
  size_t (*write) ( size_t, u8*, struct fs_node* );
  int    (*open)  ( struct fs_node* );
  int    (*close) ( struct fs_node* );
  struct fs_node* contents;
  struct fs_node* nxt;
};

typedef struct fs_node fs_node;

int fs_read(fs_node* nd, size_t len, u8* buf);
int fs_write(fs_node* nd, size_t len, u8* buf);

fs_node* get_file(char* path);
fs_node* make_file(char* name, u8 flags, size_t sz, fs_node* contents, fs_node* nxt);

void init_fs();

#endif // NIGHT_FS_H