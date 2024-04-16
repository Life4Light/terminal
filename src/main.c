#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_ARGS 10

int main()
{
    char command[100];

    while (1)
    {
        printf("> ");
        fgets(command, 100, stdin);
        command[strcspn(command, "\n")] = 0;

        char *args[MAX_ARGS];
        int arg_count = 0;

        char *token = strtok(command, " ");
        if (token == NULL)
        {
            continue;
        }
        while (token != NULL && arg_count < MAX_ARGS - 1)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        if (strcmp(args[0], "cd") == 0 && args[1] != NULL)
        {
            chdir(args[1]);
            continue;
        }

        if (strcmp(args[0], "priority") == 0 && args[1] != NULL && args[2] != NULL)
        {
            setpriority(PRIO_PROCESS, atoi(args[1]), atoi(args[2]));
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            exit(0);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}