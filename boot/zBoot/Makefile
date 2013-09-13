



#TEST = -DTEST_DRIVER

CCPREFIX = 

CC = $(CCPREFIX)gcc -m32 -march=i386 -D__KERNEL__ -D__ASSEMBLY__
AS = $(CCPREFIX)as --32
LD = $(CCPREFIX)ld -m elf_i386

SYSTEM = ../../compile/xeros
HEAD = head.o
zOBJECTS = $(HEAD) inflate.o unzip.o misc.o
ZLDFLAGS = -e startup_32
ZIMAGE_OFFSET = 0x1000
ZLINKFLAGS = -Ttext $(ZIMAGE_OFFSET) $(ZLDFLAGS)

CFLAGS = -fno-builtin -O2 -DSTDC_HEADERS $(TEST)
OBJDUMP = $(CCPREFIX)objdump
OBJDUMP_FLAGS = -k -q
OBJCOPY = $(CCPREFIX)objcopy 
OBJCOPY_FLAGS = -O binary -R .note -R .comment -R .stab -R .stabstr
TARGET=--target elf32-i386
INPUT_DATA=input_data
INPUT_LEN=input_len

.c.s:
	$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	$(AS)  -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $<

all:	zSystem

zSystem: piggy.o $(zOBJECTS)
	$(LD) $(ZLINKFLAGS) -o zSystem  $(zOBJECTS) piggy.o

head.o: head.S 
	$(CC) -traditional -c -I/usr/include head.S 

piggy.o: $(SYSTEM)
	rm -f tmppiggy tmppiggy.gz tmppiggy.lnk; \
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(SYSTEM) tmppiggy; \
	gzip -f -9 < tmppiggy > tmppiggy.gz; \
	echo "SECTIONS { .data : { input_len = .; LONG(input_data_end - input_data) input_data = .; *(.data) input_data_end = .; }}" > tmppiggy.lnk; \
	  $(LD) -m elf_i386 -r -o piggy.o -b binary tmppiggy.gz -b elf32-i386 -T tmppiggy.lnk; \
	rm -f tmppiggy tmppiggy.gz tmppiggy.lnk

clean:
	rm -f *.o
	rm -f zSystem

