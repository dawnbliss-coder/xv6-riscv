#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int nice_values[4] = {0, 5, -5, 10}; // different nice levels
    int i;
    
    for(i = 0; i < 4; i++){
        int pid = fork();
        if(pid == 0){
            // Set nice for child process (if you implemented set_nice)
            // p->nice = nice_values[i];  // done in allocproc for testing
            int j;
            for(j=0; j<100000000; j++); // CPU-bound work
            printf("Child %d (nice %d) done\n", i, nice_values[i]);
            exit(0);
        }
    }
    
    for(i=0;i<4;i++) wait(0);
    exit(0);
}
