#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/msg.h>

#define SHM_KEY 1234   // Shared memory key
#define QUEUE_KEY 1234 // Message queue key
#define MAX_SIZE 1024  // Maximum size of the message

// Define the message structure
struct message
{
    long type;
    char text[MAX_SIZE];
};

int main(void)
{

    // Get the server process ID from shared memory
    int shmid = shmget(SHM_KEY, sizeof(pid_t), 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    // Attach to the shared memory segment
    pid_t *shared_pid = shmat(shmid, NULL, 0);
    if (shared_pid == (pid_t *)-1)
    {
        perror("shmat");
        exit(1);
    }
    int server_pid = *shared_pid;

    // Get the input from the user
    int input;
    int signal;

    // Loop until the user enters 0
    while (1)
    {
        // Prompt the user for input
        printf("Enter a number (1(SIGINT), 2(SIGQUIT), or 3(SIGTERM)), or 0 to exit: ");
        scanf("%d", &input);

        // Check that the input is valid
        if (input < 0 || input > 3)
        {
            printf("Invalid input. Please enter 1, 2, or 3, or 0 to exit.\n");
            continue;
        }

        // Check if the user wants to exit
        if (input == 0)
        {
            printf("Exiting...\n");
            break;
        }

        printf("Sending message to server process ID: %d.\n", server_pid);

        // Send a signal based on the user input

        if (input == 1)
        {
            signal = SIGINT;
            // Send the signal to the server process
            kill(server_pid, signal);
            printf("Sending %s signal to server\n", strsignal(signal));
            continue;
        }

        if (input == 2)
        {
            signal = SIGTERM;
            // Send the signal to the server process
            kill(server_pid, signal);
            printf("Sending %s signal to server\n", strsignal(signal));
            continue;
        }

        if (input == 3)
        {
            signal = SIGQUIT;
            // Send the signal to the server process
            kill(server_pid, signal);
            printf("Sending %s signal to server\n", strsignal(signal));

            continue;
        }
    }

    // Detach from shared memory segment
    shmdt(shared_pid);

    // Send a message to the message queue

    return 0;
}
