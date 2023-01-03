#include "shared_memory.h"

int create_shared_memory(key_t key)
{
  int memory_segment = shmget(key, sizeof(char), 0600 | IPC_CREAT);

  if (memory_segment == -1)
  {
    perror("can't create shared memory segment\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("created shared memory segment: %d\n", memory_segment);
  }

  return memory_segment;
}

char *attach_shared_memory(int segment)
{
  char *address = (char *)shmat(segment, 0, 0);

  if (*address == -1)
  {
    perror("can't attach memory\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("attached : %p\n", address);
  }

  return address;
}

void detach_memory(char *address)
{
  int detach_memory_status = shmdt(address);

  if (detach_memory_status == -1)
  {
    perror("can't detach shared memory");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("shared memory was successfully detached.\n");
  }
}

void remove_memory(int shared_memory_segment)
{
  int remove_memory_status = shmctl(shared_memory_segment, IPC_RMID, 0);

  if (remove_memory_status == -1)
  {
    perror("can't remove memory");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("memory was successfully deleted\n");
  }
}