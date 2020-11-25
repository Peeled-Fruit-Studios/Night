#include <fs/drivers/intrid.h>
#include <fs/fs.h>



fs_node* init_intrid(u32 location) {
  u8* magic = location;
  if(magic != 0xaf) {
    kpanic("Improper Intrid magic\n");
  }
  
}