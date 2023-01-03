#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

int create_shared_memory(key_t);
char *attach_shared_memory(int segment);
void remove_memory(int shared_memory_segment);
void detach_memory(char *address);

#endif