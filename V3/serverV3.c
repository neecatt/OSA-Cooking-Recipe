#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "readerV3.h"

#define SHM_KEY 1234

// filter signals and according to the signal, call the corresponding function
void signal_handler(int signum)
{
    struct message msg;
    int msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget failed with error");
        exit(1);
    }

    while (1)
    {
        if (signum == SIGINT)
        {
            printf("SIGINT received\n");
            if (msgrcv(msgid, &msg, MAX_SIZE, 1, IPC_NOWAIT) == -1 && errno != ENOMSG)
            {
                perror("msgrcv failed with error");
                exit(1);
            }
            else if (errno != ENOMSG)
            {
                printf("Received message: %s\n", msg.text);
            }
            break;
        }
        else if (signum == SIGTERM)
        {
            printf("SIGTERM received\n");
            if (msgrcv(msgid, &msg, MAX_SIZE, 2, IPC_NOWAIT) == -1 && errno != ENOMSG)
            {
                perror("msgrcv failed with error");
                exit(1);
            }
            else if (errno != ENOMSG)
            {
                printf("Received message: %s\n", msg.text);
            }
            break;
        }
        else if (signum == SIGQUIT)
        {
            printf("SIGQUIT received\n");
            if (msgrcv(msgid, &msg, MAX_SIZE, 3, IPC_NOWAIT) == -1 && errno != ENOMSG)
            {
                perror("msgrcv failed with error");
                exit(1);
            }
            else if (errno != ENOMSG)
            {
                printf("Received message: %s\n", msg.text);
            }
            break;
        }
        else
        {
            printf("Invalid signal\n");
            break;
        }
    }
}

int main(void)
{
    // get the server's PID
    pid_t pid = getpid();
    printf("Server process ID: %d\n", pid);

    // register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);

    // create a shared memory segment
    int shmid = shmget(SHM_KEY, sizeof(pid_t), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // attach the shared memory segment
    pid_t *shared_pid = shmat(shmid, NULL, 0);
    if (shared_pid == (pid_t *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // store the server's PID in the shared memory segment
    *shared_pid = pid;

    // run an infinite loop
    while (1)
    {
        sleep(1);
    }

    return 0;
}