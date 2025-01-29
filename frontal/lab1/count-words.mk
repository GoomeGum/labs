
count-words: count-words.o
	gcc -Wall -g -m32 -o count-words count-words.o
count-words.o : count-words.c
	gcc -Wall -m32 -g -c count-words.c

