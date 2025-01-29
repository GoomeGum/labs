all: clear compile link clean

link:
	gcc -g -Wall -m32 -o virusDetector virusDetector.o

compile:
	gcc -g -Wall -m32 -c  virusDetector.c

clear:
	clear

clean:
	rm -f virusDetector.o
