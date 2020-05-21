server: server.o utils.o 
	gcc -g utils.o server.o -o server  -lpthread
server.o: server.c
	gcc -c -g server.c -o server.o -lpthread
utils.o: utils.c
	gcc -c -g utils.c -o utils.o

.PHONY:clean
clean:
	rm -rf *.o


