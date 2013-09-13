.SUFFIXES: .o .c
#
# Makefile for PC Xeros to compile on a PC running Linux.
#

CCPREFIX = 


# Things that need not be changed, usually
OS      = LINUX
DEFS	= -DBSDURG  -DVERBOSE -DPRINTERR
INCLUDE = -I../h
CFLAGS	= -Wall -Wstrict-prototypes -fno-builtin -c  ${DEFS} ${INCLUDE}
SDEFS	= -D${OS} -I../h -DLOCORE -DSTANDALONE -DAT386
LIB     = ../lib
AS      = $(CCPREFIX)as --32
AR	= $(CCPREFIX)ar
XEROS   = ./xeros
GCC     = $(CCPREFIX)gcc -m32 -march=i386
CC      = $(GCC) -D__KERNEL__ -D__ASSEMBLY__ 
LD      = $(CCPREFIX)ld -m elf_i386
CPP     = $(CC) -E
AWK     = awk


# Use the following line if you want to boot Xeros from floppy diskette
#
BRELOC = 0x100000
TEXTSPOT= 0x000000
BOOTPLOC= 0x150000

# Ignore these 
#BRELOC = 0x150000
#TEXTSPOT= 0x150000

# Linker line, do not modify this, please.
LDSTR	= -e start -Ttext ${TEXTSPOT}


#Do NOT modify these lines
SOBJ = startup.o intr.o 
IOBJ = init.o i386.o evec.o kprintf.o
UOBJ = mem.o disp.o ctsw.o syscall.o create.o user.o msg.o sleep.o

#Add your sources here
MY_OBJ = 


# Don't modiy any of this unless you are really sure
all: xeros 

xeros: Makefile ${SOBJ} ${IOBJ} ${UOBJ} ${MY_OBJ} ${LIB}/libxc.a
	$(LD) ${LDSTR} ${SOBJ} ${IOBJ} ${UOBJ} ${MY_OBJ} ${LIB}/libxc.a -o ${XEROS}

clean: 
	rm -rf *.o *.bak *.a core errs ${XEROS} ${XEROS}.boot

cleanall: 
	rm -rf *.o *.bak *.a core errs ${XEROS} ${XEROS}.boot
	(cd ${LIB}/libxc; make clean)

${LIB}/libxc.a: 
	(cd ${LIB}/libxc; make install)

intr.o: ../c/intr.S ../c/xint.s
	${CPP} ${SDEFS} ../c/intr.S | ${AS} -o intr.o

startup.o: ../c/startup.S Makefile
	${CPP} ${SDEFS} -DBRELOC=${BRELOC} -DBOOTPLOC=${BOOTPLOC} -DLINUX_XINU ../c/startup.S | ${AS} -o startup.o 

${IOBJ}: 
	${CC} ${CFLAGS} ../c/`basename $@ .o`.[c]

${UOBJ}: 
	${CC} ${CFLAGS} ../c/`basename $@ .o`.[c]

init.o: ../c/init.c ../h/i386.h ../h/xeroskernel.h
i386.o: ../c/i386.c ../h/i386.h
evec.o: ../c/evec.c ../h/i386.h
kprintf.o: ../c/kprintf.c ../h/i386.h
mem.o: ../c/mem.c ../h/xeroskernel.h
disp.o: ../c/disp.c ../h/xeroskernel.h
ctsw.o: ../c/ctsw.c ../h/xeroskernel.h
syscall.o: ../c/syscall.c ../h/xeroskernel.h
create.o: ../c/create.c ../h/xeroskernel.h
user.o: ../c/user.c ../h/xeroskernel.h
msg.o: ../c/msg.c ../h/xeroskernel.h
sleep.o: ../c/sleep.c ../h/xeroskernel.h
