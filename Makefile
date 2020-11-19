include kernel/Kernel.mk

all: kernel

kernel:
	cd kernel
	make -f Kernel.mk kernel_real

.PHONY: clean

clean: 
	cd kernel
	make -f kernel.mk kernel_clean


