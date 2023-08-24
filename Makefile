CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
         -pedantic-errors -fstack-protector-all 
PROGS = student_tests public01 public02 public03 public04 public05 \
	public06 

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) *.tmp

student_tests: student_tests.o calendar.o my_memory_checker_216.o
	$(CC) -o student_tests student_tests.o calendar.o \
	my_memory_checker_216.o

student_tests.o: student_tests.c event.h calendar.h my_memory_checker_216.h
	$(CC) $(CFLAGS) -c student_tests.c

calender.o: calender.c calender.h event.h
	$(CC) $(CFLAGS) -c calendar.c

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c

public01.o: public01.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public01.c  

public02.o: public02.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public02.c  

public03.o: public03.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public03.c  

public04.o: public04.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public04.c  

public05.o: public05.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public05.c  

public06.o: public06.c my_memory_checker_216.h event.h calendar.h
	$(CC) $(CFLAGS) -c public06.c  

public01: public01.o my_memory_checker_216.o calendar.o
	$(CC) -o public01 public01.o my_memory_checker_216.o calendar.o

public02: public02.o my_memory_checker_216.o calendar.o
	$(CC) -o public02 public02.o my_memory_checker_216.o calendar.o

public03: public03.o my_memory_checker_216.o calendar.o
	$(CC) -o public03 public03.o my_memory_checker_216.o calendar.o

public04: public04.o my_memory_checker_216.o calendar.o
	$(CC) -o public04 public04.o my_memory_checker_216.o calendar.o

public05: public05.o my_memory_checker_216.o calendar.o
	$(CC) -o public05 public05.o my_memory_checker_216.o calendar.o

public06: public06.o my_memory_checker_216.o calendar.o
	$(CC) -o public06 public06.o my_memory_checker_216.o calendar.o



