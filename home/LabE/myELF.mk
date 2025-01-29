all: clear compile link clean

link:
	gcc -g -Wall -m32 -o myELF myELF.o

compile:
	gcc -g -Wall -m32 -c  myELF.c

clear:
	clear

clean:
	rm -f myELF.o