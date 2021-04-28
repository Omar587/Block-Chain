CC = gcc
CFLAG = -Wall -ansi -std=c99
DEBUG = -g

all: peep peep_debug test_verify

peep: peep.c peepEditor.o blockChain.o
	${CC} ${CFLAG} -o peep peep.c blockChain.o peepEditor.o -lcrypto

peep_debug: peep.c blockChain.c peepEditor.c
	${CC} ${CFLAG} -o peep_g -g peep.c blockChain.c peepEditor.c -lcrypto
peepEditor.o: peepEditor.c peepEditor.h
	${CC} ${CFLAG} -c peepEditor.c

blockChain.o: blockChain.c blockChain.h
	${CC} ${CFLAG} -c blockChain.c

main: test_print_peep.c peepEditor.c blockChain.c
	${CC} ${CFLAG} ${DEBUG} -o main test_print_peep.c peepEditor.c blockChain.c -lcrypto

clean:
	rm *.o peep main a.out peep peep_g

A3.tar.gz:
	tar -cvzf A3.tar.gz *.c *.h Plan Test README.md test_input.txt makefile

test: peep	
	gdb --args peep -e new

test_verify: peep
	diff Test/new Test/new_modified; ./peep -v Test/new_modified
	


