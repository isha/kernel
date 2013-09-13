

MAKE = make

xeros:
	cd compile; $(MAKE) 
	cd boot; $(MAKE)

beros: xeros
	nice bochs

clean:
	cd compile; $(MAKE) clean
	cd boot; $(MAKE) clean
	rm -f bochsout.txt

# The following two sets of make rules should never be needed unless you have
# accidentally delated a library or the system.o files.

libxc:
	rm -f lib/libxc.a
	cd lib/libxc; $(MAKE) clean; $(MAKE)
	mv lib/libxc/libxc.a lib


rebuild: libxc clean xeros


