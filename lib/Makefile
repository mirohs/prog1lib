# @author Michael Rohs
# @date 15.10.2015
# @copyright Apache License, Version 2.0
# 
# base.c
# string.c
# 
# array.c
# int_array.c
# double_array.c
# string_array.c
# pointer_array.c
# byte_array.c
# 
# list.c
# int_list.c
# double_list.c
# string_list.c
# pointer_list.c

# gcc -std=c99 -Wall -Werror -Wpointer-arith -Wfatal-errors -g base.c string.c array.c int_array.c double_array.c string_array.c pointer_array.c byte_array.c list.c int_list.c double_list.c string_list.c pointer_list.c -o a.out && ./a.out

CC = gcc
LINKER = gcc
CFLAGS = -std=c99 -fpack-struct=4 -Wall -Werror -Wpointer-arith -Wfatal-errors
DEBUG = -g
LIBRARY = libprog1.a
SRCS = base.c basedefs.c string.c array.c int_array.c double_array.c string_array.c pointer_array.c byte_array.c list.c int_list.c double_list.c string_list.c pointer_list.c 
OBJS = $(SRCS:.c=.o) # base.o string.o...

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
