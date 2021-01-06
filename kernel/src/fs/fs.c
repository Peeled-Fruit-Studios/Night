#include <fs/fs.h>
#include <liballoc/alloc.h>
#include <libk/string.h>
#include <libk/types.h>

fs_node* root;

size_t fs_read(fs_node* nd, size_t len, u8* buf) {
  if(IS_DIRECTORY(nd) | !nd->read) {
    return 0;
  } else {
    return nd->read(len, buf, nd);
  }
}

size_t fs_write(fs_node* nd, size_t len, u8* buf) {
  if(IS_DIRECTORY(nd) | IS_READONLY(nd) | !nd->read) {
    return 0;
  } else {
    return nd->read(len, buf, nd);
  }
}

void add_dir(fs_node* dir, fs_node* file) {
  fs_node* holder;
  if(dir->contents == NULL) {
    dir->contents = file;
  } else if (dir->contents->nxt == NULL) {
    dir->contents->nxt = file;
  } else {
    holder = dir->contents->nxt;
    while(holder->nxt != NULL) {
      holder = holder->nxt;
    }
    holder->nxt = holder->nxt;
  }
  dir->sz++;
}

fs_node* get_file(char* path) {
  fs_node* rot = root->contents;
  for(int f = 0; f < 10; f++);
  char* tk;
  tk = strtok(path, "/");
  while(tk) {
    if(memcmp(rot->name, tk, strlen(tk)) == 0) {
      tk = strtok(NULL, "/");
      if(tk) {
        rot = rot->contents;
        continue;
      } else {
        // kfree(inf);
        return rot;
      }
    }
    if(rot->nxt) {
      rot = rot->nxt;
    } else {
      // kfree(inf);
      return NULL;
    }
  }
  // kfree(inf);
  return NULL;
}

fs_node* make_file(char* name, u8 flags, size_t sz, fs_node* contents, fs_node* nxt) {
  fs_node* trt = kmalloc(sizeof(fs_node));
  trt->name = name;
  trt->flags = flags;
  trt->sz = sz;
  trt->contents = contents;
  trt->nxt = nxt;
  return trt;
}

void fs_mount(fs_node* nd) {
  add_dir(root, nd);
}

void init_fs() {
  root = make_file("rt", 0, 0, NULL, NULL);
  root->flags |= 1 << 3;
}
