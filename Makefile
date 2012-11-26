CFLAGS=-W -Wall -Wextra -Wunused-function -O3
#CFLAGS=-g -ggdb
DEFINE=
IFLAGS=-I src
CC=g++
TARGET=raytracer

all: ${TARGET}

include Makefile.inc

Makefile.inc:
	./configure

${TARGET}: ${OBJS} src/main.o
	g++ ${OBJS} src/main.o ${LINK} -o $@

%.o: %.cc
	${CC} -c $^ ${DEFINE} ${CFLAGS} ${IFLAGS} -o $@

clean:
	rm -rf src/*.o *.o *.a *.so 
	rm -f `find . -name '*.o'`
	rm -f `find . -name '*~'`
	rm -f `find . -name '*#'`
	rm -f `find . -name '*.out'`
	rm -f Makefile.inc ${TARGET}
