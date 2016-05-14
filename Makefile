
# General rule

all: distrib


# Documentation

doc:
	doxygen doxyconfig


# dist folder

dist:
	mkdir -p dist


# Includes

distinclude: dist
	mkdir -p dist/include
	cp *.h dist/include


# Sources

distsrc: dist
	mkdir -p dist/src
	cp *.c dist/src
	rm dist/src/main.c dist/src/test.c


# Library

dist/lib/extlib.a: obj/extlib.def.o obj/extlib.dll.o obj/extlib.sll.o obj/extlib.vect.o obj/extlib.string.o obj/extlib.heap.o
	ar -rv $@ $^

distlib: dist
	mkdir -p dist/lib

distlibrary: distlib dist/lib/extlib.a


# Distribution version

distrib: doc distinclude distsrc distlibrary
	cp -r doc dist
	rm -r doc


# Clean

clean:
	rm -r dist

mrpropper: clean
	rm -r obj
	rm -r bin
