#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

int create_semaphore(key_t);
void set_semaphore_value(int, int, int);
void lift_semaphore(int, int);
void release_semaphore(int, int);
void delete_semaphore(int);

#endif