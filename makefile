default: all

all: sm sp utils consumer server

sm: ./src/shared_memory.c
	gcc -c ./src/shared_memory.c -o sm

sp: ./src/semaphores.c
	gcc -c ./src/semaphores.c -o sp

utils: ./src/utils.c
	gcc -c ./src/utils.h -o u

consumer: ./src/consumer.c
	gcc -o consumer ./src/consumer.c sm sp u

server: ./src/server.c
	gcc -o server ./src/server.c sm sp u

clean:
	rm -f server consumer utils sm sp