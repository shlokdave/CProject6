# Warning flags
CFLAGS = -Wall -std=c99 -g -D_GNU_SOURCE -c
LFLAGS = -o

# Math library being used
LIBS = -lm

# Object files
OBJS = catalog.o parks.o input.o

# Default target (all)
all: parks

# Final main program
parks: $(OBJS)
	gcc $(LFLAGS) parks $(OBJS) $(LIBS)

# The source files into object files
catalog.o: catalog.c catalog.h
	gcc $(CFLAGS) catalog.c

parks.o: parks.c catalog.h input.h
	gcc $(CFLAGS) parks.c

input.o: input.c input.h
	gcc $(CFLAGS) input.c

# Clean target
clean:
	rm -f $(OBJS)
	rm -f parks
