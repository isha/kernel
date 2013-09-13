
CCPREFIX = 
ZIMAGE_OFFSET = 0x1000
ENCAPS = $(CCPREFIX)encaps
OBJDUMP = $(CCPREFIX)objdump
OBJDUMP_FLAGS = -k -q
OBJCOPY = $(CCPREFIX)objcopy 
OBJCOPY_FLAGS = -O binary -R .note -R .comment -R .stab -R .stabstr
HOSTCC	=gcc -m32 -march=i386 
HOSTCCFLAGS = -O2 -fomit-frame-pointer
MAKE = make


zImage:  zBoot/zSystem build ../compile/xeros
	$(OBJCOPY) $(OBJCOPY_FLAGS) zBoot/zSystem zBoot/zSystem.out; \
	./build boot/bootsect boot/setup zBoot/zSystem.out CURRENT > zImage

zdisk: zImage 
	dd bs=8192 if=zImage of=/dev/fd0

zBoot/zSystem: ../compile/xeros
	$(MAKE) -C zBoot zSystem

build: build.c
	$(HOSTCC) $(HOSTCCFLAGS) -D__BFD__ -I/usr/include -o $@ $<

AS86	=as86 --32 -0 -a
LD86	=ld86 -0
CC	=$(HOSTCC) -D__KERNEL__ -D__ASSEMBLY__
CPP	=$(CC) -E

boot/setup: boot/setup.o
	$(LD86) -s -o $@ $<

boot/setup.o: boot/setup.s
	$(AS86) -o $@ $<

boot/setup.s: boot/setup.S   Makefile
	$(CPP) -traditional $< -o $@

boot/bootsect: boot/bootsect.o
	$(LD86) -s -o $@ $<

boot/bootsect.o: boot/bootsect.s
	$(AS86) -o $@ $<

boot/bootsect.s: boot/bootsect.S  Makefile
	$(CPP) -traditional $< -o $@

clean:
	rm -f zImage
	rm -f zBoot/zSystem.out
	$(MAKE) -C zBoot clean
