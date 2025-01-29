# Default target
addreses: addreses.o
	gcc -g -Wall -m32 -o addreses addreses.o

# Compile the .c file into an .o file
addreses.o: addreses.c
	gcc -g -Wall -m32 -c addreses.c

# Clean up build files
clean:
	rm -f addreses addreses.o