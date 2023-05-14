#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 1234
#define MAX_SIZE 1024

struct message
{
    long type;
    char text[MAX_SIZE];
};

void read_cooking_recipe(int category)
{
    int msgid;
    struct message msg;

    msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget failed with error");
        exit(1);
    }

    if (msgrcv(msgid, &msg, MAX_SIZE, category, IPC_NOWAIT) == -1)
    {
        perror("msgrcv failed with error");
        exit(1);
    }

    printf("Received message: %s\n", msg.text);
}