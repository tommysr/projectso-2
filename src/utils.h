#ifndef UTILS_H
#define UTILS_H

#define SERVER_SEMAPHORE 0
#define CONSUMER_SEMAPHORE 1

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

key_t create_key(int id);

#endif