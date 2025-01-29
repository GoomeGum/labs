all: clear compile link clean

link:
	gcc -g -Wall -m32 -o task1 task1.o

compile:
	gcc -g -Wall -m32 -c  task1.c

clear:
	clear

clean:
	rm -f task1.o
