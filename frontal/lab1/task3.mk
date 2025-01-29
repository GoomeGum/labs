all: clear compile link clean

link:
	gcc -g -Wall -m32 -o task3 task3.o

compile:
	gcc -g -Wall -m32 -c  task3.c

clear:
	clear

clean:
	rm -f task3.o
