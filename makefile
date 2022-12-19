default: all

all: sm sp utils consumer server

sm: ./src/shared_memory.c
	gcc -c ./src/shared_memory.c -o sm

sp: ./src/semaphores.c
	gcc -c ./src/semaphores.c -o sp

utils: ./src/utils.c
	gcc -c ./src/utils.c -o utils

consumer: ./src/consumer.c
	gcc ./src/consumer.c sm sp utils -o consumer

server: ./src/server.c
	gcc ./src/server.c sm sp utils -o server

clean:
	rm -f server consumer utils sm sp