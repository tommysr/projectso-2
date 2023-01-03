#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int create_semaphore(key_t key);
void set_semaphore_value(int sem_id, int sem_num, int value);
void semaphore_v(int semaphore_id, int sem_num);
void semaphore_p(int semaphore_id, int sem_num);
void delete_semaphore(int semaphore_id);

#endif