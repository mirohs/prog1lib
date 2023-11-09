CC = gcc
LINKER = gcc
CFLAGS = -std=gnu11 -Wall -Wpointer-arith -Werror -Wfatal-errors
DEBUG = -g
PROG1LIBNAME = prog1
PROG1LIBDIR = ../lib

# disable default suffixes
.SUFFIXES:

# pattern rule for compiling the library
prog1lib:
	cd $(PROG1LIBDIR) && make

# pattern rule for compiling .c-file to executable
%: %.c prog1lib
	$(CC) $(CFLAGS)	$(DEBUG) $<	-L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR) -o	$@

