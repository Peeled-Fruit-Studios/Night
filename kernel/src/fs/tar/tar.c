#include <fs/tar/tar.h>

tar_h* files[MAX_RAMDISK_FILES];
fs_node* tar_root;
extern fs_node* fs_find(char* pth, fs_node* nd);

static size_t oct2bin(unsigned char *str, int size) {
    size_t n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

void* get_addr_ofsec(void* section) {
  u32 addr = (u32)section;
  addr += 512;
  return (void*)addr;
}

size_t tar_read(size_t length, u8* buffer, fs_node* nd) {
  puts("I was Called!\n");
  int sectr = *((int*)nd->contents);
  u8* addr = (u8*)get_addr_ofsec(files[sectr]);
  memcpy(buffer, addr, length);
  return length;
}

size_t tar_write(size_t length, u8* buffer, fs_node* nd) {
  return 0;
}


static void mkfile(char* name, u32 size, int sec) {
  // printf("First Char %c\n", *((u8*)get_addr_ofsec(files[sec])));
  int* sect = kmalloc(sizeof(int));
  *sect = sec;
  fs_node* to_add = make_file(name, 0, (size_t)size, (void*)sect, NULL);
  to_add->open = NULL;
  to_add->close = NULL;
  to_add->read = &tar_read;
  to_add->write = &tar_write;
  add_dir(tar_root, to_add);
}

static u32 parse(u32 address) {
  u32 i;
 
  for (i = 0; ; i++) {
    tar_h *header = (tar_h*)address;
 
    if (header->name[0] == '\0')
      break;
 
    size_t size = oct2bin(header->size, 11);
 
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
  tar_root->open = tar_root->close = tar_root->read = NULL;
  tar_root->write = NULL;
}

fs_node* init_tarfs(void* loc) {
  init_tar_root();
  parse((u32)loc);
  return tar_root;
}