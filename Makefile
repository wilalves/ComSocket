all:
	gcc Servidor.c -o Servidor -lpthread
	gcc ClienteA.c -o ClienteA
	gcc ClienteB.c -o ClienteB

