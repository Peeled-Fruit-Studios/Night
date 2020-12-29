#ifndef NIGHT_TAR_H
#define NIGHT_TAR_H

#include <fs/fs.h>

#define MAX_RAMDISK_FILES 32

typedef struct {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag[1];
} tar_h;

fs_node* init_tarfs(void* loc);

#endif // NIGHT_TAR_H