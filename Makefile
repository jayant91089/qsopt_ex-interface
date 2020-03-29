libpath=$(PWD)/../../../lib
includepath=$(PWD)/../../../include


all: qsinterface.c
	clang-10  -fsanitize-blacklist=blacklist.txt -fno-omit-frame-pointer -I$(includepath) -g -O1 -o qsi qsinterface.c -L$(libpath) -lqsopt_ex -lgmp -fsanitize=address
