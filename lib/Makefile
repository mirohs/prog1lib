# @author Michael Rohs
# @date 15.10.2015, 25.9.2023
# @copyright Apache License, Version 2.0
# 
# base.c

CC = gcc
LINKER = gcc
CFLAGS = -std=gnu11 -Wall -Werror -Wpointer-arith -Wfatal-errors
DEBUG = -g
LIBRARY = libprog1.a
SRCS = base.c basedefs.c
OBJS = $(SRCS:.c=.o)

# disable default suffixes
.SUFFIXES:

# define pattern rule
%.o : %.c 
	@echo "Compiling $< to $@:" 
	$(CC) -c $(CFLAGS) $(DEBUG) $<
	$(CC) -MM $< > $(<:.c=.d)

$(LIBRARY): $(OBJS)
	@echo "Archiving $(OBJS) to static library $@:"
	ar rcs $(LIBRARY) $(OBJS) $(LDFLAGS)

# include dependency rules
-include $(OBJS:.o=.d)

string_list: *.o string_list.c string_list.h
	$(CC) $(CFLAGS) *.o -o $@ 

# do not treat "clean" as a file name
.PHONY: clean 

# remove produced files, invoke as "make clean"
clean: 
	rm -f $(LIBRARY)
	rm -f $(OBJS)
	rm -f $(SRCS:.c=.d)
	rm -rf $(SRCS:.c=.dSYM)
	rm -rf .DS_Store ../.DS_Store ../script_examples/.DS_Store ../lecture_examples/.DS_Store
	rm -rf doc ../script_examples/*.dSYM ../lecture_examples/*.dSYM
