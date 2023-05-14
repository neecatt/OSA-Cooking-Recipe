#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHM_KEY 1234

void handle_signal(int signal)
{

    char azeri_recipe[400] = "Yumurta Pomidor: 1. Fry tomatoes then eggs on them. 2. Add salt and pepper. 3. Serve immediately in the frying pan.\n Piti: 1. Cut meat into small pieces. 2. Add salt and pepper. 3. Add water. 4. Add vegetables. 5. Cook for 2 hours. 6. Serve.\n";
    char student_recipe[400] = " Noodles: 1. Boil water. 2. Add noodles. 3. Add salt. 4. Add pepper. 5. Add oil. 6. Add vegetables. 7. Add meat. 8. Serve.\n Pasta: 1. Boil water in the pot 2. Add pasta 3. Add salt 4. Add oil 5. Add sauce 6. Serve\n";
    char french_recipe[400] = " Ratatouille: 1. Cut vegetables. 2. Fry vegetables. 3. Add salt and pepper. 4. Add oil. 5. Serve.\n Croissant: 1. Mix flour, water, yeast, sugar, salt, and butter. 2. Roll the dough. 3. Cut the dough into triangles. 4. Roll the triangles. 5. Bake. 6. Serve.\n";

    // define a matrix of signal name to recipe category
    char *mapping[3][3] = {
        {2, "Student", student_recipe},
        {3, "Azeri", azeri_recipe},
        {15, "French", french_recipe}};

    // search for a matching signal number in the mapping
    int i;
    char *recipe_category = NULL;
    char *recipe = NULL;
    for (i = 0; i < 3; i++)
    {
        if (mapping[i][0] == signal)
        {
            recipe_category = mapping[i][1];
            recipe = mapping[i][2];
            break;
        }
    }

    // print the recipe category
    printf("Received %s signal, Recipe category: %s,\n Recipe: %s\n", strsignal(signal), recipe_category, recipe);
}

int main()
{
    pid_t pid = getpid();
    printf("Server process ID: %d\n", pid);

    // register a signal handler for SIGINT, SIGQUIT and SIGTERM
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTERM, handle_signal);

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
