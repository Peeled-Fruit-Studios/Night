import os
import sys

def build_itd():
  print("[INITRD] Now Building...")
  os.system("tar -cvf itd.tar %s" % (sys.argv[1]))


build_itd()
print("[INITRD] Complete!")