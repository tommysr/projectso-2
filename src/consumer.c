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
  int memory_segment;
  int semaphore_id;

  key_t shared_memory_key = create_key(2115);
  key_t semaphore_key = create_key(2116);

  memory_segment = create_shared_memory(shared_memory_key);
  semaphore_id = create_semaphore(semaphore_key);
  shared_memory_address = attach_shared_memory(memory_segment);
  output_file = fopen("output", "w");

  if (output_file == NULL)
  {
    perror("can't open file in write mode");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("successfully opened input file in the write mode\n");
  }

  while (character != EOF)
  {
    semaphore_p(semaphore_id, CONSUMER_SEMAPHORE);
    character = *shared_memory_address;
    sleep(1);

    if (character != EOF)
    {
      fputc(character, output_file);
      printf("(c) char = %c\n", character);
    }

    semaphore_v(semaphore_id, SERVER_SEMAPHORE);
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
  remove_memory(memory_segment);
  detach_memory(shared_memory_address);

  exit(EXIT_SUCCESS);
}
