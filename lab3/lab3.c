#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 5

static void chomp_newline(char *s) {
  if (s == NULL)
    return;
  size_t n = strlen(s);
  if (n > 0 && s[n - 1] == '\n') {
    s[n - 1] = '\0';
  }
}

static void print_history(char *history[], int count, int next_index) {
  int start = (next_index - count + MAX_HISTORY) % MAX_HISTORY;
  for (int i = 0; i < count; i++) {
    int idx = (start + i) % MAX_HISTORY;
    printf("%s\n", history[idx]);
  }
}

static void free_history(char *history[]) {
  for (int i = 0; i < MAX_HISTORY; i++) {
    free(history[i]);
    history[i] = NULL;
  }
}

int main(void) {
  char *history[MAX_HISTORY] = {NULL, NULL, NULL, NULL, NULL};
  int count = 0;
  int next_index = 0;

  char *line = NULL;
  size_t cap = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    ssize_t got = getline(&line, &cap, stdin);
    if (got == -1) {
      break;
    }

    chomp_newline(line);

    int is_print = (strcmp(line, "print") == 0);

    if (count < MAX_HISTORY) {
      history[next_index] = line;
      line = NULL;
      count++;
    } else {
      free(history[next_index]);
      history[next_index] = line;
      line = NULL;
    }
    next_index = (next_index + 1) % MAX_HISTORY;

    if (is_print) {
      print_history(history, count, next_index);
    }
  }

  free(line);
  free_history(history);
  return 0;
}
