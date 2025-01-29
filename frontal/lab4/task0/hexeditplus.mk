all: clear compile link clean

link:
	gcc -g -Wall -m32 -o hexeditplus hexeditplus.o

compile:
	gcc -g -Wall -m32 -c  hexeditplus.c

clear:
	clear

clean:
	rm -f hexeditplus.o
