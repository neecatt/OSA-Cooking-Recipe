#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SHM_KEY 1234

#define NUM_SIGNALS 3
#define NUM_TRIALS 100

// Create an array of signals
const int signals[NUM_SIGNALS] = {SIGINT, SIGQUIT, SIGTERM};

int main()
{
    // Seed the random number generator
    srand(time(NULL));

    // Get the shared memory segment ID
    int shmid = shmget(SHM_KEY, sizeof(pid_t), 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach to shared memory segment
    pid_t *shared_pid = shmat(shmid, NULL, 0);
    if (shared_pid == (pid_t *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Get the server process ID from shared memory
    int server_pid = *shared_pid;
    printf("Server process ID: %d\n", server_pid);

    int i, j;
    for (i = 0; i < NUM_TRIALS; i++)
    {
        // Pick a random signal to send
        int signal = signals[rand() % NUM_SIGNALS];
        printf("Sending %s signal\n", strsignal(signal));
        // Send the signal to the server process
        kill(server_pid, signal);
        // Wait for 1 second
        sleep(1);
    }

    // Detach from shared memory segment
    shmdt(shared_pid);

    return 0;
}