# Compiler and assembler
CC = gcc
AS = nasm

# Compiler flags
CFLAGS = -Wall -g -m32
LDFLAGS = -m32

# Object files
OBJS = main.o add.o numbers.o

# Target executable
TARGET = my_program

# Default rule to build the program
all: $(TARGET)

# Rule to link the program
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Rule to compile numbers.c
numbers.o: numbers.c
	$(CC) $(CFLAGS) -c numbers.c

# Rule to assemble add.s with NASM
add.o: add.s
	$(AS) -f elf32 -o add.o add.s   # NASM syntax for 32-bit output

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o $(TARGET)

