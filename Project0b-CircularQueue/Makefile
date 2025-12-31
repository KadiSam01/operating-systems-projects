CC = gcc

all: queueTest

queueTest: queue.o p1test.o
	$(CC) queue.o p1test.o -o queueTest

clean:
	rm *.o
    
.c.o:
	$(CC) -c $< -o $@
