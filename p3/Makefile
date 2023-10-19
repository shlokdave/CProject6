# Warning Flags
CFLAGS = -Wall -std=c99 -g -c
LFLAGS = -o

# Object files
OBJS = trader.o transaction.o account.o util.o

# Default target (all)
all: trader

# Final main program
trader: $(OBJS)
	gcc $(OBJS) -o trader

# Source files into object files
trader.o: trader.c transaction.h account.h util.h
	gcc $(CFLAGS) trader.c

transaction.o: transaction.c transaction.h account.h util.h
	gcc $(CFLAGS) transaction.c

account.o: account.c account.h
	gcc $(CFLAGS) account.c

util.o: util.c util.h
	gcc $(CFLAGS) util.c

# Clean target
clean:
	rm -f $(OBJS)
	rm -f trader