#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WORK_AMOUNT 800000
#define NUM_PROCS 4

void worker(int id) {
    int i, j;
    volatile int sum = 0;
    int start_time = uptime();
    
    printf("[Process %d] Started at tick %d (PID: %d)\n", id, start_time, getpid());
    
    // Do CPU work
    for(i = 0; i < WORK_AMOUNT; i++) {
        for(j = 0; j < 10; j++) {
            sum += i * j;
        }
        
        // Print progress at 25%, 50%, 75%
        if(i == WORK_AMOUNT/4 || i == WORK_AMOUNT/2 || i == (3*WORK_AMOUNT)/4) {
            int current = uptime();
            printf("[Process %d] %d%% complete at tick %d (runtime: %d ticks)\n", 
                   id, (i * 100) / WORK_AMOUNT, current, current - start_time);
        }
    }
    
    int end_time = uptime();
    printf("[Process %d] FINISHED at tick %d (total runtime: %d ticks)\n", 
           id, end_time, end_time - start_time);
    
    exit(0);
}

int main(void) {
    int i;
    
    printf("\n");
    printf("==========================================\n");
    printf("       SCHEDULER COMPARISON TEST\n");
    printf("==========================================\n");
    printf("\n");
    
    int start = uptime();
    
    // Create NUM_PROCS processes with slight delays
    for(i = 0; i < NUM_PROCS; i++) {
        int pid = fork();
        if(pid == 0) {
            worker(i);
        }
        // Small delay between forks (using busy wait)
        int delay_start = uptime();
        while(uptime() - delay_start < 2) {
            // busy wait
        }
    }
    
    printf("\n[Parent] All %d processes created. Waiting...\n\n", NUM_PROCS);
    
    // Wait for all children
    for(i = 0; i < NUM_PROCS; i++) {
        wait(0);
    }
    
    int end = uptime();
    
    printf("\n");
    printf("==========================================\n");
    printf("           TEST COMPLETE\n");
    printf("==========================================\n");
    printf("Total time: %d ticks\n\n", end - start);
    
    printf("EXPECTED BEHAVIOR:\n");
    printf("------------------------------------------\n");
    printf("FCFS:\n");
    printf("  - Process 0 finishes 100%% before Process 1 starts\n");
    printf("  - Processes complete in strict order: 0 -> 1 -> 2 -> 3\n");
    printf("  - No interleaving\n\n");
    
    printf("CFS:\n");
    printf("  - All processes make progress simultaneously\n");
    printf("  - Check for [Scheduler Tick] logs showing vruntime\n");
    printf("  - Fair CPU time distribution\n\n");
    
    printf("Round Robin:\n");
    printf("  - Processes interleave regularly\n");
    printf("  - Progress shown at similar intervals\n");
    printf("  - Time-sliced execution\n");
    printf("==========================================\n\n");
    
    exit(0);
}
