default: all

all: sm sp utils consumer producer

sm: ./src/shared_memory.c
	gcc -c ./src/shared_memory.c -o sm

sp: ./src/semaphores.c
	gcc -c ./src/semaphores.c -o sp

utils: ./src/utils.c
	gcc -c ./src/utils.c -o utils

consumer: ./src/consumer.c
	gcc ./src/consumer.c sm sp utils -o consumer

producer: ./src/producer.c
	gcc ./src/producer.c sm sp utils -o producer

clean:
	rm -f producer consumer utils sm sp