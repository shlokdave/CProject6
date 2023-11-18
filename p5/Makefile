CC = gcc
CFLAGS = -Wall -std=c99 -g -fno-stack-protector

# Default target
all: encrypt decrypt

# Targets
encrypt: encrypt.o io.o DES.o DESMagic.o
	$(CC) $(CFLAGS) -o encrypt encrypt.o io.o DES.o DESMagic.o

decrypt: decrypt.o io.o DES.o DESMagic.o
	$(CC) $(CFLAGS) -o decrypt decrypt.o io.o DES.o DESMagic.o

DESTest: DESTest.o DES.o DESMagic.o io.o
	$(CC) $(CFLAGS) -o DESTest DESTest.o DES.o DESMagic.o io.o

# Object files
encrypt.o: encrypt.c DES.h DESMagic.h io.h
	$(CC) $(CFLAGS) -c encrypt.c -o encrypt.o

decrypt.o: decrypt.c io.h DES.h DESMagic.h
	$(CC) $(CFLAGS) -c decrypt.c -o decrypt.o

DESTest.o: DESTest.c DES.h DESMagic.h io.h
	$(CC) $(CFLAGS) -c DESTest.c -o DESTest.o

io.o: io.c io.h DES.h
	$(CC) $(CFLAGS) -c io.c -o io.o

DES.o: DES.c DES.h DESMagic.h
	$(CC) $(CFLAGS) -c DES.c -o DES.o

DESMagic.o: DESMagic.c DESMagic.h
	$(CC) $(CFLAGS) -c DESMagic.c -o DESMagic.o

# Clean target
clean:
	rm -f *.o encrypt decrypt DESTest
