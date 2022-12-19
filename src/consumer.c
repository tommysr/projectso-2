#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "semaphores.h"
#include "shared_memory.h"
#include "utils.h"

int main()
{
  char *shared_memory_address;
  char character;
  FILE *output_file;

  key_t shared_memory_key = create_key(2115);
  key_t semaphore_key = create_key(2137);

  int memory_segment = create_shared_memory(shared_memory_key);
  int semaphore_id = create_semaphore(semaphore_key);

  shared_memory_address = attach_shared_memory(memory_segment);

  printf("waiting: %p", shared_memory_address);

  output_file = fopen("output", "w");

  if (output_file == NULL)
  {
    perror("cant open file in read mode");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("successfully opened input file in the read mode");
  }

  while (character != EOF)
  {
    release_semaphore(semaphore_id, 1);
    character = *shared_memory_address;

    if (character != EOF)
    {
      fputc(character, output_file);
      printf("(c) character = %c, address = %s \n", character, shared_memory_address);

      lift_semaphore(semaphore_id, 0);
    }
  }

  int file_close_status = fclose(output_file);

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
  mark_remove_memory(memory_segment);

  detach_memory(shared_memory_address);

  exit(EXIT_SUCCESS);
}
