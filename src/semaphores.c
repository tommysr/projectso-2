#include "semaphores.h"

int create_semaphore(key_t key)
{
  int semaphore = semget(key, 2, 0600 | IPC_CREAT);

  if (semaphore == -1)
  {
    perror("can't create semaphore");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("semaphore was created: %d\n", semaphore);
  }

  return semaphore;
}

void set_semaphore_value(int sem_id, int sem_num, int value)
{
  int set_sem_res = semctl(sem_id, sem_num, SETVAL, value);
  if (set_sem_res == -1)
  {
    perror("cant set semaphore");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("semaphore was set\n");
  }
}

void delete_semaphore(int semaphore_id)
{
  int sem_delete = semctl(semaphore_id, 0, IPC_RMID);

  if (sem_delete == -1)
  {
    perror("cannot delete semaphore");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("semaphore was deleted: %d\n", sem_delete);
  }
}

void lift_semaphore(int semaphore_id, int sem_num)
{
  struct sembuf sem_buffer;
  sem_buffer.sem_num = sem_num;
  sem_buffer.sem_op = 1;
  sem_buffer.sem_flg = SEM_UNDO;

  int sem_change = semop(semaphore_id, &sem_buffer, 1);

  if (sem_change == -1)
  {
    perror("Cannot lift semaphore\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semaphore was lifted");
  }
}

void release_semaphore(int semaphore_id, int sem_num)
{
  struct sembuf sem_buffer;
  sem_buffer.sem_num = sem_num;
  sem_buffer.sem_op = -1;
  sem_buffer.sem_flg = 0;

  int sem_change = semop(semaphore_id, &sem_buffer, 1);

  if (sem_change == -1)
  {
    if (errno == EINTR)
    {
      release_semaphore(semaphore_id, sem_num);
    }
    else
    {
      perror("Cannot release semaphore\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    printf("Semaphore was released\n");
  }
}