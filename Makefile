SOURCES=drive.c gene.c invector.c pop.c utility.c
HEADERS=drive.h gene.h invector.h pop.h utility.h
CC = gcc
DEBUG = -g
CFLAGS = -Wall -ansi -pedantic
PROGRAM=ga
OBJS = drive.o gene.o invector.o pop.o utility.o
MACRODEBUG = -D DEBUG

all: $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) -o $(PROGRAM) -lm $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $^

clean:
	rm -f *.o *.out *.exe *.zip core $(PROGRAM)

debug:
	$(CC) $(CFLAGS) $(MACRODEBUG) -o $(PROGRAM) -lm $(SOURCES)

testminfn:
	./ga minfn 4 20 20  minfnin.dat

testpcbmill:
	./ga pcbmill 12 20 20 pcbin.dat

valgrindminfn:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./ga minfn 4 10 5 minfnin.dat

valgrindpcbmill:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./ga pcbmill 12 10 5 pcbin.dat

zip:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Requirement11a.txt Requirement11b.txt minfnin.dat pcbin.dat Makefile
