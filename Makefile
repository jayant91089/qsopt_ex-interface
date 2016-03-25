libpath=$(PWD)/lib
includepath=$(PWD)/include


all: qsinterface.c
	gcc -I$(includepath) -o qsi qsinterface.c -L$(libpath) -lqsopt_ex -L$(libpath) -lgmp -g
	
test: test.c
	gcc -I$(includepath) -o test test.c -L$(libpath) -lqsopt_ex -L$(libpath) -lgmp -g
