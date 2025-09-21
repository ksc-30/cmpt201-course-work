#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *input = NULL;
  size_t length = 0;

  while (1) {
    pid_t pid = fork();

    if (pid == 0) {

      printf("Enter programs to run= \n");
      printf("> ");
      getline(&input, &length, stdin);
      input[strcspn(input, "\n")] = '\0';

      char *pointer;
      char *token = strtok_r(input, " ", &pointer);

      printf("%s\n", token);

      printf("starting exec");

      execl(token, token, NULL);

    }

    else {

      waitpid(pid, NULL, 0);
    }
  }

  free(input);
}
