libpath=$(PWD)/../../../lib
includepath=$(PWD)/../../../include


all: qsinterface.c
	gcc -I$(includepath) -g -O1 -o qsi qsinterface.c -L$(libpath) -lqsopt_ex -lgmp
