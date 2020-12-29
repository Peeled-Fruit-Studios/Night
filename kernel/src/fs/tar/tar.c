#include <fs/tar/tar.h>

tar_h* files[MAX_RAMDISK_FILES];
fs_node* tar_root;
extern fs_node* fs_find(char* pth, fs_node* nd);

static u32 getsize(const char *in) {
  u32 size = 0;
  u32 j;
  u32 count = 1;
 
  for (j = 11; j > 0; j--, count *= 8)
    size += ((in[j - 1] - '0') * count);
 
  return size;
}

size_t tar_read(size_t length, u8* buffer, fs_node* nd) {
  int sectr = *((int*)nd->contents);
  u8* addr = ((u8*)(files[sectr]) + 512);
  memcpy(buffer, addr, length);
  return length;
}
size_t tar_write(size_t length, u8* buffer, fs_node* nd) {
  return 0;
}


static void mkfile(char* name, u32 size, int sec) {
  int* sect = kmalloc(sizeof(int));
  *sect = sec;
  fs_node* to_add = make_file(name, 0, (size_t)size, (void*)sect, NULL);
  to_add->open = to_add->close = 0;
  to_add->read = tar_read;
  to_add->write = tar_write;
  add_dir(tar_root, to_add);
}

static u32 parse(u32 address) {
  u32 i;
 
  for (i = 0; ; i++) {
    tar_h *header = (tar_h*)address;
 
    if (header->name[0] == '\0')
      break;
 
    u32 size = getsize(header->size);
 
    files[i] = header;

    mkfile(header->name, size, i);
 
    address += ((size / 512) + 1) * 512;
 
    if (size % 512)
      address += 512;
 
  }
 
  return i;
 
}

static void init_tar_root() {
  tar_root = make_file("S:", 0, 0, NULL, NULL);
  tar_root->open = tar_root->close = tar_root->read = 0;
  tar_root->write = 0;
}

#ifdef USE_INITRD
fs_node* init_tarfs(void* loc) {
  init_tar_root();
  parse((u32)loc);
  return tar_root;
}
#endif