CC=gcc 
CFLAGS=-Iinclude -std=c99

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
	mkdir -p dist/include/ExtLib
	cp include/ExtLib/*.h dist/include/ExtLib


# Sources

distsrc: dist
	mkdir -p dist/src
	cp src/*.c dist/src


# Compilation

objdir:
	mkdir -p obj

obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@


# Library

dist/lib/libextlib.a: obj/Array.o obj/Collection.o obj/Common.o obj/Iterable.o obj/List.o obj/SimpleList.o obj/String.o
	ar -rv $@ $^

distlib: dist
	mkdir -p dist/lib

distlibrary: distlib objdir dist/lib/libextlib.a


# Distribution version

distrib: doc distinclude distsrc distlibrary
	cp -r doc dist
	rm -r doc


# Clean

clean:
	rm -rf obj
#	rm -rf bin

mrpropper: clean
	rm -rf dist
