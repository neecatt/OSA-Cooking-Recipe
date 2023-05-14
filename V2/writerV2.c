#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 1234
#define MAX_SIZE 1024

// Define the message structure
struct message
{
    long type;
    char text[MAX_SIZE];
};

int main(int argc, char *argv[])
{
    // Create the message queue
    int msgqid;
    struct message msg;
    int bytes_read;

    // Create or attach to the message queue
    msgqid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(1);
    }

    // Write the contents of the "recipe.txt" files to the message queue
    FILE *fp;
    char *dir_names[] = {"Student", "Azeri", "French"};
    long msg_type[] = {1, 2, 3}; // The message types
    for (int i = 0; i < 3; i++)
    {
        char filepath[MAX_SIZE];
        sprintf(filepath, "%s/recipe.txt", dir_names[i]); // Create the file path
        fp = fopen(filepath, "r");
        if (fp == NULL)
        {
            perror("fopen");
            exit(1);
        }
        // Read the contents of the file and write them to the message queue
        while ((bytes_read = fread(msg.text, 1, MAX_SIZE, fp)) > 0)
        {
            msg.type = msg_type[i]; // Set the message type
            printf("Writing message of type %ld to the message queue.\n", msg.type);
            // print message id
            printf("Message id: %d\n", msgqid);
            // Write the message to the message queue
            if (msgsnd(msgqid, &msg, bytes_read, 0) == -1)
            {
                // Print an error message and exit
                perror("msgsnd");
                exit(1);
            }
        }
        fclose(fp);
    }

    return 0;
}
