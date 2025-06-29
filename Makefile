CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
         -pedantic-errors -fstack-protector-all 
PROGS = calendar public01 public02 public03 public04 public05

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) *.tmp

$(PROGS): calendar.o
calendar: calendar.o
public%: public%.o

calendar.o: calendar.h event.h my_memory_checker_216.h
public%.o: calendar.h event.h my_memory_checker_216.h
