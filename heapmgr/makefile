
CFLAGS = -O3 -D NDEBUG
CC = gcc
all: testheapmgrgnu testheapmgrkr testheapmgrbase testheapmgr1 testheapmgr2

CC = gcc
CFLAGS = -O3 -D NDEBUG

testheapmgr1: testheapmgr.c heapmgr1.c chunk.c
	$(CC) $(CFLAGS) testheapmgr.c heapmgr1.c chunk.c -o testheapmgr1

testheapmgr2: testheapmgr.c heapmgr2.c chunk.c
	$(CC) $(CFLAGS) testheapmgr.c heapmgr2.c chunk.c -o testheapmgr2

clean:
	rm -f testheapmgr1 testheapmgr2

testheapmgrgnu: testheapmgr.o heapmgrgnu.o 
	$(CC) $(CFLAGS)  testheapmgr.o heapmgrgnu.o -o testheapmgrgnu
heapmgrgnu.o: heapmgrgnu.c heapmgr.h 
	$(CC) -c heapmgrgnu.c 


testheapmgrkr: testheapmgr.o heapmgrkr.o 
	$(CC) $(CFLAGS)  testheapmgr.o heapmgrkr.o -o testheapmgrkr
heapmgrkr.o: heapmgrkr.c heapmgr.h 
	$(CC) -c heapmgrkr.c 

testheapmgrbase: testheapmgr.o heapmgrbase.o chunkbase.o
	$(CC) -03 -D NDEBUG -o testheapmgrbase testheapmgr.o heapmgrbase.o chunkbase.o
testheapmgr.o: testheapmgr.c heapmgr.h
	$(CC) -c testheapmgr.c
heapmgrbase.o: heapmgrbase.c heapmgr.h chunkbase.h
	$(CC) -c heapmgrbase.c 
chunkbase.o: chunkbase.c chunkbase.h
	$(CC) -c chunkbase.c
