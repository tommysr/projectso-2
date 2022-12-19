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
  char character;
  FILE *input_file;

  key_t shared_memory_key = create_key(2115);
  key_t semaphore_key = create_key(2137);

  int memory_segment = create_shared_memory(shared_memory_key);
  int semaphore_id = create_semaphore(semaphore_key);
  char *shared_memory_address = attach_shared_memory(memory_segment);

  set_semaphore_value(semaphore_id, 0, 1); // server semaphore, allows transmitting data from server
  set_semaphore_value(semaphore_id, 1, 0); // consumer semaphore, allows reading data by consumer

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
      printf("character = %c, address = %s \n", character, shared_memory_address);

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

  // detach_memory(shared_memory_address);

  exit(EXIT_SUCCESS);
}
