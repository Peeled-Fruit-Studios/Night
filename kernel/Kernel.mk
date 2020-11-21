CFLAGS= -fno-PIC -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I include -I ../lib/libk/include

objs = obj/kernel/kernel.o \
       obj/cpu/gdt.o \
       obj/cpu/idt.o \
       obj/cpu/irq.o \
       obj/cpu/isrs.o \
       obj/drivers/timer.o \
       obj/drivers/kb.o \
       obj/drivers/scrn.o \
       obj/kernel/start.o

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	nasm -f elf32 -o $@ $<


kernel_real: $(objs)
	ld -T link.ld -no-PIE -melf_i386 -o kernel $(objs) libk.a


kernel_clean:
	rm -rf kernel obj libk.a
