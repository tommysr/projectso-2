#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

key_t create_key(int);
int create_semaphore(key_t);
void set_semaphore_value(int, int, int);
void lift_semaphore(int, int);
void release_semaphore(int, int);
void delete_semaphore(int);

int create_shared_memory(key_t);
void attach_shared_memory(char *, int);

int main()
{
  char *shared_memory_address;
  char character;
  FILE *input_file;

  key_t shared_memory_key = create_key(2115);
  key_t semaphore_key = create_key(2137);

  int memory_segment = create_shared_memory(shared_memory_key);
  int semaphore_id = create_semaphore(semaphore_key);

  attach_shared_memory(shared_memory_address, memory_segment);

  set_semaphore_value(semaphore_id, 0, 1); //server semaphore, allows transmitting data from server
  set_semaphore_value(semaphore_id, 1, 0); //consumer semaphore, allows reading data by consumer

  input_file = fopen("input", "r");

  if (input_file == NULL)
  {
    perror("cant open file in read mode");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("successfully opened input file in the read mode");
  }

  while (!feof(input_file))
  {
    character = fgetc(input_file);

    if (character != EOF)
    {
      release_semaphore(semaphore_id, 0);

      *shared_memory_address = character;
      printf("character = %c, address = %c \n", character, *shared_memory_address);

      lift_semaphore(semaphore_id, 1);
    }
  }

  release_semaphore(semaphore_id, 0);
  *shared_memory_address = EOF;
  lift_semaphore(semaphore_id, 1);

  int file_close_status = fclose(input_file);

  if (file_close_status == -1)
  {
    perror("cannot close file");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("file closed successfully\n");
  }

  delete_semaphore(semaphore_id);
  detach_memory(shared_memory_key, shared_memory_address);

  exit(EXIT_SUCCESS);
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

key_t create_key(int id)
{
  key_t key = ftok(".", id);

  if (key == -1)
  {
    printf("key creation failed\n");
    exit(EXIT_FAILURE);
  }

  return key;
}

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

int create_shared_memory(key_t key)
{
  int memory_segment = shmget(key, sizeof(char), 0600 | IPC_CREAT);

  if (memory_segment == -1)
  {
    perror("can't create shared memory.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("created shared memory segment: %d\n", memory_segment);
  }

  return memory_segment;
}

void attach_shared_memory(char *address, int segment)
{
  address = (char *)shmat(segment, 0, 0);

  if (*address == -1)
  {
    perror("can't attach memory\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("attached : %s\n", *address);
  }
}

void detach_memory(int shared_memory_segment, char *address)
{
  int mark_memory_status = shmctl(shared_memory_segment, IPC_RMID, 0);

  sleep(5);
  int detach_memory_status = shmdt(address);

  if (mark_memory_status == -1 || detach_memory_status == -1)
  {
    perror("cant detach shared memory");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Shared memory was successfully detached.\n");
  }
}
