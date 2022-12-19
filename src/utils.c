#include "utils.h"

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
