#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  for (int i = 0; i < 5; i++) {
    if (fork() == 0) {
      printf("Child %d running\n", i);
      // Busy work to keep CPU occupied
      for (volatile int j = 0; j < 100000000; j++);
      printf("Child %d finished\n", i);
      exit(0);
    }
  }

  // Parent waits for all children
  for (int i = 0; i < 5; i++) wait(0);
  exit(0);
}
