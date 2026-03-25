# makefile  
CC = cc
CFLAGS = -g

all :  mainprog child procident simple procmemory

mainprog : mainprog.o
	${CC} ${CFLAGS}   mainprog.o -o mainprog
 
child : child.o
	${CC} ${CFLAGS} child.o ${LIBS} -o child

procident : procident.o
	${CC} ${CFLAGS} procident.o  -o procident

simple : simple.o
	${CC} ${CFLAGS} simple.o  -o simple

procmemory : procmemory.o
	${CC} ${CFLAGS} procmemory.o  -o procmemory

clean :
	rm -f *.o *~ *# core  a.out\
	mainprog  child  procident simple procmemory

