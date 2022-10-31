COMPILER = gcc
EXES = search

compile: dmood_hw3.c
	${COMPILER} -o dmood_hw3 dmood_hw3.c

%.o: %.c %.h  makefile
	${COMPILER} $< -c 

remove:
	rm -f *.o *~ ${EXES} ${CFILES}
