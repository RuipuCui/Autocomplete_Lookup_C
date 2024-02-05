# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB = 

# define sets of header source files and object files array.c tree.c
SRC1 = dict1.c data.c list.c
SRC2 = dict2.c data.c array.c 
SRC3 = dict3.c data.c list.c tree.c 
# OBJ is the same as SRC, just replace .c with .h
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
OBJ3 = $(SRC3:.c=.o)
 
# define the executable name(s)
EXE1 = dict1
EXE2 = dict2
EXE3 = dict3

# the first target:
$(EXE1): $(OBJ1) 
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1) $(LIB)
$(EXE2): $(OBJ2) 
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ2) $(LIB)
$(EXE3): $(OBJ3) 
	$(CC) $(CFLAGS) -o $(EXE3) $(OBJ3) $(LIB)

clean:
	rm -f $(OBJ) $(EXE)


