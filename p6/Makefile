# Warning flags
CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS +=

# Default target
all: driver

# Object files
driver: driver.o value.o map.o input.o
	$(CC) $(CFLAGS) $(LDLIBS) -o driver driver.o value.o map.o input.o $(LDLIBS)

# Test programs
stringTest: stringTest.o value.o
	$(CC) $(CFLAGS) $(LDLIBS) -o stringTest stringTest.o value.o

mapTest: mapTest.o map.o value.o
	$(CC) $(CFLAGS) $(LDLIBS) -o mapTest mapTest.o map.o value.o

# Object file rules
driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

stringTest.o: stringTest.c
	$(CC) $(CFLAGS) -c stringTest.c

mapTest.o: mapTest.c
	$(CC) $(CFLAGS) -c mapTest.c

value.o: value.c value.h
	$(CC) $(CFLAGS) -c value.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

# Clean target
clean:
	rm -f driver stringTest mapTest *.o *.gcda *.gcno *.gcov