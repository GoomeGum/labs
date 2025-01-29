CC = gcc

CFLAGS =  -Wall -g -m32 
LDFLAGS = -m32

OBJS = encoder.o

TARGET =  encoder 

all :  clearscreen $(TARGET) clean

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

encoder.o : encoder.c
	$(CC) $(CFLAGS) -c encoder.c

clearscreen:
	clear
clean:
	rm -f $(OBJS)

