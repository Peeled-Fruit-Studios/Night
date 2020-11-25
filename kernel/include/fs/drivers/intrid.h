#ifndef NIGHT_INTRID_H
#define NIGHT_INTRID_H

#define MAGIC 0xaf

typedef struct {
  unsigned char magic;
  int abi_ma; // ABI version so kernel mismatch doesn't happen
  int abi_mi; // Minor version of ABI
  int fc; // Number of files in intrid.
} intrid_h;

typedef struct {
  unsigned char magic;
  int size;
  char* name;
} intrid_file;

fs_node* init_intrid(u32 location);

#endif