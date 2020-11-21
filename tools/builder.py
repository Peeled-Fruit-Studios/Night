# Night Kernel Build Script

import os
import sys

def build():
  os.system("cd lib/libk && make -f libk.mk libk_real")
  os.system("cd -")
  os.system("mv lib/libk/libk.a kernel")
  os.system("cd kernel && make -f Kernel.mk kernel_real")

def clean():
  os.system("cd kernel && make -f Kernel.mk kernel_clean")
  os.system("cd -")
  os.system("cd lib/libk && make -f libk.mk libk_clean")

if len(sys.argv) == 2 and sys.argv[1] == "clean":
  clean()
elif len(sys.argv) == 2 and sys.argv[1] == "build":
  build()