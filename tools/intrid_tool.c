#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

FILE* holdr;
int fsize;

void writeHDR(FILE* intr, int fcr) {
  intrid_h hdr = { .magic = MAGIC, .abi_ma = 1, .abi_mi = 5, .fc = fcr };
  fwrite(&hdr.magic, sizeof(unsigned char), 1, intr);
  fwrite(&hdr.abi_ma, sizeof(int), 1, intr);
  fwrite(&hdr.abi_mi, sizeof(int), 1, intr);
  fwrite(&hdr.fc, sizeof(int), 1, intr);
}

void build_file(FILE* intr, char* filename) {
  holdr = fopen(filename, "r");
  fseek(holdr, 0L, SEEK_END);
  fsize = ftell(holdr);
  fseek(holdr, 0L, SEEK_SET);
  intrid_file fk = { .magic = MAGIC, .size = fsize, .name = filename};
  fwrite(&fk.magic, sizeof(unsigned char), 1, intr);
  fwrite(&fk.size, sizeof(int), 1, intr);
  fwrite(&fk.name, sizeof(char) * strlen(filename), 1, intr);
  unsigned char* buffer = malloc(sizeof(unsigned char) * fsize);
  fread(buffer, sizeof(unsigned char) * fsize, 1, holdr);
  fwrite(buffer, sizeof(unsigned char) * fsize, 1, intr);
  fclose(holdr);
}

int main(int argc, char** argv) {
  printf("Night Kernel Intrid Builder for Night v0.5.0\n");
  printf("Copyright (c) 2020 Peeled Fruit Studios. All Rights Reserved.\n");
  printf("See LICENSE for more information.\n\n\n");
  FILE* intr = fopen("./start.img", "w");
  writeHDR(intr, (argc - 1));
  for(int k = 1; k < (argc); k++) {
    printf("Adding file %s to the Intrid Image.\n", argv[k]);
    build_file(intr, argv[k]);
  }
  fclose(intr);
  printf("Complete!\n");
}