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
    printf("semaphore was created, id: %d\n", semaphore);
  }

  return semaphore;
}

void set_semaphore_value(int sem_id, int sem_num, int value)
{
  int set_sem_res = semctl(sem_id, sem_num, SETVAL, value);

  if (set_sem_res == -1)
  {
    perror("can't set semaphore value");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("semaphore value was set\n");
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
    printf("semaphore was deleted, id: %d\n", sem_delete);
  }
}

void semaphore_v(int semaphore_id, int sem_num)
{
  struct sembuf sem_buffer;
  sem_buffer.sem_num = sem_num;
  sem_buffer.sem_op = 1;
  sem_buffer.sem_flg = 0;

  int sem_change = semop(semaphore_id, &sem_buffer, 1);

  if (sem_change == -1)
  {

    perror("cannot increment semaphore\n");
    exit(EXIT_FAILURE);
  }
  else
  {
#ifdef DEBUG
    printf("semaphore %d was incremented \n", sem_num);
#endif
  }
}

void semaphore_p(int semaphore_id, int sem_num)
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
      semaphore_p(semaphore_id, sem_num);
    }
    else
    {
      perror("cannot decrement semaphore\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
#ifdef DEBUG
    printf("semaphore %d was decremented \n", sem_num);
#endif
  }
}