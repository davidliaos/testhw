#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_JOBS 10

typedef struct {
    int arrival_time;
    int service_time;
    int remaining_time;
    int start_time;
    int termination_time;
    int turnaround_time;
    int waiting_time;
} Job;

void generate_jobs(Job jobs[]) {
    srand(time(0));
    int arrival_accum = 0;
    for (int i = 0; i < NUM_JOBS; i++) {
        jobs[i].service_time = (rand() % 21) + 5; // service time in [5, 25]
        jobs[i].arrival_time = arrival_accum + (rand() % 11); // arrival time in [0, 10]
        jobs[i].remaining_time = jobs[i].service_time;
        arrival_accum = jobs[i].arrival_time;
    }
}

void print_results(Job jobs[], int total_turnaround, int total_waiting) {
    printf("Job\tArrival\tStart\tService\tTermination\tTurnaround\tWaiting\n");
    for (int i = 0; i < NUM_JOBS; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\t\t%d\n", i+1, jobs[i].arrival_time, jobs[i].start_time,
               jobs[i].service_time, jobs[i].termination_time, jobs[i].turnaround_time, jobs[i].waiting_time);
    }
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / NUM_JOBS);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting / NUM_JOBS);
}

void round_robin(Job jobs[], int time_quantum, int context_switch) {
    int total_turnaround = 0, total_waiting = 0;
    int current_time = 0;
    int completed_jobs = 0;

    while (completed_jobs < NUM_JOBS) {
        int all_idle = 1;
        for (int i = 0; i < NUM_JOBS; i++) {
            if (jobs[i].arrival_time <= current_time && jobs[i].remaining_time > 0) {
                all_idle = 0;
                if (jobs[i].remaining_time == jobs[i].service_time) {
                    jobs[i].start_time = current_time;
                }
                if (jobs[i].remaining_time <= time_quantum) {
                    current_time += jobs[i].remaining_time;
                    jobs[i].termination_time = current_time;
                    jobs[i].remaining_time = 0;
                    jobs[i].turnaround_time = jobs[i].termination_time - jobs[i].arrival_time;
                    jobs[i].waiting_time = jobs[i].turnaround_time - jobs[i].service_time;
                    total_turnaround += jobs[i].turnaround_time;
                    total_waiting += jobs[i].waiting_time;
                    completed_jobs++;
                } else {
                    jobs[i].remaining_time -= time_quantum;
                    current_time += time_quantum;
                }
                current_time += context_switch;
            }
        }
        if (all_idle) {
            current_time++;
        }
    }
    print_results(jobs, total_turnaround, total_waiting);
}

int main() {
    Job jobs[NUM_JOBS];
    generate_jobs(jobs);

    int time_quantums[] = {1, 2, 5, 10};
    int context_switches[] = {0, 1};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            printf("\nRound Robin with Time Quantum %d and Context Switch Time %d:\n", time_quantums[i], context_switches[j]);
            for (int k = 0; k < NUM_JOBS; k++) {
                jobs[k].remaining_time = jobs[k].service_time;
                jobs[k].start_time = 0;
                jobs[k].termination_time = 0;
                jobs[k].turnaround_time = 0;
                jobs[k].waiting_time = 0;
            }
            round_robin(jobs, time_quantums[i], context_switches[j]);
        }
    }

    return 0;
}
