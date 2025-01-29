targets: clear all  clean


start.o: start.s
	nasm -f elf32 -g  -o  start.o  start.s

loader1.o: loader1.c
	gcc -m32 -c -o loader1.o loader1.c

startup.o: startup.s
	nasm -f elf32 -g  -o  startup.o  startup.s

all: loader1.o start.o startup.o
	ld -o loader1 loader1.o startup.o start.o -L/usr/lib32 -lc -T linking_script -dynamic-linker /lib32/ld-linux.so.2


clean:
	rm -f *.o

clear:
	clear