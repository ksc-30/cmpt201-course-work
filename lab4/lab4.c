#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEAP_INCREASE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(const char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len =
      snprintf(buf, BUF_SIZE, format,
               data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                             : (uintptr_t)(*(void **)data));
  ;
  if (len < 0) {
    handle_error("snprintf failed in print_out");
  }
  if (write(STDOUT_FILENO, buf, len) == -1) {
    handle_error("write failed in print_out");
  }
}

void print_byte_data(int n) {
  char buf[12];
  int len = snprintf(buf, sizeof(buf), "%d\n", n);
  if (len < 0) {
    handle_error("snprintf failed in print_byte_data");
  }
  if (write(STDOUT_FILENO, buf, len) == -1) {
    handle_error("write failed in print_byte_data");
  }
}

int main(void) {
  void *heap_start = sbrk(HEAP_INCREASE);
  if (heap_start == (void *)-1) {
    handle_error("sbrk failed to allocate memory");
  }

  struct header *block1 = (struct header *)heap_start;
  struct header *block2 = (struct header *)((char *)heap_start + BLOCK_SIZE);

  block1->size = BLOCK_SIZE;
  block1->next = NULL;

  block2->size = BLOCK_SIZE;
  block2->next = block1;

  void *data1_start = (char *)block1 + sizeof(struct header);
  void *data2_start = (char *)block2 + sizeof(struct header);
  size_t data_size = BLOCK_SIZE - sizeof(struct header);

  memset(data1_start, 0, data_size);
  memset(data2_start, 1, data_size);

  print_out("first block:       %p\n", &block1, sizeof(block1));
  print_out("second block:      %p\n", &block2, sizeof(block2));

  print_out("first block size:  %llu\n", &block1->size, sizeof(block1->size));
  print_out("first block next:  %p\n", &block1->next, sizeof(block1->next));
  print_out("second block size: %llu\n", &block2->size, sizeof(block2->size));
  print_out("second block next: %p\n", &block2->next, sizeof(block2->next));

  for (size_t i = 0; i < data_size; ++i) {
    print_byte_data(*((char *)data1_start + i));
  }

  for (size_t i = 0; i < data_size; ++i) {
    print_byte_data(*((char *)data2_start + i));
  }

  return 0;
}
