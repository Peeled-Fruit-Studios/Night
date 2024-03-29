CFLAGS = -fno-PIC -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -nostdlib
CFLAGS += -isystem ../include/libk

objs = obj/memory.o \
       obj/panic.o \
       obj/string.o

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc $(CFLAGS) -c -o $@ $<

libk_real: $(objs)
	ar rcs libk.a $(objs)

.PHONY: libk_clean

libk_clean:
	rm -rf libk.a obj
