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

#define QUEUE_KEY 1234 // Message queue key
#define MAX_SIZE 1024  // Maximum size of the message

// Define the message structure
struct message
{
    long type;
    char text[MAX_SIZE];
};

// Function to send a message to the message queue
int send_message(char *dir_name, long msg_type)
{
    // Create the message queue
    int msgqid;

    // Create or attach to the message queue
    msgqid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(1);
    }

    struct message msg;
    FILE *fp;
    char filepath[MAX_SIZE];
    sprintf(filepath, "%s/recipe.txt", dir_name); // Create the file path
    fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    // Read the file and send the message to the message queue
    while (fread(msg.text, 1, MAX_SIZE, fp) > 0)
    {
        printf("Writing message of type %ld to the message queue.\n", msg.type);
        msg.type = msg_type;
        if (msgsnd(msgqid, &msg, MAX_SIZE, 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
    fclose(fp);
    return 0;
}

int main()
{

    char *dir_names[] = {"Student", "Azeri", "French"};
    int num_categories = sizeof(dir_names) / sizeof(dir_names[0]);

    // Create the message queue
    int msgqid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(1);
    }

    // Continously check whether the message queue exits, if does not exist, then refill
    while (1)
    {
        // Check if the message queue is empty
        struct msqid_ds buf;
        if (msgctl(msgqid, IPC_STAT, &buf) == -1)
        {
            perror("msgctl");
            exit(1);
        }
        if (buf.msg_qnum == 0)
        {
            // The message queue is empty, so refill it
            printf("Message queue is empty. Refilling with recipes from all categories...\n");
            for (int i = 0; i < num_categories; i++)
            {
                send_message(dir_names[i], i + 1);
            }
        }
        sleep(1);
    }

    return 0;
}