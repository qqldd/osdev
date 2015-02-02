CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

OBJS:=boot.o kernel.o
 
CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o
 
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)
 
myos.bin: $(OBJ_LINK_LIST)
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 $(OBJ_LINK_LIST) -nostdlib -lgcc

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

boot.o: boot.s
	$(AS) boot.s -o boot.o

crti.o: crti.s
	$(AS) crti.s -o crti.o

crtn.o: crtn.s
	$(AS) crtn.s -o crtn.o

clean:
	rm -f myos.bin $(INTERNAL_OBJS)