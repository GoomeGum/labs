
bubblesort: bubblesort.o
	gcc -g -Wall -m32 -o bubblesort bubblesort.o

# Compile the .c file into an .o file
bubblesort.o: bubblesort.c
	gcc -g -Wall -m32 -c bubblesort.c

# Clean up build files
clean:
	rm -f bubblesort bubblesort.o