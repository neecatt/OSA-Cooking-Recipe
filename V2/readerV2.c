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
    msgqid = msgget(QUEUE_KEY, 0666);
    if (msgqid == -1)
    {
        // If the message queue does not exist, print an error message and exit
        perror("msgget");
        exit(1);
    }

    // Read messages from the message queue and print them to the console
    while (1)
    {
        long msg_type;
        char *dir_name;
        printf("Enter category (1 for Student, 2 for Azeri, 3 for French, 0 for exit): ");
        scanf("%ld", &msg_type);
        if (msg_type == 0)
        {
            // If the user enters 0, exit the program
            break;
        }
        else if (msg_type == 1)
        {
            dir_name = "Student";
        }
        else if (msg_type == 2)
        {
            dir_name = "Azeri";
        }
        else if (msg_type == 3)
        {
            dir_name = "French";
        }
        else
        {
            printf("Invalid category.\n");
            continue;
        }

        printf("----- %s recipes -----\n", dir_name);

        while ((bytes_read = msgrcv(msgqid, &msg, MAX_SIZE, msg_type, IPC_NOWAIT)) > 0)
        {
            // Print the message to the console
            printf("%s", msg.text);
        }
        printf("\n");
    }
    return 0;
}
