CC = gcc
LINKER = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpointer-arith -Wfatal-errors
DEBUG = -g

# disable default suffixes
.SUFFIXES:

# pattern rule for compiling .c-file to executable
%: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -L../lib -lprog1 -iquote../lib -o $@
