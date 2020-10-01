########################################################################

EXEC = clock
FILES = assignment3.c
CC = gcc

$(EXEC):
	$(CC) -o $(EXEC) $(FILES) -pthread
	rm -f *.o

clean:
	rm -f *.o $(EXEC)

###########################[ EOF: Makefile ]############################
